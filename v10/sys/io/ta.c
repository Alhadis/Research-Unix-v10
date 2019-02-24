/*
 * DSA tape class driver
 * drives TMSCP tapes
 */

#include "sys/param.h"
#include "sys/buf.h"
#include "sys/ta.h"
#include "sys/mscp.h"
#include "sys/user.h"
#include "sys/file.h"
#include "sys/conf.h"
#include "sys/mtio.h"

extern struct msaddr taaddr[];
extern struct tatape tatape[];
extern int tacnt;
static long tarefno;			/* ref seq num */
extern struct buf tabuf[];

static tacmd(), taonline(), tasonl(), tacinit();

int taopen(), taread(), tawrite(), taioctl(), tastrategy(), taclose();
struct bdevsw tabdev = bdinit(taopen, taclose, tastrategy, B_TAPE);
struct cdevsw tacdev = cdinit(taopen, taclose, taread, tawrite, taioctl);

/*
 * minor device number split
 *
 * NOCACHE disables write-behind caching; probably useless
 * DENS selects one of the eight TMSCP density values;
 * the drive is instructed to use 1<<DENS(dev) in whatever
 * density language it believes
 * here are some languages:
 * for 9-track tapes, (0) 800 bpi (1) PE (2) GCR
 * for old cartridge tapes (TK50), (3) block tape
 * for new cartridge tapes (TK70), (0) low density (1) high density
 * for other TMSCP devices, (0) is usually the right number
 */
#define	UNIT(d)	((d)&07)
#define	DENS(d)	(((d)&070)>>3)
#define	NOCACHE(d) ((d)&0100)
#define	NOREW(d) ((d)&0200)

/*
 * reused bits of buf/iobuf struct
 */

#define	b_next	av_forw		/* next buffer in queue */
#define	b_pkt	av_back		/* pointer to mscp command */
#define	b_crf	b_resid		/* saved refno for pending command */

/*
 * flags in tatape.flags
 */

#define	ONLINE	01		/* drive is online */
#define	WONLINE	02		/* waiting for online */
#define	OPEN	04		/* someone has drive open */
#define	WRITTEN	010		/* some data was written */
#define	WPROT	020		/* drive is write protected */
#define	NEEDCMD	040		/* waiting for some command to start */
#define	CMDDONE	0100		/* command is finished */
#define	CMDERR	0200		/* command finished, and error */
#define	UNHAPPY	0400		/* needs `cache data loss' clear */
#define	WONLY	01000		/* tape was opened only for writing */

/*
 * random numbers
 */

#define	PRIONL	(PZERO-1)
#define	IDTA	1		/* connection ID for MSCP */

/*
 * open a drive
 */

int taseql(), tadg();

taopen(dev, flag)
dev_t dev;
{
	register int unit;
	register struct tatape *ta;
	register struct msaddr *rp;
	int wasoff;

	unit = UNIT(dev);
	if (unit > tacnt) {
		u.u_error = ENXIO;
		return;
	}
	ta = &tatape[unit];
	rp = &taaddr[unit];
	if (ta->flags & OPEN) {
		u.u_error = EBUSY;
		return;
	}
	if (rp->ctype < 0 || rp->ctype >= nmsport
	||  (ta->port = msportsw[rp->ctype]) == NULL) {
		u.u_error = ENXIO;
		return;
	}
	ta->flags |= OPEN;
	ta->flags &=~ (WRITTEN|WPROT|WONLY);
	if ((*ta->port->mp_init)(rp->ctl, rp->ctype, 0, IDTA, taseql, tadg) == 0) {
		u.u_error = ENXIO;
		ta->flags &=~ OPEN;
		return;
	}
	tacinit(ta, rp);
	spl6();
	wasoff = (ta->flags & ONLINE) == 0;
	if ((ta->flags & ONLINE) == 0)
		taonline(ta, rp);
	spl0();
	if ((ta->flags & ONLINE) == 0) {
		u.u_error = ENXIO;
		ta->flags &=~ OPEN;
		return;
	}
	if ((ta->flags & WPROT) && (flag & FWRITE)) {
		u.u_error = ENODEV;
		if (!NOREW(dev))
			tacmd(unit, 1, OPAVL, 0, 0);
		ta->flags &=~ (OPEN|ONLINE);
		return;
	}
	ta->dens = (1<<DENS(dev));
	if (tacmd(unit, 0, OPGUS, 0, 0)		/* fill in language in dens */
	||  tacmd(unit, 0, OPSUC, NOCACHE(dev)?0:UFWBK, wasoff?ta->dens:0)) {
		u.u_error = ENXIO;
		if (!NOREW(dev))
			tacmd(unit, 1, OPAVL, 0, 0);
		ta->flags &=~ (OPEN|ONLINE);
		return;
	}
	if ((flag & (FREAD|FWRITE)) == FWRITE)
		ta->flags |= WONLY;
}

