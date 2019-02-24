/*
 * KMC11 microprocessor driver
 * quasi-general-purpose:
 * the code here is for downloading and controlling the kmc
 * dealing with particular downloaded code
 * will usually mean writing another driver
 * which, alas, must generally have magic knowledge of
 * struct kmc and the device registers
 *
 * the caller is expected to fill in
 * k_rint, k_xint, k_reset if it cares
 */

#define lobyte(X) (((unsigned char *)&X)[0])
#define hibyte(X) (((unsigned char *)&X)[1])

#include "sys/param.h"
#include "sys/kmc.h"
#include "sys/ubaddr.h"
#include "sys/conf.h"
#include "sys/user.h"

extern struct kmc kmc[];
extern struct ubaddr kmcaddr[];
extern int kmccnt;

#define	KMC11A	1
#define	KMC11B	2
#define	KASIZE	1024
#define	KBSIZE	4096

#define	RUN	(1<<7)
#define	MCLR	(1<<6)
#define	CWRT	(1<<5)
#define	LUB	(1<<4)
#define	LUA	(1<<3)
#define	ROMO	(1<<2)
#define	ROMI	(1<<1)
#define	STEP	(1<<0)

#define RDYO	0200
#define RDYI	020
#define RQI	0200
#define IEI	01
#define IEO	020

#define	STYPE	017
#define SRUN	020
#define SRINT	040
#define	SOPEN	0100

struct device {
	union {
		char	b[8];
		u_short	w[4];
	} un;
};

#define	bsel0	un.b[0]
#define	bsel1	un.b[1]
#define	bsel2	un.b[2]
#define	bsel3	un.b[3]
#define	bsel4	un.b[4]
#define	bsel5	un.b[5]
#define	bsel6	un.b[6]
#define	bsel7	un.b[7]
#define	sel0	un.w[0]
#define	sel2	un.w[1]
#define	sel4	un.w[2]
#define	sel6	un.w[3]

int kmcopen(), kmcclose(), kmcread(), kmcwrite(), kmcioctl();
struct cdevsw kmccdev = cdinit(kmcopen, kmcclose, kmcread, kmcwrite, kmcioctl);

kmcopen(dev, flag)
dev_t dev;
{	register struct device *kp;
	register struct kmc *tp;
	register sav;

	dev = minor(dev);
	if (dev>=kmccnt || (tp = &kmc[dev])->k_stat&SOPEN) {
		u.u_error = ENXIO;
		return;
	}
	if ((kp = (struct device *)ubaddr(&kmcaddr[dev])) == 0
	||  ubbadaddr(kmcaddr[dev].ubno, kp, sizeof(short))) {
		printf("kmc%d absent\n", dev);
		u.u_error = ENXIO;
		return;
	}
	tp->k_addr = kp;
	tp->k_written = 0;
	tp->k_stat |= SOPEN;
	if (tp->k_type==0) {
		kp->bsel1 = ROMO;
		kp->sel4 = 0;
		sav = kp->sel6;
		kp->sel6 = ~sav;
		if (kp->sel6 != sav) {
			tp->k_type = KMC11B;
			kp->sel6 = sav;
		} else
			tp->k_type = KMC11A;
		kp->bsel1 = 0;
	}
}

kmcclose(dev)
dev_t dev;
{
	register struct kmc *tp;

	tp = &kmc[minor(dev)];
	tp->k_stat &= ~SOPEN;
	if (tp->k_written && tp->k_reset)
		(*tp->k_reset)(minor(dev));
}

kmcread(dev)
register dev_t dev;
{	register struct device *kp;
	register ad;
	int	dsize;
	u_short	sav;

	dev = minor(dev);
	if (kmc[dev].k_stat&SRUN)
		return;
	dsize = (kmc[dev].k_type==KMC11A)?KASIZE:KBSIZE;
	kp = kmc[dev].k_addr;
	kp->bsel1 = 0;
	do {
		ad = Ltol(u.u_offset);
		if (ad<dsize*2) {
			if (ad&1) {
				u.u_error = ENXIO;
				break;
			}
			ad >>= 1;
			kp->bsel1 = ROMO;
			kp->sel4 = ad;
			passc(kp->bsel6);
			passc(kp->bsel7);
			kp->bsel1 = 0;
		} else if (ad -= dsize*2, ad<dsize) {
			kp->bsel1 = ROMO;
			kp->sel4 = 0;
			sav = kp->sel6;
			kp->bsel1 = ROMI;
			kp->sel6 = 010000|(ad&0377);	/* mov ad,mar */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->sel6 = 04000|((ad>>8)&0377);	/* mov %ad,%mar */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->sel6 = 055222;	/* mov mem,csr2|mar++ */
			kp->bsel1 = ROMI|STEP;
			passc(kp->bsel2);
			kp->bsel1 = ROMI;
			kp->sel6 = sav;
			kp->bsel1 = 0;
		} else
			break;
	} while (u.u_error==0 && u.u_count);
}

