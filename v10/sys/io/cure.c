/*
 * simple CURE datakit driver
 * -- assumes it is the first CURE, and only one allowed for now
 *	(the latter not severe, as 256 channels are permitted)
 */
#include "sys/param.h"
#include "sys/stream.h"
#include "sys/dkio.h"
#include "sys/ubaddr.h"
#include "sys/conf.h"
#include "sys/kc.h"
#include "sys/dkstat.h"
#include "sys/dkmod.h"
#include "sys/buf.h"

/*
 * hardware stuff
 */
struct device {
	unsigned short	buf;
	unsigned char	csr;
	char	fill1;
	short	fill2;
	char	reset;
};

/*
 * status bits
 */
#define	SSEQ	01	/* 1-bit command sequence # */
#define	SRRDY	02	/* receive ready */
#define	SXRDY	04	/* transmit ready */
#define	SUBERR	010	/* unibus error */
#define	SERR	020	/* protocol error */
#define	STATE	0300	/* booting state mask... */
#define	SOK	0200	 /* ready for real IO */
#define	SBOOT	0100	 /* ready for download (just been reset) */
#define	SBOOTING 0	 /* in middle of download */
#define	SRESET	0300	 /* finished download, must reset UB base, vector */

/*
 * command bits
 */
#define	CSEND	02		/* send */
#define	CSETB	03		/* announce location of control buffers */
#define	CRCV	04		/* receive */
#define	CBA	06		/* set bus address */
#define	CBC	010		/* set buffer count */
#define	CINTR	012		/* set vector address */
#define	CRESET	014		/* reset interface */
#define	CIACK	016		/* ack interrupt or reset */

#define	CXA	0140		/* high address bits */
#define	XASHIFT	11		/* shift high address bits this much */

typedef short Env;		/* datakit envelope in our buffers */

/*
 * bits in transmit buffer
 */
#define	XDATA	0400		/* data, not control */

/*
 * bits in receive buffer
 */
#define	RMARK	01000		/* channel mark */
#define	RDATA	0400		/* this is data */
#define	RSPCL	0100000		/* special (sign bit): mark or control */
#define	REOT	(RSPCL|RMARK)	/* end of receive buffer, if short */

/*
 * per-channel stuff
 */
#define	NCHAN	256
struct	kc kc[NCHAN];
int	kccnt = NCHAN;
char	kcstate[NCHAN];

/*
 * kc flags
 */
#define	DKXCL	01		/* exclusive open */
#define	DKXWANT	02		/* output pending this channel */

/*
 * per-controller stuff;
 * always just one for now
 */

extern	struct ubaddr cureaddr[];
struct kccure kccure[1];
#define	KBNO	0	/* always this kc for now */
int	kcxfirst;
int	kcxnext;
int	kcrfirst;
int	kcrnext;

/*
 * kccure flags
 */
#define	XBUSY	01	/* transmit busy */
#define	XWANT	02	/* output needed */
#define	INIT	04

/*
 * illicit linkage to other datakit code
 */

struct	dkstat	dkstat;

#define	KNO	0

#define	DKISML	16	/* smallest interesting allocation */
#define	DKITHRES 200

long kcopen();
int kcclose(), kcput(), kcosrv();

static	struct qinit kcrinit = { noput, NULL, kcopen, kcclose, 0, 0 };
	struct qinit kcwinit = { kcput, kcosrv, kcopen, kcclose, 1500, 600 };
struct	streamtab kcinfo = { &kcrinit, &kcwinit };
struct	cdevsw curecdev = cstrinit(&kcinfo);

static	kclastseq;

/*
 * open DK channel
 */
