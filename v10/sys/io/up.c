/*
 * UNIBUS SMD disk driver
 *
 * undone:
 *	Add bad sector forwarding code
 *	Check that offset recovery code works
 *	unibus mapping is suboptimal; if the queue gets long,
 *	buffered data paths will be underused
 */

#include "sys/param.h"
#include "sys/buf.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/up.h"
#include "sys/ubaddr.h"
#include "sys/subaddr.h"
#include "sys/diskio.h"
#include "sys/file.h"

#define	NOUPSEEK	1	/* because emulex doesn't do it right */

/*
 * hardware registers
 */

struct updevice
{
	unsigned short	upcs1;		/* control and status register 1 */
	short	upwc;			/* word count register */
	unsigned short	upba;		/* UNIBUS address register */
	unsigned short	upda;		/* desired address register */
	unsigned short	upcs2;		/* control and status register 2 */
	unsigned short	upds;		/* drive status */
	unsigned short	uper1;		/* error register 1 */
	unsigned short	upas;		/* attention summary */
	unsigned short	upla;		/* look ahead */
	unsigned short	updb;		/* data buffer */
	unsigned short	upmr;		/* maintenance */ 
	unsigned short	updt;		/* drive type */
	unsigned short	upsn;		/* serial number */
	unsigned short	upof;		/* offset register */
	unsigned short	updc;		/* desired cylinder address register */
	unsigned short	uphr;		/* holding register */
	unsigned short	upmr2;		/* maintenance register 2 */
	unsigned short	uper2;		/* error register 2 */
	unsigned short	upec1;		/* burst error bit position */
	unsigned short	upec2;		/* burst error bit pattern */
};

/*
 * upcs1
 */

#define	UP_SC	0100000		/* special condition */
#define	UP_TRE	0040000		/* transfer error */
#define	UP_IE	0000100		/* interrupt enable */
#define	UP_GO	0000001

#define	UP_SEEK		004		/* seek */
#define	UP_RECAL	006		/* recalibrate */
#define	UP_DCLR		010		/* drive clear */
#define	UP_OFFSET	014		/* offset */
#define	UP_RTC		016		/* return to center-line */
#define	UP_PRESET	020		/* read-in preset */
#define	UP_SEARCH	030		/* search */
#define	UP_WCOM		060		/* write */
#define	UP_RCOM		070		/* read data */

/*
 * upcs2
 */
#define	UPCS2_NED	0010000		/* nonexistent drive */
#define	UPCS2_CLR	0000040		/* controller clear */
#define	UPCS2_SEL	0000007		/* unit select */

/*
 * upds
 */
#define	UPDS_ERR	0040000		/* composite drive error */
#define	UPDS_PIP	0020000		/* positioning in progress */
#define	UPDS_MOL	0010000		/* medium on line */
#define	UPDS_DPR	0000400		/* drive present */
#define	UPDS_DRY	0000200		/* drive ready */
#define	UPDS_VV		0000100		/* volume valid */
#define	UPDS_DREADY	(UPDS_DPR|UPDS_DRY|UPDS_MOL|UPDS_VV)

/*
 * uper1
 */
#define	UPER1_DCK	0100000		/* data check */
#define	UPER1_WLE	0004000		/* write lock error */
#define	UPER1_ECH	0000100		/* ecc hard error */

/*
 * uphr: emulex hack
 */
#define	UPHR_MAXCYL	0100027		/* max cyl address */
#define	UPHR_MAXTRAK	0100030		/* max track address */
#define	UPHR_MAXSECT	0100031		/* max sector address */

/*
 * upof
 */
#define	UPOF_FMT22	0010000		/* 16 bit format */
/* THE SC21 ACTUALLY JUST IMPLEMENTS ADVANCE/RETARD... */
#define	UPOF_P400	0020		/*  +400 uinches */
#define	UPOF_M400	0220		/*  -400 uinches */
#define	UPOF_P800	0040		/*  +800 uinches */
#define	UPOF_M800	0240		/*  -800 uinches */
#define	UPOF_P1200	0060		/* +1200 uinches */
#define	UPOF_M1200	0260		/* -1200 uinches */

