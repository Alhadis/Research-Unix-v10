/*
 * MASSBUS SMD disk driver
 */

#include "sys/param.h"
#include "sys/buf.h"
#include "sys/conf.h"
#include "sys/dir.h"
#include "sys/user.h"
#include "sys/hp.h"
#include "sys/mbaddr.h"
#include "sys/mbsts.h"
#include "sys/bad144.h"
#include "sys/diskio.h"
#include "sys/file.h"

/*
 * hardware registers
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

#define	HP_SEEK		004		/* seek */
#define	HP_RECAL	006		/* recalibrate */
#define	HP_DCLR		010		/* drive clear */
#define	HP_OFFSET	014		/* offset */
#define	HP_RTC		016		/* return to center-line */
#define	HP_PRESET	020		/* read-in preset */
#define	HP_SEARCH	030		/* search */
#define	HP_WCOM		060		/* write */
#define	HP_RCOM		070		/* read data */

/*
 * hpds
 */
#define	HPDS_ERR	0040000		/* composite drive error */
#define	HPDS_PIP	0020000		/* positioning in progress */
#define	HPDS_MOL	0010000		/* medium on line */
#define	HPDS_DPR	0000400		/* drive present */
#define	HPDS_DRY	0000200		/* drive ready */
#define	HPDS_VV		0000100		/* volume valid */
#define	HPDS_DREADY	(HPDS_DPR|HPDS_DRY|HPDS_MOL|HPDS_VV)

/*
 * hper1
 */
#define	HPER1_DCK	0100000		/* data check */
#define	HPER1_WLE	0004000		/* write lock error */
#define	HPER1_ECH	0000100		/* ecc hard error */
#define	HPER1_FER	0000020		/* format error */

/*
 * hpdt
 */
#define	HPDT_TYPE	0x1ff
#define	HPDT_RP05	021
#define	HPDT_RP06	022
#define	HPDT_RP07	042
#define	HPDT_RM03	024
#define	HPDT_RM05	027
#define	HPDT_FUJI	025

/*
 * hpcc: emulex hack
 */
#define	HPHR_MAXCYL	0100027		/* max cyl address */
#define	HPHR_MAXTRAK	0100030		/* max track address */
#define	HPHR_MAXSECT	0100031		/* max sector address */

/*
 * hpof
 */
#define	HPOF_FMT22	0010000		/* 16 bit format */
#define	HPOF_P400	0020		/*  +400 uinches */
#define	HPOF_M400	0220		/*  -400 uinches */
#define	HPOF_P800	0040		/*  +800 uinches */
#define	HPOF_M800	0240		/*  -800 uinches */
#define	HPOF_P1200	0060		/* +1200 uinches */
#define	HPOF_M1200	0260		/* -1200 uinches */

/*
 * hper2
 */
#define	HPER2_BSE	0100000		/* bad sector */

#define	SECTOR	512	/* size of a hardware sector */

/*
 * monstrous size tables
 * one per type of drive
 */
