/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include "hashlib.h"

/*
 * hash table lookup
 */

#if __STDC__ || __cplusplus
char*
hashlook(register HASHTABLE* tab, const char* name, int flags, const char* value)
#else
#ifndef const
#define const
#endif
char*
hashlook(tab, name, flags, value)
register HASHTABLE*	tab;
char*			name;
int			flags;
char*			value;
#endif
{
	register HASHBUCKET*	b;
	register unsigned int	n;
	HASHTABLE*		top;
	HASHBUCKET*		prev;
	unsigned int		i;

	static const char*	lastname;
	static unsigned int	lasthash;

	tab->root->accesses++;
	top = tab;
	if (name)
	{
		hash_info.table = tab;
		if (flags & (HASH_BUCKET|HASH_INSTALL))
		{
			hash_info.last = (HASHBUCKET*)name;
			name = hashname(hash_info.last);
		}
		else hash_info.last = 0;
		lastname = name;
		if (flags & HASH_BUCKET) n = hash_info.last->hash;
		else if (tab->flags & HASH_HASHED) n = ((unsigned int)name) >> 3;
		else HASH(tab->root, name, n);
		lasthash = i = HASHVAL(n);
		for (;;)
		{
			HASHMOD(tab, n);
			for (prev = 0, b = tab->table[n]; b; prev = b, b = b->next)
			{
				if (i == HASHVAL(b->hash) && ((b->hash & (HASH_DELETED|HASH_OPAQUED)) != HASH_DELETED || (flags & (HASH_CREATE|HASH_DELETE|HASH_INSTALL))))
				{
					if (!tab->root->compare)
					{
						register char*		s1 = hashname(b);
						register const char*	s2 = name;

						if (tab->root->namesize)
						{
							register char*	s3 = s1 + tab->root->namesize;

							while (*s1++ == *s2++)
								if (s1 >= s3) goto found;
						}
						else while (*s1 == *s2++)
							if (!*s1++) goto found;
					}
					else if (tab->root->namesize)
					{
						if (!(*tab->root->compare)(hashname(b), name, tab->root->namesize)) goto found;
					}
					else if (!(*tab->root->compare)(hashname(b), name)) goto found;
				}
				tab->root->collisions++;
			}
			if (!tab->scope || (flags & (HASH_CREATE|HASH_INSTALL|HASH_NOSCOPE)) == HASH_NOSCOPE) break;
			tab = tab->scope;
			n = i;
		}
	}
	else
	{
		tab = hash_info.table;
		name = lastname;
		prev = 0;
		n = i = lasthash;
		HASHMOD(tab, n);
		if (b = hash_info.last)
		{
			/*
			 * found the bucket
			 */
		
		found:
			if (prev && !(tab->flags & HASH_SCANNING))
			{
				/*
				 * migrate popular buckets to the front
				 */
		
				prev->next = b->next;
				b->next = tab->table[n];
				tab->table[n] = b;
			}
			switch (flags & (HASH_CREATE|HASH_DELETE|HASH_INSTALL))
			{
			case HASH_CREATE:
			case HASH_INSTALL:
				if (tab != top && !(flags & HASH_SCOPE)) break;
				if (flags & HASH_OPAQUE) b->hash |= HASH_OPAQUED;
				goto exists;

			case HASH_DELETE:
				value = 0;
				if (tab == top || (flags & HASH_SCOPE))
				{
					if (flags & HASH_OPAQUE) b->hash &= ~HASH_OPAQUED;
					else if (!(tab->root->flags & HASH_BUCKET))
					{
						if (tab->root->free && b->value)
						{
							(*tab->root->free)(b->value);
							b->value = 0;
						}
						else if (tab->flags & HASH_VALUE)
						{
							value = b->value;
							b->value = 0;
						}
					}
					if (tab->frozen || (b->hash & (HASH_KEEP|HASH_OPAQUED))) b->hash |= HASH_DELETED;
					else
					{
						tab->buckets--;
						tab->table[n] = b->next;
						if (tab->root->free && (tab->root->flags & HASH_BUCKET)) (*tab->root->free)(b);
						else free(b);
					}
				}
				return((char*)value);
			default:
				if (!(b->hash & HASH_DELETED)) goto exists;
				return(0);
			}
		}
	}
	if (!(flags & (HASH_CREATE|HASH_INSTALL))) return(0);

	/*
	 * create a new bucket
	 */

	if (tab == top) prev = 0;
	else
	{
		if (prev = b)
		{
			name = (b->hash & HASH_HIDES) ? b->name : (char*)b;
			i |= HASH_HIDES;
		}
		if (!(flags & HASH_SCOPE)) tab = top;
	}

	/*
	 * check for table expansion
	 */

	if (!tab->frozen && !(tab->flags & HASH_FIXED) && tab->buckets > tab->root->meanchain * tab->size) hashsize(tab, tab->size << 1);
	if (flags & HASH_INSTALL)
	{
		b = hash_info.last;
		i |= HASH_KEEP;
	}
	else
	{
		if (flags & HASH_VALUE)
		{
			tab->flags |= HASH_VALUE;
			n = sizeof(HASHBUCKET);
		}
		else if (!(flags & HASH_FIXED)) n = sizeof(HASHHEADER);
		else if ((n = (int)value) < sizeof(HASHHEADER)) n = sizeof(HASHHEADER);
		if (!prev && (tab->flags & HASH_ALLOCATE))
		{
			int	m = tab->root->namesize ? tab->root->namesize : strlen(name) + 1;
			if (!(b = (HASHBUCKET*)calloc(1, n + m))) return(0);
			b->name = (char*)b + n;
			(void)memcpy(b->name, name, m);
		}
		else
		{
			if (!(b = (HASHBUCKET*)calloc(1, n))) return(0);
			b->name = (char*)name;
		}
	}
	b->hash = n = i;
	HASHMOD(tab, n);
	b->next = tab->table[n];
	tab->table[n] = b;
	tab->buckets++;
	if (flags & HASH_OPAQUE)
	{
		b->hash |= HASH_DELETED|HASH_OPAQUED;
		return(0);
	}
 exists:

	/*
	 * finally got the bucket
	 */

	b->hash &= ~HASH_DELETED;
	hash_info.last = b;
	hash_info.table = tab;
	switch (flags & (HASH_CREATE|HASH_VALUE))
	{
	case HASH_CREATE|HASH_VALUE:
		if (tab->root->free && !(tab->root->flags & HASH_BUCKET) && b->value) (*tab->root->free)(b->value);
		if (value && tab->root->alloc) value = (*tab->root->alloc)(value);
		b->value = (char*)value;
		return((char*)hashname(b));
	case HASH_VALUE:
		return(b->value);
	default:
		return((char*)b);
	}
}
