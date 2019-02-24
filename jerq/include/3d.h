#define	ONE	16384		/* guarantee no overflow in xformp */
typedef short fract;		/* fixed point scaled by ONE */
typedef fract matrix[4][4];
typedef short coord[4];
typedef unsigned short angle;
typedef struct point3{
	fract x, y, z, w;
}Hcoord;
typedef struct Lpoint3{
	long x, y, z, w;
}Hcoordl;
#define	PI	(angle)32768
#define	mkangle(x)	cvtangle[x]
extern angle cvtangle[];
fract isin(), icos();
long isqrt();
#define	XAXIS	0
#define	YAXIS	1
#define	ZAXIS	2
long dot();
Hcoord cross();
Hcoordl xformp();
extern Bitmap *viewport;
Hcoord hcoord();
