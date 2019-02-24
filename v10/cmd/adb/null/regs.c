/*
 * code to keep track of registers
 */

#include "defs.h"
#include "regs.h"

/*
 * the following are needed only to
 * make registers `addressable'
 * which is needed only so we can
 * examine register variables
 */

ADDR raddr[MAXREG - MINREG + 1];
int roffs[MAXREG - MINREG + 1];

/*
 * get/put registers
 * in our saved copies
 */

TREG
rget(r)
{

	error("panic: rget");
	/* NOTREACHED */
}

rput(r, v)
TREG v;
{

	error("panic: rput");
	/* NOTREACHED */
}


/*
 * print the registers
 */

printregs(c)
char c;
{
}

/*
 * translate a name to a magic register offset
 * the latter useful in rget/rput
 */

int
rname(n)
char *n;
{

	return (BADREG);
}

rrest()
{
}
