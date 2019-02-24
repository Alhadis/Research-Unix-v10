/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include "hashlib.h"

/*
 * apply walker to each active bucket in the table
 */

int
hashwalk(tab, flags, walker)
HASHTABLE*		tab;
int			flags;
register HASHINT	walker;
{
	register HASHBUCKET*	b;
	register int		v;
	HASHPOSITION		pos;

	v = 0;
	hashscan(tab, flags, &pos);
	while (b = hashnext(&pos))
		if ((v = (*walker)(hashname(b), (tab->flags & HASH_VALUE) ? b->value : (char*)b)) < 0)
			break;
	hashdone(&pos);
	return(v);
}
