/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#include <stdio.h>

#include "hashlib.h"

static void	dumproot();
static void	dumptable();
static void	dumpflags();
static void	dumpbucket();

/*
 * dump hash table accounting info
 * if tab is 0 then dump all tables in hash_info.list
 * flags are HASH_* flags that specifiy optional dump info
 */

void
hashdump(fp, tab, flags)
register FILE*		fp;
register HASHTABLE*	tab;
int			flags;
{
	register HASHROOT*	root;

	fprintf(fp, "\nhash table information:\n\n");
	if (tab) dumproot(fp, tab->root, flags);
	else for (root = hash_info.list; root; root = root->next)
		dumproot(fp, root, flags);
	fflush(fp);
}

/*
 * dump hash table root info
 */

static void
dumproot(fp, root, flags)
register FILE*		fp;
register HASHROOT*	root;
register int		flags;
{
	register HASHTABLE*	tab;

	fprintf(fp, "    root\n");
	fprintf(fp, "        address:     0x%08lx\n", (unsigned long)root);
	fprintf(fp, "        flags:       ");
	dumpflags(fp, root->flags);
	if (root->namesize) fprintf(fp, "namesize=%d ", root->namesize);
	if (root->alloc) fprintf(fp, "alloc=0x%08lx ", (unsigned long)root->alloc);
	if (root->compare) fprintf(fp, "compare=0x%08lx ", (unsigned long)root->compare);
	if (root->free) fprintf(fp, "free=0x%08lx ", (unsigned long)root->free);
	if (root->hash) fprintf(fp, "hash=0x%08lx ", (unsigned long)root->hash);
	fprintf(fp, "\n");
	fprintf(fp, "        meanchain:   %d\n", root->meanchain);
	fprintf(fp, "        accesses:    %d\n", root->accesses);
	fprintf(fp, "        collisions:  %d\n", root->collisions);
	fprintf(fp, "\n");
	for (tab = root->references; tab; tab = tab->next)
		dumptable(fp, tab, flags);
}

/*
 * dump info on a single table
 */

static void
dumptable(fp, tab, flags)
register FILE*		fp;
register HASHTABLE*	tab;
register int		flags;
{
	HASHTABLE*	scope;
	int		level;

	fprintf(fp, "        name:        %s", tab->name ? tab->name : "*no name*");
	if (scope = tab->scope)
	{
		level = 1;
		while (scope = scope->scope) level++;
		fprintf(fp, " level %d scope on 0x%08lx", level, (unsigned long)tab->scope);
	}
	fprintf(fp, "\n");
	fprintf(fp, "        address:     0x%08lx\n", (unsigned long)tab);
	fprintf(fp, "        flags:       ");
	if (tab->frozen) fprintf(fp, "frozen=%d ", tab->frozen);
	dumpflags(fp, tab->flags);
	fprintf(fp, "\n");
	fprintf(fp, "        size:        %d\n", tab->size);
	fprintf(fp, "        buckets:     %d\n", tab->buckets);
	fprintf(fp, "\n");
	if ((flags | tab->flags) & HASH_BUCKET) dumpbucket(fp, tab, flags);
}

/*
 * dump HASH_* flags
 */

static void
dumpflags(fp, flags)
register FILE*	fp;
register int	flags;
{
	if (flags & HASH_ALLOCATE) fprintf(fp, "allocate ");
	if (flags & HASH_BUCKET) fprintf(fp, "bucket ");
	if (flags & HASH_FIXED) fprintf(fp, "fixed ");
	if (flags & HASH_HASHED) fprintf(fp, "hashed ");
	if (flags & HASH_RESIZE) fprintf(fp, "resize ");
	if (flags & HASH_SCANNING) fprintf(fp, "scanning ");
	if (flags & HASH_STATIC) fprintf(fp, "static ");
	if (flags & HASH_VALUE) fprintf(fp, "value ");
}

/*
 * dump hash table bucket info
 */

/*ARGSUSED*/
static void
dumpbucket(fp, tab, flags)
register FILE*		fp;
register HASHTABLE*	tab;
int			flags;
{
	register HASHBUCKET**	sp;
	register HASHBUCKET*	b;
	HASHBUCKET**		sx;
	int			n;
	unsigned char*		s;

	sx = tab->table + tab->size;
	for (sp = tab->table; sp < sx; sp++)
	{
		n = 0;
		for (b = *sp; b; b = b->next)
			if (!(b->hash & HASH_DELETED) && (!(tab->flags & HASH_VALUE) || b->value))
				n++;
		if (n)
		{
			fprintf(fp, "%5d %2d :", sp - tab->table, n);
			for (b = *sp; b; b = b->next)
				if (!(b->hash & HASH_DELETED) && (!(tab->flags & HASH_VALUE) || b->value))
				{
					if (n = tab->root->namesize)
					{
						fprintf(fp, " 0x");
						s = (unsigned char*)hashname(b);
						while (n-- > 0)
							fprintf(fp, "%02x", *s++);
					}
					else fprintf(fp, " %s", hashname(b));
					if (b->hash & HASH_FLAGS)
					{
						fprintf(fp, "|");
						if (b->hash & HASH_HIDES) fprintf(fp, "hides|");
						if (b->hash & HASH_HIDDEN) fprintf(fp, "hidden|");
						if (b->hash & HASH_KEEP) fprintf(fp, "keep|");
						if (b->hash & HASH_OPAQUED) fprintf(fp, "opaque|");
					}
					if (tab->flags & HASH_VALUE) fprintf(fp, "=0x%08lx", (long)b->value);
				}
			fprintf(fp, "\n");
		}
	}
	fprintf(fp, "\n");
}
