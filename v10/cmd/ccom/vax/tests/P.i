typedef struct { int x, y;} Point;
extern Point map(), a, b, c;
main ()
{	register Point *pt, *ppt;
	Point x, y, z;
	x = y = z;
	*pt++ = map (*ppt++);
	a = b = c;
	(void) map(a = b = c);
	if((a = x = map()).y)
		;
	if((b = a = map()).y)
		;
}
struct a {long p, q; char s;};
struct a f();
x0x()
{	struct a v, x, y;
	if((v=f()).s);
	v = x = y;
	return(f().s);
}
a22(){
	double d;
	int i, j;
	short x, y;
	x = x/y;
	x /= y;
	i /= d;
	j = i / d;
	j = i * d; i *= d;
}
