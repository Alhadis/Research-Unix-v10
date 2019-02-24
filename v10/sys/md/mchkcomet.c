/*
 * Routines that deal closely with VAX-specific traps:
 * machine checks, memory errors, and the like
 */
#include "sys/param.h"
#include "sys/systm.h"
#include "sys/user.h"
#include "sys/mtpr.h"
#include "sys/psl.h"

#define	NMC750	8
char *mc750name[NMC750] = {
	"??",		"cs par",	"tbuf/bus/cache",	"??",
	"??",		"??",		"ucode lost",		"unused ird slot",
};

#define	BUS	2	/* the only summary code we care about */

#define	MCSR	0x17	/* machine check status */
#define	TBDR	0x24	/* translation buffer disable */
#define	CADR	0x25	/* cache disable */
#define	MCESR	0x26	/* machine check error flags */
#define	CAER	0x27	/* cache error flags */

/*
 * bits in various error registers
 */

#define	TBG1T	0x100	/* mcsr: trans buf group 1 tag error */
#define	TBG0T	0x80	/* mcsr: group 0 tag */
#define	TBG1D	0x40	/* mcsr: group 1 data */
#define	TBG0D	0x20	/* mcsr: group 0 data */
#define	NXM	0x8	/* mcsr: non-existent memory/read lock timeout */
#define	UCD	0x4	/* mcsr: uncorrectable data */
#define	TBPAR	0x4	/* mcesr: tb parity error */
#define	CDATA	0x4	/* cacherr: cache data error */
#define	CTAG	0x8	/* cacherr: cache tag error */

/*
 * saved mode from machine check
 */

#define	MODE	03	/* mask for just mode */
#define	PSLMSH	24	/* shift to get mode from psl */

/*
 * control bits
 */
#define	CACHEON	0	/* cadr */
#define	CACHEOFF 1	/* cadr */
#define	REPL	0x8	/* tbdr: force replace */
#define	RPL1	0x4	/* tbdr: which group to replace */
#define	TBMG1	0x2	/* tbdr: force miss group 1 */
#define	TBMG0	0x1	/* tbdr: force miss group 0 */

struct mc750frame {
	int	bcnt;		/* byte count == 0x28 */
	int	summary;	/* summary parameter (as above) */
	int	va;		/* virtual address register */
	int	errpc;		/* error pc */
	int	mdr;
	int	svmode;		/* saved mode register */
	int	rdtimo;		/* read lock timeout */
	int	tbgpar;		/* tb group parity error register */
	int	cacherr;	/* cache error register */
	int	buserr;		/* bus error register */
	int	mcesr;		/* machine check status register */
	int	pc;		/* trapped pc */
	int	psl;		/* trapped psl */
};

/*
 * write timeout trap
 * panic if in kernel mode
 * trap if user mode
 */

wtimeout(ps, pc)
long ps, pc;
{

	if (USERMODE(ps)) {
		/*
		 * code stolen from setrun
		 */
		runrun++;
		aston();
		psignal(u.u_procp, SIGBUS);
		return;
	}
	printf("wtmo pc %x\n", pc);
	panic("wtmo");
}

/*
 * Machine check.
 * If possible, recover and return;
 * if not but in user mode, send a signal;
 * if not and in kernel mode, panic.
 */

machinecheck(ps, f)
long ps;
struct mc750frame *f;
{
	int ok;

	mtpr(CADR, CACHEOFF);		/* should really be much earlier */
	ok = mckrec(f);
	machreset();
	printf("machine check type x%x:", f->summary);
	if (0 <= f->summary && f->summary < NMC750)
		printf(" %s\n", mc750name[f->summary]);
	else
		printf("\n");
	printf("\tva %x errpc %x mdr %x smr %x rdtimo %x tbgpar %x cacherr %x\n",
	    f->va, f->errpc, f->mdr, f->svmode,
	    f->rdtimo, f->tbgpar, f->cacherr);
	printf("\tbuserr %x mcesr %x pc %x psl %x mcsr %x\n",
	    f->buserr, f->mcesr, f->pc, f->psl,
	    mfpr(MCSR));
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
 * table of resumable instructions
 * a table of bits, indexed by opcode
 * this is taken straight from vms;
 * don't ask me to justify choices
 */

static char mrestab[256/8] = {
	0x6b,	/* rei ret svpctx */
	0x0f,	/* probe? insque remque */
	0xbf,	/* jsb */
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
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
	0x03,	/* pushr popr chm? */
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0xff,
	0x01,	/* cvtlp callg calls xfc reserved */
};

/*
 * code to decide if machine check is recoverable,
 * and recover if possible
 */

static time_t lastmchk;
static int mchkcache = CACHEON;

mckrec(f)
register struct mc750frame *f;
{
	register int x;

	mtpr(TBIA, 0);
	if (f->summary != BUS)
		return (0);
	if (f->mcesr & TBPAR) {
		printf("tb err\n");
		x = mfpr(TBDR);
		if (f->tbgpar & (TBG0D|TBG0T)) {
			if (x & TBMG1) {
				printf("both groups bad\n");
				death();
			}
			x = TBMG0 | RPL1 | REPL;
		}
		else {
			if (x & TBMG0) {
				printf("both groups bad\n");
				death();
			}
			x = TBMG1 | REPL;
		}
		if (lastmchk == time) {
			mtpr(TBDR, x);
			printf("g%d off\n", x & TBMG0 ? 0 : 1);
		}
		return (1);
	}
	if (f->cacherr & (CDATA | CTAG)) {
		printf("cache err\n");
		mtpr(CAER, f->cacherr);	/* clear error bits */
		mchkcache = CACHEOFF;
	}
	else if (f->buserr & UCD) {
		printf("hard mem err\n");
		memerr();
		return (0);	/* can't recover */
	}
	else if (f->buserr & NXM) {
		printf("nxm\n");
		return (0);	/* can't recover */
	}
	else {
		printf("unknown problem\n");
		return (0);
	}
	/*
	 * if we get here, it's potentially recoverable
	 * may recover if:
	 * haven't had one in the last second
	 * and instruction didn't change modes (?)
	 * and resumable instruction
	 */
	if (lastmchk == time)
		return (0);
	lastmchk = time;
	if ((f->svmode & MODE) != ((f->psl >> PSLMSH) & MODE))
		return (0);
	x = *(unsigned char *)f->pc;
	if (mrestab[x >> 3] & (1 << (x & 07)))
		return (1);
	return (0);
}

/*
 * reset processor error registers
 * call if we get a machine check that's really ok,
 * or perhaps when the system is started
 */

machreset()
{
	mtpr(MCESR, 0xf);
	mtpr(CADR, mchkcache);
}
