/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include "hashlib.h"

/*
 * hash table sequential scan
 *
 *	HASHPOSITION	pos;
 *	HASHBUCKET*	b;
 *	hashscan(tab, flags, &pos);
 *	while (b = hashnext(&pos)) ...;
 *	hashdone(&pos);
 */

/*
 * initialize pos for scan on table
 */

void
hashscan(tab, flags, pos)
register HASHTABLE*	tab;
register int		flags;
register HASHPOSITION*	pos;
{
	static HASHBUCKET	empty;

	pos->tab = pos->top = tab;
	pos->bucket = &empty;
	pos->slot = tab->table - 1;
	pos->limit = tab->table + tab->size;
	if (tab->scope && !(flags & HASH_NOSCOPE))
	{
		pos->flags = HASH_SCOPE;
		do
		{
			register HASHBUCKET*	b;

			if (tab->flags & HASH_SCANNING)
			{
				register HASHBUCKET**	sp = tab->table;
				register HASHBUCKET**	sx = tab->table + tab->size;

				while (sp < sx)
					for (b = *sp++; b; b = b->next)
						b->hash &= ~HASH_HIDDEN;
			}
		} while (tab = tab->scope);
		tab = pos->tab;
	}
	else pos->flags = 0;
	tab->flags |= HASH_SCANNING;
	tab->frozen++;
}

/*
 * return next scan element
 */

HASHBUCKET*
hashnext(pos)
register HASHPOSITION*	pos;
{
	register HASHBUCKET*	b;

	b = pos->bucket;
	for (;;)
	{
		if (!(b = b->next))
		{
			do
			{
				if (++pos->slot >= pos->limit)
				{
					pos->tab->flags &= ~HASH_SCANNING;
					pos->tab->frozen--;
					if (!pos->flags || !pos->tab->scope) return(0);
					pos->tab = pos->tab->scope;
					pos->limit = (pos->slot = pos->tab->table) + pos->tab->size;
					pos->tab->flags |= HASH_SCANNING;
					pos->tab->frozen++;
				}
			} while (!(b = *pos->slot));
		}
		if (!(b->hash & HASH_DELETED) && (!(pos->tab->flags & HASH_VALUE) || b->value) && (!pos->flags || !(b->hash & (HASH_HIDDEN|HASH_HIDES)))) break;
		if (b->hash & HASH_HIDES)
		{
			register HASHBUCKET*	h = (HASHBUCKET*)b->name;

			if (!(h->hash & HASH_HIDDEN))
			{
				h->hash |= HASH_HIDDEN;
				if (!(b->hash & HASH_DELETED)) break;
			}
		}
		else b->hash &= ~HASH_HIDDEN;
	}
	return(hash_info.last = pos->bucket = b);
}

/*
 * terminate scan
 */

void
hashdone(pos)
register HASHPOSITION*	pos;
{
	if (pos->tab->flags & HASH_SCANNING)
	{
		pos->tab->flags &= ~HASH_SCANNING;
		pos->tab->frozen--;
	}
}
