/*
 * code to keep track of registers
 */

#include "defs.h"
#include "regs.h"
#include "space.h"

struct reglist {
	char	*rname;
	short	roffs;
	short	rsys;
	TREG	rval;
};

struct reglist reglist[] = {
	{"ps",	PS, 0},
	{"pc",	PC, 0},
	{"sp",	SP, 0},
	{"fp",	A6, 0},
	{"a5",	A5, 0},
	{"a4",	A4, 0},
	{"a3",	A3, 0},
	{"a2",	A2, 0},
	{"a1",	A1, 0},
	{"a0",	A0, 0},
	{"d7",	D7, 0},
	{"d6",	D6, 0},
	{"d5",	D5, 0},
	{"d4",	D4, 0},
	{"d3",	D3, 0},
	{"d2",	D2, 0},
	{"d1",	D1, 0},
	{"d0",	D0, 0},
	{NULL}
};

/*
 * the following are needed only to
 * make registers `addressable'
 * which is needed only so we can
 * examine register variables
 */

ADDR raddr[MAXREG - MINREG + 1];
int roffs[MAXREG - MINREG + 1] = {
#if NOTDEF
	R0, R1, R2, R3, R4, R5
#else
	0
#endif
};

/*
 * get/put registers
 * in our saved copies
 */

TREG
rget(r)
{
	register struct reglist *rp;

	for (rp = reglist; rp->rname; rp++)
		if (rp->roffs == r)
			return (rp->rval);
	error("panic: rget");
	/* NOTREACHED */
}

rput(r, v)
TREG v;
{
	register struct reglist *rp;

	for (rp = reglist; rp->rname; rp++)
		if (rp->roffs == r) {
			rp->rval = v;
			return;
		}
	error("panic: rput");
	/* NOTREACHED */
}

/*
 * grab registers into saved copy
 * should be called before looking at the process
 */

rsnarf()
{
	register struct reglist *rp;

	for (rp = reglist+1; rp->rname; rp++) {
		rp->rval = 0;
		fget((ADDR)rp->roffs, CORF|UBLKSP, (char *)&rp->rval, SZREG);
	}
	reglist[0].rval = (WORD)sget((ADDR)PS, CORF|UBLKSP);
}

/*
 * put registers back
 */

rrest()
{
	register struct reglist *rp;

	if (pid == 0)
		return;
	for (rp = reglist+1; rp->rname; rp++)
		fput((ADDR)rp->roffs, CORF|UBLKSP, (char *)&rp->rval, SZREG);
	sput((ADDR)PS, CORF|UBLKSP, (TSHORT)reglist[0].rval);
}

/*
 * print the registers
 */

printregs(c)
char c;
{
	register struct reglist *rp;

	for (rp = reglist; rp->rname; rp++) {
		if (rp->rsys == 1 && c != 'R')
			continue;
		printf("%-8R >%s\n", rtow(rp->rval), rp->rname);
	}
	printpc();
}

/*
 * translate a name to a magic register offset
 * the latter useful in rget/rput
 */

int
rname(n)
char *n;
{
	register struct reglist *rp;

	for (rp = reglist; rp->rname; rp++)
		if (strcmp(n, rp->rname) == 0)
			return (rp->roffs);
	return (BADREG);
}
