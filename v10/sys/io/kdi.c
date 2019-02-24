/*
 * stream driver for DK via KMC-11/KDI board
 * delicately intertwined with kmc.c
 */

#include "sys/param.h"
#include "sys/stream.h"
#include "sys/ttyio.h"
#include "sys/dkio.h"
#include "sys/conf.h"
#include "sys/dkstat.h"
#include "sys/dkmod.h"
#include "sys/ubaddr.h"
#include "sys/kmc.h"
#include "sys/kdi.h"
#include "sys/buf.h"
#include "sys/dkwindow.h"

#define	KDIPRI	28

/*
 * flags
 */
#define	KMBUSY	0x01
#define	KMSTOP	0x02
#define	KMINC	0x04
#define	KMBIG	0x3c		/* rcvd lots last time, use big rcv buffer */
#define	KMXCL	0x40		/* exclusive use */
#define	KMXBIG	0x80		/* tell KMC to use large messages */
#define BIGSET(c) ((((c)<<1)|KMINC)&KMBIG | ((c)&~KMBIG))
#define BIGCLR(c) (((c)<<1)&KMBIG | ((c)&~KMBIG))

/*
 * device structure
 */
struct device {
	char	sts;
	char	x1;
	union {
		struct {
			u_short	lo;
			u_char	hi;
		} a;
		struct {
			u_char	x2;
			u_char	x3;
			u_char	ch;
			u_char	ct;
			u_char	sh;
			u_char	st;
		} q;
	} u;
};

/*
 * KMC commands
 */
#define	KC_INIT		1
#define	KC_SEND		2
#define	KC_RCVB		3
#define	KC_CLOSE	4
#define	KC_XINIT	5
#define	KC_CMD		6
#define	KC_FLAG		7
#define	KC_SOI		8

/*
 * subcommands of KC_CMD or KC_SEND
 */
#define	OFLUSH		02	/* flush output */
#define	OSPND		04	/* suspend output */
#define	ORSME		010	/* resume output */
#define	OBOTM		0200	/* send BOTM trailer, not BOT */

/*
 * KMC reports
 */
#define	KS_SEND		024
#define	KS_RDB		025
#define	KS_EOI		026
#define	KS_CNTL		027
#define	KS_ERR		030

/*
 * KC_RCV modes
 */
#define	CBLOCK		0040	/* return on block boundary */
#define	CTIME		0100	/* return when time expires */

/*
 * KS_RDB mode
 */
#define	SFULL		0001	/* buffer full */
#define	SCNTL		0002	/* cntl char recv */
#define	SABORT		0010	/* rcv aborted */
#define	SBLOCK		0040	/* block boundary */
#define	STIME		0100	/* time limit expired */

/*
 * URP control characters
 */
#define	D_DELAY	0100
#define	D_BREAK	0110

/*
 *  KMC errors
 */
#define	E_NOQB	4		/* internal buffer runout */
#define	E_DUP	5		/* duplicate send (shouldn't but does happen) */
#define	E_UMETA	7		/* unknown control character */

/*
 * tracing
 */
#define DEBUG
#ifdef	DEBUG
struct	kin ktrbuf[256];
struct	kin *ktrp = ktrbuf;
#define	TRACE(x) *ktrp++ = x; if (ktrp >= &ktrbuf[256]) ktrp = ktrbuf;
#else
#define	TRACE(x)	;
#endif

int	kdiclose(), kdiput(), kdiisrv();
long	kdiopen();
struct	qinit kdirinit = { noput, kdiisrv, kdiopen, kdiclose, 0, 0 };
struct	qinit kdiwinit = { kdiput, NULL, kdiopen, kdiclose, 512, 128 };
struct	streamtab kdiinfo = { &kdirinit, &kdiwinit };
struct cdevsw kdicdev = cstrinit(&kdiinfo);

struct	kmaddr kmcubaddr();
extern	struct kdikmc kdikmc[];
extern	struct kmcdk k[];
extern	int kdicnt;
extern	struct kmc kmc[];
extern	struct ubaddr kmcaddr[];
extern	int kmccnt;
struct	dkstat	dkstat;

