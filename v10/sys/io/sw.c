/*
 * indirect driver for swapping
 */

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/buf.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/inode.h"
#include "sys/map.h"

struct	buf rswbuf;
extern struct map swapmap[];
extern int swmapcnt;
extern struct map argmap[];
extern int argcnt;

int swopen(), swstrategy(), swread(), swwrite();

struct bdevsw swbdev = bdinit(swopen, nulldev, swstrategy, 0);

struct cdevsw swcdev = cdinit(swopen, nulldev, swread, swwrite, nodev);

/*
 * presumably called for the first time on boot
 */
swopen(dev, flag)
dev_t dev;
int flag;
{
	static int opened;

	if (opened)
		return;
	opened++;
	swfree(0);
}

swstrategy(bp)
	register struct buf *bp;
{
	register clicks_t sz;
	register daddr_t off;
	register int seg;
	dev_t dev;

	sz = btoc(bp->b_bcount);
	off = bp->b_blkno % dmmax;
	seg = bp->b_blkno / dmmax;
	dev = swdevt[seg % nswdevt].sw_dev;
	if (bdevsw[major(dev)] == 0)
		panic("swstrategy");
	bp->b_blkno = seg / nswdevt;
	bp->b_blkno *= dmmax;
	bp->b_blkno += off;
	bp->b_dev = dev;
	if (bp->b_blkno+sz > swdevt[seg % nswdevt].sw_size
	||  off+sz > dmmax) {
		bp->b_flags |= B_ERROR;
		iodone(bp);
		return;
	}
	(*bdevsw[major(dev)]->d_strategy)(bp);
}

swread(dev)
{

	physio(swstrategy, &rswbuf, dev, B_READ, minphys);
}

swwrite(dev)
{

	physio(swstrategy, &rswbuf, dev, B_WRITE, minphys);
}

/*
 * System call swapon(name) enables swapping on device name,
 * which must be in the swdevsw.  Return EBUSY
 * if already swapping on this device.
 * silly; this should just be an ioctl.
 *
 * -- assume swapping on a regular filesystem,
 * so directly calling thought bdevsw is what we want.
 * some day this will be unfortunate.
 */
vswapon()
{
	struct a {
		char *fname;
	};
	register struct inode *ip;
	dev_t dev;
	register int i;

	ip = namei(((struct a *)u.u_ap)->fname, SEGUDATA, &nilargnamei, 1);
	if (ip == NULL)
		return;
	if ((ip->i_mode&IFMT) != IFBLK || ip->i_fstyp != 0) {
		u.u_error = EINVAL;
		iput(ip);
		return;
	}
	dev = (dev_t)ip->i_un.i_rdev;
	iput(ip);
	for (i = 0; i < nswdevt; i++) {
		if (swdevt[i].sw_dev != dev)
			continue;
		if (swdevt[i].sw_freed) {
			u.u_error = EBUSY;
			return;
		}
		swfree(i);
		return;
	}
	u.u_error = ENODEV;
}

/*
 * Swfree(index) frees the index'th portion of the swap map.
 * Each of the nswdevt devices provides 1/nswdev'th of the swap
 * space, which is laid out with blocks of dmmax pages circularly
 * among the devices.
 * rmalloc starts at 1, so give the first dmmax/2 pages of the first device
 * to the arg device.
 * quietly opens the device, with no trace in inode table
 * subsequently really opening and closing the device may cause trouble?
 */
swfree(index)
	register int index;
{
	register swblk_t bno;
	register int nblk;
	register int xsize;
	extern int argcnt, swmapcnt;

	bdevopen(swdevt[index].sw_dev);
	if (u.u_error)
		return;
	xsize = swdevt[index].sw_size * nswdevt;
	xsize += index * dmmax;
	xsize -= (nswdevt - 1) * (swdevt[index].sw_size % dmmax);
	for (bno = dmmax * index; bno < xsize; bno += dmmax * nswdevt) {
		nblk = xsize - bno;
		if (nblk > dmmax)
			nblk = dmmax;
		if (bno)
			rmfree(swapmap, nblk, (int)bno);
		else {
			argdev = swdevt[0].sw_dev;
			rminit(argmap, argcnt, nblk/2-CLSIZE, CLSIZE);
			rminit(swapmap, swmapcnt, nblk/2, nblk/2);
		}
	}
	swdevt[index].sw_freed = 1;
}
