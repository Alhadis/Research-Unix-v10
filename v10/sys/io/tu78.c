/*
 * TM78/TU78 tape driver
 *
 * TODO:
 * multiple drives per formatter might work, but are untested
 */
#include "sys/param.h"
#include "sys/buf.h"
#include "sys/conf.h"
#include "sys/file.h"
#include "sys/user.h"
#include "sys/mbaddr.h"
#include "sys/mbsts.h"
#include "sys/subaddr.h"
#include "sys/mtio.h"
#include "sys/tu78.h"

/*
 * hardware stuff
 */

struct device {
	int	mtcs;		/* control/status */
	int	mter;		/* error */
	int	mtca;		/* command address, rec cnt, skp cnt */
	int	mtmr1;		/* maint */
	int	mtas;		/* attention summary */
	int	mtbc;		/* byte count */
	int	mtdt;		/* drive type */
	int	mtds;		/* drive status */
	int	mtsn;		/* serial number */
	int	mtmr2;		/* maint */
	int	mtmr3;		/* maint */
	int	mtner;		/* non-data transfer error */
	int	mtncs[TM78DRIVES];	/* non-data transfer command */
	int	mtia;		/* internal address */
	int	mtid;		/* internal data */
};

/* mtcs */
#define	MT_GO		000001		/* go bit */
#define	MT_GCR		000002		/* make generic ops GCR ops */
#define	MT_UNLOAD	000004		/* unload tape */
#define	MT_REW		000006		/* rewind */
#define	MT_SENSE	000010		/* sense */
#define	MT_WTM		000014		/* generic write tape mark */
#define	MT_SFORW	000020		/* space forward record */
#define	MT_SREV		000022		/* space reverse record */
#define	MT_SFORWF	000024		/* space forward file */
#define	MT_SREVF	000026		/* space reverse file */
#define	MT_CLS		000040		/* generic close file */
#define	MT_WRITE	000060		/* generic write */
#define	MT_READ		000070		/* read forward */
#define	MT_READREV	000076		/* read reverse */

/* mtds */
#define	MTDS_PRES	0040000		/* tape unit has power */
#define	MTDS_ONL	0020000		/* online */
#define	MTDS_PE		0004000		/* tape set for phase encoded */
#define	MTDS_BOT	0002000		/* tape at BOT */
#define	MTDS_FPT	0000400		/* write protected */
#define	MTDS_AVAIL	0000200		/* unit available */

/* mtncs */
#define	MT_COFF		8		/* bits to shift count */
#define	MTNCS_COUNT(c)	(((c)>>MT_COFF)&0377)	/* repeat count */
#define	MAXCOUNT	0377

/* mter */
#define	MTER_INTCODE(e)	((int)(e)&0377)	/* interrupt code */
#define	MTER_DPR	0400		/* drive (formatter) present */
#define	MTER_UNIT(e)	(((e)>>8)&03)	/* interrupting unit in mtner */

/* interrupt codes */
#define	MTER_DONE	001		/* operation complete */
#define	MTER_TM		002		/* unexpected tape mark */
#define	MTER_BOT	003		/* unexpected BOT detected */
#define	MTER_EOT	004		/* tape positioned beyond EOT */
#define	MTER_LEOT	005		/* unexpected LEOT detected */
#define	MTER_NOOP	006		/* no-op completed */
#define	MTER_RWDING	007		/* rewinding */
#define	MTER_FPT	010		/* write protect error */
#define	MTER_NOTRDY	011		/* not ready */
#define	MTER_NOTAVL	012		/* not available */
#define	MTER_OFFLINE	013		/* offline */
#define	MTER_NONEX	014		/* unit does not exist */
#define	MTER_NOTCAP	015		/* not capable */
#define	MTER_ONLINE	017		/* tape came online */
#define	MTER_LONGREC	020		/* long tape record */
#define	MTER_SHRTREC	021		/* short tape record */
#define	MTER_RETRY	022		/* retry */
#define	MTER_RDOPP	023		/* read opposite */
#define	MTER_UNREAD	024		/* unreadable */
#define	MTER_ERROR	025		/* error */
#define	MTER_EOTERR	026		/* EOT error */
#define	MTER_BADTAPE	027		/* tape position lost */
#define	MTER_TMFLTA	030		/* TM fault A */
#define	MTER_TUFLTA	031		/* TU fault A */
#define	MTER_TMFLTB	032		/* TM fault B */
#define	MTER_MBFLT	034		/* Massbus fault */
#define	MTER_KEYFAIL	077		/* keypad entry error */

