/*
 * Nautilus console disks
 */

#include "sys/param.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/conf.h"

/*
 * interface to console driver
 * (on nautilus only)
 * this should really be in a header somewhere
 */

extern int *cniwrite(), *cniread();
extern int cniwait();

int cbsopen(), cbsstrat();
int cbsread(), cbswrite();

struct bdevsw cbsbdev = bdinit(cbsopen, nulldev, cbsstrat, 0);
struct cdevsw cbscdev = cdinit(cbsopen, nulldev, cbsread, cbswrite, nodev);

#define	NDEVS	3

static char cbsid[NDEVS] = {
	1,		/* (0) first floppy */
	4,		/* (1) second floppy */
	2,		/* (2) winchester -- read only */
};

#define	BSCMD	0x9	/* ID to write commands */
#define	BSSTS	0x8	/* ID to read answers */
#define	SECTOR	512
#define	MAXBLK	65535	/* the limit is in the protocol */
#define	RCOM	0
#define	WCOM	1
#define	USHIFT	4
#define	STSTS(s)	((s)&017)	/* status in status */
#define	STOK	0

#define	UNIT(d)	((d)&077)	/* in case of silly 0100 bit */

static char cbsbusy, cbswant;

#define	MAXRAW	(8*SECTOR)
static struct buf cbsbuf;
static char cbsdat[MAXRAW];
struct buf *cbsmkbuf();

cbsopen(dev)
dev_t dev;
{
	if (UNIT(minor(dev)) >= NDEVS)
		u.u_error = ENXIO;
}

cbsread(dev)
dev_t dev;
{
	register struct buf *bp;

	if ((bp = cbsmkbuf(dev, B_READ)) == NULL)
		return;
	cbsstrat(bp);
	iowait(bp);
	if (u.u_error == 0)
		iomove(bp->b_un.b_addr, bp->b_bcount-bp->b_resid, B_READ);
	cbsrelse(bp);
}

cbswrite(dev)
dev_t dev;
{
	register struct buf *bp;

	if ((bp = cbsmkbuf(dev, B_WRITE)) == NULL)
		return;
	iomove(bp->b_un.b_addr, bp->b_bcount, B_WRITE);
	if (u.u_error) {
		cbsrelse(bp);
		return;
	}
	cbsstrat(bp);
	iowait(bp);
	cbsrelse(bp);
}

struct buf *
cbsmkbuf(dev, rw)
dev_t dev;
int rw;
{
	register struct buf *bp;
	register int s;

	if (u.u_count < SECTOR || (Ltol(u.u_offset) % SECTOR) != 0) {
		u.u_error = ENXIO;
		return (NULL);
	}
	bp = &cbsbuf;
	s = spl6();
	while (bp->b_flags & B_BUSY) {
		bp->b_flags |= B_WANTED;
		sleep((caddr_t)bp, PRIBIO);
	}
	bp->b_flags |= B_BUSY | rw;
	bp->b_flags &=~ B_DONE;
	splx(s);
	bp->b_bcount = min(u.u_count, MAXRAW);
	bp->b_un.b_addr = cbsdat;
	bp->b_blkno = Ltol(u.u_offset) / SECTOR;
	bp->b_dev = dev;
	return (bp);
}

cbsrelse(bp)
register struct buf *bp;
{

	if (bp->b_flags & B_WANTED)
		wakeup((caddr_t)bp);
	bp->b_flags &=~ (B_WANTED|B_BUSY);
}

cbsstrat(bp)
struct buf *bp;
{
	int s;

	if (bp->b_blkno >= MAXBLK) {
		bp->b_flags |= B_ERROR;
		iodone(bp);
		return;
	}
	s = spl6();
	while (cbsbusy) {
		cbswant++;
		sleep(&cbsbusy);
	}
	cbsbusy++;
	splx(s);
	if (bp->b_flags & B_READ)
		cbsrd(bp);
	else
		cbswr(bp);
	if (cbswant) {
		cbswant = 0;
		wakeup(&cbsbusy);
	}
	cbsbusy = 0;
}

cbsrd(bp)
register struct buf *bp;
{
	static char cmd[3];
	static char sts;
	register int unit;
	daddr_t blkno;
	register char *buf;
	register int *pst, *pdt;

	unit = UNIT(minor(bp->b_dev));
	blkno = bp->b_blkno;
	bp->b_resid = bp->b_bcount;
	buf = bp->b_un.b_addr;
	while (bp->b_resid >= SECTOR) {
		cmd[0] = RCOM|(unit<<USHIFT);
		cmd[1] = blkno;
		cmd[2] = blkno>>8;
		pst = cniread(BSSTS, &sts, 1);
		pdt = cniread(cbsid[unit], buf, SECTOR);
		cniwrite(BSCMD, cmd, 3);
		if (cniwait(pst, 5) || STSTS(sts) != STOK) {
			*pdt = 0;
			if (bp->b_resid != bp->b_bcount)
				bp->b_flags |= B_ERROR;
			break;
		}
		if (cniwait(pdt, 5)) {
			if (bp->b_resid != bp->b_bcount)
				bp->b_flags |= B_ERROR;
			break;
		}
		blkno++;
		buf += SECTOR;
		bp->b_resid -= SECTOR;
	}
	iodone(bp);
}

cbswr(bp)
register struct buf *bp;
{
	static char cmd[3];
	static char sts;
	register int unit;
	daddr_t blkno;
	register char *buf;
	register int *pst, *pdt;

	unit = UNIT(minor(bp->b_dev));
	blkno = bp->b_blkno;
	bp->b_resid = bp->b_bcount;
	buf = bp->b_un.b_addr;
	while (bp->b_resid >= SECTOR) {
		cmd[0] = WCOM|(unit<<USHIFT);
		cmd[1] = blkno;
		cmd[2] = blkno>>8;
		pst = cniread(BSSTS, &sts, 1);
		cniwrite(BSCMD, cmd, 3);
		pdt = cniwrite(cbsid[unit], buf, SECTOR);
		if (cniwait(pst, 5) || STSTS(sts) != STOK) {
			*pdt = 0;
			if (bp->b_resid != bp->b_bcount)
				bp->b_flags |= B_ERROR;
			break;
		}
		if (cniwait(pdt, 1)) {
			if (bp->b_resid != bp->b_bcount)
				bp->b_flags |= B_ERROR;
			break;
		}
		blkno++;
		buf += SECTOR;
		bp->b_resid -= SECTOR;
	}
	iodone(bp);
}
