#include <font.h>
#define	MAXLINES	(YMAX/9)	/*9=tiniest known height*/
#define newlnsz		(P->defaultfont->height)

typedef struct String{
	char *s;	/* pointer to string */
	short n;	/* number used */
	short size;	/* size of allocated area */
} String;

typedef unsigned char Nchar;	/* number of chars on a line */
typedef struct Frame{
	Rectangle rect;		/* Screen area of frame, exact #lines high */
	Rectangle scrollrect;	/* Screen area of scrollbar */
	Rectangle totalrect;	/* Screen area covered by entire frame */
	String 	str;		/* What's in the frame */
	unsigned short	s1, s2;	/* Indexes of ends of selected text */
	Point	scroll;		/* Scroll bar, 0<=(x=top),(y=bot)<=4096 */
	short	nlines;		/* Number of screen lines of text */
	short	nullsel;	/* True if last selection was null */
	short	lastch;		/* Last selected char when s1==s2 */
	short	margin;		/* margin around frame */
	Nchar	cpl[MAXLINES];	/* Number of characters per line */
} Frame;

#define	SCROLLWIDTH	10	/* width of scroll bar */
#define	SCROLLRANGE	4096	/* range of scrolling parameter */

#define	D		(P->layer)
#define	TRUE		1
#define	FALSE		0
#define	cwidth(c)	P->defaultfont->info[c].width

extern Frame 		*fralloc();
extern Rectangle	canon();
extern Point		nullpoint;
extern Point		toscreen(),ptofchar(), startline();
extern void		oprectf();
extern void		opclear();
extern Point		endpoint;	/* last position drawn during a frameop() */
extern int		complete;	/* did frameop do all it was supposed to? */
extern int		inscomplete;	/* is full insertion visible on screen? */
extern int		F_rectf;	/* function code for oprectf */
extern void		opnull();	/* nop routine for frameop side effects */
extern void		opdraw();	/* standard routine to draw the text */
char	*Ualloc();
