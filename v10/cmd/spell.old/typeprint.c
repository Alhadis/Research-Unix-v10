#include "code.h"
#include <stdio.h>

static found;
static FILE *stream;

typeprint(f,i)
FILE *f;
unsigned i;
{
	found = 0;
	stream = f;
	if(i & NOUN) pcomma("n");
	if(i & PROP_COLLECT) pcomma("pc");
	if(i & VERB) {
		if((i & VERB)== VERB) pcomma("v");
		else if((i & VERB) == (V_IRREG)) pcomma("vi");
		else if(i &ED) pcomma("ed");
	}
	if(i & ADJ) pcomma("a");
	if(i & COMP) {
		if(( i & COMP) == ACTOR) pcomma("er");
		else pcomma("comp");
	}
	if(i & DONT_TOUCH) pcomma("d");
	if(i & N_AFFIX) pcomma("na");
	if(i & ADV) pcomma("adv");
	if(i & ION) pcomma("ion");
	if(i & V_AFFIX) pcomma("va");
	if(i & MAN) pcomma("man");
	if(i & NOPREF) pcomma("nopref");
	if(i & MONO) pcomma("ms");
	if(i & IN) pcomma("in");
	if(i & _Y) pcomma("y");
	if(i & STOP) pcomma("s");
	return (i&~(ALL|STOP|DONT_TOUCH))==i ||
		(i&STOP) && (i&~STOP) ||
		(i&DONT_TOUCH) && (i&~(DONT_TOUCH|NOPREF|IN));
}

pcomma(s) 
char *s;
{
	if(found++)
		fprintf(stream,","); 
	fprintf(stream,s);
}
