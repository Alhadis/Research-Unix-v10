/*
 * Datakit URP protocol
 *
 * In the language we speak to the device driver,
 * M_DATA blocks contain data envelopes
 * M_CTL blocks contain a control envelope,
 * followed by zero or more data envelopes
 *
 * Drivers are allowed to store up blocks for a while;
 * the S_DELIM bit means that stored data must now be sent.
 * The point is to work with some broken URP implementations
 * that have found their way into Datakit controllers;
 * a driver that speaks to one might have to save data up,
 * then send it with no intervening control-nulls when S_DELIM comes.
 */

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/conf.h"
#include "sys/ttyio.h"
#include "sys/dkio.h"
#include "sys/dkstat.h"
#include "sys/dkp.h"
#include "sys/dkmod.h"

#ifdef	CAREFUL
#define	TRC(c)	*dkptrp++ = c; if (dkptrp>=&dkptrb[1024]) dkptrp=dkptrb
char	dkptrb[1024]; char *dkptrp = dkptrb;
#else
#define	TRC(c)
#endif

extern	struct	dkstat dkstat;

/*
 *  Protocol control bytes
 */
#define	SEQ	0010		/* sequence number, ends trailers */
#undef	ECHO
#define	ECHO	0020		/* echos, data given to next queue */
#define	REJ	0030		/* rejections, transmission error */
#define	ACK	0040		/* acknowledgments */
#define	BOT	0050		/* beginning of trailer */
#define	BOTM	0051		/* beginning of trailer, more data follows */
#define	BOTS	0052		/* seq update algorithm on this trailer */
#define	SOU	0053		/* start of unsequenced trailer */
#define	EOU	0054		/* end of unsequenced trailer */
#define	ENQ	0055		/* xmitter requests flow/error status */
#define	CHECK	0056		/* xmitter requests error status */
#define	INITREQ 0057		/* request initialization */
#define	INIT0	0060		/* disable trailer processing */
#define	INIT1	0061		/* enable trailer procesing */
#define	AINIT	0062		/* response to INIT0/INIT1 */
#undef	DELAY
#define	DELAY	0100		/* real-time printing delay */
#define	BREAK	0110		/* Send/receive break (new style) */

#define	OPEN	01
#define	LCLOSE	02
#define	RCLOSE	04
#define	XCHARMODE 010
#define	OPENING	020
#define	RJING	040
#define	STOPPED	0100
#define	RCHARMODE 0200

#define	DKPPRI	28
#define	DKPTIME	2

extern struct dkp dkp[];
extern	int	dkpcnt;
long	dkpopen(), cdkpopen();
int	dkpiput(), dkpisrv(), dkpoput(), dkposrv();
int	dkpclose();
static	struct qinit cdkprinit = { dkpiput,dkpisrv,cdkpopen,dkpclose,512,64 };
static	struct qinit cdkpwinit = { dkpoput,dkposrv,cdkpopen,dkpclose,128,65 };
static	struct qinit dkprinit = { dkpiput,dkpisrv,dkpopen,dkpclose,1500,512 };
static	struct qinit dkpwinit = { dkpoput,dkposrv,dkpopen,dkpclose,1500,512 };
struct	streamtab dkpstream = { &dkprinit, &dkpwinit };
struct	streamtab cdkpstream = { &cdkprinit, &cdkpwinit };

#define	MAXMSG	4096		/* max message size allowed */

long
dkpopen(q)
struct queue *q;
{
	return(rdkpopen(q, !XCHARMODE));
}

long
cdkpopen(q)
struct queue *q;
{
	return(rdkpopen(q, XCHARMODE));
}

