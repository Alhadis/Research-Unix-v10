#include "sys/param.h"
#include "sys/stream.h"
#include "sys/conf.h"
#include "sys/ttyio.h"
#include "sys/xttyld.h"

#define ICANBSIZ 256
#define CTRL(C) ((C) != '?' ? (C) & 0x1F : 0x7F)

extern int xttycnt;
extern struct xttyld xttyld[];

static long open();
static close();
static rsrv(), wsrv();

static struct qinit rinit = { putq, rsrv, open, close, 600, 300 };
static struct qinit winit = { putq, wsrv, open, close, 600, 300 };
struct streamtab xttystream = { &rinit, &winit };

static struct xttyld xttyproto = {
	B9600, B9600,		/* sg_ispeed and sg_ospeed */
	'\b', '@',		/* sg_erase and sg_kill */
	ECHO | CRMOD,		/* sg_flags */
	CTRL('?'), CTRL('\\'),	/* t_intrc and t_quitc */
	CTRL('Q'), CTRL('S'),	/* t_startc and t_stopc */
	CTRL('D'), 0377,	/* t_eofc and t_brkc */
};

static char maptab[] = {
	000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
	000,'|',000,000,000,000,000,'`','{','}',000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,000,
	000,000,000,000,000,000,000,000,000,000,000,000,'\\',000,'~',000,
	000,'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O',
	'P','Q','R','S','T','U','V','W','X','Y','Z',000,000,000,000,000,
};

static long
open(qp, dev)
	register struct queue *qp;
	dev_t dev;
{
	int i;

	for (i = 0; i < xttycnt; ++i)
		if (!xttyld[i].qp) {
			xttyld[i] = xttyproto;
			xttyld[i].qp = qp;
			qp->flag |= QDELIM;
			qp->ptr = (caddr_t) &xttyld[i];
			WR(qp)->flag |= QDELIM;
			WR(qp)->ptr = (caddr_t) &xttyld[i];
			return 1;
		}
	return 0;
}

static
close(qp)
	struct queue *qp;
{
	register struct xttyld *xt;

	xt = (struct xttyld *) qp->ptr;
	if (xt->icanb) {
		freeb(xt->icanb);
		xt->icanb = 0;
	}
	xt->qp = 0;
}

static void
ctl(qp, bp)
	struct queue *qp;
	register struct block *bp;
{
	register struct xttyld *xt;
	register u_char *data;

	xt = (struct xttyld *) qp->ptr;
	data = (u_char *) stiodata(bp);

	switch (stiocom(bp)) {
	case TIOCGETC:
		*(struct tchars *) data = xt->tc;
		bp->type = M_IOCACK;
		bp->wptr = bp->rptr + STIOCHDR + sizeof (struct tchars);
		break;
	case TIOCSETC:
		xt->tc = *(struct tchars *) data;
		bp->type = M_IOCACK;
		bp->wptr = bp->rptr + STIOCHDR;
		break;
	case TIOCGETP:
		*(struct sgttyb *) data = xt->sg;
		bp->type = M_IOCACK;
		bp->wptr = bp->rptr + STIOCHDR + sizeof (struct sgttyb);
		break;
	case TIOCSETN:
	case TIOCSETP:
		xt->sg = *(struct sgttyb *) data;
		bp->type = M_IOCACK;
		bp->wptr = bp->rptr + STIOCHDR;
		break;
	default:
		(*qp->next->qinfo->putp)(qp->next, bp);
		return;
	}
	(*OTHERQ(qp)->next->qinfo->putp)(OTHERQ(qp)->next, bp);
	if (xt->status & XTTY_FULL && !(xt->sg.sg_flags & TANDEM)) {
		xt->status &= ~XTTY_FULL;
		putctl1d(WR(xt->qp), M_DATA, xt->tc.t_startc);
	}
	if (xt->sg.sg_flags & RAW) {
		xt->status = 0;
		qenable(WR(xt->qp));
	}
	if (xt->sg.sg_flags & (RAW | CBREAK))
		xt->qp->flag &= ~QDELIM;
	else
		xt->qp->flag |= QDELIM;
}

static void
sig(xt, sig)
	register struct xttyld *xt;
	int sig;
{
	register struct queue *qp;

	qp = xt->qp;
	flushq(qp, 0);
	flushq(WR(qp), 0);
	if (xt->icanb) {
		freeb(xt->icanb);
		xt->icanb = 0;
	}
	xt->status &= ~(XTTY_ESCAPED | XTTY_NEWLINE | XTTY_STOPPED);
	qenable(WR(qp));
	putctl(qp->next, M_FLUSH);
	putctl1(qp->next, M_SIGNAL, sig);
	putctl(WR(qp)->next, M_FLUSH);
}