#define	KDICHAN	96		/* channels per kdi (== per kmc) */
#define	KMC(d)	((d)/KDICHAN)
#define	CHAN(d)	((d)%KDICHAN)

/*
 * open channel
 */
long
kdiopen(q, dev)
register struct queue *q;
dev_t dev;
{
	register struct kdi *kp;
	register struct block *bp;
	register struct kdikmc *xp;
	register int kno, chan;

	kno = KMC(minor(dev));
	chan = CHAN(minor(dev));
	if (kno >= kdicnt || kno >= kmccnt || chan >= KDICHAN)
		return (0);
	xp = &kdikmc[kno];
	if (xp->bad)
		return (0);
	if (xp->init == 0)
		if (kdiinit(dev) == 0)
			return (0);
	kp = &xp->kdi[chan];
	if (xp->state[chan] != DKCLOSED) {
		if (kp->ostate&KMXCL)	/* exclusive use until reset */
			return(0);
		if (xp->state[chan] != DKOPEN)
			return(0);	/* closing channels cannot reopen */
		return(1);
	}
	kp->dkrq = q;
	q->ptr = (caddr_t)kp;
	WR(q)->flag |= QDELIM|QBIGB;
	q->flag |= QDELIM;
	WR(q)->ptr = (caddr_t)kp;
	xp->state[chan] = DKOPEN;
	kp->ostate = KMXCL;
	kcmd(kno, KC_INIT, kp->chan, 0, 0, 0, (uaddr_t)NULL);
	if ((bp = allocb(64)) == NULL) {
		xp->state[chan] = 0;
		return(0);
	}
	kp->lastctl = 0;
	kp->ibp = bp;
	kp->imap = ubmblk(xp->ubno, bp, 0);
	kcmd(kno, KC_RCVB, chan, bp->lim-bp->wptr, 50,
	     CBLOCK|CTIME, ubadwptr(xp->ubno, bp, kp->imap));
	return(1);
}

kdiclose(q)
register struct queue *q;
{
	register struct kdi *kp = (struct kdi *)q->ptr;
	register s, i;
	register struct kdikmc *xp;

	xp = &kdikmc[kp->kno];
	if (xp->modp == NULL || xp->modp->listnrq == NULL
	||  xp->state[kp->chan]==DKRCLOSE)
		xp->state[kp->chan] = DKCLOSED;
	else if (xp->state[kp->chan] == DKOPEN) {
		xp->state[kp->chan] = DKLCLOSE;
		for (i=0; i<30 && (WR(q)->count || kp->obp); i++)
			tsleep((caddr_t)kp, KDIPRI, 1);
	}
	kp->dkrq = NULL;
	if (xp->modp && xp->modp->listnrq)
		putctl2(RD(xp->modp->listnrq), M_PRICTL, DKMCLOSE, kp->chan);
	s = spl6();
	if (kp->obp) {
		freeb(kp->obp);
		kp->obp = NULL;
	}
	splx(s);
	kp->ostate &= ~KMXCL;
	kcmd(kp->kno, KC_CLOSE, kp->chan, 0, 0, 0, (uaddr_t)NULL);
}

