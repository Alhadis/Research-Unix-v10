// String test program.

#include "String.h"

String
doubleCat(String a, String b)
{
	a.put(b);
	return a + a;
}

String
substr ( String x, int start, int length )
{
	return x(start, length);
}

main(int argc, char **argv)
{
	String	s = "Hello, ";
	cout << doubleCat ( s, "world.\n" );
	while ( argc-- ) {
		String	x(*argv++);
		for ( int i = 0; i <= length(x); i++ ) {
			cout << substr(x, 0, i) << "\t" <<
			    substr(x, i, length(x) - i) << "\t";
			// split the expression to be kind to ccom
			cout << substr(x, 0, length(x) - i) << "\t" <<
			    x(length(x) - i, i) <<  "\n";
		}
		cout << "\n";
	}
}