#define	SECTOR	512	/* size of a hardware sector */

/*
 * monstrous size tables
 * one per type of drive
 */
struct	size
{
	daddr_t	nblocks;
	daddr_t blkoff;
} up_sizes[NUPPART] = {
	15884,	0*608,		/* A=cyl 0 thru 26 */
	33440,	27*608,		/* B=cyl 27 thru 81 */
	495520,	0*608,		/* C=cyl 0 thru 814 */
	15884,	562*608,		/* D=cyl 562 thru 588 */
	55936,	589*608,		/* E=cyl 589 thru 680 */
#ifndef NOBADSECT
	81376,	681*608,		/* F=cyl 681 thru 814 */
	153728,	562*608,		/* G=cyl 562 thru 814 */
#else
	81472,	681*608,
	153824,	562*608,
#endif
	291346,	82*608,		/* H=cyl 82 thru 561 */
}, fj_sizes[8] = {
	10240,	0*320,		/* A=cyl 0 thru 31 */
	20480,	32*320,		/* B=cyl 32 thru 95 */
	232640,	96*320,		/* C=cyl 96 thru 822 */
	0,	0*320,
	0,	0*320,
	0,	0*320,
	0,	0*320,
#ifndef NOBADSECT
	0,	0*320,		/* H=cyl 155 thru 822 */
#else
	0,	0*320,
#endif
}, fj3_sizes[8] = {
	10240,	0*512,		/* A=cyl 0 thru 19 */
	20480,	20*512,		/* B=cyl 20 thru 59 */
	246784,	60*512,		/* C=cyl 60 thru 541 */
	246784,	542*512,		/* D=cyl 542 thru 1024 */
	0,	0*512,
	0,	0*512,
	0,	0*512,
#ifndef NOBADSECT
	0,	0*512,		/* H=cyl 155 thru 822 */
#else
	0,	0*512,
#endif
}, cd_sizes[8] = {
	10240,	0*160,		/* A=cyl 0 thru 63 */
	20480,	64*160,		/* B=cyl 64 thru 191 */
	100960,	192*160,		/* C=cyl 192 thru 822 */
	0,	0*160,
	0,	0*160,
	0,	0*160,
	0,	0*160,
#ifndef NOBADSECT
	100960,	0*160,		/* H=cyl 0 thru 630 */
#else
	100960,	0*160,
#endif
}, fj1_sizes[8] = {
	1024,	0*256,		/* cyl 0 through 3 */
	0,	0*256,
	0,	0*256,
	0,	0*256,
	0,	0*256,
	0,	0*256,
	0,	0*256,
	0,	0*256,
};

/*
 * tables of per-drive info, mostly sizes of things
 * indexed by magic numbers; see uputype
 */
struct	upst upst[] = {
	32, 19,	3, 4,	32*19,	823,	up_sizes,	/* 9300/cdc */
/* 9300 actually has 815 cylinders... */
	32, 10,	3, 4,	32*10,	823,	fj_sizes,	/* fujitsu 160m */
	32, 5,	3, 4,	32*5,	631,	cd_sizes,	/* CDC 76 MB */
	32, 8,	3, 4,	32*8,	4,	fj1_sizes,	/* Fixed part of FJ */
	32, 16,	3, 4,	32*16,	1024,	fj3_sizes,	/* fujitsu 330m */
};

unsigned char	up_offset[16] = {
    UPOF_P400, UPOF_M400, UPOF_P400, UPOF_M400,
    UPOF_P800, UPOF_M800, UPOF_P800, UPOF_M800, 
    UPOF_P1200, UPOF_M1200, UPOF_P1200, UPOF_M1200,
    0, 0, 0, 0
};

/*
 * things from config
 */
extern int upcnt, sccnt;
extern struct ubaddr scaddr[];
extern struct subaddr upaddr[];
extern struct scctl scctl[];
extern struct updisk updisk[];
extern struct buf upbuf[];

/*
 * controller flags, scctl.flags
 */

#define	CACTIVE	01	/* xfer in progress */
#define	CWAITING 02	/* xfering and timer has ticked */