kdiput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct kdi *kdp = (struct kdi *)q->ptr;
	register char *sp;
	register s;
	register struct kdikmc *xp;
	struct block *xbp;
	int t;

	xp = &kdikmc[kdp->kno];
	switch(bp->type) {

	case M_CTL:
	case M_DATA:
	case M_DELAY:
	case M_BREAK:
		if (xp->state[kdp->chan] < DKLCLOSE) {
			freeb(bp);
			return;
		}
		putq(q, bp);
		if ((kdp->ostate&KMBUSY)==0)
			kmstart(kdp);
		return;

	case M_PRICTL:
		switch (*bp->rptr) {
		case DKMCLOSE:
			s = bp->rptr[1];
			if (s < KDICHAN) {
				flushq(q, 1);
				if (xp->state[s] == DKOPEN) {
					xp->state[s] = DKRCLOSE;
					putctl(xp->kdi[s].dkrq->next, M_HANGUP);
				} else if (xp->state[s] == DKLCLOSE)
					xp->state[s] = DKCLOSED;
				kcmd(kdp->kno, KC_CLOSE, s, 0, 0, 0, (uaddr_t)NULL);
			}
			break;

		case DKMXINIT:
			/* retrieve channel number */
			bp->rptr++;
			s = *bp->rptr++;
	
			/* forget it if this isn't a valid channel */
			if (s >= KDICHAN || xp->state[s] != DKOPEN)
				break;
			
			/* next two bytes (if there) are the traffic type */
			if(bp->wptr-bp->rptr>=2){
				t = (*bp->rptr++<<8) & 0xff00;
				t |= *bp->rptr++ & 0xff;
	
				/* check window sizes */
				if (W_VALID(t)) {
	
					/* kmc has only two possible window sizes */
					t = W_VALUE(W_ORIG(t));
					if(t<1024)
						xp->kdi[s].ostate &= ~KMXBIG;
					else
						xp->kdi[s].ostate |= KMXBIG;
				}
			}

			/* restart urp on the channel, with the correct window */
			kdixinit(&xp->kdi[s]);
			break;

		default:
			break;
		}
		break;		/* and freeb */

	case M_IOCTL:
		sp = stiodata(bp);
		bp->type = M_IOCACK;
		switch (stiocom(bp)) {

		case TIOCSDEV:
			if (((struct ttydevb *)sp)->ispeed == 0)
				putctl2(q, M_PRICTL, DKMCLOSE, kdp->chan);
		case KIOCISURP:
			bp->wptr = bp->rptr;
			break;

		case DIOCSCTL:
			bp->wptr = bp->rptr;
			if (*sp == 0)
				break;
			if ((xbp = allocb(1)) == NULL)
				bp->type = M_IOCNAK;
			else {
				xbp->type = M_CTL;
				*xbp->wptr++ = *sp;
				putq(q, xbp);
				if ((kdp->ostate&KMBUSY) == 0)
					kmstart(kdp);
			}
			break;

		case DIOCRCTL:
			*sp = kdp->lastctl;
			kdp->lastctl = 0;
			bp->wptr = (unsigned char *)sp + 1;
			break;

		case DIOCNXCL:
			kdp->ostate &= ~KMXCL;
			bp->wptr = bp->rptr;
			break;

		case TIOCGDEV:
			((struct ttydevb *)sp)->ispeed =
			  ((struct ttydevb *)sp)->ospeed = B9600;
			break;

		case KIOCINIT:
			kdixinit(kdp);
			bp->rptr = bp->wptr;
			break;

		case KIOCSHUT:
			if (kdp->chan > 2) {	/* hackish security attempt */
				bp->type = M_IOCNAK;
				break;
			}
			kdireset(kdp->kno);
			bp->rptr = bp->wptr;
			break;

		case DIOCSTREAM:
			RD(q)->flag &= ~QDELIM;
			bp->rptr = bp->wptr;
			break;

		case DIOCRECORD:
			RD(q)->flag |= QDELIM;
			bp->rptr = bp->wptr;
			break;

		case DIOCXWIN: {
			unsigned int wins;
			/* KMC does big (756) or small (84) windows currently */
			wins = (sp[0] + (sp[1]<<8))*sp[4];
			kdp->ostate &= ~KMXBIG;
			if (wins>=756)
				kdp->ostate |= KMXBIG;
			/* will take effect next XINIT */
			bp->rptr = bp->wptr;
			break;
		    }

		default:
			bp->wptr = bp->rptr;
			bp->type = M_IOCNAK;
			break;
		}
		qreply(q, bp);
		return;

	case M_STOP:
		s = OSPND;
		kdp->ostate |= KMSTOP;
		goto dontcmd;

	case M_FLUSH:
		flushq(q, 0);
		s = spl6();
		if (kdp->obp) {
			freeb(kdp->obp);	/* this cannot be right -- too early */
			kdp->obp = 0;
		}
		kdp->ostate &= ~(KMSTOP|KMBUSY);
		splx(s);
		s = OFLUSH|ORSME;
		goto docmd;

	case M_START:
	case M_HANGUP:
		s = ORSME;
		kdp->ostate &= ~KMSTOP;
		goto dontcmd;
	docmd:
		kcmd(kdp->kno, KC_CMD, kdp->chan, 0, s, 0, (uaddr_t)NULL);
	dontcmd:
		kmstart(kdp);
		break;

	default:		/* not handled; just toss */
		break;
	}
	freeb(bp);
}

