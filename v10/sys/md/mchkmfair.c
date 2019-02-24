/*
 * Routines that deal closely with VAX-specific traps:
 * machine checks, memory errors, and the like
 * MicroVAX III version
 */
#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/mtpr.h"
#include "sys/psl.h"
#include "sys/qbio.h"

extern char *iospace;

/*
 * Machine check error recovery code.
 * Print the machine check frame and reset things.
 * If in user mode, send a signal;
 * if not and in kernel mode, panic.
 */

struct mc650frame {
	long count;
	long code;
	long va;
	long state1;
	long state2;
	long pc;
	long psl;
};

#define	CANTRST	0x8000		/* state2: instruction can't restart */

#define	MCTMAX	0xa
#define	MCTBUS	0x80
#define	MCTBUSMAX 0x83

static char *mctype[] = {
	"",
	"fpa proto", "fpa resins", "fpa sts", "fpa sts",
	"mmu", "mmu", "mmu", "mmu",
	"intr bad ipl", "movc"
};

static char *mcbus[] = {
	"read", "read", "write", "write"
};

machinecheck(ps, f)
long ps;
register struct mc650frame *f;
{
	int ok;
	char *name;

	ok = mckrec(f);
	if (f->code <= MCTMAX)
		name = mctype[f->code];
	else if (f->code >= MCTBUS && f->code <= MCTBUSMAX)
		name = mcbus[f->code - MCTBUS];
	else
		name = "";
	printf("machine check type0x%x %s va0x%x st0x%x 0x%x pc0x%x ps0x%x\n",
		f->code, name, f->va, f->state1, f->state2, f->pc, f->psl);
	if (ok)
		return;		/* recovered */
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

/*
 * sniff at the machine check,
 * and decide if it's ok to recover
 * -- memerr calls machreset; it's not worthwhile for other errors
 * -- the hardware disables the cache if necessary
 */

mckrec(f)
register struct mc650frame *f;
{

	switch (f->code) {
	case 0x1: case 0x2: case 0x3: case 0x4:		/* fpa errors */
		break;

	case 0x5: case 0x6: case 0x7: case 0x8:		/* mmu errors; can't recover */
		printf("p0br %x lr %x p1br %x lr %x sbr %x lr %x\n",
			mfpr(P0BR), mfpr(P0LR), mfpr(P1BR), mfpr(P1LR),
			mfpr(SBR), mfpr(SLR));
		return (0);

	case 0x9:			/* interrupt at illegal ipl */
	case 0xa:			/* microcode error in movc */
		return (0);

	case 0x80: case 0x81:		/* bus error on read */
		memerr();
		break;

	case 0x82: case 0x83:		/* bus error on write; can't recover */
		memerr();
		return (0);
	}
	/*
	 * might be able to recover
	 */
	if (f->psl & PSL_FPD || (f->state2 & CANTRST) == 0)
		return (1);
	return (0);
}
