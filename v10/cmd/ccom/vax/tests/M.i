struct S { unsigned char m, n;} Z;
int i;
struct S f( s )
struct S s;
{
	return f( i ? f(s) : Z );	/* movaw r0,r0 ! */
}