long
kcopen(q, dev)
register struct queue *q;
register dev_t dev;
{
	register struct kc *dkp;
	register struct kccure *kk;
	register chan;
	extern struct dkmodule *dkmodall();
	int kcclock();

	chan = minor(dev);
	if (chan<=0 || chan>=kccnt)
		return(0);
	kk = &kccure[KBNO];
	if ((kk->flags & INIT) == 0) {
		if ((kk->modp = dkmodall(dev, 0, kccnt)) == NULL)
			return (0);
		kk->modp->dkstate = kcstate;
		if (kcinit(kk, 1) == 0)
			return (0);
		kk->flags |= INIT;
		timeout(kcclock, (caddr_t)minor(dev), 10*HZ);
	}
	dkp = &kc[chan];
	if (kcstate[chan] != DKCLOSED) {	/* already open */
		if (dkp->flag & DKXCL)
			return(0);
		if (kcstate[chan] != DKOPEN)
			return(0);	/* closing channels can't reopen */
		return(1);
	}
	dkp->dkrq = q;
	q->ptr = (caddr_t)dkp;
	WR(q)->ptr = (caddr_t)dkp;
	WR(q)->flag |= QNOENB|QBIGB;
	dkp->flag = DKXCL;
	kcstate[chan] = DKOPEN;
	return(1);
}

/*
 * make sure cure is alive;
 * init data structures once
 * it might be better to deal with BDPs dynamically somehow,
 * e.g. on the comet where there are very few
 */
kcinit(kk, firsttime)
register struct kccure *kk;
{
	register struct device *reg;
	register int i;
	register struct kc *dkp;
	static ubm_t map;
	uaddr_t uaddr;
	extern cure0int(), kcreset();

	kk->kno = KNO;
	if ((reg = (struct device *)ubaddr(&cureaddr[0])) == NULL
	||  badaddr(reg, 2)) {
		printf("cure0 absent\n");
		return (0);
	}
	for (i = kccnt - 1, dkp = &kc[i]; i >= 0; --dkp, --i)
		dkp->chan = i;
	kk->addr = reg;
	if ((reg->csr&STATE) != SRESET && (reg->csr&STATE) != SOK)
		return(0);		/* check downloaded OK */
	kclastseq = (reg->csr^SSEQ) & SSEQ;
	if (kcwcmd(kk, CINTR, cureaddr[0].vec) == 0)
		return (0);
	/* allocate and announce command buffers */
	if (firsttime)
		map = ubmalloc(kk->ubno, (2*NCBUF+1)*sizeof(struct cmd), 0);
	else {		/* resetting; free blocks in use */
		for (i=0; i<NCBUF; i++) {
			if (kk->ibp[i])
				freeb(kk->ibp[i]);
			if (kk->obp[i])
				freeb(kk->obp[i]);
			kk->ibp[i] = NULL;
			kk->obp[i] = NULL;
		}
		kcxfirst = kcrfirst = kcxnext = kcrnext = 0;
	}
	uaddr = ubmaddr(kk->ubno, kk->xcbuf, (2*NCBUF+1)*sizeof(struct cmd), map);
	printf("cure reset, uaddr %x map %x\n", uaddr, map);
	kcwcmd(kk, CBA, uaddr);
	kcwcmd(kk, CBC, NCBUF);
	kcwcmd(kk, CSETB|((uaddr>>XASHIFT)&CXA), 0);
	kcwcmd(kk, CIACK, 0);	/* to delay until CSETB processed */
	if ((reg->csr&STATE)!=SOK) {
		printf("cure not ready, state %o\n", reg->csr);
		return(0);
	}
	kcistart(kk);
	return (1);
}

/*
 * close DK channel
 */
kcclose(q)
register struct queue *q;
{
	register struct kc *dkp;
	register struct kccure *kk;

	kk = &kccure[KBNO];
	dkp = (struct kc *)q->ptr;
	if (dkp == NULL)
		panic("kcclose");
	dkp->dkrq = NULL;
	dkp->flag = 0;
	if (kcstate[dkp->chan] == DKRCLOSE || kk->modp->listnrq==NULL)
		kcstate[dkp->chan] = DKCLOSED;
	else if (kcstate[dkp->chan] == DKOPEN)
		kcstate[dkp->chan] = DKLCLOSE;
	if (kk->modp->listnrq)
		putctl2(RD(kk->modp->listnrq), M_PRICTL, DKMCLOSE, dkp->chan);
}

/*
 * interrupt
 */
