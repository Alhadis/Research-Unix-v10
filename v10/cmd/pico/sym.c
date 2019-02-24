#include <stdio.h>
#include "pico.h"
#include "y.tab.h"

Symbol  *comlist = 0;	/* commands  */
Symbol  *symlist = 0;	/* keywords, functions, variables, names  */
Symbol  *lastkey = 0;

Symbol *install(), *newbltin(), *remove();

int rand(), println(), Clamp(), Abs();
int patan2(), psin(), pcos(), Psin(), Pcos(), Patan2();
int intexp(), intsqrt(), intlog(), intlog10(), intpow();
int a_polar(), r_polar(), x_cart(), y_cart();
int setcmap(), setzmap(), getcmap();
int redcmap(), grncmap(), blucmap();
int putframe(), getframe(), swap();
int X_cart(), Y_cart(), A_polar();
int putbitmap(), put0bitmap();

static struct {
	char	*name;
	int		(*faddr)();
	char				nrparams;
} bltin[] = {
	"A_polar",	A_polar,	2,
	"Atan",		Patan2,		2,
	"Cos",		Pcos,		1,
	"Sin",		Psin,		1,
	"X_cart",	X_cart,		2,
	"Y_cart",	Y_cart,		2,

	"a_polar",	a_polar,	2,
	"abs",		Abs,		1,
	"atan",		patan2,		2,
	"blucmap",	blucmap,	2,
	"clamp",	Clamp,		1,
	"cos",		pcos,		1,
	"dump",		putbitmap,	1,
	"dump0",	put0bitmap,	1,
	"exp",		intexp,		1,
	"getcmap",	getcmap,	4,
	"getframe",	getframe,	1,
	"grncmap",	grncmap,	2,
	"log",		intlog,		1,
	"log10",	intlog10,	1,
	"pow",		intpow,		2,
	"printf",	println,	1,
	"putframe",	putframe,	1,
	"r_polar",	r_polar,	2,
	"rand",		rand,		0,
	"redcmap",	redcmap,	2,
	"setcmap",	setcmap,	4,
	"sin",		psin,		1,
	"sqrt",		intsqrt,	1,
	"swap",		swap,		0,
	"x_cart",	x_cart,		2,
	"y_cart",	y_cart,		2,
	0,		0,		0,
};

static struct {
	char	*name;
	int	kval;
} keywords[] = {
	"array",	DECL,
	"blu",		BCHAN,
	"bw",		BW,
	"cmap",		CMAP,
	"do",		DO,
	"else",		ELSE,
	"for",		FOR,
	"global",	GLOBAL,
	"goto",		JUMP,
	"grn",		GCHAN,
	"i",		I,
	"if",		IF,
	"int",		INT,
	"new",		TMP,
	"old",		OLD,
	"return",	RETURN,
	"rgb",		RGB,
	"red",		RCHAN,
	"while",	WHILE,
	"x",		X,
	"y",		Y,
	0,		0,
};

static struct {
	char	*name;
	int	cval;
} consts[] = {
	"Z",	255,
	 0,	0
};

static struct {
	char	*name;
	int	kval;
} commands[] = {
	"a",		A,
	"blind",	BLIND,
	"color",	COLOR,
	"d",		DEL,
	"def",		DEF,
	"down",		DOWN,
	"f",		F,
	"faster",	FASTER,
	"framebuffer",	FRAMEBUFFER,
	"functions",	SETF,
	"get",		GET,
	"h",		H,
	"help",		HELP,
	"metheus",	METHEUS,
	"nocolor",	NOCOLOR,
	"optim",	OPTIM,
	"q",		Q,
	"r",		R,
	"ramzap",	ZAP,
	"show",		SET,
	"slower",	SLOWER,
	"tree",		SEETREE,
	"up",		UP,
	"w",		W,
	"window",	WINDOW,
	"x",		RUN,
	"zap",		ZAP,
	0,		0,
};