/*
 * restart URP on some channel:
 * send XINIT to KMC;
 * discard any pending transmit block
 *
 * should pending block really be discarded, or requeued?
 * former seems to match real cases
 */

kdixinit(kp)
register struct kdi *kp;
{
	register int s;

	s = spl6();
	kcmd(kp->kno, KC_XINIT, kp->chan, 0, 0, (kp->ostate&KMXBIG)!=0, (uaddr_t)NULL);
	if (kp->obp) {
		freeb(kp->obp);
		kp->obp = NULL;
	}
	if (kp->omap) {
		ubmfree(kdikmc[kp->kno].ubno, kp->omap);
		kp->omap = 0;
	}
	kp->ostate &=~ (KMSTOP|KMBUSY);
	splx(s);
}

kdiisrv(q)
register struct queue *q;
{
	register struct kdi *kp = (struct kdi *)q->ptr;
	register int ubno;
	register struct block *bp;

	if ((q->next->flag&QFULL)==0 && kp->ibp==NULL) {
		bp = allocb(kp->ostate&KMBIG? 1024:64);
		if (bp == NULL)
			panic("kdi: can't alloc");
		kp->ibp = bp;
		ubno = kdikmc[kp->kno].ubno;
		kp->imap = ubmblk(ubno, bp, 0);
		kcmd(kp->kno, KC_RCVB, kp->chan, bp->lim - bp->wptr,
		     50, CBLOCK|CTIME, ubadwptr(ubno, bp, kp->imap));
	}
}

kmstart(kdp)
register struct kdi *kdp;
{
	register s = spl5();
	register struct block *bp;
	register struct queue *qp;
	register c;
	register struct kdikmc *xp;

	if (kdp->ostate&(KMBUSY|KMSTOP)) {
		splx(s);
		return;
	}
	if ((qp = kdp->dkrq)==NULL || (qp = WR(qp))==NULL) {
		splx(s);
		return;
	}
	xp = &kdikmc[kdp->kno];
	if (bp = getq(qp)) switch (bp->type) {

	case M_DATA:
		c = OBOTM;
		if (bp->class&S_DELIM)
			c = 0;
		kdp->omap = ubmblk(xp->ubno, bp, 0);
		kcmd(kdp->kno, KC_SEND, kdp->chan, bp->wptr-bp->rptr,
		     0, c, ubadrptr(xp->ubno, bp, kdp->omap));
		kdp->ostate |= KMBUSY;
		dkstat.output += bp->wptr - bp->rptr;
		kdp->obp = bp;
		break;

	case M_DELAY:
		c = D_DELAY;
		while (*bp->rptr>0) {
			c++;
			*bp->rptr >>= 1;
		}
		kdp->omap = ubmblk(xp->ubno, bp, 0);
		kcmd(kdp->kno, KC_SEND, kdp->chan, 1, c, 0, ubadrptr(xp->ubno, bp, kdp->omap));
		kdp->ostate |= KMBUSY;
		kdp->obp = bp;
		break;

	case M_CTL:
		kdp->omap = ubmblk(xp->ubno, bp, 0);
		kcmd(kdp->kno, KC_SEND, kdp->chan, 0, *bp->rptr, 0, ubadrptr(xp->ubno, bp, kdp->omap));
		kdp->ostate |= KMBUSY;
		kdp->obp = bp;
		break;

	case M_BREAK:
		kdp->omap = ubmblk(xp->ubno, bp, 0);
		kcmd(kdp->kno, KC_SEND, kdp->chan, 0, D_BREAK, 0, ubadrptr(xp->ubno, bp, kdp->omap));
		kdp->ostate |= KMBUSY;
		kdp->obp = bp;
		break;

	default:
		printf("mesg %o in kdi\n", bp->type);
		freeb(bp);
		break;
	}
	splx(s);
}

