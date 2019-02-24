/*
 * code to keep track of registers
 */

#include "defs.h"
#include "space.h"
#include "regs.h"
#include "machine.h"
#include <sys/param.h>
#include <sys/dir.h>
#include <sys/user.h>

#define	UOFF(x)	(ADDR)(&((struct user *)0)->x)

struct reglist {
	char	*rname;
	short	roffs;
	short	rsys;
	TREG	rval;
};

struct reglist reglist[] = {
#define	FW	0	/* first register we may write */
	{"modpsr",	MODPSR, 0},
	{"pc",	PC, 0},
	{"sp",	SP, 0},
	{"fp",	FP, 0},
	{"r7",	R7, 0},
	{"r6",	R6, 0},
	{"r5",	R5, 0},
	{"r4",	R4, 0},
	{"r3",	R3, 0},
	{"r2",	R2, 0},
	{"r1",	R1, 0},
	{"r0",	R0, 0},
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
	R0, R1, R2, R3, R4, R5, R6, R7,
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
	register ADDR base;

	base = (ADDR)atow(aget(UOFF(u_ar0), CORF|UBLKSP));
	if (base == 0)
		return;
	base -= USRBLK;
	for (rp = reglist; rp->rname; rp++) {
		rp->rval = 0;
		fget((ADDR)(rp->roffs*SZREG+base),
			CORF|UBLKSP, (char *)&rp->rval, SZREG);
	}
}

/*
 * put registers back
 */

rrest()
{
	register struct reglist *rp;
	register ADDR base;

	if (pid == 0)
		return;
	base = (ADDR)atow(aget(UOFF(u_ar0), CORF|UBLKSP));
	if (base == 0)
		return;
	base -= USRBLK;
	for (rp = &reglist[FW]; rp->rname; rp++)
		fput((ADDR)(rp->roffs*SZREG+base),
			CORF|UBLKSP, (char *)&rp->rval, SZREG);
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
		printf("%s%6t%R %16t", rp->rname, rtow(rp->rval));
		valpr(rtow(rp->rval), (rp->roffs == PC) ? INSTSP : DATASP);
		printc(EOR);
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
