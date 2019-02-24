/* unique word count */
#include "pair_list.h"

listoutimplement(pair)

main()
{
	String	s;
	pair_list	myList;
	/* input */
	while (cin >> s) {
		pair	p(s, 1);
		myList.put(p);
	}
	/* (bubble) sort */
	int swapFlag;
	if ( myList )
		do {
			swapFlag = FALSE;
			pair_list_iterator	it(myList);
			pair	x;
			pair	*y;
			while ( it.nextX(x) && it.peekX(y) ) {
				if ( x.name > y->name ) {
					it.remove();
					it.next();
					it.r_insert(x);
					swapFlag = TRUE;
				} else if ( x.name == y->name ) {
					y->count += x.count;
					it.remove();	// remove x
				}
			}
		} while ( swapFlag );
	/* print result */
	myOut(cout, myList);
}
