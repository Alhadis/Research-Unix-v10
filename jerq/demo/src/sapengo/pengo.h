/*
 *	header file for pengo.	BLIT (and jerq) version
 */

#ifdef	JERQ
#include "jerq.h"
#define	 sbits(x)	x, 0x0000
#ifdef	MUX
#define	MPX	MPX		/* blit uses MPX */
#endif	MUX
#else
#include "blit.h"
#define	 sbits(x)	x
#endif
#include "font.h"

#undef	NULL			/* define NULL for those who need it */
#define	NULL	0

#define	HEAD_X	(13 * 16)	/* number of bits wide */
#define	HEAD_Y	203		/* number of rows deep */
#define	HEAD_COUNT	300	/* 10 seconds till static display */

#define	TRUE	1
#define	FALSE	0

#define	ALIVE	0		/* states that a character can be in */
#define	DYING	1
#define	DEAD	2		/* must be the same as SNO_BEE (defined later) */
#define	CONFUSED	3
#define	HATCHING	4
#define	SQUASH	5
#define	MOVING	6
#define	VANISH	7

#define	STILL	0		/* direction movements */
#define	DOWN	1
#define	RIGHT	2
#define	LEFT	3
#define	UP	4

#define	VERT	0	/* vertical */
#define	HORIZ	1	/* horizontal */

/* defines for the game */
#define	shorts(x)	((x)/(sizeof(Word)/sizeof(short)))
#define	MAX_X	23		/* maximum X blocks */
#define	MAX_Y	29		/* maximum Y blocks */
#define	MAXX	(MAX_X + 4)	/* used for screen co-ordinates */
#define	MAXY	(MAX_Y + 4)	/* used for screen co-ordinates */

#define	SHAPE_WIDTH	32	/* sorry, this really can't be changed */
#define	SHAPE_HEIGHT	32
#define	CHAR_HEIGHT	32
#define	CHAR_WIDTH	32
#define	NUM_DIGITS	6
#define	NUM_ADD_MAN	7
#define MINX	((NUM_DIGITS + 2) * SHAPE_WIDTH)     /* min size of screen (x) */
#define MINY	(NUM_DIGITS * SHAPE_HEIGHT + CHAR_HEIGHT)   /* min size (y) */

/* different states that a spot on the screen can be in. */
#define	BLANK	0
#define	BLOCK	1
#define	SNO_BEE	2	/* not really, but just in case */
#define	PENGO	3	/* not really, but just in case */
#define	DIAMOND	4
#define	EGG	5
#define	HATCH	6
#define	BREAK	7
#define	CONF	8	/* when sno_bee is confused */
/* used for put_char only */
#define	PENGO_D	9
#define	SALUTE	10
#define	WIGGLE	11
#define	BACK	12
/*
	13 to 15 used for top states
*/
#define	FBLOCK	13	/* special for put_char and move_block only */
#define	WALL	14
#define	BORDER	15

#define	HULA	16
#define	CHAIR	17
#define	VIDEO	18
#define	PLAYERR	19
#define	PLAYERL	20

/* movement rates. Number of clock ticks allowed between moves */
#define	MOVE_PENGO	3L	/* pengo movement ~1 sec */
#define	MOVE_BEE2	2L	/* angry or running bee */
#define	MOVE_BEE1	3L	/* annoyed/fast bee */
#define	MOVE_BEE0	4L	/* normal/slow bee state */
#define	MOVE_BLOCK	2L	/* block movement */
#define	DIE_BLOCK	2L	/* block destruction time */
#define	EGG_HATCH	4L	/* egg hatching time */

#define	BEE_MOVE	8	/* movement rate of sno_bee */
#define	PENGO_MOVE	8	/* movement rate of pengo */
#define	BLOCK_MOVE	4	/* movement rate of a block */

#define	NUM_HATCH	8	/* number of states in a hatching */
#define	NUM_BREAK	7	/* number of states in a breaking */

#define	MAX_BEES	12	/* Maximum number of bees displayable */
#define	MAX_PENGO	8	/* Maximum number of Pengo's displayable */
#define	NUM_PENGO	4	/* initial number of Pengo's */
#define	NUM_WATER	6	/* number of cycles in the mater movement */
#define	NUM_SCORES	5	/* number of scores possible on screen */

