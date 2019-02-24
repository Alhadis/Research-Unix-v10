/*
 * Routines that deal closely with VAX-specific traps:
 * machine checks, memory errors, and the like
 */
#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/mtpr.h"
#include "sys/psl.h"
#include "sys/qbio.h"

extern char *iospace;

/*
 * interesting bits in mser
 */

#define	DMAQPE	020
#define	CPUQPE	040
#define	CPULPE	0100
#define	CPUNXM	0200
#define	MEMCD0	0400
#define	MEMCD1	01000
#define	MEMCD	(MEMCD0|MEMCD1)

/*
 * Machine check error recovery code.
 * Print the machine check frame and reset things.
 * Never recoverable in Mayflower.
 * If in user mode, send a signal;
 * if not and in kernel mode, panic.
 */

machinecheck(ps, f)
long ps;
caddr_t f;
{
	int ok;

	mckrec(f);
	machreset();
	if (USERMODE(ps)) {
		/*
		 * code stolen from setrun
		 */
		runrun++;
		aston();
		psignal(u.u_procp, SIGBUS);
		return;
	}
	panic("mchk");
}

struct mc630frame {
	long count;
	long code;
	long va;
	long state;
	long pc;
	long psl;
};

mckrec(f)
register struct mc630frame *f;
{
	register char *pr;
	register struct cpuregs *c;
	register long m;

	printf("machine check type0x%x va0x%x st0x%x pc0x%x ps0x%x\n",
		f->code, f->va, f->state, f->pc, f->psl);
	switch (f->code) {
	case 0x80:
	case 0x81:
		pr = "read";
		break;

	case 0x82:
	case 0x83:
		pr = "write";
		break;

	default:
		return;
	}
	/*
	 * read or write error: what kind?
	 */
	c = &((struct iomflow *)iospace)->c;
	m = c->mser;
	printf("%s err, mser0x%x\n", pr, m);
	if (m & DMAQPE)
		printf("qbus dma par, dear0x%x\n", c->dear);
	if (m & CPUQPE)
		printf("qbus cpu par, cear0x%x\n", c->cear);
	if (m & CPULPE)
		printf("local mem par, cear0x%x\n", c->cear);
	if (m & CPUNXM)
		printf("nxm\n");
	if (m & (CPULPE|CPUQPE)) {
		switch (m & MEMCD) {
		case 0:
			pr = "Q22";
			break;

		case MEMCD0:
			pr = "onboard mem";
			break;

		default:
			pr = "exp mem";
			break;
		}
		printf("from %s\n", pr);
	}
}

/*
 * reset processor error registers
 */

machreset()
{
	register struct cpuregs *c;

	if (iospace == 0)
		return;
	c = &((struct iomflow *)iospace)->c;
	c->mser = c->mser;
}
