/*
 * machine-dependent code for
 * looking in stack frames
 * cray x-mp
 */

#include "defs.h"
#include "regs.h"
#include "sym.h"
#include "space.h"
#include "machine.h"

/*
 * stack frame, relative to *b02
 */

#define	F_PC	010	/* return pc */
#define	F_AP	020	/* saved arg pointer */
#define	F_FP	030	/* saved frame pointer */

/*
 * return an address for a local variable
 * no register vars, unfortunately, as we can't provide an address
 * gn is the procedure; ln the local name
 */

localaddr(gn, ln)
char *gn, *ln;
{
	WORD fp, ap;
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
	if (findframe(&fp, &ap) == 0)
		error("stack frame not found");
	if (ln == NULL) {
		expsp = 0;
		expv = fp;
		return;
	}
	while (localsym()) {
		if (strcmp(ln, cursym->y_name) != 0)
			continue;
		expv = laddr(cursym, fp, ap);
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

int maxargs = 20;

ctrace(modif)
char modif;
{
	register ADDR fp, ap, callpc;
	register int narg;
	register int i;

	if (adrflg) {
		fp = adrval;
		ap = watoba(atow(aget(fp+F_AP, CORF|DATASP)));
		callpc = patoba(atow(aget(fp+F_PC, CORF|DATASP)));
	} else {
		ap = (ADDR)watoba(rtow(rget(A0+6)));
		fp = (ADDR)watoba(rtow(rget(B00+2)));
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
		narg = ltow(lget(ap, CORF|DATASP))&077;	/* real size? */
		for (i = 1; --narg >= 0 && i <= maxargs; i++) {
			printf("%R", ltow(lget(ap + i * SZREG, CORF|DATASP)));
			if (narg != 0)
				printc(',');
		}
		if (narg >= 0)
			printf(",...");
		printf(") from %R\n", callpc);
		if (modif == 'C')
			locals(fp, ap);
		callpc = patoba(atow(aget(fp + F_PC, CORF|DATASP)));
		setraddr(fp);
		ap = watoba(atow(aget(fp + F_AP, CORF|DATASP)));
		fp = watoba(atow(aget(fp + F_FP, CORF|DATASP)));
		if (fp == 0 || callpc == 0)
			break;
	}
	clrraddr();
}

static
locals(fp, ap)
ADDR fp, ap;
{
	WORD val;
	register int sp;
	ADDR laddr();

	while (localsym()) {
		sp = CORF | DATASP;
		if (cursym->y_ltype == S_RSYM)
			sp = CORF | REGSP;
		val = ltow(lget(laddr(cursym, fp, ap), sp));
		if (errflg == 0)
			printf("%8t%s/%10t%R\n", cursym->y_name, val);
		else {
			printf("%8t%s/%10t?\n", cursym->y_name);
			errflg = 0;
		}
	}
}

static ADDR
laddr(sp, fp, ap)
struct sym *sp;
ADDR fp, ap;
{

	switch (sp->y_ltype) {
	case S_STSYM:
		return (sp->y_value);

	case S_LSYM:
		return (fp - sp->y_value);

	case S_PSYM:
		return (ap + sp->y_value);

	case S_RSYM:
		return (sp->y_value * SZREG);
	}
	error("bad local symbol");
	/* NOTREACHED */
}

static int
findframe(fpp, app)
ADDR *fpp, *app;
{
#ifndef NOTDEF
	error("no");
#else
	register ADDR fp, ap, pc;
	register int fl;
	struct sym *svcur;

	svcur = cursym;
	fp = rtow(rget(FP));
	ap = rtow(rget(AP));
	pc = rtow(rget(PC));
	if (errflg)
		return (0);
	clrraddr();
	for (;;) {
		findsym(pc, INSTSP);
		if (cursym == svcur)
			break;
		if (cursym && strcmp(cursym->y_name, "start") == 0) {
			clrraddr();
			return (0);
		}
		fl = stow(sget((ADDR)fp + F_FLAGS, CORF|DATASP));
		setraddr(fl, fp);
		pc = atow(aget(fp + F_PC, CORF|DATASP));
		ap = atow(aget(fp + F_AP, CORF|DATASP));
		fp = atow(aget(fp + F_FP + SALIGN(fl), CORF|DATASP));
		/* sigtramp? */
		if (errflg) {
			clrraddr();
			return (0);
		}
	}
	*fpp = fp;
	*app = ap;
	return (1);
#endif
}

/*
 * set addresses for saved registers for this frame
 */

static
setraddr(fp)
register ADDR fp;
{
#if NOTDEF
	register int r;
	register int i;
	extern ADDR raddr[];

	mask &= FFREGS;
	for (r = 0, i = 0; mask; r++)
		if (mask & (1 << r)) {
			if (MINREG <= r && r <= MAXREG)
				raddr[r - MINREG] = fp + F_REGS +
					i * SZREG;
			i++;
			mask &=~ (1 << r);
		}
#endif
}

static
clrraddr()
{
	register int i;
	extern ADDR raddr[];

	for (i = 0; i <= MAXREG - MINREG; i++)
		raddr[i] = 0;
}