rdkpopen(q, mode)
register struct queue *q;
{
	register struct dkp *dkpp;
	static timer = 0;
	int dkptimer();

	if (timer == 0) {
		timer = 1;
		timeout(dkptimer, (caddr_t)NULL, HZ);
	}
	if (q->ptr == NULL) {
		for (dkpp = dkp; dkpp->state!=0; dkpp++)
			if (dkpp >= &dkp[dkpcnt])
				return(0);
		dkpp->rdq = q;
		q->ptr = (caddr_t)dkpp;
		WR(q)->ptr = (caddr_t)dkpp;
		WR(q)->flag |= QNOENB;
		putctl(q->next, M_FLUSH);
		dkpp->timer = DKPTIME;
		dkpp->trx = 0;
		dkpp->iseq = 0;
		dkpp->lastecho = ECHO+0;
		dkpp->WS = 1;
		dkpp->WACK = 1;
		dkpp->WNX = 1;
		dkpp->XW = 3;
		dkpp->xsize = 64;
		dkpp->lastctl = 0;
		if (mode!=XCHARMODE) {
			WR(q)->flag |= QDELIM;
			dkpp->state = OPENING | RCHARMODE;
			putctl1d(WR(q)->next, M_CTL, INIT1);
		} else {
			dkpp->XW = 1;
			dkpp->state = RCHARMODE | XCHARMODE | OPEN;
			putctl1d(WR(q)->next, M_CTL, INIT0);
		}
	}
	return(1);
}

/*
 * Shut it down.
 *  The problem is to dispose of unacked stuff in the window.
 *   -- no real solution; the receiver might hang on for hours.
 *   Give it 15 seconds.
 */
dkpclose(q)
register struct queue *q;
{
	register struct dkp *dkpp;
	register s = spl5();
	register i;

	dkpp = (struct dkp *)q->ptr;
	dkpp->state |= LCLOSE;
	flushq(q, 1);
	for (i=0; dkpp->WACK < dkpp->WNX && i<15; i++)
		tsleep((caddr_t)dkpp, DKPPRI, 1);
	if (dkpp->WACK < dkpp->WNX)
		dkprack(dkpp, ACK+((dkpp->WNX-1) & 07));
	dkpinflush(dkpp);
	splx(s);
	dkpp->state = 0;
	flushq(WR(q), 1);
}


/*
 * Process a bunch of input
 *   -- for now, ignore strange control bytes
 */
dkpisrv(q)
register struct queue *q;
{
	register struct dkp *dkpp = (struct dkp *)q->ptr;
	register struct block *bp;
	register c;

	while (bp = getq(q)) {
		if (bp->type == M_CTL) {
			c = *bp->rptr & 0370;
			if (c==REJ || c==ECHO) {
				dkpp->lastecho = *bp->rptr;
				bp->class |= S_DELIM;
				(*WR(q)->next->qinfo->putp)(WR(q)->next, bp);
			} else
				freeb(bp);	/* unknown control */
			continue;
		}
		if ((q->next->flag&QFULL)==0 || bp->type>=QPCTL
		 || dkpp->state&RCLOSE) {
			TRC('G'); TRC(*bp->rptr);
			(*q->next->qinfo->putp)(q->next, bp);
		} else {
			putbq(q, bp);
			return;
		}
	}
}

/*
 * Packet arrives.
 */