/*
 * unit flags, updisk.flags
 */

#define	UACTIVE	01	/* working on this xfer */
#define	URXFR	02	/* done with any seeking; ready to roll */
#define	UWOL	04	/* waiting for offline drive */
#define	UWAITOL	010	/* waiting and timer has ticked */

/*
 * device number
 * 0100 in the minor device is (temporarily?)
 * usurped to indicate bitmapped file systems
 * quietly ignore it for now
 * when things improve, change the UNIT mask to 037
 */

#define	UNIT(d)	((minor(d)>>3) & 027)
#define	PART(d)	(minor(d) & 07)

/*
 * abuse of spare bits of struct buf
 */
#define	b_cylin b_resid		/* for disksort */
#define	b_ubm	av_back		/* this buffer's map */

int	upwstart, upwatch();		/* Have started guardian */
int	upwaitdry;

int upopen(), upstrategy(), upread(), upwrite(), upioctl();

struct cdevsw upcdev = cdinit(upopen, nulldev, upread, upwrite, upioctl);

struct bdevsw upbdev = bdinit(upopen, nulldev, upstrategy, 0);

upopen(dev, flag)
int dev, flag;
{
	register struct updisk *up;
	register struct upst *st;
	register int p;

	if (upuinit(UNIT(dev)) == 0) {
		u.u_error = ENXIO;
		return;
	}
	up = &updisk[UNIT(dev)];
	st = &upst[up->type];
	p = PART(dev);
	if ((up->pinit & (1<<p)) == 0) {
		up->nblocks[p] = st->sizes[p].nblocks;
		up->blkoff[p] = st->sizes[p].blkoff;
		if ((up->blkoff[p] % st->nspc) != 0) {
			printf("up minor %d bad blkoff\n", minor(dev));
			u.u_error = EINVAL;
			return;
		}
		up->pinit |= (1<<p);
	}
}

upuinit(unit)
register int unit;
{
	register struct updevice *reg;
	register struct updisk *up;
	register struct subaddr *ua;

	if (unit < 0 || unit > upcnt)
		return (0);
	ua = &upaddr[unit];
	if (ua->ctl < 0)
		return (0);
	up = &updisk[unit];
	if (up->ctl)
		return (1);
	if (upcinit(ua->ctl) == 0)
		return (0);
	reg = scctl[ua->ctl].addr;
	if ((up->type = uputype(reg, ua->unit)) < 0) {
		printf("up%d absent or bad type\n", unit);
		return (0);
	}
	up->unit = ua->unit;
	up->ctl = &scctl[ua->ctl];
	up->ctl->drives[ua->unit] = up;
	return (1);
}
	

/*
 * determine drive type
 * very emulex dependent; should look at drive type register too.
 * perhaps there should be a way to change it?
 * this is called by updump too;
 * be prepared to run without memory management
 * unit is the hardware unit
 * return is an index into upst
 */
int
uputype(reg, unit)
register struct updevice *reg;
int unit;
{

	reg->upcs1 = 0;		/* conservative */
	reg->upcs2 = unit;
	if (reg->upcs2&UPCS2_NED) {
		reg->upcs1 = UP_DCLR|UP_GO;
		return (-1);
	}
	reg->uphr = UPHR_MAXTRAK;
	switch (reg->uphr) {
	default:
		reg->upcs1 = UP_DCLR|UP_GO;
		return (-1);
	case 9:
		return (unit >= 4 ? 3 : 1);		/* fujitsu hack */
	case 4:
		return (2);		/* CDC 76MB hack */
	case 7:
		return (3);		/* Fixed Fujitsu hack */
	case 15:
		return (4);		/* fuji 330m hack */
	case 19:
		return (0);		/* CDC 300 MB hack */
	}
}

