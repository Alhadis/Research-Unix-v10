#include	"mk.h"

setvar(name, value)
	char *name;
	char *value;
{
	symlook(name, S_VAR, value)->value = value;
	symlook(name, S_MAKEVAR, "");
}

static char **nextv;

static void
vcopy(s)
	register Symtab *s;
{
	register char **p;
	extern char *myenv[];

	if(symlook(s->name, S_NOEXPORT, (char *)0))
		return;
	for(p = myenv; *p; p++)
		if(strcmp(*p, s->name) == 0) return;
	if(*nextv)
		free(*nextv);
	*nextv++ = envpr(s->name, s->value);
}

char **
vardump(p)
	char **p;
{
	nextv = p;
	symtraverse(S_VAR, vcopy);
	*nextv = 0;
	return(nextv);
}

static void
print1(s)
	register Symtab *s;
{
	Fprint(1, "\t%s='%s'\n", s->name, s->value);
}

dumpv(s)
	char *s;
{
	Fprint(1, "%s:\n", s);
	symtraverse(S_VAR, print1);
}

char *
envpr(name, value)
	register char *name;
	register char *value;
{
	char buf[BIGBLOCK];

	sprint(buf, "%s=", name);
	strcpy(strchr(buf, 0), value);
	return(strdup(buf));
}

#include	<ctype.h>

char *
shname(s)
	char *s;
{
	register unsigned char *a = (unsigned char *)s;

	/* always go one !! */
	a++;
	while(isalnum(*a) || (*a == '_')) a++;
	return((char *)a);
}