struct	size
{
	daddr_t	nblocks;
	daddr_t blkoff;
};
struct size hp6_sizes[8] = {
	15884,	0,		/* A=cyl 0 thru 37 */
	33440,	15884,		/* B=cyl 38 thru 117 */
	340670,	0,		/* C=cyl 0 thru 814 */
	0,	0,
	0,	0,
	0,	0,
#ifndef NOBADSECT
	291280,	49324,		/* G=cyl 118 thru 814 */
#else
	291346,	49324,
#endif
	0,	0,
};
struct size rm3_sizes[8] = {
	15884,	0,		/* A=cyl 0 thru 99 */
	33440,	16000,		/* B=cyl 100 thru 309 */
	131680,	0,		/* C=cyl 0 thru 822 */
	0,	0,
	0,	0,
	0,	0,
#ifndef NOBADSECT
	81984,	49600,		/* G=cyl 310 thru 822 */
#else
	82080,	49600,
#endif
	113280,	18400,		/* H=cyl 115 thru 708 -- ex 32v */
};
struct size rm5_sizes[8] = {
	15884,	0,		/* A=cyl 0 thru 26 */
	33440,	16416,		/* B=cyl 27 thru 81 */
	500384,	0,		/* C=cyl 0 thru 822 */
	15884,	341696,		/* D=cyl 562 thru 588 */
	55936,	358112,		/* E=cyl 589 thru 680 */
#ifndef NOBADSECT
	86240,	414048,		/* F=cyl 681 thru 822 */
	158592,	341696,		/* G=cyl 562 thru 822 */
#else
	86636,	414048,
	158688,	341696,
#endif
	291346,	49856,		/* H=cyl 82 thru 561 */
};
struct size mfj_sizes[8] = {
	10240,	0,		/* A=cyl 0 thru 31 */
	20480,	10240,		/* B=cyl 32 thru 95 */
	232640,	30720,		/* C=cyl 96 thru 822 */
	0,	0,
	0,	0,
	0,	0,
	0,	0,
	0,	0,
};
struct size eagle_sizes[8] ={
	27520,	0,		/* A=cyl 0 thru 31 */
	27520,	27520,		/* B=cyl 32 thru 63 */
	667360,	55040,		/* C=cyl 64 thru 839 */
	232640,	55040,		/* D=cyl 64 thru 340 (partial) */
	232640,	293260,		/* E=cyl 341 thru 617 (partial) */
	0,	0,
	0,	0,
	0,	0,
};
/* 48 sector Emulex Eagle */
struct size eag48_sizes[8] = {	/* cyl 841 used for bad sectors + info */
#ifndef OLDPART
	11*960,   0*960,	/* A = cyl 0 thru 10		for / */
	22*960,  11*960,	/* B = cyl 11 thru 32		for swap */
	842*960,  0*960,	/* C = all cyl 0 thru 841	for testing */
	16*960,  33*960,	/* D = cyl 33 thru 48		for /usr/guest */
	66*960,  49*960,	/* E = cyl 49 thru 114		for /usr/src */
	242*960,115*960,	/* F = cyl 115 thru 356		nearly 1/3 */
	242*960,357*960,	/* G = cyl 357 thru 598		nearly 1/3 */
	242*960,599*960,	/* H = cyl 599 thru 840		nearly 1/3 */
#else	/* old jones partitions */
	45*960,   0*960,	/* A = cyl 0 thru 44		for /tmp */
	64*960,  45*960,	/* B = cyl 45 thru 108		for swap */
	842*960,  0*960,	/* C = all cyl 0 thru 841	for testing */
	244*960,109*960,	/* D = cyl 109 thru 352		nearly 1/3 */
	244*960,353*960,	/* E = cyl 353 thru 596		nearly 1/3 */
	244*960,597*960,	/* F = cyl 597 thru 840		nearly 1/3 */
	0,	0*960,	/* G = cyl 0 thru 402		nearly 1/2 */
	0, 	0*960,	/* H = cyl 438 thru 840		nearly 1/2 */
#endif
};
struct size hp7_sizes[8] = {
	15884,	0,		/* A=cyl 0 thru 9 */
	64000,	16000,		/* B=cyl 10 thru 49 */
	1008000,0,		/* C=cyl 0 thru 629 */
	504000, 0,		/* D=cyl 0 thru 314 */
	504000, 504000,		/* E=cyl 315 thru 629 */
	928000,	80000,		/* F=cyl 50 thru 629 */
	0,	0,
	0,	0,
};

/*
 * tables of per-drive info, mostly sizes of things
 * indexed by numbers in hptypes
 */

