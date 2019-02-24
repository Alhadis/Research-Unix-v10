/*
 * MicroVAX III memory errors
 * bus errors too, as they come through the same trap
 */

#include "sys/param.h"
#include "sys/systm.h"	/* just for bloody HZ */
#include "sys/qbio.h"

extern char *iospace;
time_t mcrtime;		/* one or more */

int mcrtimeout = 60;		/* seconds between soft error reports */

/*
 * csr16
 */
#define	HARDERR	0x80000000	/* uncorrectable error */
#define	HARDLST	0x40000000	/* nested hard error */
#define	SOFTERR	0x20000000	/* corrected error */

/*
 * csr17
 */
#define	CRDENB	0x1000		/* enable reporting soft errors */

/*
 * mser
 */
#define	MSER	39
#define	C2ERR	0x60		/* second level cache or CDAL parity error */
#define	C1ERR	0x13		/* first level cache parity error */

/*
 * dser
 */
#define	QBERR	0xa0		/* cpu-to-qbus error */
#define	DMAERR	0x11		/* dma error */	

mcrinit()
{
	register struct iomfair *m;

	m = (struct iomfair *)iospace;
	m->c.memcsr16 = m->c.memcsr16;	/* clear error latches */
	m->c.memcsr17 = CRDENB;
}

mcrenable(junk)
caddr_t junk;
{
	((struct iomfair *)iospace)->c.memcsr17 = CRDENB;
}

/*
 * here on trap:
 * crd or rds interrupt
 * machine check type 0x80-0x83
 * we might be about to crash,
 * so chat just a little bit
 */

memerr()
{
	register struct iomfair *m;
	long csr16, csr17, ms, ds, qba, da;

	if ((m = (struct iomfair *)iospace) == NULL) {
		printf("mem err\n");
		return;
	}
	csr16 = m->c.memcsr16;
	csr17 = m->c.memcsr17;
	ms = mfpr(MSER);
	ds = m->c.dser;
	qba = m->c.qbear;
	da = m->c.dear;
	machreset();
	printf("mem trap mser %x dser %x csr16 %x\n", ms, ds, csr16);
	if (csr16 & (HARDERR|SOFTERR)) {
		printf("mem %s err; csr17 %x\n",
			csr16 & HARDERR ? "hard" : "soft", csr17);
		if (mcrtime != time)
			mcrtime = time;
		else {
			m->c.memcsr17 = 0;
			timeout(mcrenable, (caddr_t)0, mcrtimeout*HZ);
		}
	}
	if (ms & C2ERR)
		printf("cdal/2cache\n");
	if (ms & C1ERR)
		printf("1cache\n");
	if (ds & QBERR)
		printf("qbus addr %x\n", qba << 9);
	if (ds & DMAERR)
		printf("dma addr %x\n", da << 9);
	if (csr16 & HARDERR)
		panic("memerr");
}

/*
 * reset processor error registers
 */

machreset()
{
	register struct iomfair *q;

	mtpr(MSER, mfpr(MSER));
	if (iospace == 0)
		return;
	q = (struct iomfair *)iospace;
	q->c.dser = q->c.dser;
	q->d.cacr = q->d.cacr;
	q->c.memcsr16 = q->c.memcsr16;
}
