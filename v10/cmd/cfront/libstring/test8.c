// list test program.  Takes one arg and standard input.  Removes
// all copies of arg from input

#include "String.h"
#include "list.h"

listdeclare(String)

String_list
remove(String s, String_list ls)
{
	String_list ans;
	while ( ls ) {
		String	temp;
		ls.getX(temp);
		if ( s != temp )
			ans.put(temp);
	}
	return ans;
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
