// list test program.  Sorts standard input, prints with counts
#include "pair.h"
#include "list.h"

plistdeclare(pair)
plistimplement(pair)
plistoutimplement(pair)

pairP_list	sort(pairP_list);

main()
{
	String	s;
	pairP_list	myList;
	/* input */
	while (cin >> s)
		myList.put(new pair(s,1));
	myList = sort(myList);
	/* print result */
	operator<<(cout,myList) << "\n"; // work around iostream bug
	pair	*p;
	while (myList.getX(p))
		delete p;
}

pairP_list
sort(pairP_list aList)	// straight insertion
{
	pairP_list_iterator	unsorted(aList);
	if ( unsorted.next() ) {
		pair	*p;
		while ( unsorted.r_removeX(p) ) {
			pairP_list_iterator	sorted = unsorted;
			pair	**q;  // pointer into sorted part
			while ( sorted.r_nextX(q) && (*q)->word > p->word )
				;
			if ( (*q)->word < p->word )
				sorted.next();   // back up
			else if ( (*q)->word == p->word ) {
				(*q)->count += p->count;
				delete p;
				continue;
			}
			sorted.r_insert(p);
		}
	}
	return aList;
}

ostream&
operator<<(ostream& oo, pair *p)
{
	return oo << "(" << p->word << ", " << p->count << ")\n";
}