struct	hptype hptype[] = {
	HPDT_RM03, 32, 5, 3, 4,	32*5,	823,	rm3_sizes,	/* RM03 */
	HPDT_RM05, 32, 19, 2, 3, 32*19,	823,	rm5_sizes,	/* RM05 */
	HPDT_RP06, 22, 19, 2, 3, 22*19,	815,	hp6_sizes,	/* RP06 */
	HPDT_RP05, 22, 19, 2, 3, 22*19,	411,	hp6_sizes,	/* RP05 */
	HPDT_RP07, 50, 32, 2, 3, 50*32,	630,	hp7_sizes,	/* RP07 */
	/* HPDT_FUJI (emulex) entries must be contiguous and last */
	HPDT_FUJI, 32, 10, 2, 3, 32*10,	823,	mfj_sizes,	/* little fujitsu */
	HPDT_FUJI, 43, 20, 8, 9, 43*20,	842,	eagle_sizes,	/* eagle */
	HPDT_FUJI, 48, 20, 8, 9, 48*20,	842,	eag48_sizes,	/* eagle 48 sectors */
	0
};

#define	NOFFS	16
unsigned char hp_offset[NOFFS] = {
    HPOF_P400, HPOF_M400, HPOF_P400, HPOF_M400,
    HPOF_P800, HPOF_M800, HPOF_P800, HPOF_M800, 
    HPOF_P1200, HPOF_M1200, HPOF_P1200, HPOF_M1200,
    0, 0, 0, 0
};

/*
 * things from config
 */
extern int hpcnt;
extern struct mbaddr hpaddr[];
extern struct hpdisk hpdisk[];
extern struct buf hpbuf[];
extern struct buf hpbadbuf[];
extern struct bad144 hpbad[];

/*
 * unit flags, hpdisk.flags
 */

#define	UACTIVE	01	/* started seek */
#define	UXFER	02	/* done with any seeking; ready to transfer */
#define	UWOL	04	/* waiting for offline drive */
#define	UWAITOL	010	/* waiting and timer has ticked */
#define	UHAVBAD	020	/* have read bad block table */
#define	UREVEC	040	/* halfway through reading revectored sector */

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

int	hpwstart;
int	hpwatch();
int	hpwaitdry;

int hpopen(), hpstrategy(), hpread(), hpwrite(), hpioctl();
struct cdevsw hpcdev = cdinit(hpopen, nulldev, hpread, hpwrite, hpioctl);
struct bdevsw hpbdev = bdinit(hpopen, nulldev, hpstrategy, 0);

hpopen(dev, flag)
int dev, flag;
{
	register struct hpdisk *hp;
	register struct hptype *st;
	register int p;

	if (hpuinit(UNIT(dev)) == 0) {
		u.u_error = ENXIO;
		return;
	}
	hp = &hpdisk[UNIT(dev)];
	st = &hptype[hp->type];
	p = PART(dev);
	if ((hp->pinit & (1<<p)) == 0) {
		hp->nblocks[p] = st->sizes[p].nblocks;
		hp->blkoff[p] = st->sizes[p].blkoff;
		if ((hp->blkoff[p] % st->nspc) != 0) {
			printf("hp minor %d bad blkoff\n", minor(dev));
			u.u_error = EINVAL;
			return;
		}
		hp->pinit |= (1<<p);
	}
}

static char hponce;

hpuinit(unit)
register int unit;
{
	register struct hpdevice *reg;
	register struct hpdisk *hp;

	if (unit < 0 || unit > hpcnt)
		return (0);
	hp = &hpdisk[unit];
	if (hp->addr)
		return (1);
	if ((reg = (struct hpdevice *)mbaddr(&hpaddr[unit])) == NULL
	||  badaddr(&reg->hpcs1, sizeof(long))
	||  (reg->hpds & HPDS_DPR) == 0) {
		printf("hp%d absent\n", unit);
		return (0);
	}
	if ((hp->type = hputype(reg)) < 0) {
		printf("hp%d absent or bad type\n", unit);
		return (0);
	}
	hp->addr = reg;
	if (hponce == 0) {
		hponce++;
		hpwatch();
	}
	return (1);
}