upcinit(ctl)
int ctl;
{
	register struct scctl *sc;
	register struct updevice *reg;

	if (ctl < 0 || ctl >= sccnt)
		return (0);
	sc = &scctl[ctl];
	if (sc->addr)
		return (1);
	if ((reg = (struct updevice *)ubaddr(&scaddr[ctl])) == 0
	||  ubbadaddr(scaddr[ctl].ubno, &reg->upcs1, sizeof(short))) {
		printf("sc%d absent\n", ctl);
		return (0);
	}
	reg->upcs2 = UPCS2_CLR;
	sc->addr = reg;
	sc->ubno = scaddr[ctl].ubno;
	if (upwstart == 0) {
		timeout(upwatch, (caddr_t)0, HZ);
		upwstart++;
	}
	return (1);
}
 
upstrategy(bp)
	register struct buf *bp;
{
	register struct updisk *up;
	register struct upst *st;
	register int unit;
	register int part;
	long sz;
	int s;

	sz = (bp->b_bcount+SECTOR-1)/SECTOR;
	unit = UNIT(bp->b_dev);
	up = &updisk[unit];
	if (up->ctl == 0) {
		bp->b_flags |= B_ERROR;
		iodone(bp);
		return;
	}
	st = &upst[up->type];
	part = PART(bp->b_dev);
	if (bp->b_blkno < 0 || bp->b_blkno+sz > up->nblocks[part]) {
		if (bp->b_blkno == up->nblocks[part])
			bp->b_resid = bp->b_bcount;
		else {	/* partial read too hard for now */
			bp->b_error = ENXIO;
			bp->b_flags |= B_ERROR;
		}
		iodone(bp);
		return;
	}
	bp->b_cylin = (bp->b_blkno + up->blkoff[part])/st->nspc;
	bp->b_ubm = (struct buf *)ubmbuf(up->ctl->ubno, bp, USLP);
	s = spl6();
	disksort(&up->actf, &up->actl, bp);
	if ((up->flags & UACTIVE) == 0) {
		upustart(up);
		if (up->ctl->actf && (up->ctl->flags & CACTIVE) == 0)
			upstart(up->ctl);
	}
	splx(s);
}

/*
 * unit start:
 * if there's a block for this drive, start seeking there
 *
 * up->upcs = UP_IE cancels a pending command in the SC21
 * why not up->cs |= UP_IE?
 */
upustart(up)
register struct updisk *up;
{
	register struct buf *bp;
	register struct updevice *reg;
	register struct scctl *sc;
	register struct upst *st;
	int sn, csn;
	int didie = 0;

	sc = up->ctl;
	if ((bp = up->actf) == NULL)
		return (0);
#ifndef NOUPSEEK
	if (sc->flags & CACTIVE) {	/* can't start seek till xfer done */
		sc->softas |= 1<<up->unit;
		return (0);
	}
#endif
	reg = sc->addr;
	reg->upcs2 = up->unit;
	if ((reg->upds & UPDS_VV) == 0) {
		reg->upcs1 = UP_IE|UP_DCLR|UP_GO;
		reg->upcs1 = UP_IE|UP_PRESET|UP_GO;
		reg->upof = UPOF_FMT22;
		didie = 1;
	}
	if ((reg->upds & (UPDS_DPR|UPDS_MOL)) != (UPDS_DPR|UPDS_MOL)) {
		up->flags |= UWOL;
		return (didie);
	}
	if ((up->flags & UACTIVE) == 0) {	/* start seek if didn't already */
		up->flags |= UACTIVE;
#ifndef NOUPSEEK
		st = &upst[up->type];
		sn = bp->b_blkno%st->nspc;
		sn = (sn + st->nsect - st->sdist) % st->nsect;	/* sector to seek to */
		csn = sn - (reg->upla>>6);
		if (csn < 0)
			csn += st->nsect;
		if (bp->b_cylin != reg->updc	/* seek if off cylinder */
		||  csn > st->rdist) { 		/* or not close enough */
			reg->updc = bp->b_cylin;
			reg->upda = sn;
			reg->upcs1 = UP_IE|UP_SEARCH|UP_GO;
			return (1);
		}
#endif
	}
	if ((up->flags & URXFR) == 0) {	/* seek done, put on ctl queue */
		up->next = NULL;
		if (sc->actf == NULL)
			sc->actf = up;
		else
			sc->actl->next = up;
		sc->actl = up;
		up->flags |= URXFR;
	}
	return (didie);
}

