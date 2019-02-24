# include "mfile1.h"

lookup(name, s)	/* look up name: must agree with s w.r.t. STAG, SMOS and SHIDDEN */
register char *name; register s;
{
	register struct symtab *sp, *spstart;

# ifndef NODBG
	extern int ddebug;
	if (ddebug > 2) {
		printf("lookup(%s, %d), stwart=%d, instruct=%d\n",
			name, s, stwart, instruct);
	}
# endif

	/* compute initial hash index */
	sp = spstart = stab + ((long)name & 077777) % SYMTSZ;

	do {	 /* look for name */
		if (sp->stype == TNULL) {	/* empty slot */
			sp->sflags = s;		/* set STAG, SMOS if needed */
			sp->sname = name;
			sp->stype = UNDEF;	/* turn off all others */
			sp->sclass = SNULL;
			return (sp-stab);
		}
		if (sp->sname == name && (sp->sflags & (STAG|SMOS|SHIDDEN)) == s)
			return (sp-stab);
	} while ((++sp >= stab+SYMTSZ ? sp = stab : sp) != spstart);
	cerror("symbol table full");
}

#define HASHSIZE	1013
#define MAXHUSE		((3*HASHSIZE)/4)
#define STRTABSIZE	4096

struct ht {
	struct ht *next;
	int nused;
	char *ptable[HASHSIZE];
} hashroot;

char *
hash(str)	/* Lookup str in hash tables; if not found, make new entry. */
char *str;
{
	char *calloc(), *savestr();
	register char *cp, **hstrp, **htptable;
	register int h, i;
	register struct ht *htp;
	int sh;

	for (cp=str, h=0; *cp; )
		h = (h << 1) + *cp++;
	sh = (h & 077777) % HASHSIZE;

	/* Look through each table for name.  Use quadratic re-hash. */
	for (htp = &hashroot; htp; htp = htp->next) {
		htptable = htp->ptable;
		for (h=sh, i=1; i<HASHSIZE; h += i, i += 2) {
			if (h >= HASHSIZE)
				h -= HASHSIZE;
			hstrp = &htptable[h];
			if (*hstrp) {
				if (strcmp(*hstrp, str))
					continue;
				return *hstrp;
			} else {
				if (htp->nused > MAXHUSE)
					break;
				htp->nused++;
				return (*hstrp = savestr(str));
			}
		}
		if (htp->next == 0)
			htp->next = (struct ht *)calloc(1,sizeof(struct ht));
	}
	cerror("cannot allocate hash table");
}

char *
savestr(str)	/* Place string into permanent storage. */
register char *str;
{
	char *memcpy(), *malloc();
	static char *curstp; static int nchleft;
	register int len;

	if ((len = strlen(str)+1) > nchleft &&
	    (curstp = malloc(nchleft = len+STRTABSIZE)) == 0)
		cerror("cannot allocate string table");
	str = memcpy(curstp, str, len);
	curstp += len; nchleft -= len;
	return str;
}