/*
 * determine drive type
 * this is called by hpdump too;
 * be prepared to run without memory management
 * return is an index into hpst
 */
int
hputype(reg)
register struct hpdevice *reg;
{
	register int t, i;
	register int nsect, ntrak;

	t = reg->hpdt & HPDT_TYPE;
	for (i = 0; hptype[i].type; i++)
		if (hptype[i].type == t)
			break;
	if (hptype[i].type == 0) {
		printf("hp type 0%o unknown\n", t);
		return (-1);
	}
	if (hptype[i].type != HPDT_FUJI)
		return (i);
	/*
	 * special hackery for emulex
	 */
	reg->hpcc = HPHR_MAXTRAK;
	DELAY(2);	/* hack */
	ntrak = reg->hpcc + 1;
	ntrak &= 0xffff;
	reg->hpcc = HPHR_MAXSECT;
	DELAY(2);	/* hack */
	nsect = reg->hpcc + 1;
	nsect &= 0xffff;
	for (; hptype[i].type; i++)
		if (ntrak == hptype[i].ntrak && nsect == hptype[i].nsect)
			return (i);
	/*
	 * 48-sector eagle known to be last,
	 * so the following broken-hardware test might work
	 */
	if (nsect == 46) {
		printf("hp said ntrak %d nsect %d, eag48 assumed\n", ntrak, nsect);
		return (i - 1);
	}
	printf("hp fuji ntrak %d nsect %d unknown\n", ntrak, nsect);
	return (-1);
}
 
/*
 * a subtlety:
 * b_cylin = cylinder number;
 * later, when computing disk address,
 * we use b_cylin, and take b_blkno % cylindersize
 * i.e. there's an embedded assumption that every disk partition
 * begins on a cylinder boundary
 */
hpstrategy(bp)
	register struct buf *bp;
{
	register struct hpdisk *hp;
	register struct hptype *st;
	register int unit;
	register int part;
	long sz;
	int s;

	sz = (bp->b_bcount+SECTOR-1)/SECTOR;
	unit = UNIT(bp->b_dev);
	hp = &hpdisk[unit];
	if (hp->addr == NULL) {		/* safety check */
		bp->b_flags |= B_ERROR;
		iodone(bp);
		return;
	}
	st = &hptype[hp->type];
	part = PART(bp->b_dev);
	if (bp->b_blkno < 0 || bp->b_blkno+sz > hp->nblocks[part]) {
		if (bp->b_blkno == hp->nblocks[part])
			bp->b_resid = bp->b_bcount;
		else {	/* partial read too hard for now */
			bp->b_error = ENXIO;
			bp->b_flags |= B_ERROR;
		}
		iodone(bp);
		return;
	}
	bp->b_cylin = (bp->b_blkno + hp->blkoff[part])/st->nspc;
	s = spl6();
	disksort(&hp->actf, &hp->actl, bp);
	if ((hp->flags & UACTIVE) == 0)
		hpustart(hp);
	splx(s);
}

/*
 * unit start:
 * if there's a block for this drive, start seeking there
 */
int hpxfer();

