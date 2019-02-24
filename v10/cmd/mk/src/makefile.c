#include	"mk.h"

#define		FOLD	65

static char *first = 0;
static didit;

maketarget(s)
	char *s;
{
	if(first == 0)
		first = s;
}

addmake(n)
	Node *n;
{
	symlook(n->name, S_MAKEFILE, (char *)n);
}

static
fold(s, sep)
	char *s;
{
	int i = 0;

	while(*s){
		i++;
		if((*s == ' ') && (i > FOLD)){
			Fprint(1, "\\\n%c", sep);
			i = 0;
		} else
			Fputc(1, *s++);
	}
}

static void
vd(s)
	register Symtab *s;
{
	Fprint(1, "%s=", s->name);
	fold(symlook(s->name, S_VAR, (char *)0)->value, ' ');
	Fputc(1, '\n');
}

#define	SUB(s)	{ register char *ss = s; while(*dest++ = *ss++); dest--; copy = 0; }

static char *
vexpand(ww, dest, j)
	char **ww, *dest;
	Job *j;
{
	register char *s, *w = *ww;
	char carry, *q;
	int copy;

	assert("vexpand no $", *w == '$');
	if(*++w == '{'){
		for(s = ++w; *s != '}'; s++)
			if(*s == 0) break;
	} else
		s = shname(w);
	carry = *s;
	*s = 0;
	copy = 1;
	if(!symlook(w, S_MAKEVAR, (char *)0)){
		if(strcmp(w, "target") == 0){
			SUB("$?")
		} else if(strcmp(w, "stem") == 0){
			SUB(j->stem)
		} else if(strcmp(w, "prereq") == 0){
			SUB("$@")
		} else if(strcmp(w, "pid") == 0){
			SUB("$$$$")
		} else if(strcmp(w, "newprereq") == 0){
			SUB(wtos(j->np))
		} else if(strcmp(w, "alltarget") == 0){
			SUB(wtos(j->at))
			Fprint(2, "Warning; $alltarget expanded to '%s'\n", wtos(j->at));
		} else if((strncmp(w, "stem", 4) == 0) && (w[4] >= '0') && (w[4] <= '9')){
			int i = w[4] - '0';
			int nb = j->match[i].ep - j->match[i].sp;

			memcpy(dest, j->match[i].sp, nb);
			dest += nb;
		} else {
			SUB("1");
			Fprint(2, "Warning; $%s expanded to 1\n", w);
		}
	}
	if(copy){
		*dest++ = '$';
		*dest++ = '{';
		for(q = w; q != s;)
			*dest++ = *q++;
		*dest++ = '}';
	}
	*s = carry;
	if(carry == '}')
		s++;
	*ww = s;
	return(dest);
}

static
recipepr(s, j)
	char *s;
	Job *j;
{
	char buf[BIGBLOCK];
	register char *q = buf;

	while(*s)
		switch(*s)
		{
		case '\\':
			*q++ = *s++;
			*q++ = *s++;
			break;
		case '$':
			q = vexpand(&s, q, j);
			break;
		case '\n':
			*q = 0;
			Fprint(1, "\t%s\n", buf);
			q = buf;
			s++;
			break;
		default:
			*q++ = *s++;
			break;
		}
	*q = 0;
	if(q != buf)
		Fprint(1, "\t%s\n", buf);
}

static void
nd(s)
	Symtab *s;
{
	Job *j = (Job *)s->value;
	register Node *n = j->n;
	Arc *a;
	int i;
	char *ss, *s, c;

	if(strcmp(n->name, first) == 0){
		if(didit)
			return;
		didit = 1;
	}
	Fprint(1, "\n%s:", n->name);
	fold(wtos(j->p), ' ');
	Fputc(1, '\n');
	recipepr(j->r->recipe, j);
}

makefile()
{
	symtraverse(S_MAKEVAR, vd);
	didit = 0;
	nd(symlook(first, S_MAKEFILE, (char *)0));
	symtraverse(S_MAKEFILE, nd);
}