/* mtid */
#define	MTID_RDY	0100000		/* controller ready */
#define	MTID_CLR	0040000		/* controller clear */

#define MTTIMEOUT	10000		/* loop limit for controller test */

/*
 * system stuff
 */

int tu78open(), tu78close(), tu78read(), tu78write(), tu78ioctl(), tu78strategy();
struct cdevsw tu78cdev = cdinit(tu78open, tu78close, tu78read, tu78write, tu78ioctl);
struct bdevsw tu78bdev = bdinit(tu78open, tu78close, tu78strategy, B_TAPE);

/*
 * per-formatter stuff
 */

extern int tm78cnt;
extern struct tm78 tm78[];
extern struct mbaddr tm78addr[];

#define	T_BUSY	01		/* controller busy */
#define	NOUNIT	0377		/* no slave expected */

/*
 * per-drive stuff
 */

extern int tu78cnt;
extern struct tu78 tu78[];
extern struct subaddr tu78addr[];
extern struct buf rtu78buf[];
extern struct buf ctu78buf[];

/*
 * bits in minor device
 */
#define	TUUNIT(dev)	(minor(dev)&03)
#define	H_NOREWIND	04
#define	H_6250BPI	010

/*
 * sc_flags
 */
#define	H_BUSY	01	/* software drive busy */
#define	H_EOT	02	/* saw physical end-of-tape */
#define	H_WRITTEN 04	/* last operation was a write */
#define H_ERASED  010	/* last write retry was an erase gap */
#define	H_OPEN	020	/* device open */
#define	H_OFFLINE 040	/* device went offline; no io till close */
#define	H_WONLY	0100	/* write-only */

/*
 * scaling for block magtape blocks
 */
#define	INF	1000000L /* a block number that won't exist */
#define	TBLOCK	2	/* tape blocks per b_blkno block */
#define	TBSIZE	1024
#define	TBSHIFT	10

/*
 * overloads in struct buf; for tu78cmd
 */
#define	b_repcnt  b_bcount
#define	b_command b_resid

tu78open(dev, flag)
	dev_t dev;
	int flag;
{
	register int tuunit;
	register struct tu78 *sc;
	register struct subaddr *sa;
	int dens;

	tuunit = TUUNIT(dev);
	if (tuunit >= tu78cnt || (sc = &tu78[tuunit])->sc_flags & H_OPEN) {
		u.u_error = ENXIO;
		return;
	}
	sc->sc_flags |= H_OPEN;
	sa = &tu78addr[tuunit];
	if (tm78init(sa->ctl, sa->unit, tuunit) == 0) {
		sc->sc_flags &=~ H_OPEN;
		u.u_error = ENXIO;
		return;
	}
	sc->sc_ctl = &tm78[sa->ctl];
	sc->sc_dens = minor(dev) & H_6250BPI ? MT_GCR : 0;
	sc->sc_slave = sa->unit;
	sc->sc_flags &=~ H_OFFLINE;	/* so we can sense */
	tu78cmd(dev, MT_SENSE, 1L);
	if ((sc->sc_dsreg & MTDS_PRES) == 0) {	/* what's the right test? */
		printf("tu78 %d absent\n", tuunit);
		sc->sc_flags &=~ H_OPEN;
		u.u_error = ENODEV;
		return;
	}
	if ((sc->sc_dsreg & MTDS_ONL) == 0) {
		sc->sc_flags &=~ H_OPEN;
		u.u_error = EIO;
		return;
	}
	if ((flag&FWRITE) && (sc->sc_dsreg&MTDS_FPT)) {
		sc->sc_flags &=~ H_OPEN;
		u.u_error = EIO;
		return;
	}
	if ((sc->sc_dsreg & MTDS_BOT) == 0 && flag&FWRITE) {
		dens = sc->sc_dsreg & MTDS_PE ? 0 : MT_GCR;
		if (sc->sc_dens != dens) {
			sc->sc_flags &=~ H_OPEN;
			u.u_error = EIO;
			return;
		}
	}
	sc->sc_blkno = 0;
	sc->sc_nxrec = INF;
	if ((flag & (FREAD|FWRITE)) == FWRITE)
		sc->sc_flags |= H_WONLY;	/* stop block dev readahead */
}