hpustart(hp)
register struct hpdisk *hp;
{
	register struct buf *bp;
	register struct hpdevice *reg;
	register struct hptype *st;
	int sn, csn;

	if ((bp = hp->actf) == NULL)
		return;
	reg = hp->addr;
	if ((reg->hpds & HPDS_VV) == 0) {
		reg->hpcs1 = HP_DCLR|HP_GO;
		reg->hpcs1 = HP_PRESET|HP_GO;
		reg->hpof = HPOF_FMT22;
		hp->flags &=~ UHAVBAD;
	}
	if ((hp->flags & UHAVBAD) == 0) {
		hprbad(hp);
		bp = hp->actf;
		hp->flags |= UHAVBAD;
	}
	if ((reg->hpds & (HPDS_DPR|HPDS_MOL)) != (HPDS_DPR|HPDS_MOL)) {
		hp->flags |= UWOL;
		return;
	}
	if ((hp->flags & UACTIVE) == 0) {	/* start seek if didn't already */
		hp->flags |= UACTIVE;
		st = &hptype[hp->type];
		sn = bp->b_blkno%st->nsect - st->sdist;	/* seek here before io */
		if (sn < 0)
			sn += st->nsect;
		csn = sn - (reg->hpla>>6);
		if (csn < 0)
			csn += st->nsect;
		if (bp->b_cylin != reg->hpdc	/* seek if off cylinder */
		||  csn > st->rdist) { 		/* or not close enough */
			reg->hpdc = bp->b_cylin;
			reg->hpda = sn;
			reg->hpcs1 = HP_SEARCH|HP_GO;
			return;
		}
	}
	if ((hp->flags & UXFER) == 0) {	/* seek done, time for transfer */
		hp->flags |= UXFER;
		mbstart(&hpaddr[UNIT(bp->b_dev)], bp, hpxfer);
	}
}

/*
 * start transfer
 */
hpxfer(bp)
register struct buf *bp;
{
	register struct hpdisk *hp;
	register struct hpdevice *reg;
	register struct hptype *st;
	int sn, tn;

	hp = &hpdisk[UNIT(bp->b_dev)];
	st = &hptype[hp->type];
	sn = bp->b_blkno%st->nspc;
	tn = sn/st->nsect;
	sn %= st->nsect;
	reg = hp->addr;
	if ((reg->hpds & HPDS_DREADY) != HPDS_DREADY) {
		printf("hp%d: not ready\n", UNIT(bp->b_dev));
		hp->errcnt = 0;
		hp->actf = bp->av_forw;
		hp->flags &=~ (UACTIVE|UXFER);
		bp->b_flags |= B_ERROR;
		iodone(bp);
		return;
	}
	reg->hpdc = bp->b_cylin;
	reg->hpda = (tn << 8) + sn;
	if (bp->b_flags & B_READ)
		reg->hpcs1 = HP_RCOM|HP_GO;
	else
		reg->hpcs1 = HP_WCOM|HP_GO;
}

/*
 * interrupt, passed from MBA code
 */
hp0int(unit, mbsr, mbbc, attn)
int unit, mbsr, mbbc, attn;
{
	register struct hpdisk *hp;

	hp = &hpdisk[unit];
	if (hp->addr == 0) {
		if (hpuinit(unit) == 0) {
			printf("hp%d: hopeless interrupt\n", unit);
			return;		/* but ATTN wasn't cleared; will recur */
		}
		/* init ok, so addr nonzero, so can clear it */
	}
	hp->addr->hpas = attn;
	if (hp->flags & UXFER)
		hpxdone(hp, mbsr, mbbc);
	if ((hp->flags & UXFER) == 0)
		hpustart(hp);
}

/*
 * transfer stopped:
 * because it's done, or because of an error
 * common convention in error-sniffing routines:
 * return 1 if the error was recovered
 * and the device has been restarted
 * return 0 if this transfer has been abandoned
 * leave flags & UXFER set if we're all done;
 * clear UXFER if the transfer should be retried
 */

#define	ERRCMIN	16	/* after this many errors, try offsets */
#define	ERRCAL(e) (((e)%8)==4)	/* try recal this often */
#define	ERRMAX	28	/* after this many errors, give up on transfer */