syminit()
{	int i;
	extern int DEF_LL, DEF_NL;
	extern char *progr;

	progr = (char *) Emalloc(NPROG * sizeof(char));

	for (i = 0; keywords[i].name; i++)
		install(keywords[i].name, keywords[i].kval, 0);
	lastkey = symlist;
	for (i = 0; bltin[i].name; i++)
		newbltin(bltin[i].name, bltin[i].faddr, bltin[i].nrparams);
	install("X", VARIABLE, DEF_LL);
	install("Y", VARIABLE, DEF_NL);
	for (i = 0; consts[i].name; i++)
		install(consts[i].name, VARIABLE, consts[i].cval);
	for (i = 0; commands[i].name; i++)
		newcom(commands[i].name, commands[i].kval);
}

fakeparams(n)
{	int i, j;
	char s[128];

	for (i = j = 0; i < n-1; i++)
	{	s[j++] = 'a'+i;
		s[j++] = ',';
	}
	if (n > 0)
		s[j++] = 'a'+i;
	s[j] = '\0';

	fprintf(stderr, "(%s)", s);
}

spaces(n)
{ int i; char aa[32];
	for (i = 0; i < n; i++)
		aa[i] = ' ';
	aa[i] = '\0';
	fprintf(stderr, " %s", aa);
}

showfcts()
{	register Symbol *sym;
	register int i, j;

	for (sym = symlist, i=j=0; sym != lastkey; sym = sym->next)
		if (sym->type == FCT)
		{	if (i == 0)
				fprintf(stderr, "functions:\n");
			if ((i++)%4)
				spaces(15-j);
			else
				fprintf(stderr, "\n");
			fprintf(stderr, "%s", sym->name);
			fakeparams(sym->u.fu->nrparams);
			j = strlen(sym->name) + 2*(sym->u.fu->nrparams);
			j -= (sym->u.fu->nrparams != 0);
		}
	if (i)
		fprintf(stderr, "\n");
	for (sym = symlist, i=j=0; sym != lastkey; sym = sym->next)
		if (sym->type == BLTN)
		{	if (i == 0)
				fprintf(stderr, "builtins:\n");
			if ((i++)%4)
				spaces(15-j);
			else
				fprintf(stderr, "\n");
			fprintf(stderr, "%s", sym->name);
			fakeparams(sym->u.bl->nrparams);
			j = strlen(sym->name) + 2*(sym->u.bl->nrparams);
			j -= (sym->u.bl->nrparams != 0);
		}
	if (i)
		fprintf(stderr, "\n");
}

showsym(sym)
	Symbol *sym;
{	register int i, j, last;
	register struct AR *tmp;

	switch (sym->type) {
	case VARIABLE:
		fprintf(stderr, "%s = %d\n", sym->name, sym->u.val);
		break;
	case AUTO:
		fprintf(stderr, "%s = fct arg %d\n", sym->name, sym->u.val);
		break;
	case ARRAY:
		tmp = sym->u.ar;
		fprintf(stderr, "%s[%d]\n", sym->name, tmp->sz);
		fprintf(stderr, "\t%2d\t%5d\n", 0, last = tmp->base[0]);
		for (j = 0, i = 1; i < tmp->sz; i++)
		{	if (last != tmp->base[i])
			{	if (i > j+1)
				{	if (i > j+2)
					fprintf(stderr, "\t...\t  ...\n");
					fprintf(stderr, "\t%2d\t%5d\n", i-1, last);
				}
				fprintf(stderr, "\t%2d\t%5d\n", i, tmp->base[i]);
				last = tmp->base[j=i];
			}
		}
		if (i > j+1)
		{	if (i > j+2) fprintf(stderr, "\t...\t  ...\n");
			fprintf(stderr, "\t%2d\t%5d\n", i-1, last);
		}
		break;
	case FCT:
		fprintf(stderr, "%s", sym->name);
		fakeparams(sym->u.fu->nrparams);
		i = strlen(sym->name) + 2*(sym->u.fu->nrparams);
		i -= (sym->u.bl->nrparams != 0);
		fprintf(stderr, ": "); spaces(15-i);
		fprintf(stderr, "user defined function\n");
		break;
	case BLTN:
		fprintf(stderr, "%s", sym->name);
		fakeparams(sym->u.bl->nrparams);
		i = strlen(sym->name) + 2*(sym->u.bl->nrparams);
		i -= (sym->u.bl->nrparams != 0);
		fprintf(stderr, ": "); spaces(15-i);
		fprintf(stderr, "builtin function\n");
		break;
	case NAME:
		fprintf(stderr, "%s", sym->name);
		i = strlen(sym->name);
		fprintf(stderr, ": "); spaces(17-i);
		fprintf(stderr, "name\n");
		break;
	default:
		fprintf(stderr, "%s", sym->name);
		i = strlen(sym->name);
		fprintf(stderr, ": "); spaces(17-i);
		fprintf(stderr, "unknown type (%d)\n", sym->type);
		break;
	}
}