/*
 * Start up a transfer on a drive.
 */
upstart(sc)
register struct scctl *sc;
{
	register struct buf *bp;
	register struct updisk *up;
	register struct updevice *reg;
	register struct upst *st;
	register c;
	uaddr_t uad;
	int sn, tn;

loop:
	if ((up = sc->actf) == NULL)
		return (0);
	if ((bp = up->actf) == NULL) {
		sc->actf = up->next;
		goto loop;
	}
	sc->flags |= CACTIVE;
	st = &upst[up->type];
	sn = bp->b_blkno%st->nspc;
	tn = sn/st->nsect;
	sn %= st->nsect;
	reg = sc->addr;
	reg->upcs2 = up->unit;
	c = 0;
	while ((reg->upds&UPDS_DRY) == 0) {
		if (++c > 512)
			break;
		upwaitdry++;
	}
	if ((reg->upds & UPDS_DREADY) != UPDS_DREADY) {
		printf("up%d: not ready", UNIT(bp->b_dev));
		if ((reg->upds & UPDS_DREADY) != UPDS_DREADY) {
			printf("\n");
			sc->flags &=~ (CACTIVE|CWAITING);
			sc->errcnt = 0;
			up->actf = bp->av_forw;
			up->flags &=~ (UACTIVE|URXFR);
			bp->b_flags |= B_ERROR;
			iodone(bp);
			goto loop;
		}
		printf(" (flakey)\n");	/* inscrutable */
	}
	reg->updc = bp->b_cylin;
	reg->upda = (tn << 8) + sn;
	reg->upwc = -bp->b_bcount / sizeof(short);
	if (bp->b_flags & B_READ)
		c = UP_IE|UP_RCOM|UP_GO;
	else
		c = UP_IE|UP_WCOM|UP_GO;
	bp->b_ubm = (struct buf *)ubinspath(ubmapath(sc->ubno), (ubm_t)bp->b_ubm);
	uad = ubadbuf(sc->ubno, bp, (ubm_t)bp->b_ubm);
	reg->upba = uad;
	reg->upcs1 = c|((uad>>8)&0x300);
	return (1);
}

/*
 * Handle a disk interrupt.
 */
