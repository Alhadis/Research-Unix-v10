/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library
 */

#if !lint
static char	id[] = "\n@(#)hash (AT&T Bell Laboratories) 11/11/90\0\n";
#endif

#include "hashlib.h"

#if !__STDC__

#include <varargs.h>

#endif

struct hashinfo	_hash_info_;

/*
 * create a new hash table
 */

#if __STDC__

HASHTABLE*
hashalloc(HASHTABLE* ref, ...)

#else

HASHTABLE*
hashalloc(va_alist)
va_dcl

#endif

{
	va_list			ap;
#if !__STDC__
	register HASHTABLE*	ref;
#endif

#if __STDC__
	va_start(ap, ref);
#else
	va_start(ap);
	ref = va_arg(ap, HASHTABLE*);
#endif
	ref = hashvalloc(ref, ap);
	va_end(ap);
	return(ref);
}

/*
 * varargs interface to hashalloc()
 */

HASHTABLE*
hashvalloc(ref, ap)
register HASHTABLE*	ref;
va_list			ap;
{
	register HASHTABLE*	tab;
	int			internal;

	if (!(tab = (HASHTABLE*)calloc(1, sizeof(HASHTABLE)))) goto bad;
	if (ref)
	{
		tab->flags = ref->flags & ~HASH_RESET;
		tab->root = ref->root;
		internal = HASH_INTERNAL;
	}
	else
	{
		if (!(tab->root = (HASHROOT*)calloc(1, sizeof(HASHROOT)))) goto bad;
		tab->root->meanchain = HASHMEANCHAIN;
		internal = 0;
	}
	tab->size = HASHMINSIZE;
	for (;;) switch (va_arg(ap, int)) 
	{
	case HASH_alloc:
		if (ref) goto bad;
		tab->root->alloc = va_arg(ap, HASHPTR);
		break;
	case HASH_clear:
		tab->flags &= ~(va_arg(ap, int) & ~internal);
		break;
	case HASH_compare:
		if (ref) goto bad;
		tab->root->compare = va_arg(ap, HASHINT);
		break;
	case HASH_free:
		if (ref) goto bad;
		tab->root->free = va_arg(ap, HASHINT);
		break;
	case HASH_hash:
		if (ref) goto bad;
		tab->root->hash = va_arg(ap, HASHFUN);
		break;
	case HASH_meanchain:
		if (ref) goto bad;
		tab->root->meanchain = va_arg(ap, int);
		break;
	case HASH_name:
		tab->name = va_arg(ap, char*);
		break;
	case HASH_namesize:
		if (ref) goto bad;
		tab->root->namesize = va_arg(ap, int);
		break;
	case HASH_set:
		tab->flags |= (va_arg(ap, int) & ~internal);
		break;
	case HASH_size:
		tab->size = va_arg(ap, int);
		if (tab->size & (tab->size - 1)) tab->flags |= HASH_FIXED;
		break;
	case HASH_table:
		tab->table = va_arg(ap, HASHBUCKET**);
		tab->flags |= HASH_STATIC;
		break;
	case 0:
		if (tab->flags & HASH_SCOPE)
		{
			if (!(tab->scope = ref)) goto bad;
			ref->frozen++;
		}
		if (!tab->table && !(tab->table = (HASHBUCKET**)calloc(1, sizeof(HASHBUCKET*) * tab->size))) goto bad;
		if (!ref)
		{
			tab->root->flags = tab->flags & HASH_INTERNAL;
			tab->root->next = hash_info.list;
			hash_info.list = tab->root;
		}
		tab->next = tab->root->references;
		tab->root->references = tab;
		return(tab);
	default:
		goto bad;
	}
 bad:
	(void)hashfree(tab);
	return(0);
}