hpxdone(hp, mbsr, mbbc)
register struct hpdisk *hp;
int mbsr, mbbc;
{
	register struct hpdevice *reg;
	register struct buf *bp;

	reg = hp->addr;
	bp = hp->actf;
	if ((reg->hpds&HPDS_ERR) || (mbsr&MBSR_EBITS)) {
		if (hpwhaterr(hp, mbsr, mbbc))
			return;
		reg->hpcs1 = HP_DCLR|HP_GO;
	}
	if (hp->recal && hpmorecal(hp))
		return;
	if (hp->flags & UREVEC && hpbadcont(hp))
		return;
	if (hp->flags & UXFER) {	/* `active' means we're done */
		if (hp->errcnt >= ERRCMIN) {
			reg->hpof = HPOF_FMT22;
			reg->hpcs1 = HP_RTC|HP_GO;
			while (reg->hpds & HPDS_PIP)
				DELAY(25);
		}
		hp->errcnt = 0;
		hp->flags &=~ (UACTIVE|UXFER);
		hp->actf = bp->av_forw;
		bp->b_resid = mbbc;
		iodone(bp);
	}
}

/*
 * sort out errors
 */

hpwhaterr(hp, mbsr, mbbc)
register struct hpdisk *hp;
int mbsr, mbbc;
{
	register struct hpdevice *reg;
	register struct buf *bp;
	register int i;

	reg = hp->addr;
	bp = hp->actf;
	/*
	 * let registers settle
	 */
	i = 0;
	while ((reg->hpds & HPDS_DRY) == 0) {
		if (++i > 512)
			break;
		hpwaitdry++;
	}
	if (reg->hper1 & HPER1_WLE) {
		printf("hp%d: write locked\n", UNIT(bp->b_dev));
		bp->b_flags |= B_ERROR;
		return (0);		/* finished */
	}
	if ((reg->hper2 & HPER2_BSE || reg->hper1 & HPER1_FER)
	&&  hpbadrep(hp, mbbc))
		return (1);		/* restarted, reading replacement */
	if (++hp->errcnt > ERRMAX) {
		harderr(bp, "hp");
		printf("mbsr %o er1 %o er2 %o\n",
			mbsr, reg->hper1&0177777, reg->hper2&0177777);
		bp->b_flags |= B_ERROR;
		return (0);		/* finished */
	}
	if (bp->b_flags & B_READ
	&&  (reg->hper1 & (HPER1_DCK|HPER1_ECH)) == HPER1_DCK)
		return (hpecc(hp, mbbc));	/* restart or finish */
	/*
	 * hard error: clear and try again,
	 * perhaps with recal
	 */
	if (ERRCAL(hp->errcnt)) {
		hpstrecal(hp);
		return (1);	/* restarted */
	}
	hp->flags &=~ UXFER;
	return (0);		/* please restart me */
}

/*
 * recalibration state machine
 * walks through the sequence
 *	RECAL
 *	SEEK back to the cylinder we wanted
 *	perhaps offset the heads slightly
 *	and try the operation again
 */

hpstrecal(hp)
register struct hpdisk *hp;
{

	hp->addr->hpcs1 = HP_DCLR|HP_GO;
	hp->addr->hpcs1 = HP_RECAL|HP_GO;
	hp->recal = 1;
}

hpmorecal(hp)
register struct hpdisk *hp;
{
	register struct hpdevice *reg;

	reg = hp->addr;
	switch (hp->recal) {
	case 1:		/* did RECAL, time to seek */
		reg->hpdc = hp->actf->b_cylin;
		reg->hpcs1 = HP_SEEK|HP_GO;
		hp->recal++;
		return (1);

	case 2:		/* did seek, time for offset */
		if (hp->errcnt > ERRCMIN && hp->actf->b_flags & B_READ) {
			reg->hpof = hp_offset[hp->errcnt%NOFFS]|HPOF_FMT22;
			reg->hpcs1 = HP_OFFSET|HP_GO;
			hp->recal++;
			return (1);
		}
		/* too soon or a write, fall through */
	default:	/* time to restart the transfer */
		hp->recal = 0;
		hp->flags &=~ UXFER;
		return (0);
	}
}

/*
 * read the bad144 bad block table:
 * call on first access to drive,
 * or when VV was down
 */

