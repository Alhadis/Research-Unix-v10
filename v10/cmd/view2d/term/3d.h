#define	ONE	16384		/* guarantee no overflow in xformp */
typedef short fract;		/* fixed point scaled by ONE */
typedef fract matrix[4][4];
typedef short coord[4];
typedef unsigned short angle;
class Hcoord
{
public:
	fract x, y, z, w;
	Hcoord(fract a, fract b, fract c, fract d) {x=a; y=b; z=c; w=d;}
};
struct Hcoordl{
	long x, y, z, w;
};
#define	PI	(angle)32768
#define	mkangle(x)	cvtangle[x]
extern angle cvtangle[];
fract isin(fract), icos(fract);
long isqrt(long);
#define	XAXIS	0
#define	YAXIS	1
#define	ZAXIS	2
long dot(Hcoord, Hcoord);
Hcoord cross(Hcoord, Hcoord);
Hcoordl xformp(Hcoord);
extern Hcoord unitize(Hcoord);
extern Bitmap *viewport;
void init3(Bitmap *, short, short);
void push3(), pop3(), rot3(short, short);
extern clip3scale;