cure1int(dev) {}
cure0int(dev)
{
	register struct kccure *kk;
	register struct device *reg;
	register doneb, i, csr;


	kk = &kccure[KBNO];
	if ((reg = kk->addr) == NULL)
		return;
	csr = reg->csr;
	i = csr&STATE;
	if (i!=SOK && i!=SRESET)
		return;
	if (csr & (SERR|SUBERR))
		printf("cure status %o\n", reg->csr);
	doneb = reg->buf;
	i = doneb&0377;			/* first rcv buf not done */
	while (i != kcrfirst) {
		kcrecv(kk, kcrfirst);
		kcrfirst = (kcrfirst+1)%NCBUF;
	}
	kcistart(kk);
	i = (doneb>>8)&0377;		/* first xmit buf not done */
	while (i != kcxfirst) {
		kk->flags &=~ XBUSY;
		if (kk->obp[kcxfirst]) {
			freeb(kk->obp[kcxfirst]);
			ubmfree(kk->ubno, kk->omap[kcxfirst]);
		}
		kk->obp[kcxfirst] = NULL;
		kcxfirst = (kcxfirst+1)%NCBUF;
	}
	if (kk->xfirst)
		kcosrv((struct queue *)NULL);
}

/*
 * receive a buffer
 */
kcrecv(kk, i)
register struct kccure *kk;
{
	register struct block *bp, *nbp;
	register struct queue *q;
	register c;
	struct cmd rcbuf;

	rcbuf = kk->rcbuf[i];
	bp = kk->ibp[i];
	if (bp==NULL) {
		printf("null bp in kcrecv\n");
		return;
	}
	kk->ibp[i] = NULL;
	c = rcbuf.chan;
	if (c==0 || c>=kccnt || (q = kc[c].dkrq)==NULL) {
		if (c==0)
			dkstat.pack0++;
		else if (c>=kccnt)
			dkstat.packstrange++;
		else
			dkstat.closepack++;
		freeb(bp);
		return;
	}
	if (q->next->flag&QFULL) {	/* channel overflow */
		freeb(bp);
		return;
	}
	if (rcbuf.count==0)
		freeb(bp);
	else {			/* use small buffer if not much data */
		 if (rcbuf.count<=64 && (nbp=allocb(rcbuf.count))) {
			bcopy(bp->rptr, nbp->rptr, rcbuf.count);
			freeb(bp);
			bp = nbp;
		}
		bp->wptr += rcbuf.count;
		dkstat.input += rcbuf.count;
		(*q->next->qinfo->putp)(q->next, bp);
	}
	for (c=0; c<NCTL; c++) {
		if (rcbuf.ctl[c]==0)
			continue;
		bp = allocb(1);
		*bp->wptr++ = rcbuf.ctl[c];
		if ((rcbuf.ctl[c]&RDATA)==0)
			bp->type = M_CTL;
		(*q->next->qinfo->putp)(q->next, bp);
	}
}

/*
 * start new input buffers
 */
kcistart(kk)
register struct kccure *kk;
{
	register struct device *reg;
	register struct block *bp;
	register i;

	if ((reg = kk->addr) == NULL)
		return;
	while((kcrnext+1)%NCBUF != kcrfirst) {
		i = kcrnext;
		kk->ibp[i] = bp = allocb(1024);
		kk->imap[i] = ubmblk(kk->ubno, bp, 0);
		kk->rcbuf[i].ubaddr = ubadrptr(kk->ubno, bp, kk->imap[i]);
		kk->rcbuf[i].count = bp->lim - bp->base;
		kcwcmd(kk, CRCV, i);
		kcrnext = (i+1)%NCBUF;
	}
}

/*
 * put procedure for output
 */
