/*
 * machine-dependent code for
 * looking in stack frames
 * sequent/32032 version
 */

#include "defs.h"
#include "space.h"
#include <sys/param.h>
#include "regs.h"
#include <a.out.h>
#include "sym.h"
#include <signal.h>

int maxargs = 20;

/*
 * stuff in the stack frame
 */

#define	F_HACKPC 48	/* signal handler hack */
#define	F_ARG1	8	/* first argument */
#define	F_PC	4
#define	F_FP	0

/*
 * how to find out the number of arguments to a procedure call:
 * look at the instruction after the call (cxp)
 * it should be an adjspb with an immediate operand
 * the operand (the following byte) is the (negative) number of
 * bytes to be popped off the stack after the call;
 * i.e. the amount of space taken by parameters
 */

#define	adjspbi(x)	(((x) & 0xffff) == 0xa57c)
#define	adjoff(x)	((((x) & 0xff0000) >> 16) | 0xffffff00)

/*
 * return an address for a local variable
 * no register vars, unfortunately, as we can't provide an address
 * gn is the procedure; ln the local name
 */

extern WORD expv;
extern int expsp;

localaddr(gn, ln)
char *gn, *ln;
{
	WORD fp;
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
		error("function not found");
	if (ln == NULL) {
		expsp = 0;
		expv = fp;
		return;
	}
	while (localsym()) {
		if (strcmp(ln, cursym->y_name) != 0)
			continue;
		expv = laddr(cursym, fp, fp + F_ARG1);
		if (cursym->y_ltype == S_RSYM)
			expsp = REGSP;
		else
			expsp = NOSP;
		return;
	}
	error("local var not found");
	/* NOTREACHED */
}

/*
 * print a stack traceback
 * give locals if possible
 */

ctrace(modif)
char modif;
{
	register ADDR fp, ap, callpc;
	register int narg;
	ADDR ofp;
	ADDR newpc;
	WORD w;

	if (adrflg) {
		fp = adrval;
		callpc = atow(aget(fp + F_PC, CORF|DATASP));
	} else {
		fp = (ADDR)rtow(rget(FP));
		callpc = (ADDR)rtow(rget(PC));
	}
	clrraddr();
	ofp = fp;
	while (cntval--) {
		chkerr();
		findsym(callpc, INSTSP);
		if (cursym == NULL)
			printf("?(");
		else if (strcmp("start", cursym->y_name) == 0)
			break;
		else
			printf("%s(", cursym->y_name);
		if (strcmp("sigcode", cursym->y_name) == 0) {
			printf(") from %R\n", callpc);
			callpc = ltow(lget(ofp + F_HACKPC, CORF|DATASP));
			/* ignore saved r0 r1 r2 for now */
			continue;
		}
		newpc = atow(aget(fp + F_PC, CORF|DATASP));
		w = ltow(lget(newpc, SYMF|INSTSP));
		if (!adjspbi(w))
			narg = 0;
		else
			narg = -adjoff(w)/4;
		for (ap = fp + F_ARG1; --narg >= 0; ap += SZREG) {
			printf("%R", ltow(lget(ap, CORF|DATASP)));
			if (narg != 0)
				printc(',');
		}
		printf(") from %R\n", callpc);
		if (modif == 'C')
			locals(fp, fp + F_ARG1);
		setraddr(callpc, fp);
		callpc = newpc;
		ofp = fp;
		fp = atow(aget(fp + F_FP, CORF|DATASP));
		if (fp == 0)
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

ADDR
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

int
findframe(fpp)
ADDR *fpp;
{
	register ADDR fp, pc;
	struct sym *svcur;

	svcur = cursym;
	fp = rtow(rget(FP));
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
		setraddr(cursym->y_value, fp);
		pc = atow(aget(fp + F_PC, CORF|DATASP));
		fp = atow(aget(fp + F_FP, CORF|DATASP));
		/* sigtramp? */
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
 * the mask is that from an `enter' instruction
 */

extern ADDR raddr[];

setraddr(pc, fp)
ADDR pc;
register ADDR fp;
{
	register int r;
	register int i;
	int roff;
	register int mask;

	if ((mask = regmask(pc, &roff)) == 0)
		return;
	fp += roff;
	for (r = 7, i = 0; mask; r--)
		if (mask & (1 << r)) {
			if (MINREG <= r && r <= MAXREG)
				raddr[r - MINREG] = fp + i * SZREG;
			i++;
			mask &=~ (1 << r);
		}
}

clrraddr()
{
	register int i;

	for (i = 0; i <= MAXREG - MINREG; i++)
		raddr[i] = 0;
}

/*
 * find the mask of saved registers for this routine
 *
 * pc is the entry point.
 * if the code was run through c2, the first instruction will be `enter';
 * grab the mask.
 * set *rp to the number of bytes of local storage on the stack
 * (the registers are pushed AFTER local storage)
 * if it wasn't put through c2, the first instruction will be a branch.
 * the instruction branched to will be the `enter.'
 *
 * awful but how else to do it?
 */

#define	enter(w)	(((w)&0xff) == 0x82)
#define	entmsk(w)	(((w)>>8) & 0xff)
#define	br(w)		(((w)&0xff) == 0xea)

int
regmask(pc, rp)
ADDR pc;
int *rp;
{
	register WORD w;
	long disp();

	*rp = 0;
	w = ltow(lget(pc, SYMF|INSTSP));
	if (enter(w)) {
		*rp = disp(pc + 2);
		return (entmsk(w));
	}
	if (!br(w))
		return (0);
	w = ltow(lget(pc + disp(pc + 1), SYMF|INSTSP));
	if (enter(w))
		return (entmsk(w));
	return (0);
}

static
disp(pc)
ADDR pc;
{
	register WORD w;

	w = ltow(lget(pc, SYMF|INSTSP));
	if ((w & 0x80) == 0) {
		w &= 0x7f;
		if (w & 0x40)
			w |= 0xffffffc0;
	}
	else if ((w & 0x40) == 0) {
		w = ((w << 8) & 0x3f00) | ((w >> 8) & 0xff);
		if (w & 0x2000)
			w |= 0xffffc000;
	}
	else {
		w = ((w << 24) & 0x3f000000)
		  | ((w << 8)  & 0xff0000)
		  | ((w >> 8)  & 0xff00)
		  | ((w >> 24) & 0xff);
		if (w & 0x20000000)
			w |= 0xc0000000;
	}
	return (w);
}
