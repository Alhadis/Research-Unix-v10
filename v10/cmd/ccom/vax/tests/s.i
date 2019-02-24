struct X { unsigned char a; unsigned char b;};
struct I { char	*t; long f; long o;};
struct C { int	 g;
	struct X x;
	struct I ***i;
}

main()
{
	register struct C *r;
	struct I c;
	struct I *f();

	*( r->i[r->x.a][r->x.b] = f((struct I*)0) ) = c;
}

