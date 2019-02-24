/*
 * TM03/TE16 tape driver
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
#include "sys/te16.h"

/*
 * hardware stuff
 */
struct	device {
	int	htcs1;		/* control status */
	int	htds;		/* drive status */
	int	hter;		/* error */
	int	htmr;		/* maintenance */
	int	htas;		/* attention status */
	int	htfc;		/* frame count */
	int	htdt;		/* drive type */
	int	htck;		/* nrzi check (crc) error character */
	int	htsn;		/* serial number */
	int	httc;		/* tape control */
};

/* htcs1 */
#define	HT_GO		000001		/* go bit */
#define	HT_REWOFFL	000002		/* rewind offline */
#define	HT_REW		000006		/* rewind */
#define	HT_DCLR		000010		/* drive clear */
#define	HT_ERASE	000024		/* erase */
#define	HT_WEOF		000026		/* write tape mark */
#define	HT_SFORW	000030		/* space forward */
#define	HT_SREV		000032		/* space reverse */
#define	HT_WCOM		000060		/* write forward */
#define	HT_RCOM		000070		/* read forward */

/* htds */
#define	HTDS_ATA	0100000		/* attention active */
#define	HTDS_ERR	0040000		/* composite error */
#define	HTDS_MOL	0010000		/* medium on line */
#define	HTDS_WRL	0004000		/* write lock */
#define	HTDS_EOT	0002000		/* end of tape */
#define	HTDS_DPR	0000400		/* drive present (always 1) */
#define	HTDS_DRY	0000200		/* drive ready */
#define	HTDS_SSC	0000100		/* slace status changed */
#define	HTDS_PES	0000040		/* phase-encoded status */
#define	HTDS_TM		0000004		/* tape mark */
#define	HTDS_BOT	0000002		/* beginning of tape */
#define	HTDS_SLA	0000001		/* slave attention */

/* hter */
#define	HTER_CORCRC	0100000		/* correctible data or ecc */
#define	HTER_UNS	0040000		/* unsafe */
#define	HTER_OPI	0020000		/* operation incomplete */
#define	HTER_DTE	0010000		/* drive timing error */
#define	HTER_NEF	0004000		/* non-executable function */
#define	HTER_CSITM	0002000		/* correctable skew/illegal tape mark */
#define	HTER_FCE	0001000		/* frame count error */
#define	HTER_NSG	0000400		/* non-standard gap */
#define	HTER_PEFLRC	0000200		/* format error or lrc error */
#define	HTER_DPAR	0000040		/* data parity error */
#define	HTER_FMT	0000020		/* format error */
#define	HTER_CPAR	0000010		/* control bus parity error */
#define	HTER_RMR	0000004		/* register modification refused */
#define	HTER_ILR	0000002		/* illegal register */
#define	HTER_ILF	0000001		/* illegal function */
#define	HTER_HARD \
	(HTER_UNS|HTER_OPI|HTER_NEF|HTER_DPAR|HTER_FMT|HTER_CPAR| \
	HTER_RMR|HTER_ILR|HTER_ILF)

/* htdt */
#define	HTDT_SPR	0002000		/* slave present */

/* httc */
#define	HTTC_800BPI	0001400		/* in bits 8-10, dens=1600 */
#define	HTTC_1600BPI	0002000		/* in bits 8-10, dens=800 */
#define	HTTC_PDP11	0000300		/* in bits 4-7, pdp11 normal format */

/*
 * system stuff
 */

int te16open(), te16close(), te16read(), te16write(), te16ioctl(), te16strategy();
struct cdevsw te16cdev = cdinit(te16open, te16close, te16read, te16write, te16ioctl);
struct bdevsw te16bdev = bdinit(te16open, te16close, te16strategy, B_TAPE);

/*
 * per-formatter stuff
 */

extern int tm03cnt;
extern struct tm03 tm03[];
extern struct mbaddr tm03addr[];

#define	T_BUSY	01		/* controller busy */
#define	NOUNIT	0377		/* no unit expected */

