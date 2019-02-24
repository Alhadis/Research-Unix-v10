/*
 *  Model 5620 DMD
 */
#ifndef JERQ_H
#define JERQ_H
#include <sys/2681.h>
#line 31416

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

class Rectangle;

class Point {
public:
	short x,y;
	Point(){}
	Point(int u, int v)	{x=u; y=v;}
	Point operator-()		{return Point(-x,-y);}
	Point operator+(Point p)	{return Point(x+p.x,y+p.y);}
	Point operator-(Point p)	{return Point(x-p.x,y-p.y);}
	Point operator*(int i)	{return Point(x*i,y*i);}
	Point operator/(int i)	{return Point(x/i,y/i);}
	Point operator%(int i)	{return Point(x%i,y%i);}
	Point operator&(int i)	{return Point(x&i,y&i);}
	Point operator+=(Point p)	{return Point(x+=p.x,y+=p.y);}
	Point operator-=(Point p)	{return Point(x-=p.x,y-=p.y);}
	int operator==(Point p)	{return x==p.x && y==p.y;}
	int operator!=(Point p)	{return x!=p.x || y!=p.y;}
	int operator>=(Point p)	{return x>=p.x && y>=p.y;}
	int operator<=(Point p)	{return x<=p.x && y<=p.y;}
	int operator>(Point p)	{return x>p.x && y>p.y;}
	int operator<(Point p)	{return x<p.x && y<p.y;}
	inline int operator<(Rectangle);
	inline int operator<=(Rectangle);
	inline int operator>(Rectangle);
};

class Rectangle {
public:
	Point o,c;	/* origin, corner */
	Rectangle(){}
	Rectangle(Point p, Point q)	{o=p; c=q;}
	Rectangle(int x, int y, int u, int v)	{o.x=x; o.y=y; c.x=u; c.y=v;}
	Rectangle operator+(Point p)	{return Rectangle(o+p,c+p);}
	Rectangle operator-(Point p)	{return Rectangle(o-p,c-p);}
	Rectangle translate(Point p)	{return Rectangle(p,c+(p-o));}
	int operator<(Rectangle);
	int operator<=(Rectangle);
	Point center()		{return (o+c)/2;}
	Rectangle mbb(Point);		/* assumes a sorted Rectangle */
	Rectangle mbb(Rectangle);	/* ditto for argument */
};
inline int Point.operator<(Rectangle r) {return *this>r.o && *this<r.c;}
inline int Point.operator<=(Rectangle r) {return *this>=r.o && *this<=r.c;}
inline int Point.operator>(Rectangle r) {return !(*this<=r);}
inline int Rectangle.operator<(Rectangle r)	{return o<r && c<r;}
inline int Rectangle.operator<=(Rectangle r)	{return o<=r && c<=r;}

class Bitmap {
public:
	Word	*base;		/* Pointer to start of data */
	unsigned width;		/* width in 32 bit words of total data area */
	Rectangle rect;		/* Rectangle in data area, local coords */
	char	*_null;		/* unused, always zero */
	Bitmap(Word *b, int w, Rectangle r)	{base=b; width=w; rect=r;}
	Bitmap(Rectangle r)	{rect = r;}
};

class Menu {
public:
	char	**item;			/* string array, ending with 0 */
	char	*(*generator)(int);	/* used if item == 0 */
	short	prevhit;		/* private to menuhit() */
	short	prevtop;		/* private to menuhit() */
};

class Texture32 {
public:
	short bits[64];
};

class Texture {
public:
	short	bits[16];
};

class Layer;
class Font;

extern struct Mouse {
	Mouse()	{}
	Point	xy;
	short	buttons;
} mouse;

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

#define Pt(x,y)		Point(x,y)
#define Rect(x,y,u,v)	Rectangle(x,y,u,v)
#define Rpt(p,q)	Rectangle(p,q)
#define	muldiv(a,b,c)	((long)((a)*((long)b)/(c)))

extern Word topbits[fetchw_: expected an ack back from wild!fm_log: Error 0
], botbits[];	/* now full 32 bit words */
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
#define	NULL	((char *)0)
#define	KBD	1
#define	SEND	2
#define	MOUSE	4
#define	RCV	8
#define	CPU	16
#define ALARM	32

#define ringbell()	DUART->b_data=0x08,nap(3),DUART->b_data=0

#ifdef	MUX
#i