taclose(dev)
{
	register struct tatape *ta;
	int unit;

	unit = UNIT(dev);
	ta = &tatape[unit];
	if (ta->flags & ONLINE) {
		if (ta->flags & (WONLY|WRITTEN)) {
			tacmd(unit, 0, OPWRM, 0, 0);
			tacmd(unit, 0, OPWRM, 0, 0);
			tacmd(unit, 0, OPPOS, 0, -1);
		}
		/* annoyance: OPAVL always rewinds. */
		if (!NOREW(dev))
			tacmd(unit, 1, OPAVL, 0, 0);
	}
	ta->flags &=~ OPEN;
}

/*
 * send some non-io command to a tape drive
 * op is the MSCP opcode;
 * p0 and p1 are some parameters
 * if async == 0, don't return until command does
 */

#define	TWPRI	PZERO
#define	TCPRI	(PZERO+1)

static
tacmd(dev, async, op, p0, p1)
{
	register struct mscmd *mp;
	register struct tatape *ta;
	register struct msaddr *rp;
	register s;
	register err;

	ta = &tatape[dev];
	rp = &taaddr[dev];
	if ((ta->flags & ONLINE) == 0)
		return (1);
	mp = (*ta->port->mp_get)(rp->ctl);
	bzero((caddr_t)mp, sizeof(struct mscmd)); /* clear reserved fields */
	mp->m_crf = ++tarefno;
	mp->m_unit = rp->unit;
	mp->m_opcd = op;
	mp->m_mod = MDCSX;
	switch (op) {
	case OPPOS:		/* position: type, nobjs */
		if (p1 < 0) {
			p1 = -p1;
			mp->m_mod |= MDREV;
		}
		if (p0 == -1) {		/* -1 == rewind */
			mp->m_mod |= MDREW;
			mp->m_mod |= p1;	/* hack */
		}
		else if (p0 == 0)	/* 0 == skip files */
			mp->m_fcnt = p1;
		else {			/* 1 == skip records */
			mp->m_mod |= MDOBJ;
			mp->m_rcnt = p1;
		}
		break;

	case OPSUC:		/* set unit char: unit flags, density */
		mp->m_unfl |= p0;
		mp->m_fmt = p1;
		break;

	case OPGUS:		/* get unit status */
		mp->m_mod = p0;	/* wretched TK50 */
		break;

	default:		/* anything else: modifiers, 0 */
		mp->m_mod |= p0;
		break;
	}
	s = spl6();
	while (ta->cmdp) {
		ta->flags |= NEEDCMD;
		sleep((caddr_t)ta, TWPRI);
	}
	ta->cmdp = mp;
	ta->flags &=~ (CMDERR|CMDDONE);
	if (ta->flags & UNHAPPY) {
		ta->flags &=~ UNHAPPY;
		mp->m_mod |= MDCDL;
	}
	(*ta->port->mp_send)(rp->ctl, IDTA, mp);
	if (async == 0) {
		while ((ta->flags & CMDDONE) == 0)
			if (tsleep((caddr_t)ta, TCPRI, 0) == TS_SIG)
				break;
	}
	err = ((ta->flags & CMDERR) != 0);
	ta->cmdp = NULL;
	if (ta->flags & NEEDCMD) {
		ta->flags &=~ NEEDCMD;
		wakeup((caddr_t)ta);
	}
	splx(s);
	return (err);
}

int tastrategy();

taread(dev)
{
	physio(tastrategy, &tabuf[UNIT(dev)], dev, B_READ, minphys);
}

tawrite(dev)
{
	physio(tastrategy, &tabuf[UNIT(dev)], dev, B_WRITE, minphys);
}