/*
 * per-drive stuff
 */

extern int te16cnt;
extern struct te16 te16[];
extern struct subaddr te16addr[];
extern struct buf rte16buf[];
extern struct buf cte16buf[];

/*
 * bits in minor device
 */
#define	TEUNIT(dev)	(minor(dev)&03)
#define	H_NOREWIND	04
#define	H_1600BPI	010

/*
 * sc_flags
 */
#define	H_BUSY	01	/* software drive busy */
#define	H_CMD	02	/* hardware command pending */
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
 * overloads in struct buf; for te16cmd
 */
#define	b_repcnt  b_bcount
#define	b_command b_resid

te16open(dev, flag)
	dev_t dev;
	int flag;
{
	register int teunit;
	register struct te16 *sc;
	register struct subaddr *sa;
	register struct device *htaddr;
	register int dt;

	teunit = TEUNIT(dev);
	if (teunit >= te16cnt || (sc = &te16[teunit])->sc_flags & H_OPEN) {
		u.u_error = ENXIO;
		return;
	}
	sc->sc_flags |= H_OPEN;
	sa = &te16addr[teunit];
	if (tm03init(sa->ctl, sa->unit, teunit) == 0) {
		sc->sc_flags &=~ H_OPEN;
		u.u_error = ENXIO;
		return;
	}
	sc->sc_ctl = &tm03[sa->ctl];
	htaddr = sc->sc_ctl->cc_addr;
	sc->sc_dens = ((minor(dev)&H_1600BPI)?HTTC_1600BPI:HTTC_800BPI)|HTTC_PDP11|sa->unit;
	spl5();
	htaddr->httc = sc->sc_dens;
	/* delay here? */
	dt = htaddr->htdt;
	sc->sc_dsreg = htaddr->htds;
	spl0();
	if ((dt & HTDT_SPR) == 0) {
		printf("te16 %d absent\n", teunit);
		sc->sc_flags &=~ H_OPEN;
		u.u_error = ENODEV;
		return;
	}
	if ((sc->sc_dsreg & HTDS_MOL) == 0) {
		sc->sc_flags &=~ H_OPEN;
		u.u_error = EIO;
		return;
	}
	if ((flag&FWRITE) && (sc->sc_dsreg&HTDS_WRL)) {
		sc->sc_flags &=~ H_OPEN;
		u.u_error = EIO;
		return;
	}
	if ((sc->sc_dsreg & HTDS_BOT) == 0 && flag&FWRITE
	&&  ((minor(dev) & H_1600BPI) != 0) != ((sc->sc_dsreg & HTDS_PES) != 0)) {
		sc->sc_flags &=~ H_OPEN;
		u.u_error = EIO;
		return;
	}
	sc->sc_blkno = 0;
	sc->sc_nxrec = INF;
	if ((flag & (FREAD|FWRITE)) == FWRITE)
		sc->sc_flags |= H_WONLY;	/* stop block dev readahead */
}

tm03init(ctl, slave, dev)
register int ctl;
int slave, dev;
{
	register struct tm03 *cc;
	register int i;

	if (ctl < 0 || slave < 0 || ctl >= tm03cnt || slave >= TM03DRIVES)
		return (0);
	cc = &tm03[ctl];
	cc->cc_drives[slave] = dev;
	if (cc->cc_addr)
		return (1);
	if ((cc->cc_addr = (struct device *)mbaddr(&tm03addr[ctl])) == 0
	||  badaddr(&cc->cc_addr->htcs1, sizeof(long))
	||  (cc->cc_addr->htds & HTDS_DPR) == 0) {
		printf("tm03 %d absent\n", ctl);
		return (0);
	}
	cc->cc_mbaddr = &tm03addr[ctl];
	for (i = 0; i < TM03DRIVES; i++)
		if (i != slave)
			cc->cc_drives[i] = NOUNIT;
	/* any reset worth doing? */
	return (1);
}