showvars()
{	register Symbol *sym;

	for (sym = symlist; sym != lastkey; sym = sym->next)
		if (sym->type == VARIABLE || sym->type == AUTO)
			showsym(sym);
	for (sym = symlist; sym != lastkey; sym = sym->next)
		if (sym->type == ARRAY)
			showsym(sym);
	for (sym = symlist; sym != lastkey; sym = sym->next)
		switch (sym->type)
		{	default: showsym(sym);
			case AUTO: case VARIABLE: case ARRAY: break;
		}
}

sympurge()
{	Symbol *sym, *last = (Symbol *) 0;
	extern char nesting;

	for (sym = symlist; sym != lastkey; )
	{	if (sym->type == VARIABLE || sym->type == ARRAY)
		{	if (sym->depth >= nesting && sym->depth > 0)
			{	sym = remove(sym, last);
				continue;
			}
		} else
		if (sym->type == AUTO && nesting == 1)
		{	sym = remove(sym, last);
			continue;
		}
		last = sym;
		sym = sym->next;
	}
}

Symbol *
remove(s, l)
	Symbol *s, *l;
{
	if (s == symlist)
	{	symlist = s->next;
		return symlist;
	} else
	{	l->next = s->next;
		return l->next;
	}
}

Symbol *
lookup(s)
	char *s;
{	Symbol *sym;
	Symbol *zym;
	int found = -1;

	for (sym = symlist; sym != (Symbol *) 0; sym = sym->next)
		if (strcmp(sym->name, s) == 0 && sym->depth >= found)
		{	found = sym->depth;
			zym = sym;
		}
	if (found != -1)
		return zym;

	if (strcmp(s, "old") == 0 || strcmp(s, "new") == 0)
		yyerror("cannot redefine %s", s);

	return install(s, NAME, 0);
}

Symbol *
whichcmd(s)
	char *s;
{	Symbol *sym;

	for (sym = comlist; sym != (Symbol *) 0; sym = sym->next)
		if (strcmp(sym->name, s) == 0)
			return sym;
	return lookup(s);
}

Symbol *
newsym(s)
	char *s;
{	Symbol *tmp;

	tmp = (Symbol *) Emalloc(sizeof(Symbol));
	tmp->name = Emalloc(strlen(s)+1);
	strcpy(tmp->name, s);

	return tmp;	/* not linked in list */
}

Symbol *
install(s, t, v)
	char *s;
{	Symbol *sp;
	extern char nesting;

	sp = (Symbol *) Emalloc(sizeof(Symbol));
	sp->name = Emalloc(strlen(s)+1);
	strcpy(sp->name, s);
	sp->type = t;		/* NAME, VAR or keyword */
	sp->depth = nesting;
	sp->u.val = v;
	sp->next = symlist;
	symlist = sp;

	return sp;
}

Symbol *
newbltin(s, v, p)
	char *s;
	int (*v)();
{	Symbol *sp;
	extern char nesting;

	sp = (Symbol *) Emalloc(sizeof(Symbol));
	sp->name = Emalloc(strlen(s)+1);
	strcpy(sp->name, s);
	sp->type = BLTN;
	sp->depth = nesting;
	sp->u.bl = (struct BL *) Emalloc(sizeof(struct BL));
	sp->u.bl->faddr = v;
	sp->u.bl->nrparams = p;
	sp->u.bl->usednew = 0;
	sp->next = symlist;
	symlist = sp;

	return sp;
}