dkpiput(q, bp)
struct queue *q;
register struct block *bp;
{
	register struct dkp *dkpp;
	register i;
	register struct block *nbp;

	if ((dkpp = (struct dkp *)q->ptr)==NULL) {
		freeb(bp);
		return;
	}
	switch (bp->type) {

	moredata:
		bp->rptr++;
		bp->type = M_DATA;
	case M_DATA:
		bp->class &= ~S_DELIM;
		if (bp->rptr >= bp->wptr||q->flag&QFULL||dkpp->state&LCLOSE) {
			freeb(bp);
			return;
		}
		if (dkpp->state & RCHARMODE) {
			putq(q, bp);
			return;
		}
		switch (dkpp->trx) {

			case 1:
			case 2:
				dkpp->trbuf[dkpp->trx++] = *bp->rptr;
				goto moredata;
			
			default:
				dkpp->trx = 0;
			case 0:
				break;
		}
		bp->next = NULL;
		if (dkpp->indata > MAXMSG) { 	/* protect against garbage */
			freeb(bp);
			return;
		}
		if (dkpp->inp) {
			dkpp->inpe->next = bp;
			dkpp->inpe = bp;
		} else {
			dkpp->inp = bp;
			dkpp->inpe = bp;
		}
		dkpp->indata += bp->wptr - bp->rptr;
		return;

	case M_CTL:
		switch (*bp->rptr) {

		case ENQ:
			putctl1(WR(q)->next, M_CTL, dkpp->lastecho);
		case CHECK:
			putctl1d(WR(q)->next, M_CTL, ACK+dkpp->iseq);
			dkpinflush(dkpp);
			goto moredata;

		case AINIT:
			dkpp->state &= ~OPENING;
			dkpp->state |= OPEN;
			qenable(WR(q));
			dkpinflush(dkpp);
			goto moredata;

		case INIT0:
		case INIT1:
			putctl1d(WR(q)->next, M_CTL, AINIT);
			if (*bp->rptr==INIT0 && (dkpp->state&RCHARMODE)==0) {
				dkpp->state |= RCHARMODE;
				dkpp->XW = 1;	/* why? */
				q->flag &= ~QDELIM;
			} else if (*bp->rptr==INIT1 && (dkpp->state&RCHARMODE)) {
				dkpp->state &= ~RCHARMODE;
				dkpp->XW = 3;	/* why? */
				q->flag |= QDELIM;
			}
			dkpinflush(dkpp);
			dkpp->iseq = 0;
			wakeup((caddr_t)dkpp);
			goto moredata;

		case INITREQ:
			if (dkpp->state&XCHARMODE)
				putctl1d(WR(q)->next, M_CTL, INIT0);
			else {
				if (dkpp->WS < dkpp->WNX)
					dkprack(dkpp, ECHO+((dkpp->WNX-1)&07));
				dkpp->WS = 1;
				dkpp->WACK = 1;
				dkpp->WNX = 1;
				putctl1d(WR(q)->next, M_CTL, INIT1);
			}
			dkpinflush(dkpp);
			goto moredata;

		case BREAK:
			qpctl(q, M_BREAK);
			dkpp->indata++;
			goto moredata;

		case BOT:
		case BOTS:
		case BOTM:
			dkpp->trx = 1;
			dkpp->trbuf[0] = *bp->rptr;
			goto moredata;

		case REJ+0: case REJ+1: case REJ+2: case REJ+3:
		case REJ+4: case REJ+5: case REJ+6: case REJ+7:
			if (dkpp->state&XCHARMODE)
				goto moredata;
			TRC('r');
			if (((*bp->rptr+1)&07) == (dkpp->WACK&07)
			 && (dkpp->state&RJING) == 0) {
				dkstat.dkprxmit++;
				for (i=dkpp->WACK; i<dkpp->WNX; i++) {
					TRC('Z');
					TRC('0' + (i&07));
					dkpp->state |= RJING;
					dkpxmit(WR(q), dkpp->xb[i&07], i);
				}
			}
			goto moredata;
		
		case ACK+0: case ACK+1: case ACK+2: case ACK+3:
		case ACK+4: case ACK+5: case ACK+6: case ACK+7:
		case ECHO+0: case ECHO+1: case ECHO+2: case ECHO+3:
		case ECHO+4: case ECHO+5: case ECHO+6: case ECHO+7:
			dkprack(dkpp, *bp->rptr);
			goto moredata;

		case SEQ+0: case SEQ+1: case SEQ+2: case SEQ+3:
		case SEQ+4: case SEQ+5: case SEQ+6: case SEQ+7:
			i = *bp->rptr & 07;
			if (dkpp->state & RCHARMODE) {
				TRC('e');
				qpctl1(q, M_CTL, ECHO+i);
				goto moredata;
			}
			if (dkpp->trx !=3
			 || dkpp->indata != dkpp->trbuf[1] + (dkpp->trbuf[2]<<8)
			 || i != ((dkpp->iseq+1)&07)) {	/* reject? */
				if (dkpp->trx != 3)
					dkstat.dkprjtrs++;
				else if (i != ((dkpp->iseq+1)&07))
					dkstat.dkprjseq++;
				else
					dkstat.dkprjpks++;
				dkpinflush(dkpp);
				if (dkpp->trbuf[0]==BOTS)
					dkpp->iseq = i;
				TRC('R'); TRC('0'+dkpp->iseq);
				TRC(dkpp->trx!=3?'t':(i!=(dkpp->iseq+1)&07?'s':'c'));
				qpctl1(q, M_CTL, REJ+dkpp->iseq);
				goto moredata;
			}
			/* accept */
			if (dkpp->inpe==NULL)
				dkpp->inp = dkpp->inpe = allocb(0);
			if (dkpp->trbuf[0] != BOTM && q->flag&QDELIM)
				dkpp->inpe->class |= S_DELIM;
			while (nbp = dkpp->inp) {
				dkpp->inp = nbp->next;
				putq(q, nbp);
			}
			TRC('A'); TRC('0'+i);
			dkpp->inpe = NULL;
			dkpp->trx = 0;
			dkpp->indata = 0;
			dkpp->iseq = i;
			qpctl1(q, M_CTL, ECHO+i);
			goto moredata;

		default:
			if (*bp->rptr < 0200)	/* non-supervisory */
				dkpp->indata++;
			dkpp->lastctl = *bp->rptr;
			qpctl1(q, M_CTL, *bp->rptr);
			goto moredata;
		}

	case M_HANGUP:
		dkpp->state |= RCLOSE;
		flushq(WR(q), 1);
		dkprack(dkpp, ECHO+((dkpp->WNX-1) & 07));
		putq(q, bp);
		return;

	case M_IOCACK:
	case M_IOCNAK:
		(*q->next->qinfo->putp)(q->next, bp);
		return;

	case M_PRICTL:
		if (*bp->rptr != DKMXINIT) {
			(*q->next->qinfo->putp)(q->next, bp);
			return;
		}
		bp->type = M_CTL;
		bp->wptr = bp->rptr;
		*bp->wptr++ = INITREQ;
		dkpiput(q, bp);
		putctl1d(WR(q)->next, M_CTL, INITREQ);
		return;

	default:
		freeb(bp);
		return;
	}
}