sc0int(ctl)
int ctl;
{
	register struct updevice *reg;
	register struct scctl *sc;
	register struct updisk *up;
	register struct buf *bp;
	register int unit, i, as;
	int needie;

	if (ctl < 0 || ctl >= sccnt) {
		printf("sc%d bad intr\n");
		return;
	}
	sc = &scctl[ctl];
	if ((reg = sc->addr) == 0) {
		printf("sc%d: stray intr\n");
		return;
	}
	needie = 1;
	as = (reg->upas & 0377) | sc->softas;
	sc->softas = 0;
	if ((sc->flags & CACTIVE) == 0) {	/* must be a seek */
		if (reg->upcs1 & UP_TRE)
			reg->upcs1 = UP_TRE;
		goto doattn;
	}
	up = sc->actf;
	bp = up->actf;
	reg->upcs2 = up->unit;
	if ((reg->upds&UPDS_ERR) || (reg->upcs1&UP_TRE)) {
		i = 0;
		while ((reg->upds & UPDS_DRY) == 0) {
			if (++i > 512)
				break;
			upwaitdry++;
		}
		if (reg->uper1&UPER1_WLE) {
			printf("up%d: write locked\n", UNIT(bp->b_dev));
			bp->b_flags |= B_ERROR;
		} else if (++sc->errcnt > 27) {
			harderr(bp, "up");
			printf("cs2=%o er1=%o er2=%o\n",
			    reg->upcs2&0177777, reg->uper1&0177777, reg->uper2&0177777);
			bp->b_flags |= B_ERROR;
		} else {
			/*
			 * soft ecc, try to correct
			 */
			sc->flags &=~ (CACTIVE|CWAITING);	 /* force retry */
			if ((reg->uper1&(UPER1_DCK|UPER1_ECH))==UPER1_DCK)
				if (upecc(up))
					return;	/* probably wrong */
		}
		/*
		 * `hard' error. clear and try again
		 */
		reg->upcs1 = UP_TRE|UP_IE|UP_DCLR|UP_GO;
		needie = 0;
		if ((sc->errcnt&07) == 4 && (sc->flags & CACTIVE) == 0) {
			reg->upcs1 = UP_RECAL|UP_IE|UP_GO;
			sc->recal = 0;
			goto nextrecal;
		}
	}
	/*
	 * Advance recalibration finite state machine
	 * if recalibrate in progress, through
	 *	RECAL
	 *	SEEK
	 *	OFFSET (optional)
	 *	RETRY
	 */
	switch (sc->recal) {
	case 1:
		reg->updc = bp->b_cylin;
		reg->upcs1 = UP_SEEK|UP_IE|UP_GO;
		goto nextrecal;
	case 2:
		if (sc->errcnt < 16 || (bp->b_flags&B_READ) == 0)
			goto donerecal;
		reg->upof = up_offset[sc->errcnt & 017] | UPOF_FMT22;
		reg->upcs1 = UP_IE|UP_OFFSET|UP_GO;
	nextrecal:
		sc->recal++;
		sc->flags |= CACTIVE;
		return;

	donerecal:
	case 3:
		sc->recal = 0;
		sc->flags &=~ CACTIVE;
		break;
	}
	if (sc->flags & CACTIVE) {	/* `active' means we're done */
		if (sc->errcnt >= 16) {
			reg->upof = UPOF_FMT22;
			reg->upcs1 = UP_RTC|UP_GO|UP_IE;
			while (reg->upds & UPDS_PIP)
				DELAY(25);
			needie = 0;
		}
		sc->flags &=~ (CACTIVE|CWAITING);
		sc->errcnt = 0;
		sc->actf = up->next;
		up->flags &=~ (UACTIVE|URXFR);
		up->actf = bp->av_forw;
		bp->b_resid = (-reg->upwc * sizeof(short));
		ubmfree(sc->ubno, (ubm_t)bp->b_ubm);
		iodone(bp);
		if (up->actf)
			if (upustart(up))
				needie = 0;
	}
	as |= reg->upas;
	as &= ~(1<<up->unit);
doattn:
	/*
	 * Process other units which need attention.
	 */
	for (unit = 0, i = 1; unit < NSCUP && as; i <<= 1, unit++) {
		if ((as & i) == 0)
			continue;
		as &= ~i;
		reg->upas = i;
		if ((up = sc->drives[unit]) != NULL && upustart(up))
			needie = 0;
	}
	if (sc->actf && (sc->flags & CACTIVE) == 0)
		if (upstart(sc))
			needie = 0;
	if (needie)
		reg->upcs1 = UP_IE;	/* why bother? */
}

upread(dev)
	dev_t dev;
{
	physio(upstrategy, &upbuf[UNIT(dev)], dev, B_READ, minphys);
}

upwrite(dev)
	dev_t dev;
{
	physio(upstrategy, &upbuf[UNIT(dev)], dev, B_WRITE, minphys);
}

upioctl(dev, cmd, addr, flag)
dev_t dev;
int cmd;
caddr_t addr;
int flag;
{
	register struct updisk *up;
	long parts[2];

	up = &updisk[UNIT(dev)];
	switch (cmd) {
	case DIOSSIZ:
		if ((flag & FWRITE) == 0) {
			u.u_error = EBADF;
			return;
		}
		if (copyin(addr, (caddr_t)parts, sizeof(parts)) < 0) {
			u.u_error = EFAULT;
			return;
		}
		/*
		 * why test this?  see comments above upstrategy
		 */
		if ((parts[0] % upst[up->type].nspc) != 0) {
			u.u_error = EINVAL;
			return;
		}
		up->blkoff[PART(dev)] = parts[0];
		up->nblocks[PART(dev)] = parts[1];
		return;

	case DIOGSIZ:
		parts[0] = up->blkoff[PART(dev)];
		parts[1] = up->nblocks[PART(dev)];
		if (copyout((caddr_t)parts, addr, sizeof(parts)) < 0)
			u.u_error = EFAULT;
		return;

	default:
		u.u_error = ENOTTY;
		return;
	}
}

