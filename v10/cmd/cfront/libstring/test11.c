// list test program.  sort standard input
#include "String.h"
#include "list.h"

listdeclare(String)
listoutimplement(String)

String_list
sort(String_list in)	// quicksort
{
	String_list	low;
	String_list	high;
	String	middle;
	if (in.length() <= 1)
		return in;
	in.getX(middle);
	while(in) {
		String	temp;
		in.getX(temp);
		int	c;
		if ((c = temp.compare(middle)) < 0 )
			low.put(temp);
		else if ( c > 0 )
			high.put(temp);
	}
	if ( low.length() > high.length() )
		high.put(middle);
	else
		low.put(middle);
	return sort(low) + sort(high);
}

main()
{
	String	s;
	String_list	myList;
	/* input */
	while (cin >> s)
		myList.put(s);
	cout << sort(myList) << "\n";
}