/*
 * --- Output processor
 */

/*
 * accept data from writer
 *  -- handle most non-data messages
 */

int dkpwbig = 200;

dkpoput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct dkp *dkpp = (struct dkp *)q->ptr;
	register unsigned char *sp;
	register x, s;
	struct block *xbp;

	if (dkpp->state & RCLOSE) {
		freeb(bp);
		return;
	}
	switch (bp->type) {

	case M_STOP:
		dkpp->state |= STOPPED;
		freeb(bp);
		return;

	case M_START:
		dkpp->state &= ~STOPPED;
		freeb(bp);
		qenable(q);
		return;

	case M_FLUSH:
		/* annul data for blocks in transit */
		freeb(bp);
		s = spl5();	/* in case an ECHO is about to arrive */
		for (x=0; x<8; x++)
			if ((bp = dkpp->xb[x]) != NULL)
				bp->wptr = bp->rptr;
		splx(s);
		flushq(q, 0);
		return;

	case M_IOCTL:
		sp = (unsigned char *)stiodata(bp);
		switch (stiocom(bp)) {

		case TIOCSDEV:
			x = ((struct ttydevb *)sp)->ispeed;
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			qreply(q, bp);
			if (x==0)
				putctl(OTHERQ(q), M_HANGUP);
			return;

		case TIOCGDEV:
			((struct ttydevb *)sp)->ispeed = ((struct ttydevb *)sp)->ospeed = B9600;
			bp->wptr = bp->rptr + sizeof(struct ttydevb) + STIOCHDR;
			if (bp->wptr >= bp->lim)
				panic("dkioc");
			bp->type = M_IOCACK;
			qreply(q, bp);
			return;

		case DIOCSTREAM:
			RD(q)->flag &= ~QDELIM;
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			qreply(q, bp);
			return;
		
		case DIOCRECORD:
			if ((dkpp->state&RCHARMODE) == 0) {
				RD(q)->flag |= QDELIM;
				bp->type = M_IOCACK;
			} else
				bp->type = M_IOCNAK;
			bp->wptr = bp->rptr;
			qreply(q, bp);
			return;

		case KIOCINIT:
			if (dkpp->state&XCHARMODE)
				putctl1d(q->next, M_CTL, INIT0);
			else {
				s = spl5();
				if (dkpp->WS < dkpp->WNX)
					dkprack(dkpp, ECHO+((dkpp->WNX-1)&07));
				dkpp->WS = 1;
				dkpp->WACK = 1;
				dkpp->WNX = 1;
				splx(s);
				putctl1d(q->next, M_CTL, INIT1);
			}
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			qreply(q, bp);
			return;

		case KIOCISURP:
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			qreply(q, bp);
			return;

		case DIOCSCTL:
			bp->type = M_IOCACK;
			bp->wptr = bp->rptr;
			if (*sp == 0) {
				qreply(q, bp);
				return;
			}
			if ((xbp = allocb(1)) == NULL)
				bp->type = M_IOCNAK;
			else {
				xbp->type = M_CTL;
				xbp->class |= S_DELIM;
				*xbp->wptr++ = *sp;
				putq(q, xbp);
				if (dkpp->WNX < dkpp->WS+dkpp->XW)
					qenable(q);
			}
			qreply(q, bp);
			return;

		case DIOCRCTL:
			*sp = dkpp->lastctl;
			dkpp->lastctl = 0;
			bp->type = M_IOCACK;
			bp->wptr = sp + 1;
			qreply(q, bp);
			return;

		case DIOCXWIN:
			bp->type = M_IOCACK;
			if (sp[4] >= 8 || sp[4] <= 0)
				bp->type = M_IOCNAK;
			else if ((x = sp[0] + (sp[1]<<8)) < 0 || x > MAXMSG)
				bp->type = M_IOCNAK;
			else {
				dkpp->xsize = x;
				dkpp->XW = sp[4];
				if (x > dkpwbig)
					q->flag |= QBIGB;
				else
					q->flag &=~ QBIGB;
			}
			bp->wptr = bp->rptr;
			qreply(q, bp);
			return;

		default:
			(*q->next->qinfo->putp)(q->next, bp);
			return;
		}

	case M_DELAY:
		x = *bp->rptr;
		*bp->rptr = DELAY;
		bp->type = M_CTL;
		while (x) {
			(*bp->rptr)++;
			x >>= 1;
		}
		goto putonq;

	case M_PRICTL:
		(*q->next->qinfo->putp)(q->next, bp);
		return;

	default:
		freeb(bp);
		return;

	case M_BREAK:
		bp->type = M_CTL;
		*bp->wptr++ = BREAK;
	case M_DATA:
	case M_CTL:
	putonq:
		putq(q, bp);
		if (dkpp->WNX < dkpp->WS+dkpp->XW)
			qenable(q);
		return;
	}
}