static int
icanon(xt, bp)
	register struct xttyld *xt;
	register struct block *bp;
{
	register struct block *icanb;
	register int c, esc;

	icanb = xt->icanb;
	esc = xt->sg.sg_flags & CBREAK ? 0 : xt->status & XTTY_ESCAPED;
	xt->status &= ~XTTY_ESCAPED;
	while (bp->rptr < bp->wptr) {
		if (xt->status & XTTY_STOPPED) {
			xt->status &= ~XTTY_STOPPED;
			qenable(WR(xt->qp));
		}
		c = *bp->rptr++;
		if (xt->sg.sg_flags & CRMOD && c == '\r')
			c = '\n';
		if (xt->sg.sg_flags & LCASE && c >= 'A' && c <= 'Z')
			c += 'a' - 'A';
		if (esc) {
			if (icanb->wptr > icanb->rptr
			&& (c == xt->sg.sg_erase || c == xt->sg.sg_kill
			|| c == xt->tc.t_eofc || c == xt->tc.t_brkc
			|| c == xt->tc.t_intrc || c == xt->tc.t_quitc
			|| c == xt->tc.t_startc || c == xt->tc.t_stopc))
				--icanb->wptr;
			if (c < sizeof maptab && maptab[c])
				c = maptab[c];
			c |= 0x100;
		}
		if (c == xt->tc.t_intrc || c == xt->tc.t_quitc) {
			bp->rptr = bp->wptr;
			sig(xt, c == xt->tc.t_intrc ? SIGINT : SIGQUIT);
			return 0;
		}
		if (c == xt->tc.t_stopc) {
			xt->status |= XTTY_STOPPED;
			return 0;
		}
		if (c == xt->tc.t_startc)
			return 0;
		if (xt->sg.sg_flags & ECHO)
			putctl1d(WR(xt->qp), M_DATA, c);
		if (!(xt->sg.sg_flags & CBREAK)) {
			if (c == xt->sg.sg_erase) {
				if (icanb->wptr > icanb->rptr)
					--icanb->wptr;
				continue;
			}
			if (c == xt->sg.sg_kill) {
				icanb->wptr = icanb->rptr;
				if (xt->sg.sg_flags & ECHO)
					putctl1d(WR(xt->qp), M_DATA, '\n');
				continue;
			}
		}
		if ((xt->sg.sg_flags & CBREAK) || c != xt->tc.t_eofc)
			if (icanb->wptr < icanb->lim)
				*icanb->wptr++ = c;
			else {
				*--bp->rptr = c;
				return 1;
			}
		if (!(xt->sg.sg_flags & CBREAK)) {
			if (c == xt->tc.t_eofc || c == xt->tc.t_brkc || c == '\n')
				return 1;
			esc = (c & 0xFF) == '\\' ? XTTY_ESCAPED : 0;
		}
	}
	xt->status |= esc;
	return xt->sg.sg_flags & CBREAK;
}
				
static
rsrv(qp)
	struct queue *qp;
{
	register struct xttyld *xt;
	struct block *bp;

	xt = (struct xttyld *) qp->ptr;
	if (!xt->qp)
		return;
	while ((qp->next->flag & QFULL) == 0) {
		if (xt->sg.sg_flags & RAW && xt->icanb) {
			(*qp->next->qinfo->putp)(qp->next, xt->icanb);
			xt->icanb = 0;
			continue;
		}
		bp = getq(qp);
		if (!bp)
			break;
		if (!(xt->sg.sg_flags & RAW) && bp->type == M_DATA) {
			do {
				if (!xt->icanb) {
					if (xt->sg.sg_flags & CBREAK)
						xt->icanb = allocb(bp->rptr - bp->wptr);
					else
						xt->icanb = allocb(ICANBSIZ);
					if (!xt->icanb) {
						putbq(qp, bp);
						qp->next->flag |= QWANTW;
						return;
					}
					xt->icanb->type = M_DATA;
				}
				if (icanon(xt, bp)) {
					if (!(xt->sg.sg_flags & CBREAK))
						xt->icanb->class |= S_DELIM;
					(*qp->next->qinfo->putp)(qp->next, xt->icanb);
					xt->icanb = 0;
				}
			} while (bp->rptr != bp->wptr);
			freeb(bp);
		} else if (bp->type == M_IOCTL)
			ctl(qp, bp);
		else {
			switch (bp->type) {
			case M_BREAK:
				if (xt->sg.sg_flags & RAW) {
					bp->type = M_DATA;
					bp->class |= S_DELIM;
					if (bp->wptr < bp->lim)
						*bp->wptr++ = '\0';
				} else {
					freeb(bp);
					sig(xt, SIGINT);
					return;
				}
				break;
			case M_FLUSH:
				if (xt->icanb)
					xt->icanb->wptr = xt->icanb->rptr;
				break;
			}
			bp->class &= ~S_DELIM;
			(*qp->next->qinfo->putp)(qp->next, bp);
		}
	}
	if ((xt->sg.sg_flags & (RAW | TANDEM)) == TANDEM) {
		if (!(xt->status & XTTY_FULL) && qp->next->flag & QFULL) {
			xt->status |= XTTY_FULL;
			putctl1d(WR(qp), M_DATA, xt->tc.t_stopc);
		}
		if (xt->status & XTTY_FULL && (!qp->next->flag & QFULL)) {
			xt->status &= ~XTTY_FULL;
			putctl1d(WR(qp), M_DATA, xt->tc.t_startc);
		}
	}
	if (qp->count)
		qp->next->flag |= QWANTW;
}