/*
 * strategy routine;
 * send the packet right away
 */

tastrategy(bp)
register struct buf *bp;
{
	register struct tatape *ta;
	register struct mscmd *mp;
	register int unit;
	register struct msaddr *rp;
	int count;

	unit = UNIT(minor(bp->b_dev));
	ta = &tatape[unit];
	rp = &taaddr[unit];
	count = bp->b_bcount;
	spl6();
	if ((ta->flags & ONLINE) == 0 && taonline(ta, rp) == 0) {
		bp->b_flags |= B_ERROR;
		iodone(bp);
		spl0();
		return;
	}
	mp = (*ta->port->mp_get)(rp->ctl);
	mp->m_crf = ++tarefno;
	mp->m_unit = rp->unit;
	mp->m_opcd = (bp->b_flags & B_READ) ? OPRD : OPWR;
	mp->m_mod = MDCSX;
	if (ta->flags & UNHAPPY && (bp->b_flags & B_READ) == 0) {
		ta->flags &=~ UNHAPPY;
		mp->m_mod |= MDCDL;
	}
	mp->m_unfl = 0;
	mp->m_bcnt = count;
	/* seek on block device? later */
	(*ta->port->mp_map)(rp->ctl, mp, bp);
	bp->b_pkt = (struct buf *)mp;
	bp->b_crf = mp->m_crf;
	bp->b_next = NULL;
	if (ta->actf)
		ta->actl->b_next = bp;
	else
		ta->actf = bp;
	ta->actl = bp;
	if ((bp->b_flags & B_READ) == 0)
		ta->flags |= WRITTEN;
	(*ta->port->mp_send)(rp->ctl, IDTA, mp);
	spl0();
}

/*
 * ioctl
 * a subset of the berkeley ones
 */

taioctl(dev, cmd, addr, flag)
dev_t dev;
caddr_t addr;
{
	struct mtop mt;
	int func, p0, p1;

	switch (cmd) {
	default:
		u.u_error = ENOTTY;
		break;
	/* does anything use the other ones? */

	case MTIOCTOP:
		if (copyin(addr, (caddr_t)&mt, sizeof(struct mtop)) < 0) {
			u.u_error = EFAULT;
			return;
		}
		if (mt.mt_op == MTWEOF) {	/* oddball */
			if ((flag & FWRITE) == 0) {
				u.u_error = EBADF;
				return;
			}
			while (mt.mt_count-- > 0)
				if (tacmd(UNIT(dev), 0, OPWRM, 0, 0)) {
					u.u_error = EIO;
					return;
				}
			return;
		}
		if (mt.mt_op == MTRST) {	/* another oddball */
			(*tatape[UNIT(dev)].port->mp_init)(taaddr[UNIT(dev)].ctl, taaddr[UNIT(dev)].ctype, 1, IDTA, taseql, tadg);
			return;
		}
		p1 = mt.mt_count;
		func = OPPOS;
		switch (mt.mt_op) {	/* arcane arguments to tapos */
		case MTBSF:
			p1 = -p1;
		case MTFSF:
			p0 = 0;
			break;

		case MTBSR:
			p1 = -p1;
		case MTFSR:
			p0 = 1;
			break;

		case MTREW:
			p1 = 0;
			p0 = -1;
			break;

		case MTOFFL:
			func = OPAVL;
			p1 = 0;
			p0 = MDUNL;
			break;

		case MTNOP:
			return;	/* silly */
		}
		if (tacmd(UNIT(dev), 0, func, p0, p1))
			u.u_error = EIO;
		break;
	}
}

/*
 * here when the port gets a sequential message
 */