/*
 * Out server:
 *  if space in window, process queue
 * This is the only place that WNX is incremented,
 * and the only place xb[i] is set nonzero
 * (remember these facts when thinking about races)
 */
dkposrv(q)
register struct queue *q;
{
	register struct dkp *dkpp = (struct dkp *)q->ptr;
	register struct block *bp, *xbp;
	register int seqno;
	register struct block **bpp;
	register int s;

	if (dkpp->state & (STOPPED|OPENING))
		return;
	while (dkpp->WNX < dkpp->WS+dkpp->XW) {
		if ((bp = getq(q)) == NULL)
			break;
		/*
		 * Bite off one URP block; put back the rest.
		 * The initial blocks are passed by reference.
		 */
		if (bp->type==M_DATA && bp->wptr-bp->rptr > dkpp->xsize) {
			xbp = dupb(bp);
			bp->rptr += dkpp->xsize;
			xbp->wptr = xbp->rptr+dkpp->xsize;
			xbp->class &= ~S_DELIM;
			putbq(q, bp);
			bp = xbp;
		}
		if (dkpp->state & XCHARMODE) {
			dkpp->outcnt += bp->wptr - bp->rptr;
			(*q->next->qinfo->putp)(q->next, bp);
			if (dkpp->outcnt >= dkpp->xsize) {
				putctl1d(q->next, M_CTL, SEQ+(dkpp->WNX&07));
				dkpp->WNX++;
				dkpp->WACK = dkpp->WNX;
				dkpp->outcnt = 0;
			}
			continue;
		}
		TRC('x'); TRC('0'+dkpp->WS/10); TRC('0'+dkpp->WS%10);
		TRC('.'); TRC('0'+dkpp->WNX/10); TRC('0'+dkpp->WNX%10);
		bpp = &dkpp->xb[dkpp->WNX&07];
		if (*bpp) {
			s = spl5();	/* in case of last-minute ECHO */
			if (*bpp) {
				freeb(*bpp);
				printf("dkp losing block %x\n", *bpp);
				*bpp = 0;	/* if ECHO comes later, don't free twice */
			}
			splx(s);
		}
		*bpp = bp;
		seqno = dkpp->WNX++;		/* in case ECHO comes right away */
		dkpxmit(q, bp, seqno);
		/* what if INIT[01] arrived between `bpp = ...' and here? */
	}
}

/*
 *  Send out a message, with trailer.
 */
