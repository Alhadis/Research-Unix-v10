/*
 * machine-dependent code for
 * looking in stack frames
 * vax version
 */

#include "defs.h"
#include "regs.h"
#include "sym.h"
#include "space.h"

int maxargs = 20;

/*
 * PDP-11 C stack frame
 */

#define	F_FP	0	/* link to next frame */
#define	F_PC	2	/* return pc */

#define	EVEN(x)	((x)&~1)

/*
 * return an address for a local variable
 * no register vars, unfortunately, as we can't provide an address
 * gn is the procedure; ln the local name
 */

localaddr(gn, ln)
char *gn, *ln;
{
	WORD fp;
	extern WORD expv;
	extern int expsp;
	ADDR laddr();

	if (gn) {
		if (findrtn(gn) == 0)
			error("function not found");
	}
	else {
		findsym((WORD)atow(rget(PC)), INSTSP);
		if (cursym == NULL)
			error("function not found");
	}
	if (findframe(&fp) == 0)
		error("stack frame not found");
	if (ln == NULL) {
		expsp = 0;
		expv = fp;
		return;
	}
	while (localsym()) {
		if (strcmp(ln, cursym->y_name) != 0)
			continue;
		expv = laddr(cursym, fp);
		if (cursym->y_ltype == S_RSYM)
			expsp = REGSP;
		else
			expsp = NOSP;
		return;
	}
	error("bad local variable");
	/* NOTREACHED */
}

/*
 * print a stack traceback
 * give locals if possible
 */

ctrace(modif)
char modif;
{
	register ADDR fp, callpc;

	if (adrflg) {
		fp = EVEN(adrval);
		callpc = atow(aget(fp + F_PC, CORF|DATASP));
	} else {
		fp = (ADDR)EVEN(rtow(rget(R5)));
		callpc = (ADDR)rtow(rget(PC));
	}
	clrraddr();
	while (cntval--) {
		chkerr();
		findsym(callpc, INSTSP);
		if (cursym == NULL)
			printf("?(");
		else
			printf("%s(", cursym->y_name);
		/* args stuff */
		printf(") from %R\n", callpc);
		if (modif == 'C')
			locals(fp);
		callpc = atow(aget(fp + F_PC, CORF|DATASP));
		setraddr(fp);
		fp = EVEN(atow(aget(fp + F_FP, CORF|DATASP)));
		if (fp == 0)
			break;
	}
	clrraddr();
}

static
locals(fp)
ADDR fp;
{
	WORD val;
	register int sp;
	ADDR laddr();

	while (localsym()) {
		sp = CORF | DATASP;
		if (cursym->y_ltype == S_RSYM)
			sp = CORF | REGSP;
		val = ltow(lget(laddr(cursym, fp), sp));
		if (errflg == 0)
			printf("%8t%s/%10t%R\n", cursym->y_name, val);
		else {
			printf("%8t%s/%10t?\n", cursym->y_name);
			errflg = 0;
		}
	}
}

static ADDR
laddr(sp, fp)
struct sym *sp;
ADDR fp;
{

	switch (sp->y_ltype) {
	case S_STSYM:
		return (sp->y_value);

	case S_LSYM:
	case S_PSYM:
		return (fp + (short)sp->y_value);

	case S_RSYM:
		return (sp->y_value * SZREG);
	}
	error("bad local symbol");
	/* NOTREACHED */
}

static int
findframe(fpp)
ADDR *fpp;
{
	register ADDR fp, pc;
	register int fl;
	struct sym *svcur;

	svcur = cursym;
	fp = EVEN(rtow(rget(R5)));
	pc = rtow(rget(PC));
	if (errflg)
		return (0);
	clrraddr();
	for (;;) {
		findsym(pc, INSTSP);
		if (cursym == svcur)
			break;
		setraddr(fp);
		pc = atow(aget(fp + F_PC, CORF|DATASP));
		fp = EVEN(atow(aget(fp + F_FP, CORF|DATASP)));
		if (errflg) {
			clrraddr();
			return (0);
		}
	}
	*fpp = fp;
	return (1);
}

/*
 * set addresses for saved registers for this frame
 */

static
setraddr(fp)
register ADDR fp;
{
	register int r;
	register int i;
	extern ADDR raddr[];

	/* all wrong */
}

static
clrraddr()
{
	register int i;
	extern ADDR raddr[];

	for (i = 0; i <= MAXREG - MINREG; i++)
		raddr[i] = 0;
}
