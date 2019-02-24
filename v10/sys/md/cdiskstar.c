/*
 * Star console floppy
 * -- traces of multiple drives; this was derived from Nautilus.
 * It is probably easier to leave the traces in for future need.
 */

#include "sys/param.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/conf.h"

/*
 * interface to console driver
 * this should really be in a header somewhere
 */

extern int *cniwrite(), *cniread();
extern int cniwait();

int cbsopen(), cbsread(), cbswrite();

struct cdevsw cbscdev = cdinit(cbsopen, nulldev, cbsread, cbswrite, nodev);

#define	NDEVS	1

static char cbsid[NDEVS] = {
	1,		/* (0) floppy */
};

#define	BSCMD	0x9	/* ID to write commands */
#define	BSSTS	0x2	/* ID to read answers */
#define	SECTOR	128
#define	SECTRK	26
#define	MAXBLK	(77*SECTRK)
#define	RCOM	0
#define	WCOM	1
#define	USHIFT	4
#define	STSTS(s)	((s)&0200)	/* status in status */
#define	STOK	0

#define	UNIT(d)	((d)&077)	/* in case of silly 0100 bit */

static char cbsbusy, cbswant;

#define	MAXRAW	(4*SECTOR)
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
	cbsdoio(bp, (daddr_t)(Ltol(u.u_offset)/SECTOR));
	iowait(bp);
	if (u.u_error == 0)
		iomove(bp->b_un.b_addr, bp->b_bcount-bp->b_resid, B_READ);
	cbsrelse(bp);
}

cbswrite(dev)
dev_t dev;
{
	register struct buf *bp;
	daddr_t sno;

	if ((bp = cbsmkbuf(dev, B_WRITE)) == NULL)
		return;
	sno = Ltol(u.u_offset)/SECTOR;
	iomove(bp->b_un.b_addr, bp->b_bcount, B_WRITE);
	if (u.u_error) {
		cbsrelse(bp);
		return;
	}
	cbsdoio(bp, sno);
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
	bp->b_flags = B_BUSY | rw;
	splx(s);
	bp->b_bcount = min(u.u_count, MAXRAW);
	bp->b_un.b_addr = cbsdat;
	bp->b_dev = dev;
	/* b_blkno unused by any code we'll call */
	return (bp);
}

cbsrelse(bp)
register struct buf *bp;
{

	if (bp->b_flags & B_WANTED)
		wakeup((caddr_t)bp);
	bp->b_flags &=~ (B_WANTED|B_BUSY);
}

cbsdoio(bp, sno)
struct buf *bp;
daddr_t sno;
{
	int s;

	if (sno >= MAXBLK) {
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
		cbsrd(bp, sno);
	else
		cbswr(bp, sno);
	if (cbswant) {
		cbswant = 0;
		wakeup(&cbsbusy);
	}
	cbsbusy = 0;
}

cbsrd(bp, sno)
register struct buf *bp;
daddr_t sno;
{
	static char cmd[2];
	static unsigned char sts;
	register int unit;
	register char *buf;
	register int *pst, *pdt;

	unit = UNIT(minor(bp->b_dev));
	bp->b_resid = bp->b_bcount;
	buf = bp->b_un.b_addr;
	while (bp->b_resid >= SECTOR) {
		cmd[0] = RCOM|(unit<<USHIFT);
		pst = cniwrite(BSCMD, cmd, 1);
		if (cniwait(pst, 5)) {
			bp->b_flags |= B_ERROR;
			break;
		}
		cmd[0] = (sno%SECTRK) + 1;
		cmd[1] = sno / SECTRK;
		pst = cniread(BSSTS, &sts, 1);
		pdt = cniread(cbsid[unit], buf, SECTOR);
		cniwrite(cbsid[unit], cmd, 2);
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
		sno++;
		buf += SECTOR;
		bp->b_resid -= SECTOR;
	}
	iodone(bp);
}

cbswr(bp, sno)
register struct buf *bp;
daddr_t sno;
{
	static char cmd[2];
	static unsigned char sts;
	register int unit;
	register char *buf;
	register int *pst, *pdt;

	unit = UNIT(minor(bp->b_dev));
	bp->b_resid = bp->b_bcount;
	buf = bp->b_un.b_addr;
	while (bp->b_resid >= SECTOR) {
		cmd[0] = WCOM|(unit<<USHIFT);
		pst = cniwrite(BSCMD, cmd, 1);
		if (cniwait(pst, 5)) {
			bp->b_flags |= B_ERROR;
			break;
		}
		cmd[0] = (sno%SECTRK) + 1;
		cmd[1] = sno / SECTRK;
		pst = cniread(BSSTS, &sts, 1);
		pdt = cniwrite(cbsid[unit], cmd, 2);
		if (cniwait(pdt, 5)) {
			*pst = 0;
			bp->b_flags |= B_ERROR;
			break;
		}
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
		sno++;
		buf += SECTOR;
		bp->b_resid -= SECTOR;
	}
	iodone(bp);
}
