/*
 * Versatec model 122 matrix printer/plotter
 * dma interface driver
 *
 */
#include "sys/param.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/ubaddr.h"

#include "sys/vplot.h"
#include "sys/vplotcmd.h"

struct device {
	short	csr;
	short	buf;
};

#define	ERROR		0100000
#define	SPP		0020000
#define	BTCNT		0010000
#define	ADDR		0004000
#define	PP		0002000
#define	SWPBT		0001000
#define	READY		0000200
#define	IENABLE		0000100
#define XMEM		0000060
#define xmem(a)		(((a) >> 12) & XMEM)
#define	REMOTE		0000016
#define	GO		0000001

#define MASK		(SPP|PP|SWPBT|REMOTE)

#define	RESET		(06 << 1)

/*
 * config glue
 */
int vplotopen(), vplotclose(), vplotwrite(), vplotioctl();

extern struct vplot vplot[];
extern struct ubaddr vplotaddr[];
extern int vplotcnt;
struct cdevsw vplotcdev =
	cdinit(vplotopen, vplotclose, nodev, vplotwrite, vplotioctl);

#define	VPLOTPRI	(PZERO-1)

vplotopen(d, f)
{
	register int dev;
	register struct vplot *vp;

	if((dev = minor(d)) >= vplotcnt) {
		u.u_error = ENODEV;
		return;
	}
	if((vp = &vplot[dev])->open) {
		u.u_error = EBUSY;
		return;
	}
	if((vp->addr = (struct device *)ubaddr(&vplotaddr[dev])) == 0
	  || ubbadaddr(vplotaddr[dev].ubno, (caddr_t)vp->addr, sizeof(u_short))) {
		printf("vplot%d absent\n", dev);
		u.u_error = ENODEV;
		return;
	}
	vp->open = 1;
	vp->addr->csr = RESET;
	if(vplotwait(vp)) {
		u.u_error = ENXIO;
		vplotclose(d);
		return;
	}
	vp->addr->csr |= IENABLE;
	vplottimo(vp);
	if(u.u_error)
		vplotclose(d);
}

vplotclose(d)
{
	register struct vplot *vp = &vplot[minor(d)];

	vp->open = 0;
	vp->addr->csr = 0;
}

vplotstrategy(bp)
register struct buf *bp;
{
	register struct vplot *vp = &vplot[minor(bp->b_dev)];
	register ubm_t ubm;
	register uaddr_t uaddr;
	register int cc, e, s;

	s = spl4();
	(void) vplotwait(vp);
	ubm = ubmbuf(vplotaddr[minor(bp->b_dev)].ubno, bp, UBDP|USLP);
	uaddr = ubadbuf(vplotaddr[minor(bp->b_dev)].ubno, bp, ubm);
	if(e = vplotwait(vp))
		goto out;
	if((cc = -(bp->b_bcount)) != 0) {
		vp->addr->csr |= IENABLE | ADDR;
		vp->addr->csr &= ~BTCNT;
		vp->addr->buf = uaddr;
		vp->addr->csr |= BTCNT;
		vp->addr->buf = cc;
		vp->addr->csr |= xmem(uaddr) | GO;
	}
	e = vplotwait(vp);
out:
	(void) splx(s);
	ubmfree(vplotaddr[minor(bp->b_dev)].ubno, ubm);
	if(e)
		bp->b_flags |= B_ERROR;
	iodone(bp);
}

int	vplotblock = 16384;

unsigned
minvplotph(bp)
struct buf *bp;
{
	if(bp->b_bcount > vplotblock)
		bp->b_bcount = vplotblock;
}

/*ARGSUSED*/
vplotwrite(d)
{
	if(u.u_count == 0)
		return;
	physio(vplotstrategy, &vplot[minor(d)].buf, d, B_WRITE, minvplotph);
}

vplotwait(vp)
register struct vplot *vp;
{
	register int e;

	for(;;) {
		if(vp->addr->csr & (ERROR|READY))
			break;
		if(tsleep((caddr_t)vp, VPLOTPRI, HZ) != TS_OK)
			break;
	}
	if((vp->addr->csr & ERROR) == 0)
		return 0;
	/* Check for NXM - ERROR remains 1 if NOT NXM) */
	vp->addr->csr &= ~ERROR;
	if((e = (vp->addr->csr & ERROR)) == 0)
		printf("vplot%d: NXM\n", vp - vplot);
	return e;
}

/*ARGSUSED*/
vplotioctl(dev, cmd, addr, flag)
caddr_t addr;
{
	register struct vplot *vp = &vplot[minor(dev)];
	u_short csr;
	int m;

	switch(cmd) {

	case VGETSTATE:
		m = vp->addr->csr;
		if(copyout((caddr_t)&m, addr, sizeof(m)))
			u.u_error = EFAULT;
		break;

	case VSETSTATE:
		if(copyin(addr, (caddr_t)&m, sizeof(m))) {
			u.u_error = EFAULT;
			return;
		}
		(void) spl4();
		csr = (vp->addr->csr & ~MASK) | (m & MASK);
		vp->addr->csr = csr;
		if(vplotwait(vp))
			u.u_error = EIO;
		(void) spl0();
		break;

	default:
		u.u_error = ENOTTY;
		break;
	}
}

vplottimo(vp)
register struct vplot *vp;
{
	if(vp->open)
		timeout(vplottimo, (caddr_t)vp, HZ/10);
	vplot0int(vp - vplot);
}

vplot0int(d)
{
	wakeup((caddr_t)&vplot[d]);
}
