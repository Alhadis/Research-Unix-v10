#include "jerq.h"
#include "font.h"

#define	TRUE	1
#define	FALSE	0

/*
 * how many shorts go into a Word?
 */
#define shorts(x)	((x)/(sizeof(Word)/sizeof(short)))

#define	Texture16	Texture
#define	texture16	texture

#define urnd(a, b) ((a) + rand()%((b) - (a)))	/* a <= urnd (a, b) < b */

#ifdef mc68000
#define	beep	(*((char*)(384*1024L+062)) = 2) /* blit only */
#else
#define beep	ringbell()
#endif

#define max(a, b)	((a > b) ? (a) : (b))
#define min(a, b)	((a < b) ? (a) : (b))
#define	div2(expr)	((expr) >> 1)	/* quick divide by 2	*/

/* 'mushorigin' returns of origin of mushroom (x, y) in scr. coords.	*/
#define	mushorigin(x, y) (add(Drect.origin, Pt((x) << 5, (y) << 5)))

/* 'put_object' displays 'obj' at 'obj.rect.origin' on the display.	*/
#define	put_object(obj) (bitblt ((obj).looks, (obj).looks->rect, &display, (obj).rect.origin, F_XOR))

/* 'show_extra' (un)displays extra players at top of screen.	*/
#define	show_extra(num)	(bitblt (&melooks, melooks.rect, &display, mushorigin((num)+1, 0), F_XOR))

/* 'moveobj' undisplays and redisplays objects to move them. It is a
 * 	'#define' for efficiency reasons only.
 */
#define moveobj(obj)	if ((obj.dirchoose) (&obj)) { \
				put_object (obj); \
				obj.rect = obj.next; \
				put_object (obj); \
			} \
			else ;

/* mushroom parameters	*/
#define	CHUNKMAX	4	/* Number of times a mushroom must be
					shot before it is destroyed.	*/
#define	MUSHROWS	33	/* Maximum number of rows of mushrooms	*/
#define	MUSHCOLS	25	/* Maximum number of cols of mushrooms	*/
#define	MUSHWIDTH	32	/* Width of mushrooms in pixels	*/
#define	MUSHHEIGHT	32	/* Width of mushrooms in pixels	*/
#define	MUSHVALUE	2	/* Points for shooting entire mushroom.	*/
/* If < these mushrooms in bottom of screen then the slug drops more.	*/
#define minbotmush()	((round_cnt > 10) ? (round_cnt/2) : 6)

/* 'mushx' and 'mushy' convert screen coord. values to indicies in
 *	the matricies containing mushroom data (e.g. 'mushspace').
 */
#define mushx(col)	((int) ((col - Drect.origin.x) >> 5))
#define mushy(row)	((int) ((row - Drect.origin.y) >> 5))

/* Player parameters	*/
#define	MEWIDTH	32	/* Width of player object in pixels	*/
#define MEHEIGHT	32	/* Height of player object in pixels	*/
#define ME_MAX  3	/* Number of player objects at start of game	*/
#define	MERATE	0	/* Normal player movement rate.	(every time)	*/

/* Scorpion parameters	*/
#define	SCORPRATE	03	/* Scorpion movement rate	*/
#define SCORPSPEED	10	/* Pixels moved by socrpion	*/
#define	SCORPVALUE	1000	/* Points for shooting a scorpion	*/
#define	SCORPGONE	500 /* Max. Ticks that scorpion is off screen */
/* 'scorpgone' becomes less and less as game progresses	*/
#define scorpgone()	(max(SCORPGONE - 30 * round_cnt, 0))

#ifdef notdef
/* Player's missile parameters	*/
#define SHOTRATE	01	/* moves every other time */
#define	SHOTSPEED	(-20)	/* Pixels moved by missile	*/
#else
#define SHOTRATE	0	/* moves every other time */
#define	SHOTSPEED	(-10)	/* Pixels moved by missile	*/
#endif

/* Slug (mushroom dropper) parameters	*/
#define	SLUGCHECK	077	/* Check for mushroom dropper (ticks)	*/
#define	SLUGVALUE	200	/* Points for killing a slug	*/
#define SLUGRATE	01	/* Slug movement rate	*/
#define	SLOWSLUG	10	/* Pixels moved by slow slug	*/
#define	FASTSLUG	25	/* Pixels moved by fast slug	*/

