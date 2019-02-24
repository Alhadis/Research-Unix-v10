/*
 * machine-dependent process control
 */

#include "defs.h"
#include "space.h"
#include "bkpt.h"

/*
 * install (f != 0) or remove (f == 0) a breakpoint
 */

#define	BPT	0xf2	/* NS32000 */

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
		cput(bk->loc, sp, wtoc(bk->ins));
	else {
		bk->ins = ctow(cget(bk->loc, sp));
		cput(bk->loc, sp, wtoc(BPT));
		if (errflg) {
			printf("cannot set breakpoint: ");
			/* stuff */
			prints(errflg);
		}
	}
}
