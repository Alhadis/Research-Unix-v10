#include "String.h"
#include "list.h"

listdeclare(String)

int	bigger(String&, String&);

main()
{
	String	s;
	String_list	myList;
	/* input */
	while (cin >> s)
		myList.put(s);
	myList.sort(bigger);
	/* print result */
	cout << myList << "\n";
}

int
bigger(String& a, String& b)
{
	return a < b;
}

listimplement(String)
listoutimplement(String)
