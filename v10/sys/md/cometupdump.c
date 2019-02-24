/*
 * code to take a crash dump
 * on a disk attached to an SMD disk controller
 * assumes UP at standard address
 * (but so does the boot code)
 * calls up.c (unfortunately) to figure out disk geometry
 */

#include "sys/param.h"
#include "sys/up.h"

/*
 * hardware parameters
 */

#define	PHYS(x)	((long)(x)&~KSTART)
#define	UBMAP	((long *)0xf30800)	/* unibus map */
#define	UBREG	0xfc0000		/* addr 0 in unibus space */
#define	UP	((struct device *)(0776700+UBREG))

#define	MRV	0x80000000		/* map register valid bit */

#define	CHUNK	8			/* write this many sectors at once */
#define	SECSIZE	512			/* size of a sector */

/*
 * hardware registers
 */

struct device
{
	u_short	upcs1;		/* control and status register 1 */
	short	upwc;		/* word count register */
	u_short	upba;		/* UNIBUS address register */
	u_short	upda;		/* desired address register */
	u_short	upcs2;		/* control and status register 2 */
	u_short	upds;		/* drive status */
	u_short	uper1;		/* error register 1 */
	u_short	upas;		/* attention summary */
	u_short	upla;		/* look ahead */
	u_short	updb;		/* data buffer */
	u_short	upmr;		/* maintenance */ 
	u_short	updt;		/* drive type */
	u_short	upsn;		/* serial number */
	u_short	upof;		/* offset register */
	u_short	updc;		/* desired cylinder address register */
	u_short	uphr;		/* holding register */
	u_short	upmr2;		/* maintenance register 2 */
	u_short	uper2;		/* error register 2 */
	u_short	upec1;		/* burst error bit position */
	u_short	upec2;		/* burst error bit pattern */
};

/*
 * upcs1
 */

#define	UP_SC	0100000		/* special condition */
#define	UP_TRE	0040000		/* transfer error */
#define	UP_RDY	0000200		/* controller ready */
#define	UP_GO	0000001

#define	UP_DCLR		010		/* drive clear */
#define	UP_PRESET	020		/* read-in preset */
#define	UP_WCOM		060		/* write */

/*
 * upcs2
 */
#define	UPCS2_CLR	0000040		/* controller clear */

/*
 * upof
 */
#define	UPOF_FMT22	0010000		/* 16 bit format */

static upxtype;
extern struct upst upst[];

static upxwrite(), upxinit();

updump(unit, low, size)
int unit;
daddr_t low, size;
{
	register long p;
	register int i;
	extern int physmem;

	if (size > physmem)
		size = physmem;
	size -= CHUNK-1;
	if (upxinit(UP, unit))
		return (1);
	for (p = 0; p < size; p += CHUNK) {
		for (i = 0; i < CHUNK; i++)
			UBMAP[i] = MRV|(p+i);
		if (upxwrite(UP, unit, low+p))
			return (1);
	}
	return (0);
}

/*
 * clear the controller and reset it
 * hereafter, upcs2 has the unit number we want (we assume)
 */
static
upxinit(rp, unit)
register struct device *rp;
int unit;
{

	rp->upcs2 = UPCS2_CLR;
	if ((upxtype = uputype(rp, unit)) < 0) {
		printf("updump: can't init\n");
		return (1);
	}
	rp->upcs1 = UP_PRESET|UP_GO;
	rp->upof = UPOF_FMT22;
	return (0);
}

static
upxwrite(rp, unit, bno)
register struct device *rp;
int unit;
register long bno;
{
	register struct upst *sp;
	register int ts;		/* track and sector */

	sp = &upst[upxtype];
	ts = bno % sp->nsect;
	bno /= sp->nsect;
	ts |= (bno%sp->ntrak)<<8;
	bno /= sp->ntrak;
	rp->updc = bno;
	rp->upda = ts;
	rp->upwc = -(CHUNK*SECSIZE)/2;
	rp->upba = 0;
	rp->upcs1 = UP_WCOM|UP_GO;
	while ((rp->upcs1 & UP_RDY) == 0)
		;
	if (rp->upcs1 & (UP_TRE|UP_SC))
		return (1);
	return (0);
}
