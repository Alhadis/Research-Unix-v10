struct S { char c[2]; };
struct S f( s, i )
struct S s; {}
main()
{	struct S a;
	(void) f( f(a, 0), 1 );
}
