/*
 * code to keep track of registers
 */

#include "defs.h"
#include "regs.h"
#include "space.h"
#include "machine.h"
#include <sys/param.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/dir.h>
#include <sys/proc.h>
#include <sys/user.h>

/*
 * register table
 * the numbers in regs.h are indices into this,
 * and the code that follows is very sensitive;
 * take care
 */

struct reglist {
	char	*rname;
	short	rint;		/* is this especially interesting? */
	TREG	rval;
};

struct reglist reglist[] = {
#include "regtab.i"
};

#define	BIGREG	(sizeof(reglist)/sizeof(reglist[0]))

/*
 * the following are needed only to
 * make registers `addressable'
 * which is needed only so we can
 * examine register variables
 */

ADDR raddr[MAXREG - MINREG + 1];
int roffs[MAXREG - MINREG + 1] = {
	0
};

/*
 * get/put registers
 * in our saved copies
 */

TREG
rget(r)
{
	register struct reglist *rp;

	if (r >= 0 && r < BIGREG)
		return (reglist[r].rval);
	error("panic: rget");
	/* NOTREACHED */
}

rput(r, v)
TREG v;
{

	if (r >= 0 && r < BIGREG) {
		reglist[r].rval = v;
		return;
	}
	error("panic: rput");
	/* NOTREACHED */
}

/*
 * grab registers into saved copy
 * should be called before looking at the process
 */

static struct user xu;

rsnarf()
{
	register struct reglist *rp;
	register word *wp;
	register int i;

	if (fget((char *)&xu.u_xp-(char *)&xu, CORF|UBLKSP,
		(char *)&xu.u_xp, (char *)&xu.u_savev[7][64]-(char *)&xu.u_xp) == 0)
		return;
	reglist[PC].rval = patoba(xu.u_xp.p);
	wp = (word *)&xu.u_xp;
	rp = &reglist[A0];
	for (i = 0; i < 010; i++)
		rp++->rval = *wp++ & 0xffffff;	/* hm? */
	wp = &xu.u_xp.s0;
	rp = &reglist[S0];
	for (i = 0; i < 010; i++)
		rp++->rval = *wp++;
	rp = &reglist[B00];
	rp++->rval = xu.u_b00;
	wp = xu.u_saveb;
	for (i = 0; i < 077; i++)
		rp++->rval = *wp++;
	wp = xu.u_savet;
	rp = &reglist[T00];
	for (i = 0; i < 0100; i++)
		rp++->rval = *wp++;
	/* leave vectors for later */
}

/*
 * put registers back
 */

rrest()
{
	register struct reglist *rp;
	register word *wp;
	register int i;

	if (pid == 0)
		return;
	xu.u_xp.p = batopa(reglist[PC].rval);
	wp = (word *)&xu.u_xp;
	rp = &reglist[A0];
	for (i = 0; i < 010; i++) {
		*wp &=~ 0xffffff;
		*wp++ |= rp++->rval & 0xffffff;
	}
	wp = &xu.u_xp.s0;
	rp = &reglist[S0];
	for (i = 0; i < 010; i++)
		*wp++ = rp++->rval;
	rp = &reglist[B00];
	rp++->rval = xu.u_b00;
	wp = xu.u_saveb;
	for (i = 0; i < 077; i++)
		*wp++ = rp++->rval;
	wp = xu.u_savet;
	rp = &reglist[T00];
	for (i = 0; i < 0100; i++)
		*wp++ = rp++->rval;
	fput((char *)&xu.u_xp-(char *)&xu, CORF|UBLKSP,
		(char *)&xu.u_xp, (char *)&xu.u_savev[7][64]-(char *)&xu.u_xp);
}

/*
 * print the registers
 */

printregs(c)
char c;
{
	register struct reglist *rp;

	for (rp = reglist; rp->rname; rp++) {
		if (rp->rint == 0 && c != 'R')
			continue;
		if (rp->rval == 0)
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
			return (rp - reglist);
	return (BADREG);
}