kcput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct kc *dkp;
	register struct kccure *kk;
	register int n;

	dkp = (struct kc *)q->ptr;
	kk = &kccure[KBNO];
	switch (bp->type) {

	case M_IOCTL:
		switch (stiocom(bp)) {
		case DIOCNXCL:
			dkp->flag &=~ DKXCL;
			bp->wptr = bp->rptr;
			bp->type = M_IOCACK;
			break;

		case KIOCINIT:
			/* eventually, reset things here */

		default:
			bp->type = M_IOCNAK;
			break;
		}
		qreply(q, bp);
		return;

	case M_CTL:
	case M_DATA:
		dkstat.output += bp->wptr - bp->rptr;
		putq(q, bp);
		n = spl5();
		if ((dkp->flag & DKXWANT) == 0) {
			if (kk->xfirst == NULL)
				kk->xfirst = dkp;
			else
				kk->xlast->link = dkp;
			dkp->link = NULL;
			kk->xlast = dkp;
			dkp->flag |= DKXWANT;
		}
		if ((kk->flags & XBUSY) == 0)
			qenable(q);
		splx(n);
		return;

	case M_PRICTL:
		switch (*bp->rptr) {
		case DKMCLOSE:
			n = bp->rptr[1];
			if (n < kccnt) {
				if (kcstate[n] == DKOPEN) {
					kcstate[n] = DKRCLOSE;
					putctl(kc[n].dkrq->next, M_HANGUP);
				} else if (kcstate[n] == DKLCLOSE)
					kcstate[n] = DKCLOSED;
			}
			freeb(bp);
			return;

		case DKMXINIT:
			n = bp->rptr[1];
			if (n < kccnt && kcstate[n] == DKOPEN)
				(*kc[n].dkrq->next->qinfo->putp)(kc[n].dkrq->next, bp);
			else
				freeb(bp);
			return;				

		default:
			freeb(bp);
			return;
		}

	default:
		freeb(bp);
		return;
	}
}

kcosrv(junk)
struct queue *junk;
{
	register struct kccure *kk = &kccure[KBNO];
	register s;

	s = spl5();
	while (kk->xfirst) {
		if ((kcxnext+1)%NCBUF == kcxfirst) {
			kk->flags |= XBUSY;
			return;
		}
		if (kcosrvbuf(kcxnext))
			kcxnext = (kcxnext+1)%NCBUF;
	}
	splx(s);
}

/*
 * fill up one buffer and send it
 */

kcosrvbuf(i)
register i;
{

	register struct kccure *kk;
	register struct block *bp;
	register struct queue *q;
	register struct device *reg;
	register struct kc *dkp;
	int c;

	kk = &kccure[KBNO];
	reg = kk->addr;
	dkp = kk->xfirst;
more:
	if (dkp==NULL) {
		printf("null dkp in kcosrvbuf\n");
		return 0;
	}
	if (dkp->dkrq==NULL) {
		dkp->flag &=~ DKXWANT;
		dkp = dkp->link;
		if ((kk->xfirst = dkp) == NULL) {
			kk->xlast = NULL;
			return 0;
		}
		goto more;
	}
	q = WR(dkp->dkrq);
	kk->xcbuf[i].chan = dkp->chan;
	c = 0;
	kk->xcbuf[i].count = 0;
	while ((bp = getq(q)) != NULL) {
		if (bp->type == M_CTL || bp->rptr+NCTL >= bp->wptr) {
			while (bp->rptr < bp->wptr && c < NCTL) {
				kk->xcbuf[i].ctl[c++] = *bp->rptr++ |
				   (bp->type==M_CTL?0:XDATA);
				bp->type = M_DATA;
			}
			if (bp->rptr >= bp->wptr) {
				freeb(bp);
				continue;
			}
		} else if (c==0 && kk->xcbuf[i].count==0) {
			kk->omap[i] = ubmblk(kk->ubno, bp, 0);
			kk->xcbuf[i].ubaddr = ubadrptr(kk->ubno, bp, kk->omap[i]);
			kk->xcbuf[i].count = bp->wptr - bp->rptr;
			kk->obp[i] = bp;
			continue;
		}
		putbq(q, bp);
		break;
	}
	if (bp==NULL) {
		dkp->flag &=~ DKXWANT;
		dkp = dkp->link;
		if ((kk->xfirst = dkp) == NULL)
			kk->xlast = NULL;
	}
	while (c<NCTL)
		kk->xcbuf[i].ctl[c++] = 0;
	kcwcmd(kk, CSEND, i);
	return 1;
}

