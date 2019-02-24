/*
 * nautilus-specific traps
 */

#include "sys/param.h"
#include "sys/mtpr.h"
#include "sys/psl.h"
#include "sys/systm.h"
#include "sys/user.h"

extern long *mcrcsr;		/* must read mem csr to clear NMI faults! */

/*
 * nautilus-specific registers and bits
 */

#define	MCSTS	0x26	/* machine check status */
#define	NMIFSR	0x82	/* NMI fault status */
#define	NMIEAR	0x84	/* error address */
#define	CLRTOS	0x88	/* clear timeout sts */
#define	COR	0x85	/* cache on */

/*
 * nmi fault
 */

nmiflt()
{
	register long fsr, addr;
	register junk;

	fsr = mfpr(NMIFSR);
	addr = mfpr(NMIEAR);
	junk = mcrcsr[5];	/* sigh */
	printf("nmi fault %x addr %x\n", fsr, addr);
}

/*
 * machine check things
 */
struct mck {
	int count;
	int mcsts;
	int upc;
	int va;
	int iber;
	int cber;
	int eber;
	int nmifsr;
	int nmiear;
	int pc;
	int ps;
};


#define	MCABORT	0x1		/* mcsts: can't possibly restart */

#define	FSRBCOD	0x0c000000	/* nmifsr: timeout buffer code */
#define	RTO	0x08000000	/* read timeout */
#define	WTO	0x04000000	/* write timeout */

#define	CBCERR	0x00303f00	/* cber: cache error */
#define	CBTERR	0x00070010	/* cber: trans buf error */
#define	CBRDS	0x00000006	/* cber: probably memory RDS */
#define	CBOXBAD	0x00000008	/* cber: various fatal errors */

#define	IBOXBAD	0x000007ff	/* iber: various fatal errors */

#define	EBBERR	0x80		/* eber (each byte): b-side error */
#define	EBAERR	0x40		/* eber: a-side error */
#define	EBBSRC	0x38		/* eber: b-side source */
#define	EBASRC	0x03		/* eber: a-side source */

static time_t lastcache;
static int cacheon;
/*
 * machine check
 * if we get here, there's no desire to trap it with nofault
 */
machinecheck(ps, f)
long ps;
struct mck *f;
{
	register ok;

	cacheon = mfpr(COR);
	mtpr(COR, 0);		/* in case that's what's broken */
	/*
	 * always print for now
	 * delays prevent console congestion
	 */
	printf("machine check:\n");
	DELAY(100000);
	printf("mcsts %x upc %x va %x\n", f->mcsts, f->upc, f->va);
	DELAY(100000);
	printf("iber %x cber %x eber %x\n", f->iber, f->cber, f->eber);
	DELAY(100000);
	printf("nmifsr %x nmiear %x\n", f->nmifsr, f->nmiear);
	printf("pc %x ps %x\n", f->pc, f->ps);
	DELAY(100000);
	ok = mckrec(f);
	machreset();
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
 * decide if the machine check is recoverable
 * logic inspired by VAX/VMS
 * roughly, if the microcode said it wasn't restartable (MCABORT)
 * or if it seems a sufficiently nasty error (bus timeout, most ibox errors),
 * say no.
 */

mckrec(f)
struct mck *f;
{
	register int i, x;

	switch (f->nmifsr & FSRBCOD) {
	case RTO:
		printf("read timeout\n");
		break;

	case WTO:
		printf("write timeout\n");
		break;
	}
	if (f->cber & CBCERR) {
		printf("cache\n");
		if (time < lastcache + 5) {
			cacheon = 0;
			printf("turned off\n");
		}
		lastcache = time;
	}
	if (f->cber & CBTERR)
		printf("tbuf\n");
	if (f->cber & CBRDS)
		printf("mchk hard mem err\n");
	/* call mem err intr? */
	mtpr(COR, cacheon);
	if (f->mcsts & MCABORT)
		return (0);
	if (f->iber & IBOXBAD
	||  f->cber & CBOXBAD)
		return (0);
	if ((f->nmifsr & FSRBCOD) != 0)
		return (0);
	for (x = f->eber, i = 0; i < 4; x >>= 8, i++) {
		if (x & EBAERR)
			switch (x & EBASRC) {
			case 0:
			case 4:
				return (0);
			}
		if (x & EBBERR)
			switch (x & EBBSRC) {
			case 4:
				return (0);
			}
	}
	return (1);
}

/*
 * reset things after a machine check
 */

machreset()
{
	register junk;

	mtpr(MCSTS, mfpr(MCSTS));
	if (mcrcsr)
		junk = mcrcsr[5];
	mtpr(CLRTOS, 0);
}
