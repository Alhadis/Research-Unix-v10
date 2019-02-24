#include "pair_list.h"

listimplement(pair)

pair&
pair::operator=(pair& pp)
{
	name = pp.name;
	count = pp.count;
	return *this;
}

ostream&
operator<< ( ostream& oo, pair &p )
{
	oo << "(" << p.name << ", " << p.count << ")";
	return  oo;
}

ostream&
myOut ( ostream& oo, pair_list &p )
{
	pair_list_iterator	pi(p);
	pair	aPair;
	while ( pi.nextX(aPair) )
		oo << aPair << NL;
	return  oo;
}
