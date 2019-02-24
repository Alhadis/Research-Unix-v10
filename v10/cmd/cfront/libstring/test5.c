// list test program.  Takes one argument, reads standard input.
// removes copies of argument from input

#include "String.h"
#include "list.h"

listdeclare(String)

String_list
remove(String s, String_list ls)
{
	String	temp;
	if ( ls.getX(temp) ) {
		if ( s == temp )
			return remove(s, ls);
		else return remove(s,ls).unget(temp);
	} else return ls;
}


main(int argc, char **argv)
{
	String_list	slist;
	String	temp;
	if ( argc != 2 ) {
		cout << "must supply an arg\n";
		exit(1);
	}
	String	arg(argv[1]);
	while ( cin >> temp)
		slist.put(temp);
	cout << remove(arg, slist) << "\n";
}