newcom(s, t)
	char *s;
{	Symbol *sp;

	sp = (Symbol *) Emalloc(sizeof(Symbol));
	sp->name = Emalloc(strlen(s)+1);
	strcpy(sp->name, s);
	sp->type = t;	/* only commands */
	sp->next = comlist;
	comlist = sp;
}

decarray(s, d)
	Symbol *s;
	long d;
{	register int i;
	extern char nesting, isglobal;

	if (s->type == ARRAY)
	{	if (s->depth == nesting || (s->depth == 0 && isglobal))
		{	fprintf(stderr, "warning: array %s re-declared\n", s->name);
			if (isglobal) s->depth = 0;
			if (s->u.ar->sz != d)
			{	free(s->u.ar->base);
				goto newa;
			} else	goto olda;
		} else if (s->depth < nesting)
		{	decarray(install(s->name, NAME, 0), d);
			return;
		}
	} else
		s->type = ARRAY;

	s->depth = (isglobal)?0:nesting;
	s->u.ar = (struct AR *) Emalloc(sizeof(struct AR));
newa:	s->u.ar->sz = d;
	s->u.ar->base = (long *) Emalloc(d * sizeof(long));
olda:	for (i = 0; i < d; i++)
		s->u.ar->base[i] = 0;
}

decsame(t, s, d)
	short t;
	Symbol *s;
	long d;
{	extern char nesting, isglobal;

	if (s->type == t)
	{	if (s->depth == nesting || (s->depth == 0 && isglobal))
		{	fprintf(stderr, "warning: variable %s re-declared\n", s->name);
			if (isglobal) s->depth = 0;
			s->u.val = d;
		} else if (s->depth < nesting)
			install(s->name, t, d);

		return;
	}

	s->type = t;
	s->depth = (isglobal)?0:nesting;
	s->u.val = d;
}

decvar(s, d)
	Symbol *s;
	long d;
{
	decsame(VARIABLE, s, d);
}

automa(s, d)
	Symbol *s;
	long d;
{
	decsame(AUTO, s, d);
}

char *thisfun;

prefunct(s, nrp)
	Symbol *s;
{	extern char *progr;

	if (s->type != FCT)
	{	s->type = FCT;
		s->u.fu = (struct FU *) Emalloc(sizeof(struct FU));
	} else
	{	fprintf(stderr, "function %s redefined\n", s->name);
		if (s->u.fu->usednew != -1) free(s->u.fu->fct);
	}
	s->u.fu->fct = progr;
	thisfun = s->name;
	s->u.fu->usednew = -1;
	s->u.fu->nrparams = nrp;
}

notrecurcall(s)
	Symbol *s;
{
	return strcmp(s->name, thisfun);
}

notafunc()
{
	extern char nesting;
	return (nesting == 0 && thisfun == NULL);
}

postfunct(s)
	Symbol *s;
{	extern char nesting, usednew, usedold, *progr;

	progr = (char *) Emalloc(NPROG * sizeof(char));
	thisfun = '\0';
	s->depth = nesting;
	s->u.fu->usednew = usednew;
	s->u.fu->usedold = usedold;
}

#include "pico2.h"

Node *mapred(n)
	Node *n;
{	return new(CCALL, new(ACOMMA, AREG(IREG), n, Z), Z, redcmap);
}

Node *mapgrn(n)
	Node *n;
{	return new(CCALL, new(ACOMMA, AREG(IREG), n, Z), Z, grncmap);
}

Node *mapblu(n)
	Node *n;
{	return new(CCALL, new(ACOMMA, AREG(IREG), n, Z), Z, blucmap);
}

Node *
mapall(n)
	Node *n;
{
	if (n->type == COMP)
		return new(CCALL, new(ACOMMA, AREG(IREG), n, Z), Z,  setcmap);
	else
		return new(CCALL, new(ACOMMA, AREG(IREG), n, Z), Z,  setzmap);
}
