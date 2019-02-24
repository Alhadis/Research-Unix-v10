/*
 * code to take a crash dump
 * on a disk attached to an SMD disk controller
 * calls hp.c (unfortunately) to figure out disk geometry
 * unit number is drive | (mba number << 8)
 */

#include "sys/param.h"
#include "sys/hp.h"
#include "sys/mbsts.h"

#define	MBANO(u)	(((u)>>8)&0xf)
#define	DRVNO(u)	((u)&0xf)

/*
 * hardware parameters
 */

#define	PHYS(x)	((long)(x)&~KSTART)
#define	MAXMBA	4
static long mbxaddr[] = {0xf28000, 0xf2a000, 0xf2c000, 0xf2e000};

#define	MBAUNITS	8
#define	NMAP	256

struct mbaregs {
	long conf;	/* configuration register; unused */
	long cr;	/* control register */
	long sr;	/* status register */
	long va;	/* address */
	long bcr;	/* count */
	long _junk0[251];	/* pad up to 0x400 */
	long devreg[MBAUNITS][32];
	long map[NMAP];
};

#define	CRINIT	0x1	/* cr -- init adapter */
#define	MRV	0x80000000	/* map register valid bit */

#define	CHUNK	8			/* write this many sectors at once */
#define	SECSIZE	512			/* size of a sector */

/*
 * disk registers
 */

struct hpdevice {
	int	hpcs1;		/* control and status register 1 */
	int	hpds;		/* drive status */
	int	hper1;		/* error register 1 */
	int	hpmr;		/* maintenance */ 
	int	hpas;		/* attention summary */
	int	hpda;		/* desired address register */
	int	hpdt;		/* drive type */
	int	hpla;		/* look ahead */
	int	hpsn;		/* serial number */
	int	hpof;		/* offset register */
	int	hpdc;		/* desired cylinder address register */
	int	hpcc;		/* current cylinder */
/* on an rp drive, mr2 is called er2 and er2 is called er3 */
/* we use rm terminology here */
	int	hpmr2;		/* maintenance register 2 */
	int	hper2;		/* error register 2 */
	int	hpec1;		/* burst error bit position */
	int	hpec2;		/* burst error bit pattern */
};

/*
 * hpcs1
 */

#define	HP_GO	0000001

#define	HP_DCLR		010		/* drive clear */
#define	HP_PRESET	020		/* read-in preset */
#define	HP_WCOM		060		/* write */

/*
 * hpof
 */
#define	HPOF_FMT22	0010000		/* 16 bit format */

/*
 * hpds
 */
#define	HPDS_ERR	040000		/* error summary */

static hpxtype;
static struct hpdevice *hpxdev;
static struct mbaregs *hpxmba;
extern struct hptype hptype[];

static hpxwrite(), hpxinit();

hpdump(unit, low, size)
int unit;
daddr_t low, size;
{
	register long p;
	register int i;
	register long *map;
	extern int physmem;

	if (size > physmem)
		size = physmem;
	size -= CHUNK-1;
	if (hpxinit(unit))
		return (1);
	map = hpxmba->map;
	for (p = 0; p < size; p += CHUNK) {
		for (i = 0; i < CHUNK; i++)
			map[i] = MRV|(p+i);
		if (hpxwrite(low+p))
			return (1);
	}
	return (0);
}

/*
 * clear the controller and reset it
 * hereafter, upcs2 has the unit number we want (we assume)
 */
static
hpxinit(unit)
int unit;
{
	register struct hpdevice *rp;
	register int i;

	if ((i = MBANO(unit)) >= MAXMBA)
		return (1);
	hpxmba = (struct mbaregs *)mbxaddr[i];
	if ((i = DRVNO(unit)) >= MBAUNITS)
		return (1);
	hpxdev = rp = (struct hpdevice *)hpxmba->devreg[i];
	hpxmba->cr = CRINIT;
	if ((hpxtype = hputype(rp)) < 0) {
		printf("hpdump: can't init\n");
		return (1);
	}
	rp->hpcs1 = HP_DCLR|HP_GO;
	rp->hpcs1 = HP_PRESET|HP_GO;
	rp->hpof = HPOF_FMT22;
	return (0);
}

static
hpxwrite(bno)
register long bno;
{
	register struct hpdevice *rp;
	register struct mbaregs *mp;
	register struct hptype *sp;
	register int ts;		/* track and sector */

	rp = hpxdev;
	mp = hpxmba;
	sp = &hptype[hpxtype];
	ts = bno % sp->nsect;
	bno /= sp->nsect;
	ts |= (bno%sp->ntrak)<<8;
	bno /= sp->ntrak;
	rp->hpdc = bno;
	rp->hpda = ts;
	mp->bcr = -(CHUNK*SECSIZE);
	mp->va = 0;
	rp->hpcs1 = HP_WCOM|HP_GO;
	while ((mp->sr & MBSR_DTCMP) == 0)
		;
	if (mp->sr & MBSR_EBITS || rp->hpds & HPDS_ERR)
		return (1);
	return (0);
}