tm78init(ctl, slave, dev)
register int ctl;
int slave, dev;
{
	register struct tm78 *cc;
	register int i;

	if (ctl < 0 || slave < 0 || ctl >= tm78cnt || slave >= TM78DRIVES)
		return (0);
	cc = &tm78[ctl];
	cc->cc_drives[slave] = dev;
	if (cc->cc_mbaddr)
		return (1);
	if ((cc->cc_addr = (struct device *)mbaddr(&tm78addr[ctl])) == 0
	||  badaddr(&cc->cc_addr->mtcs, sizeof(long))
	||  (cc->cc_addr->mter & MTER_DPR) == 0) {
		printf("tm78 %d absent\n", ctl);
		return (0);
	}
	for (i = 0; i < TM78DRIVES; i++)
		if (i != slave)
			cc->cc_drives[i] = NOUNIT;
	if (tm78reset(ctl) == 0)
		return (0);
	cc->cc_mbaddr = &tm78addr[ctl];
	return (1);
}

/*
 * reset the formatter
 * initially or after some awful error
 */
tm78reset(ctl)
int ctl;
{
	register struct tm78 *cc;
	register struct device *tmaddr;
	register struct tu78 *sc;
	register struct buf *bp;
	register int i;

	cc = &tm78[ctl];
	tmaddr = cc->cc_addr;
	tmaddr->mtid = MTID_CLR;
	while ((sc = cc->cc_actf) != NULL) {
		cc->cc_actf = sc->sc_next;
		while ((bp = sc->sc_actf) != NULL) {
			sc->sc_actf = bp->av_forw;
			bp->b_flags |= B_ERROR;
			iodone(bp);
		}
		sc->sc_flags &=~ H_BUSY;
	}
	cc->cc_flags &=~ T_BUSY;
	DELAY(200);
	for (i = 0; i < MTTIMEOUT; i++) {
		if (tmaddr->mtid & MTID_RDY)
			return (1);		/* done */
		DELAY(50);		/* wait a bit more */
	}
	printf("tm78 %d: never ready\n", ctl);
	return (0);
}

tu78close(dev)
	register dev_t dev;
{
	register struct tu78 *sc = &tu78[TUUNIT(dev)];

	if (sc->sc_flags & (H_WONLY|H_WRITTEN))
		tu78cmd(dev, MT_CLS|sc->sc_dens, 1L);
	if ((minor(dev)&H_NOREWIND) == 0)
		tu78cmd(dev, MT_REW, 0L);
	sc->sc_flags &= H_BUSY;
}

tu78cmd(dev, com, count)
dev_t dev;
int com;
register daddr_t count;
{
	register struct buf *bp;
	register int some;

	bp = &ctu78buf[TUUNIT(dev)];
	(void) spl5();
	while (bp->b_flags&B_BUSY) {
		if(bp->b_repcnt == 0 && (bp->b_flags&B_DONE))
			break;
		bp->b_flags |= B_WANTED;
		sleep((caddr_t)bp, PRIBIO);
	}
	bp->b_flags = B_BUSY|B_READ;
	(void) spl0();
	bp->b_dev = dev;
	bp->b_command = com;
	bp->b_blkno = 0;
	do {
		if (count <= MAXCOUNT)
			some = count;
		else
			some = MAXCOUNT;
		bp->b_repcnt = some;
		tu78strategy(bp);
		if (count == 0) {
			if (bp->b_flags & B_DONE)
				geterror(bp);
			return;		/* sic */
		}
		iowait(bp);
	} while (u.u_error == 0 && (count -= some) > 0);
	if (bp->b_flags&B_WANTED)
		wakeup((caddr_t)bp);
	bp->b_flags &=~ B_BUSY;
}

tu78strategy(bp)
	register struct buf *bp;
{
	register struct tu78 *sc;

	sc = &tu78[TUUNIT(bp->b_dev)];
	bp->av_forw = NULL;
	(void) spl5();
	if (sc->sc_actf == NULL)
		sc->sc_actf = bp;
	else
		sc->sc_actl->av_forw = bp;
	sc->sc_actl = bp;
	if ((sc->sc_flags & H_BUSY) == 0)
		tu78start(sc);
	if (sc->sc_ctl->cc_actf && (sc->sc_ctl->cc_flags & T_BUSY) == 0)
		tm78start(sc->sc_ctl);
	(void) spl0();
}

