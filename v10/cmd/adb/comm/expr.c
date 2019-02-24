/*
 *
 *	UNIX debugger
 *
 */

#include "defs.h"
#include "regs.h"
#include "sym.h"
#include "space.h"

WORD	var[NVARS];

extern	int	radix;

extern	char	lastc, peekc;

extern	ADDR	ditto;
extern	int	ditsp;
extern	int	dotsp;
WORD	expv;
int	expsp;

WORD
defval(w)
WORD w;
{
	if (expr(0))
		return (expv);
	else
		return (w);
}

expr(a)
register int a;
{	/* term | term dyadic expr |  */
	register int	rc;
	register WORD	lhs;

	rdc();
	reread();
	expsp = NOSP;
	rc=term(a);
	while (rc) {
		lhs = expv;
		switch ((int)readchar()) {

		case '+':
			term(a|1);
			expv += lhs;
			break;

		case '-':
			term(a|1);
			expv = lhs - expv;
			break;

		case '#':
			term(a|1);
			expv = round(lhs,expv);
			break;

		case '*':
			term(a|1);
			expv *= lhs;
			break;

		case '%':
			term(a|1);
			expv = lhs/expv;
			break;

		case '&':
			term(a|1);
			expv &= lhs;
			break;

		case '|':
			term(a|1);
			expv |= lhs;
			break;

		case ')':
			if ((a&2)==0)
				error("unexpected `)'");

		default:
			reread();
			return(rc);
		}
	}
	return(rc);
}

term(a)
register int a;
{	/* item | monadic item | (expr) | */

	switch ((int)readchar()) {

	case '*':
		term(a|1);
		expv=atow(aget((ADDR)expv, CORF|DATASP));
		expsp = NOSP;
		chkerr();
		return(1);

	case '@':
		term(a|1);
		expv=atow(aget((ADDR)expv, SYMF|INSTSP));
		expsp = NOSP;
		return(1);

	case '-':
		term(a|1);
		expv = -expv;
		return(1);

	case '~':
		term(a|1);
		expv = ~expv;
		return(1);

	case '(':
		expr(2);
		if (readchar()!=')')
			error("syntax error: `)' expected");
		return(1);

	case '%':
		term(a|1);
		expsp = REGSP;
		return(1);

	default:
		reread();
		return(item(a));
	}
}

item(a)
register int a;
{	/* name [ . local ] | number | . | ^ | <var | <register | 'x | | */
	int	base;
	char	savc;
	register struct sym *symp;
	char gsym[MAXSYM], lsym[MAXSYM];
	WORD ascval();

	readchar();
	if (symchar(0)) {	
		readsym(gsym);
		if (lastc=='.') {
			readchar();	/* ugh */
			if (!symchar(0))
				localaddr(gsym, (char *)NULL);
			else {
				readsym(lsym);
				localaddr(gsym, lsym);
			}
		}
		else {
			if ((symp = lookup(gsym)) == NULL)
				error("symbol not found");
			expv = symp->y_value;
		}
		reread();
	} else if (getnum(readchar)) {
		;
	} else if (lastc=='.') {	
		readchar();
		if (!symchar(0)) {
			expv = dot;
			expsp = dotsp;
		} else {
			readsym(lsym);
			localaddr((char *)NULL, lsym);
		}	
		reread();
	} else if (lastc=='"') {
		expv=ditto;
		expsp = ditsp;
	} else if (lastc=='+') {
		expv=inkdot(dotinc);
		expsp = ditsp;
	} else if (lastc=='^') {
		expv=inkdot(-dotinc);
		expsp = ditsp;
	} else if (lastc=='<') {
		savc=rdc();
		base = getreg(savc);
		if (base != BADREG)
			expv = rtow(rget(base));
		else if ((base = varchk(savc)) != -1)
			expv = var[base];
		else
			error("bad variable");
	}
	else if (lastc=='\'')
		expv = ascval();
	else if (a)
		error("address expected");
	else {	
		reread();
		return(0);
	}
	return(1);
}

#define	MAXBASE	16

/* service routines for expression reading */
getnum(rdf)
int (*rdf)();
{
	register char *cp;
	register int base, d;
	BOOL fpnum;
	char num[MAXLIN];
	WORD fpin();

	base = radix;
	fpnum = FALSE;
	if (lastc == '#') {
		base = 16;
		(*rdf)();
	}
	if (convdig(lastc) >= MAXBASE)
		return (0);
	if (lastc == '0')
		switch ((*rdf)()) {
		case 'x':
		case 'X':
			base = 16;
			(*rdf)();
			break;

		case 't':
		case 'T':
			base = 10;
			(*rdf)();
			break;

		case 'o':
		case 'O':
			base = 8;
			(*rdf)();
			break;
		default:
			if (base == 0)
				base = 8;
			break;
		}
	if (base == 0)
		base = 10;
	expv = 0;
	for (cp = num, *cp++ = lastc; ;(*rdf)()) {
		if ((d = convdig(lastc)) < base) {
			expv *= base;
			expv += d;
			*cp++ = lastc;
		}
		else if (lastc == '.') {
			fpnum = TRUE;
			*cp++ = lastc;
		}
		else {
			reread();
			break;
		}
	}
	if (fpnum)
		expv = fpin(num);
	return (1);
}

readsym(isymbol)
register char *isymbol;
{
	register char	*p;

	p = isymbol;
	do {
		if (p < &isymbol[MAXSYM-1])
			*p++ = lastc;
		readchar();
	} while (symchar(1));
	*p = 0;
}

convdig(c)
register char c;
{
	if (isdigit(c))
		return(c-'0');
	else if (!isxdigit(c))
		return(MAXBASE);
	else if (isupper(c))
		return(c-'A'+10);
	else
		return(c-'a'+10);
}

symchar(dig)
{
	if (lastc=='\\') {
		readchar();
		return(TRUE);
	}
	return(isalpha(lastc) || lastc=='_' || dig && isdigit(lastc));
}

varchk(name)
{
	if (isdigit(name))
		return(name-'0');
	if (isalpha(name))
		return((name&037)-1+10);
	return(-1);
}