taseql(ctl, type, ep)
int ctl, type;
register struct msend *ep;
{
	register struct buf *bp;
	register struct tatape *ta;
	register int unit;
	register struct buf *obp;
	int sts;

	if (ep->m_opcd == 0 && ep->m_sts == STRST) {
		tareset(ctl, type);
		return;
	}
	/* get rid of this wretched loop somehow */
	for (unit = 0; unit < tacnt; unit++)
		if (taaddr[unit].ctl == ctl
		&&  taaddr[unit].ctype == type
		&&  taaddr[unit].unit == ep->m_unit)
			break;
	if (unit >= tacnt) {
		printf("tmscp stray unit: ctl%d typ%d ta%d sts %o opcode %o\n",
			ctl, type, ep->m_unit, ep->m_sts, ep->m_opcd);
		return;
	}
	ta = &tatape[unit];
	if (ep->m_flgs & EFCDL)
		ta->flags |= UNHAPPY;
	switch (ep->m_opcd & 0377) {
	case OPEND:		/* eg invalid command */
		printf("tmscp ctl%d ta%d ill cmd crf %d off %d\n", ctl,
			ep->m_unit, ep->m_crf, ep->m_sts>>8);
		if (ta->cmdp && ta->cmdp->m_crf == ep->m_crf)
			goto fincmd;
		/* else maybe it's a transfer; fall in */
	case OPRD|OPEND:
	case OPWR|OPEND:
		for (bp = ta->actf, obp = NULL; bp; obp = bp, bp = bp->b_next)
			if (ep->m_crf == bp->b_crf)
				break;
		if (bp == NULL) {
			printf("ta%d stray end: crf %d sts x%x opcode 0%o\n",
				unit, ep->m_crf, ep->m_sts, ep->m_opcd & 0377);
			return;
		}
		if (obp)
			obp->b_next = bp->b_next;
		else
			ta->actf = bp->b_next;
		if (bp == ta->actl)
			ta->actl = obp;
		sts = ep->m_sts & STMSK;
		if (sts == STAVL || sts == STOFL)
			ta->flags &=~ ONLINE;	/* help! */
		bp->b_resid = bp->b_bcount - ep->m_bcnt;
		if (sts == STTPM)
			sts = STSUC;	/* tape mark -> empty read */
		if (sts != STSUC) {
			bp->b_flags |= B_ERROR;
			switch (sts) {
			case STRDT:
			case STOFL:
			case STAVL:
			case STWPR:	/* well ... */
				break;
			default:
				printf("err on ta%d block %D: sts 0%o flgs 0%o\n", unit, bp->b_blkno, ep->m_sts, ep->m_flgs);
			}
		}
		if (ep->m_flgs & EFEOT && (bp->b_flags & B_READ) == 0) {
			bp->b_error = ENOSPC;
			bp->b_flags |= B_ERROR;
		}
		(*ta->port->mp_unmap)(ctl, (struct mscmd *)bp->b_pkt);
		iodone(bp);
		return;

	case OPGUS|OPEND:
		ta->dens &= TFMASK;
		ta->dens |= ep->m_menu & ~TFMASK;
		goto fincmd;

	case OPAVL|OPEND:
		if (ep->m_sts == STSUC)	/* sic */
			ta->flags &=~ ONLINE;
	case OPWRM|OPEND:
	case OPPOS|OPEND:
	case OPFLS|OPEND:
	case OPSUC|OPEND:
	fincmd:
		if ((ep->m_sts & STMSK) == STAVL || (ep->m_sts & STMSK) == STOFL)
			ta->flags &=~ ONLINE;
		if (ta->cmdp) {
			ta->flags |= CMDDONE;
			if ((ep->m_sts & STMSK) != STSUC) {
				printf("ta%d: cmd 0%o sts 0%o flgs 0%o\n", unit, ep->m_opcd&0377, ep->m_sts, ep->m_flgs);
				ta->flags |= CMDERR;
			}
			wakeup((caddr_t)ta);
		}
		return;

	case OPONL|OPEND:
		tasonl(ta, ep);
		return;

	case OPSCC|OPEND:
		if ((ep->m_sts & STMSK) != STSUC)
			printf("tmscp ctl%d typ%d: bad init\n", ctl, type);
		return;

	default:
		printf("stray tmscp msg ta%d opcd 0%o sts x%x\n",
			unit, ep->m_opcd&0377, ep->m_sts&0177777);
		return;
	}
}

/*
 * controller was reset
 * discard all pending io,
 * awake all sleepers,
 * mark everything offline
 */

