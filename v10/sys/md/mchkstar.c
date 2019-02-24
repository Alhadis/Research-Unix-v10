/*
 * routines that deal closely with
 * machine checks and the like
 * VAX-11/780 version
 */

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/psl.h"
#include "sys/mtpr.h"
#include "sys/user.h"

/*
 * SBI-related registers
 */

#define	SBIFS	48		/* SBI fault and status */
#define	SBIMT	51		/* SBI and cache maint */
#define	SBIER	52		/* SBI error */
#define	SBITA	53		/* SBI timeout address */

#define	FSNEF	0x2000000	/* nested error flag in fault status */
#define	FSIE	0x40000		/* interrupt enable for sbi faults */

#define	ERCLR	0x70c0		/* latches in error register: */
				/* ib timeout, ib rds, cp timeout, rds, crd */

#define	TAMODE	0xc0000000	/* mode of failure for wtmo, 3 == user */
#define	TAPROT	0x20000000	/* protection checked reference; off -> pt */

struct mframe {
	long count;
	long type;
	long ces;
	long upc;
	long vaddr;
	long dr;
	long tb0;
	long tb1;
	long paddr;
	long par;
	long sbi;
	long pc;
	long psl;
};

#define	NMCK	16		/* number of possible type codes */
static char *macher[NMCK] = {
	"read timeout",
	"control store parity",
	"trans buffer parity",
	"cache parity",
	"-",
	"read data substitute",
	"microcode lost",
	"-",
	"-",
	"-",
	"ib trans buffer parity",
	"-",
	"ib read data substitute",
	"ib read timeout",
	"-",
	"ib cache parity",
};

int mchkcnt[NMCK];	/* number of mchecks of each type */
int mchktime[NMCK];	/* time of last mcheck */

/*
 * table of resumable instructions
 * a table of bits, indexed by opcode
 * this is taken straight from vms;
 * don't ask me to justify choices
 */

static char mrestab[256/8] = {
	0x3b,	/* rei ldpctx svpctx */
	0x3c,	/* insque remque cvtps cvtsp */
	0xff,
	0xff,
	0x0,	/* packed decimal stuff */
	0xff,
	0xff,
	0xfe,	/* editpc */
	0xff,
	0xff,
	0x2f,	/* emodf cvtfd adawi */
	0x0,	/* more interlocked instructions */
	0x0,	/* double precision floating point */
	0x0f,	/* more double stuff */
	0x4a,	/* more double/quad */
	0xc1,	/* .. */
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xf3,	/* pushr popr */
	0xff,
	0xff,
	0xff,
	0xf4,	/* adwc sbwc mfpr */
	0x3f,	/* bbssi bbcci */
	0xff,
	0xff,
	0x0,	/* ashp cvtlp callg calls xfc reserved */
};

/*
 * bits in machine check type code
 */

#define	TYPE	0xf		/* the real type code */
#define	IB	0x8		/* set if detected by instruction prefetch */
#define	ABORT	0xf0		/* this was an abort */
#define	TIMEOUT	0x100		/* timeout pending */

/*
 * types
 */

#define	CPTIMEOUT	0	/* read timeout */
#define	CSPAR		1	/* control store parity */
#define	TBUFPAR		2	/* translation buffer parity */
#define	CACHEPAR	3	/* cache parity */
#define	RDSUBST		5	/* read data substitute */
#define	IBROMCHK	6	/* confused microcode */
#define	IBTBUF		10	/* ib-detected translation buffer parity */
#define	IBRDSUB		12	/* ib-detected read substitute */
#define	IBTIMEOUT	13	/* ib-detected read timeout */
#define	IBCACHE		15	/* ib-detected cache parity */

/*
 * bits in cache error register
 */

#define	G0OK	0x3f8		/* `ok' bits for group 0 */

/*
 * bits in sbi maint register
 */

#define	G1REPL	0x2000		/* force replace in group 1 */
#define	G0REPL	0x4000		/* force replace in group 0 */
#define	G1MISS	0x8000		/* force miss in group 1 */
#define	G0MISS	0x10000		/* force miss in group 0 */
#define	SBIINV	0x200000	/* enable sbi invalidate */

#define	CACHEOFF	(G0REPL|G1MISS|G0MISS|SBIINV)

static int mchkkill;	/* number of times we've recovered by killing */
static int mchklock;	/* reentrancy lock */
static int mchkcache = SBIINV;	/* current state of cache disable bits */

