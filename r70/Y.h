typedef unsigned char byte;

typedef struct Point
{
	short x, y;
} Point;
#define	Pt(x,y)	((((unsigned short)(y))<<16)|((unsigned short)(x)))
extern Point add(), sub(), midpt();

typedef struct Rectangle
{
	Point min, max;
} Rectangle;
#define	Rect(a,b,c,d)	Pt(a,b), Pt(c,d)
#define	Rpt(a,b)	a, b
extern Rectangle inset(), raddp(), rsubp(), rcanon();

typedef struct Color
{
	byte r, g, b, a;
} Color;
#define	Col(r,g,b,a)	((((byte)(a))<<24)|(((byte)(b))<<16)|(((byte)(g))<<8)|((byte)(r)))

typedef struct Font
{
	char name[32];
	short index;
	byte sizex, sizey;
} Font;

typedef struct Gcontext
{
	Color c;
	Font *f;
	short mode;
	short curson;
	Point p;
	Rectangle cdest;	/* rectangle containing the cursor on screen */
	Rectangle csrc;		/* rectangle source for cursor */
	Point coffset;		/* offset (rel to csrc.min) of focus */
	Color cmap[256];
} Gcontext;
extern Gcontext gc[];
#define		GC_STORE	0
#define		GC_CLR		1
#define		GC_SET		2
#define		GC_OR		3
#define		GC_XOR		4
#define		GC_AND		5
/* separate flags */
#define		GC_CURSORON	1
#define		GC_CRECT	2

#define	NMBUF		65536

typedef struct Window
{
	char name[32];
	short fd;
	short id;
	short bits;
	Rectangle r;
	char *bp;
	char buf[NMBUF];
} Window;
extern Window *newwindow();

extern void *ymalloc();
extern void arc(), sarc(), bitblt(), dbox(), fbox(), circle(), getcmap();
extern void setcolor(), offcursor(), setcursor(), defcursor(), flush(), line();
extern void lineto(), pan(), wrpix(), dpolygon(), fpolygon(), reset(), sync();
extern void zoom(), setmode(), overlay(), setpoint(), setcmap();
extern int getmode();
extern Point getpoint();

#define	NWINDOWS	16
