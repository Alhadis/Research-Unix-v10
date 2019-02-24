/*
 * adb - symbol table routines
 */
#include "defs.h"
#include "sym.h"
#include "space.h"

struct sym *cursym;
struct sym *symtab;

static int gsname(), lsname();

/*
 * Lookup a symbol by name
 * leave a pointer to the symbol in cursym
 */
struct sym *
lookup(symstr)
	register char *symstr;
{
	register struct sym *sp;

	cursym = 0;
	for (sp = symtab; sp; sp = sp->y_next)
		if (eqsym(sp, symstr))
			return(cursym = sp);
	return (0);
}

/*
 * find the symbols for a routine
 */

int
findrtn(rtn)
char *rtn;
{
	register struct sym *sp;

	cursym = 0;
	for (sp = symtab; sp; sp = sp->y_next)
		switch (sp->y_type) {
		case S_TEXT:
			if (eqsym(sp, rtn)) {
				cursym = sp;
				return (1);
			}
		}
	return (0);
}

/*
 * Find the closest symbol to val, and return
 * the difference between val and the symbol found.
 * Leave a pointer to the symbol found as cursym.
 */
WORD
findsym(w, type)
	WORD w;
	int type;
{
	register unsigned long diff;
	register unsigned long val;
	register struct sym *sp;

	val = w;
	cursym = 0;
	diff = HUGE;
	type &= SPTYPE;
	if (type == NOSP || symtab == 0)
		return (diff);
	for (sp = symtab; sp; sp = sp->y_next) {
		switch (type) {
		case INSTSP:
			if (sp->y_type != S_TEXT && sp->y_type != S_ABS)
				continue;
			break;

		case DATASP:
			if (sp->y_type != S_DATA && sp->y_type != S_ABS)
				continue;
			break;
		}
		if (val >= (unsigned long)sp->y_value
		&&  val - (unsigned long)sp->y_value < diff) {
			diff = val - (unsigned long)sp->y_value;
			cursym = sp;
			if (diff == 0)
				break;
		}
	}
	if (cursym && cursym->y_value == 0 && diff != 0)
		return (HUGE);
	return (diff);
}

/*
 * advance cursym to the next local variable
 * return 0 if the next variable is a global
 */
int
localsym()
{
	register struct sym *sp;

	if ((sp = cursym) == NULL)
		return (0);
	if (sp->y_type != S_STAB)
		sp = sp->y_locals;
	else
		sp = sp->y_next;
	cursym = sp;
	return (cursym != 0);
}

/*
 * Print value v and then the string s.
 * If v is not zero, then we look for a nearby symbol
 * and print name+offset if we find a symbol for which
 * offset is small enough.
 */
psymoff(v, type, s)
	WORD v;
	int type;
	char *s;
{
	WORD w;

	if ((type & SPTYPE) == REGSP) {
		printf("%%%R", v);
		printf(s);
		return;
	}
	if (v)
		w = findsym(v, type);
	if (v==0 || w >= maxoff)
		printf("%R", v);
	else {
		printf("%s", cursym->y_name);
		if (w)
			printf("+%R", w);
	}
	printf(s);
}

/*
 * Print value v symbolically if it has a reasonable
 * interpretation as name+offset.  If not, print nothing.
 * Used in printing out registers $r.
 */
valpr(v, idsp)
	WORD v;
{
	WORD d;

	d = findsym(v, idsp);
	if (d >= maxoff)
		return;
	printf("%s", cursym->y_name);
	if (d)
		printf("+%R", d);
}

printsym()
{
	register struct sym *sp;
	register struct sym *lp;

	for (sp = symtab; sp; sp = sp->y_next) {
		printf("%8R %c %s\n", sp->y_value, gsname(sp->y_type), sp->y_name);
		for (lp = sp->y_locals; lp; lp = lp->y_next)
			printf("%8R %c %s\n", lp->y_value, lsname(lp->y_ltype), lp->y_name);
	}
}

static
gsname(t)
int t;
{
	switch (t) {
	case S_TEXT:
		return ('T');
	case S_DATA:
		return ('D');
	case S_ABS:
		return ('A');
	case S_STAB:
		return ('L');
	default:
		return ('?');
	}
}

static
lsname(t)
int t;
{
	switch (t) {
	case S_LSYM:
		return ('l');
	case S_PSYM:
		return ('p');
	case S_RSYM:
		return ('r');
	case S_STSYM:
		return ('s');
	default:
		return ('?');
	}
}
