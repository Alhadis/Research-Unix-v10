/*
 * machine-dependent code for controlling a process
 * this for cray
 */

#include "defs.h"
#include "bkpt.h"
#include "regs.h"
#include "space.h"

/*
 * install (f != 0) or remove (f == 0) a breakpoint
 */

#define	BPT	0	/* err */

extern ADDR txtsize;
bkput(bk, f)
register BKPT *bk;
{
	register int sp;

	if (bk->loc < txtsize)
		sp = CORF | INSTSP;
	else
		sp = CORF | DATASP;
	if (f == 0)
		sput(bk->loc, sp, wtos(bk->ins));
	else {
		bk->ins = stow(sget(bk->loc, sp));
		sput(bk->loc, sp, wtos(BPT));
		if (errflg) {
			printf("cannot set breakpoint: ");
			/* stuff */
			prints(errflg);
		}
	}
}