dkpxmit(q, bp, seqno)
register struct queue *q;
register struct block *bp;
{
	register size;
	register struct block *xbp;
	register struct dkp *dkpp = (struct dkp *)q->ptr;

	if (bp==NULL) {
		printf("null bp in dkpxmit\n");
		return;
	}
	size = bp->wptr - bp->rptr;
	seqno &= 07;
	/* send ptr to block, if non-empty */
	if (size) {
		if ((xbp = dupb(bp)) == NULL)
			return;
		xbp->class &=~ S_DELIM;
		TRC('X'); TRC('0'+seqno);
		(*q->next->qinfo->putp)(q->next, xbp);
	}
	/* send trailer */
	if ((xbp = allocb(3)) == NULL)
		return;
	xbp->type = M_CTL;
	*xbp->wptr++ = bp->class&S_DELIM? BOT: BOTM;
	*xbp->wptr++ = size;
	*xbp->wptr++ = size >> 8;
	(*q->next->qinfo->putp)(q->next, xbp);
	putctl1d(q->next, M_CTL, SEQ + seqno);
	dkpp->timer = DKPTIME;
}

/*
 * Receive an ack of some sort for a transmitted message.
 *  Advance various windows.
 * should be called at spl5 to avoid races
 */
dkprack(dkpp, msg)
register struct dkp *dkpp;
{
	register struct block **bpp;
	register seqno, i;

	seqno = msg & 07;
	msg &= 0370;
	/* invariants: 0 <= WS <= WACK <= WNX; seqno maximal < WNX; WS < 8 */
	if (seqno >= dkpp->WNX)
		seqno -= 8;
	else if (seqno+8 < dkpp->WNX)
		seqno += 8;
	dkpp->state &= ~RJING;
	for (i=dkpp->WS; i<=seqno; i++) {
		bpp = &dkpp->xb[i&07];
		if (*bpp) {
			freeb(*bpp);
			*bpp = NULL;
		}
	}
	if ((int)dkpp->WACK <= seqno)
		dkpp->WACK = seqno+1;
	if (msg==ECHO) {
		TRC('E'); TRC('0'+(seqno&07));
		if (dkpp->WS <= seqno) {
			dkpp->timer = DKPTIME;	/* push off timeout */
			dkpp->WS = seqno+1;
			if (dkpp->WNX<dkpp->WS+dkpp->XW && WR(dkpp->rdq)->count)
				qenable(WR(dkpp->rdq));
		}
	} else {
		for (i=dkpp->WACK; i<dkpp->WNX; i++) {
			if (dkpp->xb[i&07]==0)
			 printf("WS %d WACK %d WNX %d i %d seqno %d\n",
			   dkpp->WS, dkpp->WACK, dkpp->WNX, i, seqno);
			dkpxmit(WR(dkpp->rdq), dkpp->xb[i&07], i);
			dkstat.dkprxmit++;
		}
	}
	if (dkpp->WS >= 8) {
		dkpp->WS -= 8;
		dkpp->WACK -= 8;
		dkpp->WNX -= 8;
	}
}

dkptimer()
{
	register struct dkp *dkpp;
	register struct queue *q;
	register int i;

	for (dkpp = dkp, i = dkpcnt; i > 0; dkpp++, --i) {
		if ((dkpp->state&(OPEN|OPENING)) == 0)
			continue;
		if (--dkpp->timer>0)
			continue;
		q = WR(dkpp->rdq)->next;
		if (q->flag&QFULL)
			continue;
		if (dkpp->state & XCHARMODE) {
			if (dkpp->WS < dkpp->WNX)
				putctl1d(q, M_CTL, SEQ+((dkpp->WNX-1)&07));
			dkpp->timer = 10;
			continue;
		}
		if (dkpp->state&OPENING)
			putctl1d(q, M_CTL, INIT1);
		if (dkpp->WS != dkpp->WNX)
			putctl1d(q, M_CTL, ENQ);
		dkpp->timer = DKPTIME;
	}
	timeout(dkptimer, (caddr_t)NULL, HZ);
}

/*
 * throw away data in front of the barrier, and clear the trailer buffer
 */
dkpinflush(dkpp)
register struct dkp *dkpp;
{
	register struct block *bp;

	while (bp = dkpp->inp) {
		dkpp->inp = bp->next;
		freeb(bp);
	}
	dkpp->inpe = NULL;
	dkpp->trx = 0;
	dkpp->indata = 0;
}
