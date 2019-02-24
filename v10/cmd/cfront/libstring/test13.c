/* sort standard input and print unique word count */
#include "pair.h"
#include "list.h"

listdeclare(pairST)
listoutimplement(pairST)

pairST_list	sort(pairST_list);

main()
{
	String	s;
	pairST_list	myList;
	/* input */
	while (cin >> s)
		myList.put(new pair(s,1));
// cout << myList << "\n";
	myList = sort(myList);
	/* print result */
	cout << myList << "\n";
}

pairST_list
sort(pairST_list myList)
{
	/* (bubble) sort */
	int swapFlag;
	if ( myList )
		do {
			swapFlag = FALSE;
			pairST_list_iterator	it(myList);
			pair	*x;
			pair	*y;
			while ( it.nextX(x) && it.peekX(y) ) {
// cout << "Compare " << x << " and " << y << "\n";
				if ( x->word > y->word ) {
// cout << "greater\n";
					it.remove();
					it.next();
					it.r_insert(x);
					swapFlag = TRUE;
				} else if ( x->word == y->word ) {
// cout << "equal\n";
					it.remove();
					y->count += x->count;
					delete x;
				}
			}
// cout << myList;
		} while ( swapFlag );
	return myList;
}


ostream&
operator<<(ostream& oo, pairST p)
{
	return oo << "\n" << p->word << "\t" << p->count;
}
