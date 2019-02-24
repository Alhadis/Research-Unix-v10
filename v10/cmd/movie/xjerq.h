#ifndef	JERQ_H
#define	JERQ_H
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

#ifndef NULL
#define NULL 0
#endif

typedef int	Word;		/* 32 bits */

typedef unsigned int	UWord;	/* 32 bits */

typedef struct Point {
	short	x;
	short	y;
} Point;

typedef struct Rectangle {
	Point origin;
	Point corner;
} Rectangle;

typedef struct Bitmap { /* a Bitmap, but offset */
	Drawable dr;
	Rectangle rect;
	int flag;
} Bitmap;

/*
 * Flag bits
 */
#define BI_PIXMAP	1

typedef struct Menu{
	char	**item;			/* string array, ending with 0 */
	char	*(*generator)();	/* used if item == 0 */
	short	prevhit;		/* private to menuhit() */
	short	prevtop;		/* private to menuhit() */
} Menu;

struct Mouse {
	Point xy;
	int buttons;
	unsigned long time;
};

struct JProc {
	int	state;	/* only one we need */
};
#define RESHAPED	1		/* window has been changed */
#define KBD		2		/* we have keyboard input */
#define RCV		4		/* recevied from "host" proc */
#define SEND		8		/* sent to "host" proc */	
#define MOUSE		16		/* we always have the mouse */
#define CPU		32		/* we always have the cpu */

typedef int Code;

#define Texture		Pixmap
#define Font		XFontStruct
#define	wait(x)		jwait(x)

/* The faster implementation of Rect, Pt, and Rpt is machine dependent
   (depends on structure packing and argument passing).  To avoid
   machine dependencies, and make lint happy, use -Dsafe */
#ifdef safe
#define Rect(a,b,c,d)	SRect(a,b,c,d)
extern	Point		Pt();
extern	Rectangle	SRect();
extern	Rectangle	Rpt();
#else
#define	Pt(x, y)	 ((((short)(x))<<16)|((unsigned short)(y)))
#define	Rect(x1, y1, x2, y2)	Pt(x1, y1), Pt(x2, y2)
#define	Rpt(p1, p2)		(p1), (p2)
#endif

#define	muldiv(a,b,c)	((long)((a)*((long)b)/(c)))

/*
 * Function Codes
 */
#define	F_STORE	0		/* target = source */
#define	F_OR	1		/* target |= source */
#define	F_CLR	2		/* target &= ~source */
#define	F_XOR	3		/* target ^= source */

#define button(i)		(mouse.buttons&(8>>i))
#define button1()		(mouse.buttons&4)
#define button2()		(mouse.buttons&2)
#define button3()		(mouse.buttons&1)
#define button12()		(mouse.buttons&6)
#define button13()		(mouse.buttons&5)
#define button23()		(mouse.buttons&3)
#define button123()		(mouse.buttons&7)

Rectangle getrectb(), getrect();
#define getrect1()		getrectb(4)
#define getrect2()		getrectb(2)
#define getrect3()		getrectb(1)
#define getrect12()		getrectb(6)
#define getrect13()		getrectb(5)
#define getrect23()		getrectb(3)
#define getrect123()		getrectb(7)

extern Point add(), sub(), mul(), div(), string();
extern Rectangle rsubp(), raddp(), inset();
extern Bitmap *balloc();
extern char *gcalloc(), *calloc();
extern void bfree(), gcfree();
extern void rectf(), rectfD(), bitblt(), bitbltD(),
	texture(), evtomouse();

#define alloc(n) calloc(n,1)
#define realtime(x) (mouse.time)
#define jstrwidth(b) strwidth(&defont,b)

extern Cursor ToCursor(), *cursswitch();
extern Pixmap ToPixmap();
extern Cursor normalcursor;
extern Rectangle Drect;
extern Bitmap	display;
extern GC	gcs[];
extern Display *dpy;
extern unsigned long	fgpix;
extern unsigned long	bgpix;
extern Colormap colormap;
extern XColor	fgcolor, bgcolor;
extern struct	Mouse mouse;
extern struct	JProc *P;
extern Font	defont;

#endif JERQ_H