kmcwrite(dev)
register dev_t dev;
{	register struct device *kp;
	register ad;
	int	dsize;
	short	ins;
	u_short	sav;

	dev = minor(dev);
	if (kmc[dev].k_stat&SRUN)
		return;
	dsize = (kmc[dev].k_type==KMC11A)?KASIZE:KBSIZE;
	kp = kmc[dev].k_addr;
	kp->bsel1 = 0;
	kmc[dev].k_written = 1;
	while (u.u_error==0 && u.u_count) {
		ad = Ltol(u.u_offset);
		if (ad<dsize*2) {
			if (ad&1) {
				u.u_error = ENXIO;
				break;
			}
			kp->bsel1 = ROMO;
			kp->sel4 = ad>>1;
			lobyte(ins) = cpass();
			hibyte(ins) = cpass();
			kp->sel6 = ins;
			kp->bsel1 |= CWRT;
			kp->bsel1 = 0;
		} else if (ad -= dsize*2, ad<dsize) {
			kp->bsel1 = ROMO;
			kp->sel4 = 0;
			sav = kp->sel6;
			kp->bsel1 = ROMI;
			kp->sel6 = 010000|(ad&0377);	/* mov ad,mar */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->sel6 = 04000|((ad>>8)&0377);	/* mov %ad,%mar */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->bsel2 = cpass();
			kp->sel6 = 0136440;	/* mov csr2,mem|mar++ */
			kp->bsel1 = ROMI|STEP;
			kp->bsel1 = ROMI;
			kp->sel6 = sav;
			kp->bsel1 = 0;
		} else
			break;
	}
}

kmcioctl(dev, cmd, arg, mode)
caddr_t arg;
register dev_t dev;
{	register struct device *kp;
	register struct kmc *tp;
	struct kmcntl kk;
	short	csr[4];
	u_short	sav;

	dev = minor(dev);
	if (cmd != KCSETA) {
		u.u_error = EINVAL;
		return;
	}
	if (copyin(arg, &kk, sizeof(kk))) {
		u.u_error = EFAULT;
		return;
	}
	tp = &kmc[dev];
	kp = tp->k_addr;
	switch (kk.kmd) {
	case KMCLR:
	case KRESET:
		spl7();
		kp->bsel1 = MCLR;
		spl0();
	case KSTOP:
		tp->k_stat &= ~SRUN;
		kp->bsel1 = 0;
		if (kk.kmd == KRESET) {
			tp->k_stat = 0;
			/* flush here */
		}
		return;
	case KMS:
		if (tp->k_stat&SRUN)
			break;
		kp->bsel1 = ROMI|ROMO;
		sav = kp->sel6;
		kp->bsel1 = ROMI;
		kp->sel6 = kk.kval;
		kp->bsel1 = ROMI|STEP;
		kp->bsel1 = ROMI;
		kp->sel6 = sav;
		kp->bsel1 = 0;
		goto lcsr;
	case KSTEP:
		if (tp->k_stat&SRUN)
			break;
		kp->bsel1 |= STEP;
		kp->bsel1 = 0;
	case KCSR:
	lcsr:
		csr[0] = kp->sel0;
		csr[1] = kp->sel2;
		csr[2] = kp->sel4;
		csr[3] = kp->sel6;
		if (copyout(csr, kk.kcsr, sizeof csr))
			u.u_error = EFAULT;
		return;
	case KWRCR:
		if (tp->k_stat&SRINT)
			break;
		kp->sel6 = kk.kval;
		return;
	case KRUN:
		if (tp->k_stat&SRUN)
			break;
		tp->k_stat &= ~STYPE;
		tp->k_stat |= (kk.kval&STYPE)|SRUN;
		kp->bsel1 |= RUN;
		if (tp->k_stat&SRINT) {
			spl5();
			kmc0int(dev);
			spl0();
		}
		return;
	case KLU:
		kp->bsel1 = kk.kval&(LUA|LUB);
		return;
	}
	u.u_error = EIO;
}

kmc0int(dev)
{
	register int (*p)();

	if ((p = kmc[dev].k_rint) != 0)
		(*p)(dev);
}

kmc1int(dev)
{
	register int (*p)();

	if ((p = kmc[dev].k_xint) != 0)
		(*p)(dev);
}
