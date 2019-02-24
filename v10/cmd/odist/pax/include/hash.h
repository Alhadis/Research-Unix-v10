/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * hash table library interface definitions
 */

#ifndef HASH_ALLOCATE

#define hash_info		_hash_info_
#define vhashalloc		hashvalloc

#ifndef VOID
#define VOID	char
#endif

#define HASH_ALLOCATE	(1<<0)		/* allocate new key names	*/
#define HASH_FIXED	(1<<1)		/* fixed bucket/table size	*/
#define HASH_HASHED	(1<<6)		/* key names already hashed	*/
#define HASH_RESIZE	(1<<2)		/* table has been resized	*/
#define HASH_SCANNING	(1<<3)		/* currently scanning scope	*/
#define HASH_SCOPE	(1<<4)		/* push scope / create in bot	*/
#define HASH_STATIC	(1<<5)		/* static table allocation	*/

#define HASH_CREATE	(1<<8)		/* create bucket if not found	*/
#define HASH_DELETE	(1<<9)		/* delete bucket if found	*/
#define HASH_LOOKUP	0		/* default op			*/

#define HASH_BUCKET	(1<<11)		/* name is installed bucket	*/
#define HASH_INSTALL	(1<<12)		/* install allocated bucket	*/
#define HASH_NOSCOPE	(1<<13)		/* top scope only		*/
#define HASH_OPAQUE	(1<<14)		/* opaque bucket		*/
#define HASH_VALUE	(1<<15)		/* value bucket field used	*/

#define HASH_GET	(HASH_LOOKUP|HASH_VALUE)
#define HASH_NEW	(HASH_CREATE|HASH_FIXED)
#define HASH_PUT	(HASH_CREATE|HASH_VALUE)

#define HASH_DELETED	(1<<(8*sizeof(int)-1))	/* deleted placeholder	*/
#define HASH_KEEP	(1<<(8*sizeof(int)-2))	/* no free on bucket	*/
#define HASH_HIDDEN	(1<<(8*sizeof(int)-3))	/* hidden by scope	*/
#define HASH_HIDES	(1<<(8*sizeof(int)-4))	/* hides lower scope	*/
#define HASH_OPAQUED	(1<<(8*sizeof(int)-5))	/* opaqued placeholder	*/

#define HASH_RESET	(HASH_RESIZE|HASH_SCOPE|HASH_STATIC)
#define HASH_INTERNAL	(HASH_BUCKET|HASH_RESIZE|HASH_SCANNING|HASH_STATIC)
#define HASH_FLAGS	(HASH_DELETED|HASH_HIDDEN|HASH_HIDES|HASH_KEEP|HASH_OPAQUED)

#define HASH_alloc		1
#define HASH_clear		2
#define HASH_compare		3
#define HASH_free		4
#define HASH_hash		5
#define HASH_meanchain		6
#define HASH_name		7
#define HASH_namesize		8
#define HASH_set		9
#define HASH_size		10
#define HASH_table		11

#include <hashpart.h>

#define hashclear(t,f)		((t)->flags &= ~((f) & ~HASH_INTERNAL))
#define hashdel(t,n)		hashlook(t, (char*)(n), HASH_DELETE, (char*)0)
#define hashget(t,n)		hashlook(t, (char*)(n), HASH_LOOKUP|HASH_VALUE, (char*)0)
#define hashlast(t)		(hash_info.last)
#define hashname(b)		((((b)->hash&HASH_HIDES)?((HASHBUCKET*)((b)->name)):(b))->name)
#define hashput(t,n,v)		(char*)hashlook(t, (char*)(n), HASH_CREATE|HASH_VALUE, (char*)(v))
#define hashscope(t)		((t)->scope)
#define hashset(t,f)		((t)->flags |= ((f) & ~HASH_INTERNAL))

#define Hashbin_t		HASHBUCKET
#define Hashhdr_t 		HASHHEADER
#define Hashpos_t 		HASHPOSITION
#define Hashtab_t		HASHTABLE

typedef struct hashbucket	HASHBUCKET;
typedef struct hashheader	HASHHEADER;
typedef struct hashposition	HASHPOSITION;
typedef struct hashroot		HASHROOT;
typedef struct hashtable	HASHTABLE;
typedef unsigned int		(*HASHFUN)();
typedef int			(*HASHINT)();
typedef char*			(*HASHPTR)();