static int
ocanon(xt, bp, qp)
	register struct xttyld *xt;
	register struct block *bp;
	struct queue *qp;
{
	register struct block *ocanb;
	register int c, d, t;

	while (bp->rptr < bp->wptr) {
		ocanb = allocb(bp->wptr - bp->rptr + 8);
		if (!ocanb)
			return 0;
		ocanb->class |= S_DELIM;
		d = 0;
		while (bp->rptr < bp->wptr && ocanb->wptr < ocanb->lim - 8) {
			c = *bp->rptr++;
			t = 8 - (xt->col & 7);
			if (xt->status & XTTY_NEWLINE)
				xt->status &= ~XTTY_NEWLINE;
			else if (xt->sg.sg_flags & CRMOD) {
				if (c == '\n') {
					--bp->rptr;
					c = '\r';
					xt->status |= XTTY_NEWLINE;
				} else if (c == '\r') {
					*--bp->rptr = '\n';
					xt->status |= XTTY_NEWLINE;
				}
			}
			switch (c) {
			case '\v':
			case '\f':
				xt->col = 0;
				if (xt->sg.sg_flags & VTDELAY)
					d = 127;
				break;
			case '\r':
				xt->col = 0;
				switch (xt->sg.sg_flags & CRDELAY) {
				case CR1:
					d = 5;
					break;
				case CR2:
					d = 10;
					break;
				case CR3:
					d = 20;
					break;
				}
				break;
			case '\t':
				xt->col += t;
				switch (xt->sg.sg_flags & TBDELAY) {
				case TAB1:
					d = t;
					if (d < 5)
						d = 0;
					break;
				case XTABS:
					while (t--)
						*ocanb->wptr++ = ' ';
					continue;
				}
				break;
			case '\n':
				xt->col = 0;
				switch (xt->sg.sg_flags & NLDELAY) {
				case NL1:
					d = max((xt->col >> 4) + 3, 6);
					break;
				case NL2:
					d = 6;
					break;
				}
				break;
			case '\b':
				if (--xt->col < 0)
					xt->col = 0;
				break;
			default:
				++xt->col;
				break;
			}
			*ocanb->wptr++ = c;
			if (d)
				break;
		}
		(qp->qinfo->putp)(qp, ocanb);
		if (d)
			putctl1d(qp, M_DELAY, d);
	}
	freeb(bp);
	return 1;
}

static
wsrv(qp)
	register struct queue *qp;
{
	register struct xttyld *xt;
	register struct block *bp;

	xt = (struct xttyld *) qp->ptr;
	while ((qp->next->flag & QFULL) == 0 && (bp = getq(qp))) {
		if (xt->status & XTTY_STOPPED && bp->type == M_DATA) {
			putbq(qp, bp);
			qp->flag |= QFULL;
			return;
		}
		if (!(xt->sg.sg_flags & RAW)
		&& (xt->sg.sg_flags & (ALLDELAY | CRMOD))
		&& bp->type == M_DATA) {
			if (!ocanon(xt, bp, qp->next)) {
				putbq(qp, bp);
				qp->next->flag |= QWANTW;
				return;
			}
		} else if (bp->type == M_IOCTL)
			ctl(qp, bp);
		else
			(*qp->next->qinfo->putp)(qp->next, bp);
	}
	if (qp->count)
		qp->next->flag |= QWANTW;
}
