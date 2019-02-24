#include	"mk.h"

mninlist(name, list, stem)
	register char *name, *stem;
	Word *list;
{
	for(; list; list = list->next){
		if(match(name, list->s, stem))
			return(1);
	}
	return(0);
}

static
isatomic(p)
	char *p;
{
	while(*p){
		if((*p == '.') || (*p == '/'))
			return(0);
		p++;
	}
	return(1);
}

match(name, template, stem)
	char *name, *template, *stem;
{
	register char *p, *q;
	char *b;
	char *ostem = stem;

	for(p = name, q = template; *p && *q && !PERCENT(*q);)
		if(*p++ != *q++) return(0);
	if(!PERCENT(*q))
		return(0);
	b = p;
	while(*p) p++;
	while(*q) q++;
	for(q--; !PERCENT(*q); q--)
		if(*--p != *q) return(0);
	while(b < p)
		*stem++ = *b++;
	*stem = 0;
	if(*q == '&')
		return(isatomic(ostem));
	return(1);
}

subst(stem, template, dest)
	register char *stem, *template, *dest;
{
	while(*template){
		if(PERCENT(*template)){
			template++;
			while(*stem)
				*dest++ = *stem++;
		} else
			*dest++ = *template++;
	}
	*dest = 0;
}