int kcmdovern[10];
long kcmdovert[10];

kcmd(kno, type, chan, len, ctl, mode, addr)
uaddr_t addr;
{
	register i;
	register struct device *dp = kmc[kno].k_addr;
	register struct kin *kp;
	register s;
	static struct kmaddr nulladr = {0, 0};
	static serno;
	register long loop;

	/*
	 *  if the kmc is dead, don't ask it to do things
	 */
	if(dp->sts ==3)
		return;

	if (dp->u.q.x3)
		dp->u.q.x3 = 0;
busywait:
	/*
	 *  wait at low priority till there's a free command.  No
	 *  sense slowing all processes down.
	 */
	if(((dp->u.q.ch+1)%NKMB)==dp->u.q.ct) {
		loop = 0;
		while (((dp->u.q.ch+1)%NKMB)==dp->u.q.ct) {
			if (loop == 1000)
				printf("waiting for kdi%d\n", kno);
			if(loop++>1000000) {
				printf("kmc%d died, restart\n", kno);
				kdireset(kno);
				return;
			}
		}
		kcmdovern[kno]++;
		if(kcmdovert[kno]<loop)
			kcmdovert[kno] = loop;
	}
	s = spl5();
	/*
	 *  someone else might have gotten in at low priority.  check
	 *  again.
	 */
	if(((dp->u.q.ch+1)%NKMB)==dp->u.q.ct) {
		splx(s);
		goto busywait;
	}
	i = dp->u.q.ch;
	if (i >= NKMB)
		panic("kcmd");
	kp = &k[kno].cmd[i];
	kp->type = type;
	kp->serno = ++serno;
	kp->chan = chan;
	kp->len = len;
	kp->ctl = ctl;
	kp->mode = mode;
	kp->addr.hi = addr>>16;
	kp->addr.lo = addr;
	i++;
	if (i >= NKMB)
		i = 0;
	dp->u.q.ch = i;
	TRACE(*kp);
	splx(s);
}

u_char kdiseqno[10];

/*
 * Interrupt routine-- unload status buffer:
 * release write blocks, collect input
 */