hprbad(hp)
register struct hpdisk *hp;
{
	register struct buf *xbp, *bp;
	register struct hptype *st;

	st = &hptype[hp->type];
	xbp = hp->actf;
	bp = &hpbadbuf[UNIT(xbp->b_dev)];
	if (xbp == bp)
		return;		/* cheap reentry protection */
	bp->b_flags = B_BUSY|B_READ;
	bp->b_dev = xbp->b_dev;
	bp->b_un.b_addr = (caddr_t)&hpbad[UNIT(xbp->b_dev)];
	bp->b_bcount = sizeof(struct bad144);
	bp->b_resid = 0;
	bp->b_blkno = st->ncyl * st->nspc - st->nsect;	/* fake-ish */
	bp->b_cylin = st->ncyl - 1;
	bp->av_forw = xbp;
	hp->actf = bp;
	bzero(bp->b_un.b_addr, sizeof(struct bad144));
}

/*
 * here when a bad block is detected:
 * find the replacement block, and restart transfer for it
 */
hpbadrep(hp, mbbc)
register struct hpdisk *hp;
int mbbc;
{
	register daddr_t bno;
	register struct hptype *st;
	struct buf *bp;
	register int i;

	if (hp->flags & UREVEC)
		return (0);
	st = &hptype[hp->type];
	bp = hp->actf;
	hp->badsec = (bp->b_bcount - mbbc) / SECTOR;
	bno = bp->b_cylin * st->nspc + bp->b_blkno % st->nspc;	/* true lbn */
	bno += hp->badsec;
	i = bno / st->nspc;
	bno %= st->nspc;
	i = bad144rep(&hpbad[UNIT(bp->b_dev)], i, (int)bno/st->nsect, (int)bno%st->nsect);
	if (i < 0)
		return (0);
	bno = st->ncyl*st->nspc - st->nsect - 1 - i;
	hp->addr->hpcs1 = HP_DCLR|HP_GO;
	if (mbbc > SECTOR)
		mbbc = SECTOR;
	hpcontin(hp, bno, hp->badsec*SECTOR, mbbc);
	hp->flags |= UREVEC;
	return (1);
}

/*
 * here after replacing the bad block:
 * now do the rest of the original transfer
 */
hpbadcont(hp)
register struct hpdisk *hp;
{
	register struct buf *bp;
	int next, resid;
	daddr_t bno;
	int nspc;

	hp->flags &=~ UREVEC;
	bp = hp->actf;
	next = (hp->badsec + 1) * SECTOR;
	resid = bp->b_bcount - next;
	if (resid <= 0)
		return (0);
	nspc = hptype[hp->type].nspc;
	bno = bp->b_cylin*nspc + bp->b_blkno%nspc;
	bno += hp->badsec + 1;
	hpcontin(hp, bno, next, resid);
	return (1);
}

/*
 * correct an ECC error and restart the transfer
 * the error is (hpec1-1) bits into the current sector;
 * at that point, the bits set in hpec2 are wrong.
 */
hpecc(hp, bc)
register struct hpdisk *hp;
int bc;
{
	register struct hpdevice *reg;
	register struct buf *bp;
	register int i;
	int nxf;
	unsigned int mask;
	long a, lasta;
	struct mbaddr *ap;
	register daddr_t bno;
	register struct hptype *st;

	if ((bp = hp->actf) == NULL)
		panic("hpecc");
	reg = hp->addr;
	ap = &hpaddr[UNIT(bp->b_dev)];
	lasta = mbcuraddr(ap);
	nxf = bp->b_bcount - bc;
	i = reg->hpec1 - 1;		/* -1 makes 0 origin */
	a = lasta - (nxf > SECTOR ? SECTOR : nxf) + ((i&~07)>>3);
	mask = reg->hpec2;
	mask <<= i&07;
	for (; a < lasta && mask; mask >>= 8, a++)
		mbputc(ap, a, mbgetc(ap, a)^mask);
	st = &hptype[hp->type];
	bno = bp->b_cylin*st->nspc + bp->b_blkno%st->nspc + nxf/SECTOR;
	printf("hp%d: soft ecc sec %ld\n", UNIT(bp->b_dev), bno - 1);
	if (bc == 0)
		return (0);
	reg->hpcs1 = HP_DCLR|HP_GO;
	hpcontin(hp, bno, bp->b_bcount - bc, bc);
	return (1);
}

