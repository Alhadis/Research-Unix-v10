/*
 * to do:
 *	check overflow of user read (bp->b_count)
 *	does the address loaded into the registers
 *	have to be on a 2k boundary?
 */
#include "sys/param.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/ubaddr.h"
#include "sys/pte.h"

#include "sys/rco.h"
#include "sys/rcocmd.h"

/*
 * hardware
 */
struct device {
	u_short csr;		/* control/status register */
	u_short vr;		/* virtual register (write only)*/
	u_short ar;		/* address register */
	u_short xar;		/* extended address register */
};

#define	GO	01
#define ERROR	016		/* 02 = FAULT, 04 = TIMEOUT, 010 = OVERFLO */
#define RESL	060
#define HALF	0100
#define READY	0200
#define UBACT	07400
#define UBALOW	010000
#define TEOP	020000
#define	IENABLE	040000

#define NUBMREG	4
#define IOCHUNK (NBPG*NUBMREG)

/*
 * config glue
 */
int rcoopen(), rcoclose(), rcoread(), rcoioctl();

extern struct rco rco[];
extern struct ubaddr rcoaddr[];
extern int rcocnt;
struct cdevsw rcocdev = cdinit(rcoopen, rcoclose, rcoread, nodev, rcoioctl);

#define OPEN	0x01
#define BUSY	0x02

rcoopen(d, f)
{
	register int dev;
	register struct rco *rp;

	if((dev = minor(d)) >= rcocnt) {
		u.u_error = ENODEV;
		return;
	}
	if((rp = &rco[dev])->flags&OPEN) {
		u.u_error = EBUSY;
		return;
	}
	if((rp->addr = (struct device *)ubaddr(&rcoaddr[dev])) == 0
	  || ubbadaddr(rcoaddr[dev].ubno, (caddr_t)rp->addr, sizeof(u_short))) {
		printf("rco%d absent\n", dev);
		u.u_error = ENODEV;
		return;
	}
	rp->flags = OPEN;
	rp->addr->csr = 0;
	rp->dither = 0;
	rp->resol = 2;
}

rcoclose(d)
{
	rco[minor(d)].flags &= ~OPEN;
}

rcominp(bp)
struct buf *bp;
{
}

rcostrategy(bp)
register struct buf *bp;
{
	register struct rco *rp = &rco[minor(bp->b_dev)];
	register uaddr_t uaddr;
	register int s, ubno;
	u_short csr;

	csr = IENABLE | ((rp->dither << 6) & HALF) | ((rp->resol << 4) & RESL);
	ubno = rcoaddr[minor(bp->b_dev)].ubno;
	rp->ubm = ubmalloc(ubno, IOCHUNK+(IOCHUNK-1), UBDP|USLP);
	rp->chunkubm = (rp->ubm + (NUBMREG-1)) & ~(NUBMREG-1);
	rp->pte = btopte(bp);
	uaddr = ubmsetmap(ubno, rp->pte, NUBMREG, rp->chunkubm);
	rp->pte += NUBMREG;
	s = spl5();
	rp->flags |= BUSY;
	rp->addr->ar = uaddr & 0x7FE;		/* bits 10-1 */
	rp->addr->xar = (uaddr >> 8) & 0x3FF8;	/* bits 21-11 */
	while((rp->addr->csr&READY) == 0)
		;
	rp->addr->csr = csr;
	for(rp->mreg = NUBMREG; rp->mreg; rp->mreg--)
		rp->addr->vr = 1;
	DELAY(50000);
	rp->addr->csr = csr | GO;
	if(tsleep((caddr_t)bp, PRIBIO+1, 20) != TS_OK) {
		bp->b_flags |=  B_ERROR;
		iodone(bp);
	}
	rp->addr->csr = 0;
	rp->flags &= ~BUSY;
	splx(s);
	bp->b_resid = 0;
	ubmfree(ubno, rp->ubm);
}

rcoread(d)
{
	if((((int)u.u_base) & (IOALIGN-1))) {
		u.u_error = EFAULT;
		return;
	}
	physio(rcostrategy, &rco[minor(d)].buf, d, B_READ, rcominp);
}

rcoioctl(dev, cmd, addr, flag)
caddr_t addr;
{
	register struct rco *rp = &rco[minor(dev)];
	int i;

	if(copyin(addr, (caddr_t)&i, sizeof(i))) {
		u.u_error = EFAULT;
		return;
	}
	switch(cmd) {

	case RCORES:
		rp->resol = i;
		break;

	case RCODITHER:
		rp->dither = i;
		break;

	default:
		u.u_error = ENXIO;
	}
}

rco0int(d)
{
	register struct rco *rp = &rco[d];
	register struct buf *bp;
	ubm_t ubm;
	u_short csr;

	if((rp->flags & (BUSY|OPEN)) != (BUSY|OPEN)) {
		printf("rco%d: not busy interrupt\n", d);
		rp->addr->csr = 0;
		return;
	}
	if((csr = rp->addr->csr) & GO) {
		ubm = rp->chunkubm + rp->mreg++;
		(void) ubmsetmap(rcoaddr[d].ubno, rp->pte++, 1, ubm);
		if(rp->mreg > (NUBMREG-1))
			rp->mreg = 0;
		rp->addr->vr = 1;
		return;
	}
	bp = &rp->buf;
	if((csr & ERROR) || (csr & TEOP) == 0) {
		printf("rco%d: csr=0%o\n", d, csr);
		bp->b_flags |= B_ERROR;
	}
	rp->addr->csr = 0;
	iodone(bp);
}
