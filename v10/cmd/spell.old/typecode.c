#include <stdio.h>
#include <ctype.h>
#include "code.h"

#define Tolower(c) (isupper(c)?tolower(c):(c))
typedef struct {
	char *codename;
	int	bits;
} Codeclass;
Codeclass codea[]  = {
	{"a",ADJ},
	{"adv",ADV},
	0
};
Codeclass codec[] =  {
	{ "comp", COMP},
	0
};
Codeclass coded[]  = {
	{ "d", DONT_TOUCH},
	0
};

Codeclass codee[] = {
	{ "ed",	ED},
	{ "er", ACTOR },
	0
};

Codeclass codei[] = {
	{"in", IN},
	{"ion", ION},
	0
};

Codeclass codem[] = {
	{"man", MAN},
	{"ms", MONO},
	0
};

Codeclass coden[]  = {
	{ "n", NOUN},
	{ "na",N_AFFIX},
	{ "nopref",NOPREF},
	0
};

Codeclass codep[] = {
	{ "pc" , PROP_COLLECT},
	0
};
Codeclass codes[] = {
	{ "s" , STOP},
	0
};

Codeclass codev[] =  {
	{ "v", VERB},
	{"va", V_AFFIX},
	{ "vi", V_IRREG },
	0
};

Codeclass codey[] = {
	{ "y", _Y },
	0
};

Codeclass codez[] =  {
0
};
Codeclass *codetab[] = {
	codea,
	codez,
	codec,
	coded,
	codee,
	codez,
	codez,
	codez,
	codei,
	codez,
	codez,
	codez,
	codem,
	coden,
	codez,
	codep,
	codez,
	codez,
	codes,
	codez,
	codez,
	codev,
	codez,
	codez,
	codey,
	codez,
};

int
typecode (str)
register char *str;
{
	register char *s,*sp,*sc;
	Codeclass *p;
	char c = 0;
	register flag = 0;

	if(!(s = str)) return(0);
	for( ; *str && c!='\n'; str = ++s) {
		if(!isalpha(*s)) continue;
		for( ; *s && *s!=',' && *s!='\n'; s++) continue;
		c = *s; *s = 0;
		for(p = codetab[*str-'a']; sp = p->codename; p++) {
			for(sc = str; *sc == *sp && *sc; sc++)sp++;
			 if(!*sc && !*sp) {
				flag |= p->bits;
				break;
			}
		}
		if(*sc || *sp) fprintf(stderr,
			"Unknown affix code \"%s\"\n", str);
	}
	return((flag)?flag:ALL);
}
		
