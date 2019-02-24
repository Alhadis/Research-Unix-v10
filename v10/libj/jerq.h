#define	DADDR		((unsigned short *)(0x500000)) /* display base address/4 */
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

#define TGROW	1
#define TMOVE	0

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

#ifdef lint
extern	Point		Pt();
extern	Rectangle	Rect();
extern	Rectangle	Rpt();
#else
#define	Pt(x, y)	((((short)(y))<<16)|((unsigned short)(x)))
#define	Rect(x1, y1, x2, y2)	Pt(x1, y1), Pt(x2, y2)
#define	Rpt(p1, p2)		(p1), (p2)
#endif

typedef struct Bitmap {
	int jname;		/* symbolic name agreed on by host and terminal */
	Rectangle rect;		/* rectangle in data area, local coords */
} Bitmap;

typedef struct Menu{
	char	**item;			/* string array, ending with 0 */
	char	*(*generator)();	/* used if item == 0 */
} Menu;

typedef struct Texture32 {
	Word bits[32];
} Texture32;

typedef struct Texture {
	short	bits[16];
} Texture;

typedef struct Mouse {
	Point	xy;
	short	buttons;
} Mouse;


extern Mouse mouseQ[];
extern int Qnum;

typedef struct Fontchar
{
	short x;		/* left edge of bits */
	unsigned char top;	/* first non-zero scan-line */
	unsigned char bottom;	/* last non-zero scan-line */
	char left;		/* offset of baseline */
	unsigned char width;	/* width of baseline */
	short junk;		/* there are two bytes of junk in the DMD files */
} Fontchar;

typedef struct Font
{
	short n;		/* number of chars in font */
	char height;		/* height of bitmap */
	char ascent;		/* top of bitmap to baseline */
	long unused;		/* in case we think of more stuff */
	Fontchar info[1];		/* n+1 character descriptors */
} Font;

extern Font *getfont();		/* takes the font name char *name */
extern Font *fntab[];
#define	defont	(*fntab[0])

extern int mousebuttons();
extern Point mousexy();
#define button(i)		(mousebuttons()&(8>>i))
#define button1()		(mousebuttons()&4)
#define button2()		(mousebuttons()&2)
#define button3()		(mousebuttons()&1)
#define button12()		(mousebuttons()&6)
#define button13()		(mousebuttons()&5)
#define button23()		(mousebuttons()&3)
#define button123()		(mousebuttons()&7)

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
extern Rectangle Drect;
extern Bitmap BMT[];
#define	display	BMT[0]

typedef int	Code;
#define	F_STORE	((Code) 0)	/* target = source */
#define	F_OR	((Code) 1)	/* target |= source */
#define	F_CLR	((Code) 2)	/* target &= ~source */
#define	F_XOR	((Code) 3)	/* target ^= source */
Point add(), sub(), mul(), div(), jstring(), string();
Rectangle rsubp(), raddp(), inset();
Bitmap *balloc();
Texture *cursswitch();
Point trackarc(), trackbox(), trackcircle();
Point trackdisc(), trackelarc(), trackeldisc();
Point trackellipse(), trackline(), trackrect();
Point trackstring();

#ifndef NULL
#define	NULL	((char *)0)
#endif

#define	KBD	1
#define	SEND	2
#define	MOUSE	4
#define	RCV	8
#define	CPU	16
#define ALARM	32

/* states */
#define	RUN		1	/* ready to be scheduled */
#define	BUSY		2	/* active */
#define	BLOCKED		4	/* blocked by user with ^S */
#define	USER		8	/* a user-68ld'd process */
#define	KBDLOCAL	16	/* has requested the KBD */
#define	MOUSELOCAL	32	/* has requested the MOUSE */
#define	GOTMOUSE	64	/* currently owns MOUSE */
#define	WAKEUP		128	/* tell CONTROL to issue setrun(p) */
#define	MOVED		256	/* layer got moved */
#define	UNBLOCKED	512	/* Has been unblocked */
#define	ZOMBIE		1024	/* proc died horribly; waiting for debugger */
#define	RESHAPED	2048	/* layer got reshaped */
#define	ZOMBOOT		4096	/* put in ZOMBIE state after booting */
#define	ALARMREQD	8192	/* has requested an alarm */	

