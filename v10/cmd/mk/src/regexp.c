#include	"mk.h"

Rule *patrule;

regerror(s)
	char *s;
{
	if(patrule)
		Fprint(2, "mk: %s:%d: regular expression error; %s\n",
			patrule->file, patrule->line, s);
	else
		Fprint(2, "mk: %s:%d: regular expression error; %s\n",
			infile, inline, s);
	Exit();
}

rsub(src, r, a, proto, dest)
	char *src, *dest, *proto;
	Rule *r;
	Arc *a;
{
	rdump("rsub", a->match);
	print("rsub(src=%d '%s' proto=%d '%s')\n", src, src, proto, proto);
	*dest = 0;
}

rdump(s, m)
	char *s;
	register regsubexp *m;
{
	register i;

	print("rdump: %s\n", s);
	for(i = 0; i < NREGEXP; i++, m++)
		print("\t%d: %d - %d '%s'\n", i, m->sp, m->ep, m->sp);
}