tu78start(sc)
register struct tu78 *sc;
{
	register struct buf *bp;
	register struct device *tmaddr;
	register struct tm78 *cc;
	register daddr_t bno;
	register int cmd, dist;

	cc = sc->sc_ctl;
loop:
	if ((bp = sc->sc_actf) == 0)
		return;
	tmaddr = cc->cc_addr;
	sc->sc_flags &= ~H_WRITTEN;
	if (sc->sc_flags & H_OFFLINE) {
		bp->b_flags |= B_ERROR;
		goto done;
	}
	/*
	 * command, start it
	 */
	if (bp == &ctu78buf[TUUNIT(bp->b_dev)]) {
		sc->sc_flags |= H_BUSY;
		cmd = bp->b_command|MT_GO;
		cmd |= bp->b_repcnt << MT_COFF;
#if BABBLE
printf("cmd %o\n", cmd);
#endif
		tmaddr->mtncs[sc->sc_slave] = cmd;
		return;
	}
	/*
	 * transfer, see if position needed (wretched block magtape)
	 */
	bno = bp->b_blkno/TBLOCK;	
	if (bno > sc->sc_nxrec) {
		bp->b_error = ENXIO;
		bp->b_flags |= B_ERROR;
		goto done;
	}
	if (bp->b_flags & B_READ	/* at EOF, or really just writing? */
	&&  (bno == sc->sc_nxrec || sc->sc_flags & H_WONLY)) {
		bp->b_resid = bp->b_bcount;
		goto done;
	}
	if ((bp->b_flags&B_READ)==0)
		sc->sc_nxrec = bno + 1;
	if (sc->sc_blkno != bno) {	/* need position */
		sc->sc_flags |= H_BUSY;
		if (sc->sc_blkno < bno) {
			sc->sc_flags &=~ H_EOT;
			dist = bno - sc->sc_blkno;
			cmd = MT_SREV;
		} else {
			dist = sc->sc_blkno - bno;
			cmd = MT_SFORW;
		}
		if (dist > MAXCOUNT)
			dist = MAXCOUNT;
		tmaddr->mtncs[sc->sc_slave] = (dist<<MT_COFF)|cmd|MT_GO;
		return;
	}
	/*
	 * time for a transfer
	 */
	if ((bp->b_flags&B_READ) == 0 && sc->sc_flags & H_EOT) {
		bp->b_resid = bp->b_bcount;
done:
		sc->sc_actf = bp->av_forw;
		iodone(bp);
		goto loop;
	}
	cc = sc->sc_ctl;
	sc->sc_next = NULL;
	if (cc->cc_actf == NULL)
		cc->cc_actf = sc;
	else
		cc->cc_actl->sc_next = sc;
	cc->cc_actl = sc;
	sc->sc_flags |= H_BUSY;
}

tm78start(cc)
register struct tm78 *cc;
{
	register struct tu78 *sc;
	int tm78xfer();

	if ((sc = cc->cc_actf) == NULL)
		return;
	if (sc->sc_actf == NULL)
		panic("tm78start");
	cc->cc_flags |= T_BUSY;
	mbstart(cc->cc_mbaddr, sc->sc_actf, tm78xfer);
}

tm78xfer(bp)
register struct buf *bp;
{
	register struct tu78 *sc;
	register struct device *tmaddr;

	sc = &tu78[TUUNIT(bp->b_dev)];
	tmaddr = sc->sc_ctl->cc_addr;
	tmaddr->mtca = sc->sc_slave;
	tmaddr->mtbc = bp->b_bcount;
#if BABBLE
printf("xfer go\n");
#endif
	if (bp->b_flags & B_READ)
		tmaddr->mtcs = MT_READ|MT_GO;
	else
		tmaddr->mtcs = MT_WRITE|sc->sc_dens|MT_GO;
}

/*
 * massbus interrupt
 * if a transfer was pending, assume it finished;
 * if attn set, something else finished too
 */
tm780int(ctl, mbsr, mbbc, attn)
int ctl, mbsr, mbbc, attn;
{
	register struct tm78 *cc;

	cc = &tm78[ctl];
	if (cc->cc_addr == NULL) {
		printf("tm78 %d: stray intr\n", ctl);
		return;
	}
	if (attn)
		tu78attn(ctl, mbsr, attn);
	if (cc->cc_flags & T_BUSY)
		tu78dtint(ctl, mbsr);
	if (cc->cc_actf)
		tm78start(cc);
}

/*
 * here with an attention:
 * a boring interrupt, a disaster, or a command finished
 * don't clear the attn until we've read the registers
 */