te16close(dev)
	register dev_t dev;
{
	register struct te16 *sc = &te16[TEUNIT(dev)];

	if (sc->sc_flags & (H_WONLY|H_WRITTEN)) {
		te16cmd(dev, HT_WEOF, 1);
		te16cmd(dev, HT_WEOF, 1);
		te16cmd(dev, HT_SREV, 1);
	}
	if ((minor(dev)&H_NOREWIND) == 0)
		te16cmd(dev, HT_REW, 0);
	sc->sc_flags &= H_CMD|H_BUSY;
}

te16cmd(dev, com, count)
	dev_t dev;
	int com, count;
{
	register struct buf *bp;

	bp = &cte16buf[TEUNIT(dev)];
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
	bp->b_repcnt = count;
	bp->b_blkno = 0;
	te16strategy(bp);
	if (count == 0) {
		if (bp->b_flags & B_DONE)
			geterror(bp);
		return;
	}
	iowait(bp);
	if (bp->b_flags&B_WANTED)
		wakeup((caddr_t)bp);
	bp->b_flags &=~ B_BUSY;
}

te16strategy(bp)
	register struct buf *bp;
{
	register struct te16 *sc;

	sc = &te16[TEUNIT(bp->b_dev)];
	bp->av_forw = NULL;
	(void) spl5();
	if (sc->sc_actf == NULL)
		sc->sc_actf = bp;
	else
		sc->sc_actl->av_forw = bp;
	sc->sc_actl = bp;
	if ((sc->sc_flags & H_BUSY) == 0)
		te16start(sc);
	if (sc->sc_ctl->cc_actf && (sc->sc_ctl->cc_flags & T_BUSY) == 0)
		tm03start(sc->sc_ctl);
	(void) spl0();
}

te16start(sc)
register struct te16 *sc;
{
	register struct buf *bp;
	register struct device *htaddr;
	register struct tm03 *cc;
	register daddr_t bno;

	cc = sc->sc_ctl;
loop:
	if ((bp = sc->sc_actf) == 0)
		return;
	htaddr = cc->cc_addr;
	htaddr->httc = sc->sc_dens;
	if (htaddr->htds & HTDS_SSC)
		htaddr->htcs1 = HT_DCLR|HT_GO;
	sc->sc_dsreg = htaddr->htds;
	sc->sc_erreg = htaddr->hter;
	sc->sc_resid = htaddr->htfc;
	sc->sc_flags &= ~H_WRITTEN;
	if ((htaddr->htdt & HTDT_SPR) == 0 || (htaddr->htds & HTDS_MOL) == 0)
		if (sc->sc_flags & H_OPEN)
			sc->sc_flags |= H_OFFLINE;
	if (sc->sc_flags & H_OFFLINE) {
		bp->b_flags |= B_ERROR;
		goto done;
	}
	/*
	 * command, start it
	 */
	if (bp == &cte16buf[TEUNIT(bp->b_dev)]) {
		sc->sc_flags |= H_BUSY|H_CMD;
		htaddr->htfc = -bp->b_bcount;
		htaddr->htcs1 = bp->b_command|HT_GO;
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
		sc->sc_flags |= H_BUSY|H_CMD;
		if (sc->sc_blkno < bno) {
			htaddr->htfc = sc->sc_blkno - bno;
			htaddr->htcs1 = HT_SFORW|HT_GO;
		} else {
			htaddr->htfc = bno - sc->sc_blkno;
			htaddr->htcs1 = HT_SREV|HT_GO;
		}
		return;
	}
	/*
	 * special pleading for `erase gap' write error recovery
	 * -- is this in the right place?  why bother?
	 */
	if ((bp->b_flags&B_READ) == 0) {
		if (cc->cc_errcnt) {
			if ((sc->sc_flags & H_ERASED) == 0) {
				sc->sc_flags |= H_ERASED|H_BUSY|H_CMD;
				htaddr->htcs1 = HT_ERASE|HT_GO;
				return;
			}
			sc->sc_flags &= ~H_ERASED;
		}
		if (htaddr->htds & HTDS_EOT) {		/* oops */
			bp->b_resid = bp->b_bcount;
done:
			sc->sc_actf = bp->av_forw;
			iodone(bp);
			goto loop;
		}
	}
	/*
	 * time for a transfer
	 */
	cc = sc->sc_ctl;
	sc->sc_next = NULL;
	if (cc->cc_actf == NULL)
		cc->cc_actf = sc;
	else
		cc->cc_actl->sc_next = sc;
	cc->cc_actl = sc;
	sc->sc_flags |= H_BUSY;
}

