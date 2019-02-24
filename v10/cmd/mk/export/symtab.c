#include	"mk.h"

#define	NHASH	4099
#define	HASHMUL	79L	/* this is a good value */
static Symtab *hash[NHASH];

syminit()
{
	register Symtab **s, *ss;

	for(s = hash; s < &hash[NHASH]; s++){
		for(ss = *s; ss; ss = ss->next)
			free((char *)ss);
		*s = 0;
	}
}

Symtab *
symlook(sym, space, install)
	char *sym;
	char *install;
{
	register long h;
	register char *p;
	register Symtab *s;

	for(p = sym, h = space; *p; h += *p++)
		h *= HASHMUL;
	if(h < 0)
		h = ~h;
	h %= NHASH;
	for(s = hash[h]; s; s = s->next)
		if((s->space == space) && (strcmp(s->name, sym) == 0))
			return(s);
	if(install == 0)
		return((Symtab *)0);
	s = (Symtab *)Malloc(sizeof(Symtab));
	s->space = space;
	s->name = sym;
	s->value = install;
	s->next = hash[h];
	hash[h] = s;
	return(s);
}

symdel(sym, space)
	char *sym;
{
	register long h;
	register char *p;
	register Symtab *s, *ls;

	for(p = sym, h = space; *p; h += *p++)
		h *= HASHMUL;
	if(h < 0)
		h = ~h;
	h %= NHASH;
	for(s = hash[h], ls = 0; s; ls = s, s = s->next)
		if((s->space == space) && (strcmp(s->name, sym) == 0)){
			if(ls)
				ls->next = s->next;
			else
				hash[h] = s->next;
			free((char *)s);
		}
}

symtraverse(space, fn)
	void (*fn)();
{
	register Symtab **s, *ss;

	for(s = hash; s < &hash[NHASH]; s++)
		for(ss = *s; ss; ss = ss->next)
			if(ss->space == space)
				(*fn)(ss);
}

symstat()
{
	register Symtab **s, *ss;
	register n;
	int l[1000];

	memset((char *)l, 0, sizeof(l));
	for(s = hash; s < &hash[NHASH]; s++){
		for(ss = *s, n = 0; ss; ss = ss->next)
			n++;
		l[n]++;
	}
	for(n = 0; n < 1000; n++)
		if(l[n]) Fprint(1, "%ld of length %d\n", l[n], n);
}
