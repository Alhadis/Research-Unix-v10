/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include "hashlib.h"

/*
 * free (remove) a hash table
 * can be called for partially constructed tables
 * scope covered table pointer is returned
 * root info freed when last reference freed
 */

HASHTABLE*
hashfree(tab)
register HASHTABLE*	tab;
{
	register HASHBUCKET**	sp;
	register HASHBUCKET*	b;
	register HASHBUCKET*	p;
	HASHBUCKET**		sx;
	HASHROOT*		rp;
	HASHTABLE*		tp;
	HASHINT			freevalue;
	HASHINT			freebucket;

	if (!tab) return(0);
	if (tab->table)
	{
		freebucket = (HASHINT)free;
		freevalue = 0;
		if (tab->root->free)
		{
			if (tab->root->flags & HASH_BUCKET) freebucket = tab->root->free;
			else freevalue = tab->root->free;
		}
		sx = &tab->table[tab->size];
		sp = &tab->table[0];
		while (sp < sx)
		{
			b = *sp++;
			while (b)
			{
				if (freevalue && b->value) (void)(*freevalue)(b->value);
				p = b;
				b = b->next;
				if (!(p->hash & HASH_KEEP)) (*freebucket)(p);
			}
		}
		if ((tab->flags & (HASH_RESIZE|HASH_STATIC)) != HASH_STATIC) free(tab->table);
	}
	if (tab->root)
	{
		/*
		 * remove from the table lists
		 */

		if ((tp = tab->root->references) != tab)
		{
			for (; tp; tp = tp->next)
				if (tp->next == tab)
				{
					tp->next = tab->next;
					break;
				}
		}
		else if (!(tab->root->references = tp->next))
		{
			if ((rp = hash_info.list) != tab->root)
			{
				for (; rp; rp = rp->next)
					if (rp->next == tab->root)
					{
						rp->next = tab->root->next;
						break;
					}
			}
			else hash_info.list = rp->next;
		}
		if (!(tab->root->references)) free(tab->root);
	}
	if (tp = tab->scope) tp->frozen--;
	free(tab);
	return(tp);
}