tm03start(cc)
register struct tm03 *cc;
{
	register struct te16 *sc;
	int tm03xfer();

	if ((sc = cc->cc_actf) == NULL)
		return;
	if (sc->sc_actf == NULL)
		panic("tm03start");
	cc->cc_flags |= T_BUSY;
	mbstart(cc->cc_mbaddr, sc->sc_actf, tm03xfer);
}

tm03xfer(bp)
register struct buf *bp;
{
	register struct te16 *sc;
	register struct device *htaddr;

	sc = &te16[TEUNIT(bp->b_dev)];
	htaddr = sc->sc_ctl->cc_addr;
	htaddr->httc = sc->sc_dens;
	htaddr->htfc = -bp->b_bcount;
	sc->sc_flags |= H_CMD;
	if (bp->b_flags & B_READ)
		htaddr->htcs1 = HT_RCOM|HT_GO;
	else
		htaddr->htcs1 = HT_WCOM|HT_GO;
}

/*
 * massbus interrupt
 * if a transfer was pending, assume it finished;
 * if attn set, something else finished too
 *
 * the loop to see which drives wanted attention is somewhat doubtful
 * and has not been tested with multiple drives
 */
tm030int(ctl, mbsr, mbbc, attn)
int ctl, mbsr, mbbc, attn;
{
	register struct tm03 *cc;
	register struct device *htaddr;
	register struct te16 *sc;
	register int i;

	cc = &tm03[ctl];
	if ((htaddr = cc->cc_addr) == NULL) {
		printf("tm03 %d: stray intr\n", ctl);
		return;
	}
	htaddr->htas = attn;		/* before we do anything */
	if (cc->cc_flags & T_BUSY)
		te16dtint(cc, mbsr);
	if (attn) {
		for (i = 0; i < TM03DRIVES; i++) {
			if (cc->cc_drives[i] == NOUNIT)
				continue;
			sc = &te16[cc->cc_drives[i]];
			htaddr->httc = sc->sc_dens;	/* select drive */
			if ((sc->sc_flags & H_CMD) == 0) {
				sc->sc_dsreg = htaddr->htds;	/* for BOT detection */
				wakeup((caddr_t)sc);
				if (htaddr->htds & HTDS_SSC)
					htaddr->htcs1 = HT_DCLR|HT_GO;
				continue;
			}
			if ((htaddr->htds & HTDS_DRY) == 0)
				continue;
			te16attn(sc);
		}
	}
	if (cc->cc_actf)
		tm03start(cc);
}

/*
 * error recovery is somewhat doubtful
 * particular specialties:
 * - short reads show up as frame count errors,
 *   so ignore them for the raw device
 * - reading at tape mark seems to produce a frame count error too
 * - quietly ignore CRC errors the drive corrected
 */