/*
 * sbi fault
 * yell and hope for the best
 */

sbifault()
{
	register int fs, er;

	fs = mfpr(SBIFS);
	er = mfpr(SBIER);
	machreset();
	printf("sbi fault: fs %x er %x\n", fs, er);
}

/*
 * SBI write timeout
 * no recovery;
 * panic if kernel or if updating page table
 * else just signal
 */

machwtmo()
{
	register int fs, er, ta;

	fs = mfpr(SBIFS);
	er = mfpr(SBIER);
	ta = mfpr(SBITA);
	machreset();
	printf("sbi write timeout: fs %x er %x ta %x\n", fs, er, ta);
	if ((ta & (TAMODE | TAPROT)) != (TAMODE | TAPROT))
		panic("wtmo");
	runrun++;
	aston();
	psignal(u.u_procp, SIGBUS);
}

/*
 * clear latches in sbi error registers
 */

machreset()
{
	mtpr(SBIFS, (mfpr(SBIFS) &~ FSNEF)|FSIE);
	mtpr(SBIER, mfpr(SBIER) | ERCLR);
}

/*
 * Machine check.
 * If possible, recover and return;
 * if not but in user mode, send a signal;
 * if not and in kernel mode, panic.
 */

machinecheck(ps, f)
long ps;
struct mframe *f;
{
	int ok;

	ok = mckrec(f);
	machreset();
	printf("\nMachine check, type %x\n", f->type);
	if ((f->type & ABORT) == 0)
		printf("%s fault\n", macher[f->type & TYPE]);
	else
		printf("%s abort\n", macher[f->type & TYPE]);
	printf("pc %x psl %x\n", f->pc, f->psl);
	printf("v/p addr %x/%x\n", f->vaddr, f->paddr<<2);
	printf("ces %x sbi %x\n", f->ces, f->sbi);
	if (ok)
		return;
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
 * here to look at machine check type
 * and arrange for recovery if possible
 * cache is still turned off when we start
 * return nonzero if we recovered
 */

int
mckrec(f)
register struct mframe *f;
{
	register int x;

	mchkcnt[f->type & TYPE]++;
	switch (f->type & TYPE) {
	case TBUFPAR:		/* trans buffer parity */
	case IBTBUF:
		mtpr(TBIA, 0);	/* clear the buffer */
		mtpr(SBIMT, mchkcache);
		break;

	case IBROMCHK:		/* unexpected microcode confusion */
	case CSPAR:		/* control store parity */
		mtpr(SBIMT, mchkcache);
		return (0);	/* can never recover */

	case CACHEPAR:		/* cache parity error */
	case IBCACHE:
		/*
		 * force bad stuff to be replaced
		 * in both cache groups
		 */
		x = *(char *)f->vaddr;
		mtpr(SBIMT, CACHEOFF);
		x = 0;		/* defeat optimizer */
		x = *(char *)f->vaddr;
		if ((f->par & G0OK) != G0OK) {
			printf("group 0 off\n");
			mchkcache |= G0MISS | G0REPL;
			mchkcache &=~ G1REPL;	/* don't replace both groups */
		}
		else {
			printf("group 1 off\n");
			mchkcache |= G1MISS | G1REPL;
			mchkcache &=~ G0REPL;
		}
		mtpr(SBIMT, mchkcache);
		break;

	case CPTIMEOUT:
	case IBTIMEOUT:
		mtpr(SBIMT, mchkcache);
		break;

	case RDSUBST:		/* read data substitute == hard mem err */
	case IBRDSUB:
		mtpr(SBIMT, mchkcache);
		memerr();
		return (0);

	default:
		mtpr(SBIMT, mchkcache);
		panic("weird mcheck");
	}
	/*
	 * if we get here, it's potentially recoverable
	 * may recover if:
	 * haven't had one in the last second
	 * not an abort or timeout
	 * detected by lookahead, or resumable instruction
	 */
	if (mchktime[f->type & TYPE] == time)
		return (0);
	mchktime[f->type & TYPE] = time;
	if (f->type & (ABORT | TIMEOUT))
		return (0);
	if (f->type & IB)
		return (1);
	x = *(char *)f->pc & 0377;
	if (mrestab[x >> 3] & (1 << (x & 07)))
		return (1);
	return (0);
}
