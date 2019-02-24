/* sort standard input and print unique Strings */

#include "String.h"
#include "list.h"

listdeclare(String)

main()
{
	String	s;
	String_list	myList;
	/* input */
	while (cin >> s)
		myList.put(s);
	/* (bubble) sort */
	int	swapFlag;
	int	loopCount = 0, swapCount = 0;
	if ( myList )
		do {
			swapFlag = FALSE;
			String_list_iterator	it(myList);
			String	x;
			String	y;
			while ( it.nextX(x) && it.peekX(y) ) {
				loopCount++;
				if ( x > y ) {
					it.replace(y);
					it.r_replace(x);
					swapFlag = TRUE;
					swapCount++;
				} else if ( x == y )
					it.remove();
			}
		} while ( swapFlag );
	/* print result */
	cout << myList;
	cout << "\nloopCount = " << loopCount << ", swapCount = " << swapCount << "\n";
}