/*
 * the #define's avoid union tags
 */

#define HASH_HEADER			/* common bucket header		*/ \
	HASHBUCKET*	next;		/* next in collision chain	*/ \
	unsigned int	hash;		/* hash flags and value		*/ \
	char*		name		/* key name			*/

#define HASH_DEFAULT			/* HASH_VALUE bucket elements	*/ \
	char*		value		/* key value			*/

struct hashheader			/* bucket header		*/
{
	HASH_HEADER;
};

struct hashbucket			/* prototype bucket		*/
{
	HASH_HEADER;
	HASH_DEFAULT;
};

struct hashposition			/* hash scan bucket position	*/
{
	HASHTABLE*	tab;		/* table pointer		*/
	HASHTABLE*	top;		/* top scope table pointer	*/
	int		flags;		/* scan flags			*/
	HASHBUCKET*	bucket;		/* bucket			*/
	HASHBUCKET**	slot;		/* table slot			*/
	HASHBUCKET**	limit;		/* slot limit			*/
};

struct hashroot				/* root hash table information	*/
{
	int		flags;		/* flags: see HASH_[A-Z]*	*/
	int		namesize;	/* fixed name size: 0 => string	*/
	int		meanchain;	/* resize mean chain length	*/
	HASHFUN		hash;		/* name hash routine		*/
	HASHINT		compare;	/* name comparision routine	*/
	HASHPTR		alloc;		/* value allocation routine	*/
	HASHINT		free;		/* value free routine		*/
	int		accesses;	/* number of accesses		*/
	int		collisions;	/* number of collisions		*/
	HASHROOT*	next;		/* next in list	of all roots	*/
	HASHTABLE*	references;	/* referencing table list	*/
};

struct hashtable			/* hash table information	*/
{
	HASHROOT*	root;		/* root hash table information	*/
	HASHTABLE*	scope;		/* scope covered table		*/
	short		flags;		/* flags: see HASH_[A-Z]*	*/
	short		frozen;		/* table freeze nesting		*/
	HASHBUCKET**	table;		/* hash slot table		*/
	int		size;		/* table size			*/
	int		buckets;	/* active bucket count		*/
	char*		name;		/* table name			*/
	HASHTABLE*	next;		/* root reference list link	*/
};

struct hashinfo				/* library hash info		*/
{
	HASHBUCKET*	last;		/* most recent lookup bucket	*/
	HASHTABLE*	table;		/* most recent lookup table	*/
	HASHROOT*	list;		/* root table list		*/
};

#if __cplusplus
extern "C" {
#endif

extern struct hashinfo	hash_info;

#if __cplusplus
}
#endif

#if __STDC__ || __cplusplus || c_plusplus

#include <stdarg.h>
#include <stdio.h>

#if __cplusplus
extern "C" {
#endif
extern HASHTABLE*	hashalloc(HASHTABLE*, ...);
extern HASHTABLE*	hashvalloc(HASHTABLE*, va_list);
extern void		hashdone(HASHPOSITION*);
extern void		hashdump(FILE*, HASHTABLE*, int);
extern HASHTABLE*	hashfree(HASHTABLE*);
extern char*		hashlook(HASHTABLE*, const char*, int, const char*);
extern HASHBUCKET*	hashnext(HASHPOSITION*);
extern void		hashscan(HASHTABLE*, int, HASHPOSITION*);
extern void		hashsize(HASHTABLE*, int);
extern int		hashwalk(HASHTABLE*, int, HASHINT);

extern unsigned int	memhash(const char*, int);
extern unsigned long	memsum(const char*, int, unsigned long);
extern unsigned int	strhash(const char*);
extern unsigned long	strsum(const char*, unsigned long);
#if __cplusplus
}
#endif
#else
extern HASHTABLE*	hashalloc();
extern HASHTABLE*	hashvalloc();
extern void			hashdone();
extern void		hashdump();
extern HASHTABLE*	hashfree();
extern char*		hashlook();
extern HASHBUCKET*	hashnext();
extern void		hashscan();
extern void		hashsize();
extern int		hashwalk();

extern unsigned int	memhash();
extern unsigned long	memsum();
extern unsigned int	strhash();
extern unsigned long	strsum();
#endif

#endif