tu78attn(ctl, mbsr, attn)
int ctl, mbsr, attn;
{
	register struct tm78 *cc;
	register struct tu78 *sc;
	register struct buf *bp;
	register struct device *tmaddr;
	register int unit;
	daddr_t bno;
	register int i;
	register long rt;	/* register temp to avoid compiler botches */

	cc = &tm78[ctl];
	tmaddr = cc->cc_addr;
	rt = tmaddr->mtner;
	i = MTER_UNIT(rt);
#if BABBLE
printf("attn er%o ds%o cs%o\n", tmaddr->mtner&0xffff, tmaddr->mtds&0xffff, tmaddr->mtncs[i]&0xffff);
#endif
	switch (MTER_INTCODE(rt)) {
	case MTER_ONLINE:
	case MTER_RWDING:
		tmaddr->mtas = attn;
		return;			/* boring */

	case MTER_MBFLT:
	case MTER_TMFLTB:
		printf("tm78 %d fault er%o mbsr%o\n", ctl, tmaddr->mtner, mbsr);
		tm78reset(ctl);
		tmaddr->mtas = attn;
		return;
	}
	if ((unit = cc->cc_drives[i]) == NOUNIT) {
		printf("tm78 %d unit %d stray intr er%o\n", ctl, i, tmaddr->mtner);
		tmaddr->mtas = attn;
		return;
	}
	sc = &tu78[unit];
	rt = tmaddr->mtds; sc->sc_dsreg = rt;
	rt = tmaddr->mtner; sc->sc_erreg = rt;
	rt = tmaddr->mtncs[i]; sc->sc_resid = MTNCS_COUNT(rt);
	tmaddr->mtas = attn;
	if ((bp = sc->sc_actf) == NULL) {
		printf("tu78 %d: odd intr er%o\n", unit, sc->sc_erreg);
		return;
	}
	switch (MTER_INTCODE(sc->sc_erreg)) {
	case MTER_EOT:
		sc->sc_flags |= H_EOT;
		/* fall through */
	case MTER_TM:
	case MTER_LEOT:
	case MTER_BOT:
	case MTER_NOOP:
	case MTER_DONE:
		/*
		 * this was a block magtape seek,
		 * or we're closing (so sc_blkno is boring),
		 * or it's raw magtape (ditto)
		 */
		bno = bp->b_blkno / TBLOCK;
		if (sc->sc_blkno > bno)
			sc->sc_blkno += sc->sc_resid;
		else
			sc->sc_blkno -= sc->sc_resid;
		if (sc->sc_flags & H_EOT)
			sc->sc_nxrec = sc->sc_blkno;
		if (bp != &ctu78buf[unit])
			sc->sc_flags &=~ H_BUSY;	/* done with seek */
		break;

	case MTER_NOTAVL:
	case MTER_OFFLINE:
	case MTER_NONEX:
		if (sc->sc_flags & H_OPEN)
			sc->sc_flags |= H_OFFLINE;
		bp->b_error = ENXIO;
		bp->b_flags |= B_ERROR;
		break;

	default:
		printf("tu78 %d: hard error bn%d er%o mbsr%o\n",
			unit, bp->b_blkno, sc->sc_erreg, mbsr);
		bp->b_flags |= B_ERROR;
		break;
	}
	if (sc->sc_flags & H_BUSY) {
		sc->sc_actf = bp->av_forw;
		iodone(bp);
		sc->sc_flags &=~ H_BUSY;
	}
	if (sc->sc_actf)
		tu78start(sc);
}