tareset(ctl, type)
int ctl, type;
{
	register int unit;
	register struct tatape *ta;
	register struct buf *bp, *nbp;

	for (unit = 0; unit < tacnt; unit++)  {
		if (taaddr[unit].ctl != ctl || taaddr[unit].ctype != type)
			continue;
		ta = &tatape[unit];
		for (bp = ta->actf; bp; bp = nbp) {
			nbp = bp->b_next;
			(*ta->port->mp_unmap)(ctl, (struct mscmd *)bp->b_pkt);
			bp->b_flags |= B_ERROR;
			iodone(bp);
		}
		ta->actf = ta->actl = NULL;
		ta->flags &=~ (ONLINE|WONLINE);
		if (ta->cmdp)
			ta->flags |= CMDDONE|CMDERR;
		wakeup((caddr_t)ta);
	}
}

/*
 * here with a datagram message
 * explanations really shouldn't be in the driver
 */

static char *taevents[] = {
	"ok",
	"inv cmd",
	"op aborted",
	"offline",
	"available",
	"med fmt",
	"write prot",
	"comp err",
	"data err",
	"host buf access err",
	"cntl err",
	"drive err",
};
#define	MAXEVT	0xb

tadg(ctl, type, ep)
int ctl, type;
register struct mserl *ep;
{

	if (ep->l_evnt == STSEX)	/* boring, at least for now */
		return;
	printf("ta%d ctl%d typ%d seq %d: %s err; fmt x%x ev x%x fl x%x\n",
		ep->l_unit, ctl, type, ep->l_seq,	/* phys unit, not log */
		ep->l_flgs&(LFSUC|LFCON) ? "soft" : "hard",
		ep->l_fmt, ep->l_evnt, ep->l_flgs&0377);
	if ((ep->l_evnt & STMSK) <= MAXEVT)
		printf("%s; ", taevents[ep->l_evnt & STMSK]);
	switch (ep->l_fmt) {
	case FMCNT:
		/* now the thing should be marked disastrously bad */
		printf("oops\n");
		break;

	case FMBAD:
		printf("host mem access; addr x%x\n", ep->l_badr);
		break;

	case FMTAPE:
		printf("lvl x%x retry x%x\n", ep->l_lvl, ep->l_rtry);
		break;

	default:
		printf("\n");
		break;
	}
}

/*
 * unit is believed offline
 * try to bring it on
 */

static
taonline(ta, rp)
register struct tatape *ta;
register struct msaddr *rp;
{
	register struct mscmd *mp;
	int s;

	s = spl6();
	if ((ta->flags & WONLINE) == 0) {
		mp = (*ta->port->mp_get)(rp->ctl);
		bzero((caddr_t)mp, sizeof(struct mscmd)); /* clear reserved fields */
		mp->m_crf = ++tarefno;
		mp->m_unit = rp->unit;
		mp->m_opcd = OPONL;
		mp->m_mod = MDCSX|MDXCL;
		if (ta->flags & UNHAPPY) {
			ta->flags &=~ UNHAPPY;
			mp->m_mod |= MDCDL;
		}
		mp->m_unfl = 0;
		mp->m_fmt = 0;	/* ta->dens? */
		(*ta->port->mp_send)(rp->ctl, IDTA, mp);
		ta->flags |= WONLINE;
	}
	while (ta->flags & WONLINE)
		tsleep((caddr_t)ta, PRIONL, 60);
	splx(s);
	if ((ta->flags & ONLINE) == 0)
		return (0);
	return (1);
}

static
tasonl(ta, ep)
register struct tatape *ta;
register struct msend *ep;
{

	if (ta->flags & WONLINE) {
		ta->flags &=~ WONLINE;
		wakeup((caddr_t)ta);
	}
	if ((ep->m_sts & STMSK) != STSUC)
		return;
	ta->flags |= ONLINE;
	if (ep->m_unfl & UFWPH)
		ta->flags |= WPROT;
}

/*
 * controller init
 * set characteristics to turn off host timeouts
 */

static
tacinit(ta, rp)
struct tatape *ta;
struct msaddr *rp;
{
	register struct mscmd *mp;
	register int s;

	mp = (*ta->port->mp_get)(rp->ctl);
	bzero((caddr_t)mp, sizeof(struct mscmd)); /* clear reserved fields */
	mp->m_crf = ++tarefno;
	mp->m_opcd = OPSCC;
	mp->m_mod = MDCSX;
	mp->m_cntf = CFMSC | CFTHS;
	mp->m_vrsn = MSCPVER;
	s = spl6();
	(*ta->port->mp_send)(rp->ctl, IDTA, mp);
	splx(s);
}
