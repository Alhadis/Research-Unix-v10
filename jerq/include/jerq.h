#ifndef	JERQ_H
#define	JERQ_H
/*
 *  Model 5620 DMD
 */

#include <sys/2681.h>

#define	DUART		((struct duart *)0x200000)
#define	TVIDEO		(DUART->scc_sopbc = 0x02)
#define	RVIDEO		(DUART->scc_ropbc = 0x02)
#define	BonW()		RVIDEO
#define	WonB()		TVIDEO
#define	DADDR		((unsigned short *)(0x500000))	/* display base address/4 */
#define	YMOUSE		((short *)(0x400000))
#define	XMOUSE		((short *)(0x400002))
#define	WORDSHIFT	5
#define	WORDSIZE	32
#define	WORDMASK	(WORDSIZE-1)
#define	ONES		0xFFFFFFFF
#define	FIRSTBIT	((unsigned)0x80000000)
#define	LASTBIT		((unsigned)0x1)
#define	XMAX		800
#define	YMAX		1024

/*
 *	Graphics definitions
 */

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

typedef struct Bitmap {
	Word	*base;		/* pointer to start of data */
	unsigned width;		/* width in 32 bit words of total data area */
	Rectangle rect;		/* rectangle in data area, local coords */
	char	*_null;		/* unused, always zero */
} Bitmap;

typedef struct Menu{
	char	**item;			/* string array, ending with 0 */
	char	*(*generator)();	/* used if item == 0 */
	short	prevhit;		/* private to menuhit() */
	short	prevtop;		/* private to menuhit() */
} Menu;

typedef struct Texture32 {
	Word bits[32];
} Texture32;

typedef struct Texture {
	short	bits[16];
} Texture;

extern struct Mouse {
	Point	xy;
	short	buttons;
} mouse;

#ifdef lint
extern	Point		Pt();
extern	Rectangle	Rect();
extern	Rectangle	Rpt();
#else
#define	Pt(x, y)	((((short)(x))<<16)|((unsigned short)(y)))
#define	Rect(x1, y1, x2, y2)	Pt(x1, y1), Pt(x2, y2)
#define	Rpt(p1, p2)		(p1), (p2)
#endif

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

#define	muldiv(a,b,c)	((long)((a)*((long)b)/(c)))

extern Word topbits[], botbits[];	/* now full 32 bit words */
extern Rectangle Jrect;
extern Bitmap display;

/*
 * Function Codes
 */
typedef int	Code;
#define	F_STORE	((Code) 0)	/* target = source */
#define	F_OR	((Code) 1)	/* target |= source */
#define	F_CLR	((Code) 2)	/* target &= ~source */
#define	F_XOR	((Code) 3)	/* target ^= source */
Point add(), sub(), mul(), div(), jstring(), string();
Rectangle rsubp(), raddp(), inset();
Word *addr();
char	*alloc(), *gcalloc();
Bitmap *balloc();
Texture *cursswitch();
#define	NULL	((char *)0)
#define	KBD	1
#define	SEND	2
#define	MOUSE	4
#define	RCV	8
#define	CPU	16
#define ALARM	32

#define ringbell()	DUART->b_data=0x08,nap(3),DUART->b_data=0

#ifdef	MUX
#include <mux.h>
#include <setup.h>
extern Rectangle Drect;
#else
#ifndef	MUXTERM
#define	sw(n)
#define	request(n)
#define sleep(n) nap(n)
#define	exit()	reboot()
#define	transform(p)	(p)
#define	rtransform(r)	(r)
#endif
#define	Drect	Jrect
#endif
#endif JERQ_H
