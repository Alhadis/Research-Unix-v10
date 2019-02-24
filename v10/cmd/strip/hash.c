#include "hash.h"

static struct st *curstp;
static int nchleft;

hashinit()
{
	register struct ht *htp;
	register struct st *stp;
	for (htp = hroot; htp; htp = htp->next)
		free(htp);
	for (stp = sroot; stp; stp = stp->next)
		free(stp);
	if ((hroot = Calloc(struct ht, 1)) == 0)
		fatal("cannot calloc struct ht", "");
	if ((sroot = Calloc(struct st, 1)) == 0)
		fatal("cannot calloc struct st", "");
	stcount = 1;
	curstp = sroot;
	nchleft = STRTABSIZE;
}

char *
savestr(str)	/* Place string into permanent storage. */
register char *str;
{
	char *strcpy();
	register int len; register struct st *stp = curstp;

	if ((len = strlen(str)+1) > nchleft) {
		int stsize = sizeof(struct st);
		if (len > STRTABSIZE) {
			stsize += len-STRTABSIZE;
			nchleft = len;
		} else
			nchleft = STRTABSIZE;
		stp->next = (struct st *)malloc(stsize);
		if ((curstp = stp = stp->next) == 0)
			fatal("malloc failed in savestr", "");
		stp->next = 0;
		stp->nused = 0;
		++stcount;
	}
	str = strcpy(&stp->strtab[stp->nused], str);
	stp->nused += len; nchleft -= len;
	return str;
}

char *
hash(str)	/* Lookup str in hash tables; if not found, make new entry. */
char *str;
{
	register char *cp, *hstr, **hstrp;
	register int h, i;
	struct ht *htp;
	int sh;

	if (*(cp = str) == '_')
		++cp;
	for (h = 0; *cp; )
		h = (h << 1) + *cp++;
	sh = (h & 077777) % HASHSIZE;
	cp = (char *)(*str != '_');

	/* Look through each table for name.  Use quadratic re-hash. */
	for (htp = hroot; htp; htp = htp->next) {
		for (h=sh, i=1; i<HASHSIZE; h += i, i += 2) {
			if (h >= HASHSIZE)
				h -= HASHSIZE;
			hstrp = &htp->ptable[h];
			if (hstr = *hstrp) {
				if (cp && *hstr == '_')
					++hstr;
				if (strcmp(hstr, str))
					continue;
				return hstr;
			} else {
				if (htp->nused > MAXHUSE)
					break;
				htp->nused++;
				return (*hstrp = savestr(str));
			}
		}
		if (htp->next == 0)
			htp->next = Calloc(struct ht, 1);
	}
	fatal("calloc failed in hash", "");
}

prthash()
{
	register struct ht *htp;
	register struct st *stp;
	register ntot = 0;
	for (htp = hroot; htp; htp = htp->next) {
		printf("%s %d", (htp == hroot ? "hash usage:" : " +"),
		    htp->nused);
		ntot += htp->nused;
	}
	printf(" = %d\n", ntot);
	ntot = 0;
	for (stp = sroot; stp; stp = stp->next) {
		printf("%s %d", (stp == sroot ? "string usage:" : " +"),
		    stp->nused);
		ntot += stp->nused;
	}
	printf(" = %d\n", ntot);
}