kcwcmd(kk, cmd, buf)
register struct kccure *kk;
{
	register ocsr;
	register count = 0;
	register state;

	ocsr = kk->addr->csr;
	state = ocsr&STATE;
	if (state != SOK) {
		if (state!=SRESET)
			return(0);
		switch(cmd&037) {
		case CSETB:
		case CBA:
		case CBC:
		case CINTR:
		case CIACK:
			break;
		default:
			if (kcinit(kk, 0)==0)
				return(0);
			break;
		}
	}
	/* wait for last command to be accepted */
	while ((kk->addr->csr&SSEQ) == kclastseq) {
		if (++count > 100000) {
			printf("cure not responding: csr %o\n", kk->addr->csr);
			return(0);
		}
	}
	if (kk->addr->csr & (SERR|SUBERR))
		printf("cure csr %o\n", kk->addr->csr);
	kclastseq = kk->addr->csr&SSEQ;
	kk->addr->buf = buf;
	kk->addr->csr = cmd;
	return(1);
}

kcclock(dev)
caddr_t dev;
{
	cure0int(minor((int)dev));
	timeout(kcclock, dev, 10*HZ);
}

/*
 * raw cure device for downloading
 */
long rcureopen();
int rcureclose(), rcureoput();

static struct qinit rcurerinit = {
	noput, NULL, rcureopen, nulldev, 0, 0
};
static struct qinit rcurewinit = {
	rcureoput, NULL, rcureopen, nulldev, 200, 100
};
struct streamtab rcureinfo = {
	&rcurerinit, &rcurewinit
};

/*
 * config glue
 */
extern struct ubaddr rcureaddr[];		/* one per device */
extern int rcurecnt;			/* one per device or what? */
struct cdevsw rcurecdev = cstrinit(&rcureinfo);

long
rcureopen(q, d)
register struct queue *q;
{
	register dev;
	register struct device *mp;

	if((dev = minor(d)) >= rcurecnt)
		return 0;
	if((mp = (struct device *)ubaddr(&rcureaddr[dev])) == 0
	  || badaddr(mp, sizeof(u_char))) {
		printf("cure %d absent\n", d);
		return 0;
	}
	WR(q)->ptr = q->ptr = (caddr_t) dev;
	return 1;
}

rcureoput(q, bp)
register struct queue *q;
register struct block *bp;
{
#define	DV	017		/* actual device number */
#define	BOOTOK	020		/* initialization flag */

	register struct device *mp =
	     (struct device *)ubaddr(&rcureaddr[(int)q->ptr & DV]);
	register int csr, n;

	switch(bp->type) {

	case M_IOCTL:
		bp->type = M_IOCNAK;
		bp->wptr = bp->rptr;
		switch(stiocom(bp)) {
		case KIOCINIT:			/* reboot cure */
			mp->reset = 1;
			delay(10);
			mp->reset = 0;
			bp->type = M_IOCACK;
			break;
		}
		qreply(q, bp);
		return;

	case M_DATA:
		if (((int)q->ptr&BOOTOK) == 0) {		/* first write */
			if ((mp->csr&STATE) != SBOOT) {
				printf("cure not ready to load, %o\n", mp->csr);
				goto err;
			}
			mp->buf = 070707;	/* magic number starts bootload */
			delay(10);
			if ((mp->csr&STATE) != SBOOTING) {
				printf("cure load err0 %o\n", mp->csr);
				goto err;
			}
			q->ptr = (caddr_t)((int)q->ptr | BOOTOK);
		}
		while (bp->rptr < bp->wptr) {
			if ((mp->csr&STATE) != SBOOTING) {
				printf("cure load err1\n");
				goto err;
			}
			csr = mp->csr;
			mp->buf = *bp->rptr++;
			for (n=0; ((csr ^ mp->csr)&SSEQ)==0 && n<100000; ++n) {
				if (mp->csr&SERR || (mp->csr&STATE)!=SBOOTING) {
					printf("cure load err2 %o\n", mp->csr);
					goto err;
				}
			}
			if (n==100000) {
				printf("cure load err3\n");
				goto err;
			}
		}
		break;
	}
	freeb(bp);
	return;
  err:
	bp->type = M_HANGUP;
	qreply(q, bp);
}