kdiintr(kno)
{
	register struct device *dp = kmc[kno].k_addr;
	register struct kin *sp;
	register struct kdi *kp;
	register struct block *bp;
	register c;
	register struct kdikmc *xp;

	xp = &kdikmc[kno];
	if (dp->u.q.x3)
		dp->u.q.x3 = 0;
	for ( ; (c = dp->u.q.st) != dp->u.q.sh; dp->u.q.st = c) {
		if (c >= NKMB) {
			printf("kdi stat buf is %d\n", c);
			panic("kdiintr");
		}
		sp = &k[kno].stat[c];
		TRACE(*sp);
		c++;
		if (c >= NKMB)
			c = 0;
		if (sp->chan >= KDICHAN) {
			printf("kdi%d stat chan is 0%o\n", kno, sp->chan);
			printf("type: %o len: 0%o mode: 0%o\n");
			continue;
		}
		if(sp->serno!=kdiseqno[kno]){
			printf("kdi%d stat chan is 0%o\n", kno, sp->chan);
			printf("seqno is %d, should be %d\n",sp->serno,
				kdiseqno[kno]);
		}
		kdiseqno[kno]=(sp->serno+1)%256;
		kp = &xp->kdi[sp->chan];
		switch (sp->type) {

		case KS_SEND:
			if (kp->obp) {
				freeb(kp->obp);
				kp->obp = NULL;
			}
			kp->ostate &= ~KMBUSY;
			if (kp->omap) {
				ubmfree(xp->ubno, kp->omap);
				kp->omap = 0;
			}
			if (kp->dkrq && WR(kp->dkrq)->first)
				kmstart(kp);
			break;

		case KS_RDB:
			if (kp->imap) {
				ubmfree(xp->ubno, kp->imap);
				kp->imap = 0;
			}
			bp = kp->ibp;
			kp->ibp = NULL;
			if (kp->dkrq==NULL || xp->state[kp->chan]!=DKOPEN) {
				if (bp)
					freeb(bp);
				break;
			}
			if (sp->mode & SABORT) {
				printf("kdi%d rcv abort chan %d mode %o bp %x\n",
				  kno, kp->chan, sp->mode, bp);
				if (bp)
					kp->ibp = bp;
				kp->rsize = 0;
				kp->ostate &= ~KMBIG;
				break;
			}
			if (bp == NULL) {
				printf("kdi%d: no ibp\n", kno);
				break;
			}
			/* special hacks */
			if (sp->mode&020
			||  sp->mode==0100 && bp->wptr+sp->len==bp->lim) {
				/*
				 *  Something no one understands.  Print out
				 *  some info so that we might figure out
				 *  why this freeb is in the code.
				 */
				printf("kdi%d rcv chan %d mode %o bp %x len %d\n",
				  kno, kp->chan, sp->mode, bp, sp->len);
				freeb(bp);
			} else {
				bp->wptr = bp->lim - sp->len;
				kp->rsize += bp->wptr - bp->rptr;
				dkstat.input += bp->wptr - bp->rptr;
				if(sp->mode&(SBLOCK|STIME)) {
					bp->class |= S_DELIM;
					if (kp->rsize >= 512)
						kp->ostate = BIGSET(kp->ostate);
					else
						kp->ostate = BIGCLR(kp->ostate);
					kp->rsize = 0;
				}
				(*kp->dkrq->next->qinfo->putp)
				       (kp->dkrq->next, bp);
				kp->ibp = NULL;
				if (sp->mode&SCNTL) {
					switch (sp->ctl) {

					case D_BREAK:
						putctl(kp->dkrq->next, M_BREAK);
						break;

					default:
						kp->lastctl = sp->ctl;
						putctl1(kp->dkrq->next, M_CTL, sp->ctl);
						break;

					}
				}
			}
			if ((kp->dkrq->next->flag&QFULL) == 0) {
				bp = allocb(kp->ostate&KMBIG? 1024:64);
				if (bp == NULL)
					panic("kdi: can't alloc");
				kp->ibp = bp;
				kp->imap = ubmblk(xp->ubno, bp, 0);
				kcmd(kno, KC_RCVB, kp->chan, bp->lim - bp->wptr,
				     50, CBLOCK|CTIME, ubadwptr(xp->ubno, bp, kp->imap));
			}
			break;
		
		case KS_EOI:
			printf("kdi%d chan %d rcv EOI %x\n", kno, sp->chan, sp->len);
			break;

		case KS_CNTL:
			/*
			printf("kdi%d chan %d rcv ctl %x\n", kno, sp->chan, sp->len);
			*/
			break;

		case KS_ERR:
			printf("kdi%d chan %d error %d\n", kno, sp->chan, sp->len);
			if (sp->len==E_NOQB || sp->len==E_UMETA
			 || sp->len==E_DUP) {
				printf("ignored\n");
				break;
			}
			kdireset(kno);
			break;

		}
	}
}

/*
 * init things, including data structures
 * -- assume KDICHAN struct kdis and KDICHAN chars for state
 * will fit in a buffer.  when BSIZE is 4096, it's true.
 */
int kdireload();