tu78dtint(ctl, mbsr)
int ctl, mbsr;
{
	register struct tm78 *cc;
	register struct tu78 *sc;
	register struct buf *bp;
	register struct device *tmaddr;
	register int rt;	/* reg temp to avoid compiler botches */

	cc = &tm78[ctl];
	tmaddr = cc->cc_addr;
	if ((sc = cc->cc_actf) == NULL
	||  (bp = sc->sc_actf) == NULL)
		panic("tu78dtint");
	rt = tmaddr->mtbc; sc->sc_resid = rt;
	rt = tmaddr->mter; sc->sc_erreg = rt;
#if BABBLE
printf("dtint er%o bc%o mbs%o\n", sc->sc_erreg, sc->sc_resid, mbsr);
#endif
	if (mbsr & MBSR_HARD) {
		printf("tu78 %d: mb error er%o mbsr%o\n",
			TUUNIT(bp->b_dev), sc->sc_erreg, mbsr);
		bp->b_flags |= B_ERROR;
	} else switch (MTER_INTCODE(sc->sc_erreg)) {
	case MTER_MBFLT:
	case MTER_TMFLTB:
		printf("tm78 %d fault er%o mbsr%o\n", ctl, sc->sc_erreg, mbsr);
		tm78reset(ctl);
		return;

	case MTER_EOT:
		sc->sc_flags |= H_EOT;
		/* fall through */
	case MTER_LEOT:
	case MTER_TM:
		bp->b_resid = bp->b_bcount;
		sc->sc_nxrec = sc->sc_blkno;
		break;

	case MTER_LONGREC:
		sc->sc_blkno++;
		bp->b_error = ENOMEM;
		bp->b_flags |= B_ERROR;
		break;

	case MTER_SHRTREC:
		if (bp != &rtu78buf[TUUNIT(bp->b_dev)])	/* block tape */
			bp->b_flags |= B_ERROR;
		/* fall in */
	case MTER_DONE:
		bp->b_resid = bp->b_bcount - sc->sc_resid;
		sc->sc_blkno++;
		break;

	case MTER_RETRY:
		cc->cc_flags &=~ T_BUSY;
		break;

	case MTER_NOTAVL:
	case MTER_OFFLINE:
	case MTER_NONEX:
		if (sc->sc_flags & H_OPEN)
			sc->sc_flags |= H_OFFLINE;
		bp->b_error = ENXIO;
		bp->b_flags |= B_ERROR;
		break;

	case MTER_NOTCAP:
	case MTER_BADTAPE:
		if (sc->sc_flags & H_OPEN)
			sc->sc_flags |= H_OFFLINE;	/* really position lost */
		/* fall through */
	default:
		printf("tu78 %d: hard error bn%d er%o mbsr%o\n",
			TUUNIT(bp->b_dev), bp->b_blkno, sc->sc_erreg, mbsr);
		sc->sc_blkno++;
		bp->b_flags |= B_ERROR;
		break;
	}
	if (cc->cc_flags & T_BUSY) {
		cc->cc_actf = sc->sc_next;
		sc->sc_actf = bp->av_forw;
		iodone(bp);
		sc->sc_flags &=~ H_BUSY;
		if (sc->sc_actf)
			tu78start(sc);
		cc->cc_flags &=~ T_BUSY;
	}
}

tu78read(dev)
dev_t dev;
{

	tu78phys(dev);
	physio(tu78strategy, &rtu78buf[TUUNIT(dev)], dev, B_READ, minphys);
}

tu78write(dev)
dev_t dev;
{

	tu78phys(dev);
	physio(tu78strategy, &rtu78buf[TUUNIT(dev)], dev, B_WRITE, minphys);
}

tu78phys(dev)
dev_t dev;
{
	register struct tu78 *sc;

	sc = &tu78[TUUNIT(dev)];
	sc->sc_blkno = Lshift(u.u_offset, TBSHIFT);
	sc->sc_nxrec = sc->sc_blkno + 1;
}

tu78ioctl(dev, cmd, addr, flag)
dev_t dev;
int cmd;
caddr_t addr;
int flag;
{
	struct mtop mtop;

	switch (cmd) {
	case MTIOCTOP:		/* tape operation */
		if (copyin((caddr_t)addr, (caddr_t)&mtop, sizeof(mtop))) {
			u.u_error = EFAULT;
			return;
		}
		if (mtop.mt_count == 0)
			mtop.mt_count = 1;
		switch(mtop.mt_op) {
		case MTWEOF:
			tu78cmd(dev, MT_WTM|tu78[TUUNIT(dev)].sc_dens, mtop.mt_count);
			break;

		case MTFSF:
			tu78cmd(dev, MT_SFORWF, mtop.mt_count);
			break;

		case MTBSF:
			tu78cmd(dev, MT_SREVF, mtop.mt_count);
			break;

		case MTFSR:
			tu78cmd(dev, MT_SFORW, mtop.mt_count);
			break;

		case MTBSR:
			tu78cmd(dev, MT_SREV, mtop.mt_count);
			break;

		case MTREW:
			tu78cmd(dev, MT_REW, 1L);
			break;

		case MTOFFL:
			tu78cmd(dev, MT_UNLOAD, 1L);
			break;

		default:
			u.u_error = ENXIO;
			break;
		}
		return;

	default:
		u.u_error = ENXIO;
		return;
	}
}
