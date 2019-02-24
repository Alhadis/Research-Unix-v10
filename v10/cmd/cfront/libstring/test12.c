// list test program.  Sorts standard input, prints with counts

#include "pair_list.h"

listoutimplement(pair)

pair_list
sort(pair_list aList)	// straight insertion
{
	pair_list_iterator	unsorted(aList);
	if ( unsorted.next() ) {
		pair	p;
		while ( unsorted.r_removeX(p) ) {
			pair_list_iterator	sorted = unsorted;
			pair	*q;  // pointer into sorted part
			while ( sorted.r_nextX(q) && q->name > p.name )
				;
			if ( q->name < p.name )
				sorted.next();   // back up
			else if ( q->name == p.name ) {
				q->count++;
				continue;
			}
			sorted.r_insert(p);
		}
	}
	return aList;
}
main()
{
	String	s;
	pair_list	myList;
	while (cin >> s)
		myList.put(pair(s));
	myOut(cout, sort(myList));
}
