#include "String.h"
#include "list.h"
#define NL "\n"

struct pair {
	int	count;
	String	name;
		pair() : name() {}
		pair(String s) : name(s), count(1) {}
		pair(String s, int i) : name(s), count(i) {}
		pair(pair& pp) : name(pp.name), count(pp.count) {}
		~pair() {}
	bit	operator==(pair& pp) { return count == pp.count &&
			name == pp.name; }
	bit	operator!=(pair& pp) { return !(*this == pp); }
	pair&	operator=(pair& pp);
};

ostream&	operator<< ( ostream& oo, pair &p );

listdeclare(pair)

ostream&	myOut ( ostream& oo, pair_list &p );