/*
 * correct an ECC error and restart the transfer
 * the error is (upec1-1) bits into the current sector;
 * at that point, the bits set in upec2 are wrong.
 *
 * should be able just to set the GO bit and proceed,
 * but emulex insists we do DCLR first, or so the book says,
 * because DCK sets ERR in upds
 */
upecc(up)
register struct updisk *up;
{
	register struct updevice *reg;
	register struct scctl *sc;
	register struct buf *bp;
	register int i;
	int nxf;
	unsigned int mask;
	uaddr_t uad, lastua;
	int xc, xa;

	if ((bp = up->actf) == NULL || (sc = up->ctl) == NULL)
		panic("upecc");
	reg = sc->addr;
	ubmflush(sc->ubno, ubmpath((ubm_t)bp->b_ubm));
	lastua = reg->upba + ((reg->upcs1&0x300)<<8);
	nxf = bp->b_bcount + (reg->upwc * sizeof(short));
	i = reg->upec1 - 1;		/* -1 makes 0 origin */
	uad = lastua - (nxf > SECTOR ? SECTOR : nxf) + ((i&~07)>>3);
	mask = reg->upec2;
	mask <<= i&07;
	for (; uad < lastua && mask; mask >>= 8, uad++)
		ubputc(sc->ubno, uad, ubgetc(sc->ubno, uad)^mask);
	printf("up%d%o: soft ecc sec %ld\n", UNIT(bp->b_dev), PART(bp->b_dev),
		bp->b_blkno + nxf/SECTOR - 1);
	sc->flags |= CACTIVE;	/* either complete or continuing */
	if (reg->upwc == 0)
		return (0);
#ifdef notdef
	reg->uper1 = 0;
	reg->upcs1 |= UP_GO;
#else		/* clear wretched emulex error */
	xc = reg->updc;
	xa = reg->upda;
	/* ba, wc undisturbed by DCLR */
	reg->upcs1 = UP_TRE|UP_IE|UP_DCLR|UP_GO;
	reg->updc = xc;
	reg->upda = xa;
	reg->upba = lastua;
	i = (lastua >> 8) & 0x300;
	i |= UP_IE|UP_GO|UP_RCOM;
	reg->upcs1 = i;
#endif
	return (1);
}

/*
 * check for offline drives and hung controllers
 */

upwatch()
{
	register struct scctl *sc;
	register struct updisk *up;
	register struct updevice *reg;
	register struct buf *bp;
	register int ounit;
	register int s;

	s = spl6();
	timeout(upwatch, (caddr_t)0, 15*HZ);
	for (up = &updisk[upcnt-1]; up >= updisk; up--) {
		if ((up->flags & UWOL) == 0)
			continue;
		reg = up->ctl->addr;
		ounit = reg->upcs2 & UPCS2_SEL;
		reg->upcs2 = up->unit;
		if ((reg->upds & (UPDS_DPR|UPDS_MOL)) != (UPDS_DPR|UPDS_MOL)) {
			if ((up->flags & UWAITOL) == 0) {
				up->flags |= UWAITOL;
				reg->upcs2 = ounit;
				continue;
			}
			printf("up%d offline\n", up - updisk);
			while ((bp = up->actf) != NULL) {
				bp->b_flags |= B_ERROR;
				up->actf = bp->av_forw;
				ubmfree(up->ctl->ubno, (ubm_t)bp->b_ubm);
				iodone(bp);
			}
		}
		up->flags &=~ (UWAITOL|UWOL);
		upustart(up);
		reg->upcs2 = ounit;
	}
	for (sc = &scctl[sccnt-1]; sc >= scctl; sc--) {
		if (sc->flags & CACTIVE) {
			if ((sc->flags & CWAITING) == 0)
				sc->flags |= CWAITING;
			else {
				sc->addr->upcs2 = UPCS2_CLR;
				printf("sc%d hung, kicked\n", sc - scctl);
				sc->flags &=~ (CWAITING|CACTIVE);
			}
		}
		if (sc->actf && (sc->flags & CACTIVE) == 0)
			upstart(sc);
	}
	splx(s);
}