#define	DYING_TIME	10L	/* time for dying loop */
#define	P_DYING		10L	/* pengo dying time */
#define	PENGO_D_TIME	4L	/* time while pengo is dying */
#define	PENGO_W_TIME	6L	/* time while pengo is wiggling */
#define	CLR_SCORE	120L	/* score on the screen time */
#define	FLASH_DIAMOND	3L	/* diamond flashing time */
#define	WATER_TIME	5L	/* water movement delay */
#define	SHOW_EGGS	150L	/* time that eggs are on screen for */
#define	RUN_DELAY	600L	/* 10 seconds till it runs */
#define	NUM_FLASH	10	/* number of flashes in the diamonds */
#define	DIE_BEE		10L	/* a sno_bee dying */
#define	TIME_CONFUSED	18L	/* six seconds of confusion */
#define	NUM_CONFUSED	20	/* 20 * 18 = 360 ticks */
#define	NUM_WIGGLE	20	/* number of wiggles done */
#define	NUM_HULA	3	/* number of hula loops */
#define	NUM_HULA0	5	/* number each side */
#define	SHOW_SCORE	150L	/* time score shown on screen */
#define	PENGO_SALUTE	180L	/* time that pengo's salute for */
#define	DANCE_PENGO	4L	/* while pengo's are moving during levels */
#define	PENGO_THROES	10	/* dying throes */
#define	PENGO_HULA	4L	/* hula dance of pengo's */
#define	SHOW_VIDEO	8L	/* time video player is hown for */
#define	NUM_BOUNCE	8	/* number of bounces at video table (must be even) */

#define	SNO_BEE_CHANGE	30	/* 30 % chance of changing direction */
#define	SNO_BEE_RANDOM	40	/* 40 % chance of moving in a random direction */
#define	SNO_BEE_BREAK	10	/* 10 % chance of breaking block */

/* type definitions */

typedef	struct	element	{
	int	x;		/* x position */
	int	y;		/* y position */
#ifdef	JERQ
	int	movex;		/* x-movement */
	int	movey;		/* y movement */
	int	exists;		/* is it on the screen? */
	int	count;		/* what stage it is up to */
	int	other;
#else	JERQ
	char	movex;		/* x-movement */
	char	movey;		/* y movement */
	char	exists;		/* is it on the screen? */
	char	count;		/* what stage it is up to */
	char	other;
#endif	JERQ
} element;
/* externs for files */

extern	int	pengo_left;	/* number of pengos left */
extern	int	sno_bee_left;	/* number of sno_bees left */
extern	int	egg_count_flag;	/* # eggs left */
extern	int	manptr;		/* index into new_man array */
extern	int	hit;		/* button 2 state */
extern	int	seggs;		/* are eggs on display ? */
extern	char	move_water;	/* side wall movement */
extern	char	sdiamond;	/* has the diamond's score been added ? */
extern	char	s[MAXX][MAXY];	/* storage for which blocks are on */
extern	long	score;		/* current score */
extern	long	high_score;	/* current high score */
extern	long	my_time;	/* 60th of a second counter for play time */
extern	long	old_score;	/* Previous score.. Temp variable */
extern	long	new_man[];	/* scores at which a new man is awarded */
extern	Menu	menu3;		/* menu for button 3 */
extern	element	pengo;		/* pengo definition */
extern	element	sno_bee[4];	/* which blocks are really sno_bees */
extern	element	eggs[MAX_BEES];	/* which blocks are eggs */
extern	element	diamond[3];	/* which blocks are really diamonds */
extern	element	moving;		/* block which is moving or breaking */
extern	element	breaking;	/* block which is being broken */
extern	element	bee_block[4];	/* blocks being broken by bees */

extern	Point	old_mouse;	/* initial mouse position */
extern	Point	new_mouse;	/* current mouse position */
extern	Point	move_mouse;	/* difference of old & new */
extern	Point	home_mouse;	/* mouse home position */

extern	Bitmap	bit16;		/* 16 * 16 generic Bitmap */
extern	Bitmap	bit32;		/* 32 * 32 generic Bitmap */
extern	Bitmap	bonus;		/* 10000 point bonus bitmap */
extern	Bitmap	disp_head;	/* startup and waiting display */

extern	Rectangle	board;
extern	Rectangle	board0;
extern	Rectangle	board1;
extern	Rectangle	header;
extern	Rectangle	bwater;
extern	Rectangle	lwater;
extern	Rectangle	rwater;
extern	Rectangle	twater;
extern	Rectangle	counter;

