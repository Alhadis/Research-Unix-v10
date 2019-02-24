// list test program.  Takes one argument, reads standard input.
// removes copies of argument from input

#include "String.h"
#include "list.h"

listdeclare(String)
listoutimplement(String)

String_list
remove(String s, String_list ls)
{
	String_list_iterator it(ls);
	String temp;
	while ( it.nextX(temp) ) {
		if ( temp == s ) {
			it.remove();
		}
	}
	return ls;
}


main(int argc, char **argv)
{
	String_list	s_list;
	String	temp;
	if ( argc != 2 ) {
		cout << "must supply an arg\n";
		exit(1);
	}
	String	arg(argv[1]);
	while ( cin >> temp)
		s_list.put(temp);
	cout << remove(arg, s_list) << "\n";
}
