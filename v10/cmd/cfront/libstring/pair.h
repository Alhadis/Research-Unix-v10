#include "String.h"

struct pair {
	String	word;
	int	count;
	pair(String w, int c) : word(w), count(c) {}
	~pair() {}
};

ostream& operator<<(ostream&, pair*);

typedef pair	*pairST;