extern	short	N[];		/* storage for digits */
extern	short	char_0_9[];	/* 16 * 16 digits */
extern	short	char_A_Z[];	/* 16 * 16 A->Z */
extern	short	char_colon[];	/* 16 * 16 ':' */
extern	short	char_space[];	/* 16 * 16 space */
extern	short	char_dot[];	/* 16 * 16 '.' */
extern	short	char_blank[];	/* 32 * 32 space */
extern	short	char_block[];	/* 32 * 32 block */
extern	short	flip_block[];	/* 32 * 32 block */
extern	short	char_pengo[];	/* 32 * 32 pengo */
extern	short	char_sno_bee[];	/* 32 * 32 sno_bee */
extern	short	char_0diamond[];/* 32 * 32 diamond 0 */
extern	short	*char_1diamond;	/* 32 * 32 diamond 1 */
extern	short	char_null[];	/* 32 * 32 NULL pattern */
extern	short	char_egg[];	/* 32 * 32 egg shape */
extern	short	nw[];		/* still water */
extern	short	tw[];		/* top water barrier */
extern	short	bw[];		/* bottom water barrier */
extern	short	lw[];		/* left barrier */
extern	short	rw[];		/* right barrier */
extern	Texture	skull;		/* skull and X bones for quit */

/* access routines for the RAM screen map */

#define	screen(x, y)	s[((x) + 2)][((y) + 2)]

#define	max(a, b)	((a > b) ? (a) : (b))
#define	min(a, b)	((a < b) ? (a) : (b))
#define abs(a)		((a < 0) ? (-a) : (a))
#define	R3		Rect(0, 0, 32, 32)
#define	RX(a, b)	rectXrect(raddp(R3,Pt(a.x,a.y)),raddp(R3,Pt(b.x,b.y)))
#define	P3		Rect(8, 8, 24, 24)	/* pengo shape (smaller) */
#define	PX(a, b)	rectXrect(raddp(P3,Pt(a.x,a.y)),raddp(R3,Pt(b.x,b.y)))
#define	addchar(c,p,f)	bitblt(bchar(c), Rect(0, 0, 16, 16), &display, p, f)
/* functions called for add_event */
extern	int	clr_bee();
extern	int	die_pengo();
extern	int	add_score();
extern	int	move_player();
extern	int	move_sno_bee();
extern	int	move_block();
extern	int	die_block();
extern	int	add_man();
extern	Bitmap	*bchar();	/* return bitmap of character */
extern	int	swill_water();
extern	int	egg_hatch();
extern	int	clear_egg();
extern	int	show_eggs();
extern	int	sbd;
extern	int	npd;
extern	Rectangle	pengom;
extern	Rectangle	sno_bm;
extern	Rectangle	dboard;
extern	Rectangle	dsp0;
extern	Rectangle	dsp1;
extern	Rectangle	dsp2;
extern	short	spengo[];
extern	short	sbee[];
extern	int	run_bee();
extern	int	flash_diamond();
extern	int	run_sno_bee();
extern	int	bee_run;
extern	int	bee_break();
extern	char	blockx;
extern	char	blocky;
extern	int	vanish_bee();
extern	int	confused_bee();
extern	short	conf1[];
extern	short	conf2[];
extern	short	char_100[];
extern	short	char_400[];
extern	short	char_500[];
extern	short	char_1600[];
extern	short	char_3200[];
extern	short	char_6400[];
extern	short	char_0bee[];
extern	short	char_1bee[];
extern	short	char_0pengo[];
extern	short	char_1pengo[];
extern	short	char_2pengo[];
extern	short	char_3pengo[];
extern	short	char_4pengo[];
extern	short	char_5pengo[];
extern	short	char_6pengo[];
extern	short	char_7pengo[];
extern	short	char_8pengo[];
extern	short	*char_still;
extern	short	*char_sorry;
extern	short	*char_happy;
extern	short	char_0dead[];
extern	short	char_1dead[];
extern	short	*pengo_moves[];
extern	short	char_question[];
extern	short	char_lbrkt[];
extern	short	char_rbrkt[];
extern	short	char_uscore[];
extern	short	char_slash[];
extern	short	char_pback[];
extern	short	char_salute[];
extern	short	char_0wiggle[];
extern	short	char_1wiggle[];
extern	int	reshaped;	/* for use in the stand alone world.. */
extern	int	sb_change;
extern	int	sb_random;
extern	int	sb_break;
extern	short	ch_sq_0down[];
extern	short	ch_sq_0left[];
extern	short	ch_sq_0right[];
extern	short	ch_sq_0up[];
extern	short	ch_sq_1down[];
extern	short	ch_sq_1left[];
extern	short	ch_sq_1right[];
extern	short	ch_sq_1up[];
extern	short	*squash_moves[];
extern	short	char_header[];
extern	short	char_0hula[];
extern	short	char_1hula[];
extern	short	char_2hula[];
extern	short	char_3hula[];
extern	short	char_4hula[];
extern	short	*hula_moves[];
extern	short	char_lchair[];
extern	short	char_rchair[];
extern	short	char_0video[];
extern	short	char_0lplayer[];
extern	short	char_1lplayer[];
extern	short	char_0rplayer[];
extern	short	char_1rplayer[];