/* Spider parameters	*/
#define	SPIDERWIDTH	48	/* Width of spider in pixels	*/
#define	SPIDERHEIGHT	32	/* Height of spider in pixels	*/
#define SPIDERRATE	03	/* Normal spider movement rate.	*/
#define	SPIDERSPEED	5	/* Pixels moved by spider	*/
#define	SPIDERVALUE	300	/* Minimum worth of spider	*/
#define	SPIDERGONE	150 /* No. of game ticks spider stays off screen*/

/* Centipede parameters	*/
#define	WORMMAX	15	/* Max. number of centipede segments	*/
#define WORMRATE	07	/* Centipede movement rate	*/
#define	WORMXSPEED	16	/* Pixels moved horiz. by centipede	*/
#define	WORMYSPEED	32	/* Pixels moved vertically by centipede	*/
#define	WORMVALUE	100	/* Points for shooting a non-head seg.	*/
#define	HEADVALUE	200	/* Points for shooting a head	*/

#define	WAITRATE	037	/* Game ticks between each 'wait(CPU)'	*/
#define	NEWMANSTEP	12000	/* New player at every 'NEWMANSTEP' pts.*/

/* Miscellaneous attributes <object>.atts' */
#define EXISTS	001	/* TRUE if object is currently on screen.	*/
#define PLUMMETING	002	/* TRUE if centipede seg. is plummeting	*/
#define	IS_HEAD	004	/* TRUE if object is a centipede head	*/
#define HIT_BOTTOM	010	/* TRUE if centipede seg. has hit the
					bottom of the screen.	*/
#define CLIMBMODE	020	/* TRUE if centipede seg. is going up.	*/

/* attribute manipulating functions	*/

/* 'IS_SET' returns TRUE iff 'att' is TRUE for 'obj'	*/
#define IS_SET(obj, att)	((obj).atts & (att))
/* 'SET_ATT' sets 'att' to TRUE for 'obj'	*/
#define	SET_ATT(obj, att)	((obj).atts |= (att))
/* 'CLEAR_ATT' sets att to FALSE for 'obj'	*/
#define CLEAR_ATT(obj, att)	((obj).atts &= ~(att))

typedef	struct	object { /* Everything that moves in this game is
				treated as an object. */
	Rectangle	rect,	/* current screen location of object	*/
			next;	/* Scr. loc. to move object to next.	*/
	int	dxdt,	/* Horizontal movement rate in pixels	*/
		dydt,	/* Vertical movement rate in pixels	*/
		type,	/* Not used	*/
		value; /* worth or hit points */
	int	(*dirchoose)();	/* function for deciding next move	*/
	Bitmap	*looks;	/* appearance of object	*/
	int	atts; /* bit vector of other attributes. */
};

/* The following stuff is in 'globals.c'	*/
extern	Point	oldmouse;
extern	Rectangle	botrect,
			playerrect;
extern	Texture16	plain,
			fancy;
extern	Bitmap	mushlooks,
		melooks,
		spiderlooks,
		facelooks,
		wormlooks,
		headlooks,
		chunk1looks,
		chunk2looks,
		chunk3looks,
		chunk4looks,
		glowlooks,
		gl1looks,
		gl2looks,
		gl3looks,
		gl4looks;
		
extern	struct	object	me,
			worm[],
			slug,
			spider,
			shot,
			scorpion;
extern	char	glowmush[MUSHCOLS][MUSHROWS],
		mushspace[MUSHCOLS][MUSHROWS],
		spidbuf[4];
extern	int	botmush_cnt,
	firstworm,
	me_cnt,
	mush_cnt,
	mushcols,
	mushrows,
	round_cnt,
	scorp_fuse,	
	spider_fuse,	
	start_over,	
	topofbottom;
extern	long	score,
		newmanat;

extern	Bitmap	N[10]; /* in 'digits.c' */

extern	int	personchoose();
extern	int	slugchoose();
extern	int	wormchoose();
extern	int	spiderchoose();
extern	int	shotchoose();
extern	int	scorpchoose();

