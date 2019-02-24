#include "list.h"

ostream&
operator<<(ostream& oo, list(voidP)& ll)
{
	oo<<"(";
	if ( ll.length() ) {
		listiterator(voidP)	it(ll);
		voidP	t;
		it.nextX(t);
		oo<< (int)t;
		while (it.nextX(t))
			oo << " " << (int)t;
	} else
		oo<<"EMPTYLIST";
	oo<<")";
	return oo;
}
/*
ZOTZlistoutimplement(voidP)
*/
