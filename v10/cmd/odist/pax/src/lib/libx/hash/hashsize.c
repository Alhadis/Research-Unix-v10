/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include "hashlib.h"

/*
 * change table size and rehash
 * size must be a power of 2
 */

void
hashsize(tab, size)
register HASHTABLE*	tab;
int			size;
{
	register HASHBUCKET**	old_s;
	register HASHBUCKET**	new_s;
	register HASHBUCKET*	old_b;
	register HASHBUCKET*	new_b;
	HASHBUCKET**		old_sx;
	unsigned int		index;

	if (size > 0 && size != tab->size && !(size & (size - 1)))
	{
		if (!(new_s = (HASHBUCKET**)calloc(size, sizeof(HASHBUCKET*)))) tab->flags |= HASH_FIXED;
		else
		{
			old_sx = (old_s = tab->table) + tab->size;
			tab->size = size;
			while (old_s < old_sx)
			{
				old_b = *old_s++;
				while (old_b)
				{
					new_b = old_b;
					old_b = old_b->next;
					index = new_b->hash;
					HASHMOD(tab, index);
					new_b->next = new_s[index];
					new_s[index] = new_b;
				}
			}
			if ((tab->flags & (HASH_RESIZE|HASH_STATIC)) != HASH_STATIC) free(tab->table);
			tab->table = new_s;
			tab->flags |= HASH_RESIZE;
		}
	}
}
