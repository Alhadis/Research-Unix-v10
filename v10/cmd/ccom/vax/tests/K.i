main()
{	register struct { int a:3, b:3;} *x;
	register int i;
	(*(--x)).b = 12;
	(*(--x)).b += 1;
	x[i++].b += 1;
}
