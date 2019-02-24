struct S { char c[2]; };

struct S f();
main()
{
	static struct S X[4];
	register int i;
	(void) f(X[i]);
}