kdiinit(dev)
int dev;
{
	register struct device *kp;
	register i;
	register struct kdikmc *xp;
	register int kno;
	struct kmaddr ka;
	extern time_t time;

	kno = KMC(minor(dev));
	/* close all open channels? */
	if ((kp = kmc[kno].k_addr) == NULL)
		return (0);
	xp = &kdikmc[kno];
	if (xp->badtime+30 > time)	/* UGH */
		return(0);
	kp->sts = 0;			/* initialize KMC */
	if (xp->buf == NULL) {
		if ((xp->buf = geteblk()) == NULL)
			return (0);
		clrbuf(xp->buf);
		xp->kdi = (struct kdi *)xp->buf->b_un.b_addr;
		xp->state = (char *)&xp->kdi[KDICHAN];
		for (i = 0; i < KDICHAN; i++) {
			xp->kdi[i].chan = i;
			xp->kdi[i].kno = kno;
		}
	}
	if ((xp->modp = dkmodall(dev, kno*KDICHAN, (kno+1)*KDICHAN)) == 0)
		return (0);
	xp->modp->dkstate = xp->state;
	xp->ubno = kmcaddr[kno].ubno;
	if (xp->umap == 0) {
		if ((xp->umap = ubmalloc(xp->ubno, sizeof(struct kmcdk), 0)) == 0) {
			printf("kdi%d: no map\n", kno);
			return (0);
		}
	}
	xp->ubad = ubmaddr(xp->ubno, (caddr_t)&k[kno], sizeof(struct kmcdk), xp->umap);
	k[kno].kinit.cmdaddr = kmcubaddr((caddr_t)k[kno].cmd, (caddr_t)&k[kno], xp->ubad);
	k[kno].kinit.stataddr = kmcubaddr((caddr_t)k[kno].stat, (caddr_t)&k[kno], xp->ubad);
	k[kno].kinit.bufaddr = kmcubaddr((caddr_t)k[kno].kmcbuf, (caddr_t)&k[kno], xp->ubad);
	ka = kmcubaddr((caddr_t)&k[kno].kinit, (caddr_t)&k[kno], xp->ubad);
	kp->u.a.lo = ka.lo;
	kp->u.a.hi = ka.hi;
	xp->badtime = time;
	kp->sts = 1;			/* start handshake */
	for (i = 0; i<100000; i++) {
		if (kp->sts == 2) {
			kmc[kno].k_rint = kdiintr;
			kmc[kno].k_xint = kdiintr;
			kmc[kno].k_reset = kdireload;
			xp->init = 1;
			return(1);			/* KMC is running OK */
		}
		if (kp->sts == 3) {
			printf("kdi%d: entered state 3\n", kno);
			return(0);
		}
	}
	printf("kdi%d: kmc not ready\n", kno);
	return(0);
}

struct kmaddr
kmcubaddr(memaddr, membase, ubbase)
caddr_t memaddr, membase;
uaddr_t ubbase;
{
	register struct kmaddr r;
	register long a;

	a = (long)memaddr - (long)membase + (long)ubbase;
	r.hi = a >> 16;
	r.lo = (u_short)a;
	return(r);
}

/*
 * kmc reload
 */
kdireload(kno)
{
	register struct kdikmc *xp;

	xp = &kdikmc[kno];
	xp->init = 0;
	xp->modp = 0;
	xp->bad = 0;
}

/*
 * zap all open channels
 */
kdireset(kno)
{
	register struct kdi *kp;
	register struct kdikmc *xp;
	register struct device *dp;

	xp = &kdikmc[kno];
	if (xp->modp==0)
		return;
	if ((dp = kmc[kno].k_addr) != NULL)
		dp->sts = 3;	/* disable kdi kmc code */
	for (kp = &xp->kdi[KDICHAN-1]; kp >= xp->kdi; --kp) {
		register state = xp->state[kp->chan];
		if ((state==DKOPEN || state==DKLCLOSE) && kp->dkrq) {
			flushq(WR(kp->dkrq), 1);
			xp->state[kp->chan] = state==DKOPEN? DKRCLOSE: DKCLOSED;
			putctl(kp->dkrq->next, M_HANGUP);
		}
		if (kp->imap)
			ubmfree(xp->ubno, kp->imap);
		if (kp->omap)
			ubmfree(xp->ubno, kp->omap);
		kp->imap = kp->omap = 0;
	}
	xp->bad = 1;
}