te16dtint(cc, mbsr)
register struct tm03 *cc;
int mbsr;
{
	register struct device *htaddr;
	register struct buf *bp;
	register struct te16 *sc;
	int ds, er, mbs;

	htaddr = cc->cc_addr;
	if ((sc = cc->cc_actf) == NULL)
		panic("te16dtint");
	sc->sc_flags &=~ H_CMD;
	bp = sc->sc_actf;
	ds = sc->sc_dsreg = htaddr->htds;
	er = sc->sc_erreg = htaddr->hter;
	sc->sc_resid = htaddr->htfc;
#if BABBLE
printf("dt: ds%o er%o fc%o mbs%o\n", ds, er, sc->sc_resid, mbsr);
printf("bp: bn%d bc%d fl%o\n", bp->b_blkno, bp->b_bcount, bp->b_flags);
#endif
	mbs = mbsr;
	if (er)
		htaddr->htcs1 = HT_DCLR|HT_GO;
	sc->sc_blkno++;
	if((bp->b_flags & B_READ) == 0)
		sc->sc_flags |= H_WRITTEN;
	if (bp == &rte16buf[TEUNIT(bp->b_dev)] || ds & HTDS_TM) {
		if (er & HTER_FCE) {
			er &=~ HTER_FCE;
			mbs &=~ (MBSR_DTABT|MBSR_MBEXC);
		}
	}
	if (er & HTER_HARD || (ds & HTDS_MOL) == 0 || mbs & MBSR_EBITS)
		bp->b_flags |= B_ERROR;
	else if (er) {				/* soft error */
		if (++cc->cc_errcnt >= 7)
			bp->b_flags |= B_ERROR;
		else
			cc->cc_flags &=~ T_BUSY;
	}
	if (bp->b_flags & B_ERROR)
		printf("te16 %d: hard error bn%d mbsr=%o er=%o ds=%o\n",
		  TEUNIT(bp->b_dev), bp->b_blkno/TBLOCK, mbsr, sc->sc_erreg, sc->sc_dsreg);
	if ((bp->b_flags & B_READ) == 0)
		bp->b_resid = -sc->sc_resid;
	else {
		if (ds & HTDS_TM) {
			bp->b_resid = bp->b_bcount;
			sc->sc_nxrec = bp->b_blkno/TBLOCK;
		} else if (sc->sc_resid > bp->b_bcount) {
			bp->b_flags |= B_ERROR;
			bp->b_error = ENOMEM;
		} else
			bp->b_resid = bp->b_bcount - sc->sc_resid;
	}
	if (cc->cc_flags & T_BUSY) {	/* we decided we were finished */
		sc->sc_actf = bp->av_forw;
		iodone(bp);
		cc->cc_actf = sc->sc_next;
		cc->cc_errcnt = 0;
		cc->cc_flags &=~ T_BUSY;
	}
	sc->sc_flags &=~ H_BUSY;
	if (sc->sc_actf)
		te16start(sc);
}

te16attn(sc)
register struct te16 *sc;
{
	register struct device *htaddr;
	register struct buf *bp;
	register struct tm03 *cc;
	register int er, ds;
	daddr_t bno;

	sc->sc_flags &=~ H_CMD;
	cc = sc->sc_ctl;
	htaddr = cc->cc_addr;
	ds = sc->sc_dsreg = htaddr->htds;
	er = sc->sc_erreg = htaddr->hter;
	sc->sc_resid = htaddr->htfc;
	if (er)
		htaddr->htcs1 = HT_DCLR|HT_GO;
	wakeup((caddr_t)sc);
	if ((bp = sc->sc_actf) == 0)
		return;
#if BABBLE
printf("at: ds%o er%o fc%o\n", sc->sc_dsreg, sc->sc_erreg, sc->sc_resid);
printf("bp: bn%d bc%d fl%o cm%o\n", bp->b_blkno, bp->b_bcount, bp->b_flags, bp->b_command);
#endif
	bno = bp->b_blkno/TBLOCK;
	if (bp == &cte16buf[TEUNIT(bp->b_dev)]) {
		switch ((int)bp->b_command) {
		case HT_REWOFFL:
			/* offline is on purpose; don't do anything special */
			ds |= HTDS_MOL;	
			break;
		case HT_SREV:
			/* if backspace file hit bot, its not an error */
		        if (er == (HTER_NEF|HTER_FCE) && ds&HTDS_BOT
			&&  bp->b_repcnt == INF)
				er &= ~HTER_NEF;
			break;
		}
		er &= ~HTER_FCE;
		if (er == 0)
			ds &= ~HTDS_ERR;
	}
	if ((ds & (HTDS_ERR|HTDS_MOL)) != HTDS_MOL) {
		if ((ds & HTDS_MOL) == 0 && sc->sc_flags & H_OPEN)
			sc->sc_flags |= H_OFFLINE;
		printf("te16 %d: hard error bn%d er%o ds%o\n",
		    TEUNIT(bp->b_dev), bno, sc->sc_erreg, sc->sc_dsreg);
		bp->b_flags |= B_ERROR;
	}
	if (bp == &cte16buf[TEUNIT(bp->b_dev)])
		bp->b_resid = -sc->sc_resid;
	else
		sc->sc_flags &=~ H_BUSY;	/* done seeking, now xfer */
	if ((ds & HTDS_TM) == 0)
		sc->sc_blkno = bno;
	else if (sc->sc_blkno > bno) {
		sc->sc_nxrec = bno - sc->sc_resid;
		sc->sc_blkno = sc->sc_nxrec;
	} else {
		sc->sc_blkno = bno + sc->sc_resid;
		sc->sc_nxrec = sc->sc_blkno - 1;
	}
	if (sc->sc_flags & H_BUSY) {
		sc->sc_actf = bp->av_forw;
		iodone(bp);
		sc->sc_flags &=~ H_BUSY;
	}
	if (sc->sc_actf)
		te16start(sc);
}

