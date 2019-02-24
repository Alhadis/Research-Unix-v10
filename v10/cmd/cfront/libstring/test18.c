#include "String.h"

struct Foo {
		Foo() {}
		Foo(Foo& x) { *this = x; }
		~Foo() {}
	Foo&	operator=(Foo& x) { s[0] = x.s[0]; s[1] = x.s[1]; return *this; }
	String	s[2];
};

Foo
zot(Foo x)
{
	x.s[0] += "zot";
	x.s[1] += "zotzot";
	return x;
}

ostream&
operator<<(ostream& o, Foo& x)
{
	return o << "Foo::s[*]: " << x.s[0] << " " << x.s[1];
}

main()
{
	Foo	x;
	x.s[0] = "hello";
	x.s[1] = "hello";
	cout << zot(x) << "\n";
}
