foo(t1)
{	register int i;
	char j;
	enum {x} b;
	int t3;
	register *ar1;
	t3 = *(ar1++ + i) %= t1;
	t3 = *ar1++ *= t1;
	j = (int) foo >> 12;
	t3 = *ar1++ &= t1;
	t3 = *ar1++ <<= t1;
	t3 = *ar1++ >>= t1;
	f((short)b);
}
extern	struct	_iobuf {
	int	_cnt;
	unsigned char	*_ptr;
	unsigned char	*_base;
	short	_flag;
	char	_file;
} _iob[120];
a() {
	register *x;
	((int)(*((&_iob[1]))->_ptr++=(*x++ >> 8)));
}
int g[1];
b()
{	int i, j;
	register *a;
	i = *a++ >>= j;
	g[i] >>= 1;
}
struct  vstr    {
	unsigned int	v_type:6;
	unsigned int	v_amt:10;
};
struct	xx {
	struct	vstr	v[10];
} x;
maina()
{	int i;
	x.v[i].v_type = 1;
	a(x.v[i].v_type);
}
struct foo { short a, b; } x3; int c ; char w;

mainb() { sub((w=0, x3)); }
typedef struct Hinge
{
	unsigned fa:8, fb:8, ea:4, eb:4, reverse:1;
} Hinge;
Hinge hinges[10];
int nhinges;

mainc()
{	hinges[nhinges].fa = ggg();
}
short a3;
zx()
{	register int *b;
	register short *c;
	if(a3 & *b++);
	if(*b++ & a3);
	if(*++b & *c++);
	if(*c++ & *++b);
}
a33()
{	int i;
	int c;
	i = (c >>= 1);
}
int **noderepr();
lenitem()
{
	register n;
	return a(noderepr(n)[n/2]);
}
typedef struct {
	int x, y;
} coord;

struct thing {
	coord t_pos;
	int x;
};
rndmove(who)
struct thing *who;
{	static coord ret;
	ret = who->t_pos;
}
typedef struct {int x,y;} point;
point pt[10], lastpt, map();
axx()
{	int i;
	pt[i] = lastpt = map();
}
struct a{int x,y;};
maind()
{	static struct a b, c, *x, *y;
	b = c;
	*x = *y = c;
}
struct {
     int  d;
     int  x;
} s, *sp;

mainx() {
	int x;  float f;

     sp->x = x + f * x;
}
