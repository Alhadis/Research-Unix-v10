// String test program.  exercise SubStrings

#include "String.h"

void
spitIt(String x)
{
	String	s = "<<x>>";
	for ( int i = 0; i <= length(x); i++ ) {
		for ( int j = 0; j <= length(x) - i; j++ ) {
			String	 y = x;
			y ( i, j ) = s;
			cout << y << "\n";
		}
	}
}

main(int argc, char **argv)
{
	while ( argc-- ) {
		String	x(*argv++);
		cout << x << "\n";
		spitIt ( x );
		cout << x << "\n";
	}
}