/*
 * continue the current transfer,
 * which was interrupted:
 * move size bytes to or from the current buffer at offset off,
 * starting with disk sector bno
 * used for bad sector replacement,
 * and to continue after bad sectors or ECC correction
 */
hpcontin(hp, bno, off, size)
register struct hpdisk *hp;
daddr_t bno;
int off, size;
{
	register struct hpdevice *reg;
	struct mbaddr *ap;
	register struct buf *bp;
	register struct hptype *st;

	reg = hp->addr;
	bp = hp->actf;
	ap = &hpaddr[UNIT(bp->b_dev)];
	st = &hptype[hp->type];
	mbadj(ap, off, size);
	reg->hpdc = bno / st->nspc;
	bno %= st->nspc;
	reg->hpda = ((bno/st->nsect)<<8) | (bno%st->nsect);
	mbcontin(ap);
	if (bp->b_flags & B_READ)
		reg->hpcs1 = HP_RCOM|HP_GO;
	else
		reg->hpcs1 = HP_WCOM|HP_GO;
}

hpread(dev)
	dev_t dev;
{
	physio(hpstrategy, &hpbuf[UNIT(dev)], dev, B_READ, minphys);
}

hpwrite(dev)
	dev_t dev;
{
	physio(hpstrategy, &hpbuf[UNIT(dev)], dev, B_WRITE, minphys);
}

hpioctl(dev, cmd, addr, flag)
dev_t dev;
int cmd;
caddr_t addr;
int flag;
{
	register struct hpdisk *hp;
	long parts[2];

	hp = &hpdisk[UNIT(dev)];
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
		 * why test this?  see comments above hpstrategy
		 */
		if ((parts[0] % hptype[hp->type].nspc) != 0) {
			u.u_error = EINVAL;
			return;
		}
		hp->blkoff[PART(dev)] = parts[0];
		hp->nblocks[PART(dev)] = parts[1];
		return;

	case DIOGSIZ:
		parts[0] = hp->blkoff[PART(dev)];
		parts[1] = hp->nblocks[PART(dev)];
		if (copyout((caddr_t)parts, addr, sizeof(parts)) < 0)
			u.u_error = EFAULT;
		return;

	default:
		u.u_error = ENOTTY;
		return;
	}
}

/*
 * check for offline drives and hung controllers
 */

hpwatch()
{
	register struct hpdisk *hp;
	register struct hpdevice *reg;
	register struct buf *bp;
	register int s;

	s = spl6();
	timeout(hpwatch, (caddr_t)0, 15*HZ);
	for (hp = &hpdisk[hpcnt-1]; hp >= hpdisk; hp--) {
		if ((hp->flags & UWOL) == 0 || (reg = hp->addr) == 0)
			continue;
		if ((reg->hpds & (HPDS_DPR|HPDS_MOL)) != (HPDS_DPR|HPDS_MOL)) {
			if ((hp->flags & UWAITOL) == 0) {
				hp->flags |= UWAITOL;
				continue;
			}
			printf("hp%d offline\n", hp - hpdisk);
			while ((bp = hp->actf) != NULL) {
				bp->b_flags |= B_ERROR;
				hp->actf = bp->av_forw;
				iodone(bp);
			}
		}
		hp->flags &=~ (UWAITOL|UWOL);
		hpustart(hp);
	}
	splx(s);
}