te16read(dev)
dev_t dev;
{

	te16phys(dev);
	physio(te16strategy, &rte16buf[TEUNIT(dev)], dev, B_READ, minphys);
}

te16write(dev)
dev_t dev;
{

	te16phys(dev);
	physio(te16strategy, &rte16buf[TEUNIT(dev)], dev, B_WRITE, minphys);
}

te16phys(dev)
dev_t dev;
{
	register struct te16 *sc;

	sc = &te16[TEUNIT(dev)];
	sc->sc_blkno = Lshift(u.u_offset, TBSHIFT);
	sc->sc_nxrec = sc->sc_blkno + 1;
}

/*ARGSUSED*/
te16ioctl(dev, cmd, addr, flag)
	dev_t dev;
	int cmd;
	caddr_t addr;
	int flag;
{
	register struct te16 *sc = &te16[TEUNIT(dev)];
	register struct buf *bp = &cte16buf[TEUNIT(dev)];
	register callcount;
	int fcount, htcmd;
	struct mtop mtop;

	switch (cmd) {
		case MTIOCTOP:
		if (copyin((caddr_t)addr, (caddr_t)&mtop, sizeof(mtop))) {
			u.u_error = EFAULT;
			return;
		}
		if (mtop.mt_count <= 0) {
			u.u_error = ENXIO;
			return;
		}
		switch(mtop.mt_op) {
		case MTWEOF:
			htcmd = HT_WEOF;
			callcount = mtop.mt_count;
			fcount = 1;
			break;
		case MTFSF: case MTBSF:
			htcmd = (mtop.mt_op == MTFSF) ? HT_SFORW : HT_SREV;
			callcount = mtop.mt_count;
			fcount = INF;
			break;
		case MTFSR: case MTBSR:
			htcmd = (mtop.mt_op == MTFSF) ? HT_SFORW : HT_SREV;
			callcount = 1;
			fcount = mtop.mt_count;
			break;
		case MTREW: case MTOFFL:
			te16cmd(dev, HT_REW, 1);
			if (mtop.mt_op == MTREW)
				return;
			spl5();
			while ((sc->sc_dsreg & HTDS_BOT) == 0)
				sleep((caddr_t)sc, PZERO);
			spl0();
			te16cmd(dev, HT_REWOFFL, 0);
			return;
		default:
			u.u_error = ENXIO;
			return;
		}
		while (--callcount >= 0) {
			te16cmd(dev, htcmd, fcount);
			if (bp->b_resid
			&&  (mtop.mt_op == MTFSR || mtop.mt_op == MTBSR)) {
				u.u_error = EIO;
				break;
			}
			if (u.u_error || sc->sc_dsreg&HTDS_BOT)
				break;
		}
		return;

	default:
		u.u_error = ENXIO;
		return;
	}
}
