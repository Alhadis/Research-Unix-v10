/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library private definitions
 */

#include <limits.h>
#include <hash.h>

#define HASHMINSIZE	(1<<4)		/* min table slots (power of 2)	*/
#define HASHMEANCHAIN	2		/* def resize mean chain len	*/

#define HASHMOD(t,h)	(h &= (t->size - 1))
#define HASHVAL(x)	((x)&~HASH_FLAGS)

#define HASH(r,n,h)	if (r->hash) h = r->namesize ? (*r->hash)(n, r->namesize) : (*r->hash)(n);\
			else\
			{\
				register const char*	_hash_s1 = n;\
				h = 0;\
				if (r->namesize)\
				{\
					register const char*	_hash_s2 = _hash_s1 + r->namesize;\
					while (_hash_s1 < _hash_s2) HASHPART(h, *_hash_s1++);\
				}\
				else while (*_hash_s1) HASHPART(h, *_hash_s1++);\
			}

extern char*	calloc();
extern char*	malloc();
extern char*	memcpy();

extern int	strlen();

extern void	free();
