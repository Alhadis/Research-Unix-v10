/*
 * first cut at NI port in DEBNA
 * slower than it need be, because of all the copying;
 * disallows use of the storage port
 */
#include "sys/param.h"
#include "sys/biaddr.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/stream.h"
#include "sys/biic.h"
#include "sys/ethernet.h"
#include "sys/enio.h"
#include "sys/pte.h"
#include "sys/bvp.h"
#include "sys/debna.h"

caddr_t remqhi();


#define	PCMDQ	0	/* just one command queue */
#define	PFREQ	0	/* just one free queue for now */

/*
 * internal ideas about packets
 */

/*
 * bounds of an Ethernet packet
 * min is probably uninteresting
 * max is constrained by the port, which will fuss and shut down
 * if handed a buffer bigger than ETHERMAXTU+ETHERCKSUM
 */
#define	ETHERMINTU	60
#define	ETHERMAXTU	1514
#define	ETHERCKSUM	4

#define	NIRCVBUF	(ETHERMAXTU+ETHERCKSUM)

struct dgi {
	struct bvpdg h;		/* header */
	char d[NIRCVBUF];
};

struct mst {
	struct bvpmsg h;
	struct stptdb p;
};

/*
 * config stuff
 */
extern struct bnactl bna[];
extern struct bnabuf bnabuf[];
extern struct biaddr bnaaddr[];
extern int bnacnt;

long bnaopen();
int bnaclose(), bnaput();

static struct qinit bnarinit = { noput, NULL, bnaopen, bnaclose, 0, 0 };
static struct qinit bnawinit = { bnaput, NULL, bnaopen, bnaclose, 4*ETHERMAXTU, 64 };
static struct streamtab bnasinfo = { &bnarinit, &bnawinit };
struct cdevsw bnacdev = cstrinit(&bnasinfo);

long
bnaopen(q, dev)
register struct queue *q;
dev_t dev;
{
	register int unit, chan;
	register struct bnachan *bc;

	unit = minor(dev);
	chan = unit % BNACHAN;
	unit /= BNACHAN;
	if (unit >= bnacnt)
		return (0);
	if (bnainit(unit) == 0)
		return (0);
	bc = &bna[unit].chan[chan];
	if (bc->rq)
		return (0);
	bc->unit = unit;
	bc->proto = 0;
	q->ptr = (caddr_t)bc;
	q->flag |= QDELIM;
	WR(q)->ptr = (caddr_t)bc;
	WR(q)->flag |= QDELIM|QBIGB;
	bc->rq = q;
	return (1);
}

/*
 * init the bvp port registers and pqb
 * set up our list of free transmit buffers,
 * and the free queue for the port
 * -- only one free queue for now
 */
bnainit(unit)
register int unit;
{
	register char *cp;
	register struct bnactl *bn;
	register int i;

	bn = &bna[unit];
	if (bvpinit(&bn->bvp, &bnaaddr[unit], NIREGS) == 0) {
		printf("debna %d: can't init\n", unit);
		return (0);
	}
	if (bn->bvp.d->f[PFREQ].f_size == 0) {	/* somewhat sleazy init flag */
		bn->bvp.d->f[PFREQ].f_size = BNABSIZE-BVPHSIZE;
		cp = bnabuf[unit].rbuf;
		cp = (char *)(((int)cp + 7) & ~07);	/* quad-align */
		for (i = 0; i < BNARBUF; i++, cp += BNABSIZE) {
			if (((int)cp&PGOFSET) > (((int)cp+sizeof(struct bvpdg))&PGOFSET))
				cp = (char *)(((int)cp+PGOFSET)&~PGOFSET);
			bnarbuf(bn, (struct dgi *)cp);
		}
		cp = bnabuf[unit].xbuf;
		cp = (char *)(((int)cp + 7) & ~07);	/* quad-align */
		for (i = 0; i < BNAXBUF; i++, cp += BNABSIZE) {
			if (((int)cp&PGOFSET) > (((int)cp+sizeof(struct bvpdg))&PGOFSET))
				cp = (char *)(((int)cp+PGOFSET)&~PGOFSET);
			bnaxbuf(bn, (struct dgi *)cp);
		}
	}
	bcopy(bn->bvp.r->addr, bn->myaddr, sizeof(bn->myaddr));
	return (1);
}

bnaclose(q)
register struct queue *q;
{
	register struct bnachan *bc;

	bc = (struct bnachan *)q->ptr;
	/* turn off PTDB? */
	bc->rq = NULL;
	bc->proto = 0;
}

/*
 * output put
 */
bnaput(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct bnachan *bc;
	register int s;

	bc = (struct bnachan *)q->ptr;
	switch (bp->type) {
	case M_IOCTL:
		bnaioctl(q, bp);
		return;

	case M_DATA:
		putq(q, bp);
		if (bp->class & S_DELIM)
			break;
		return;

	default:
		freeb(bp);
		return;
	}
	/*
	 * at least a whole packet on the queue
	 */
	bc->dcnt++;
	s = spl6();
	while (bnasend(bc->unit))
		;
	splx(s);
}

bnaioctl(q, bp)
register struct queue *q;
register struct block *bp;
{
	register struct bnachan *bc;
	register struct bnactl *bn;

	bc = (struct bnachan *)q->ptr;
	bn = &bna[bc->unit];
	bp->type = M_IOCACK;
	switch (stiocom(bp)) {
	case ENIOTYPE:		/* set proto */
		bc->proto = *((int *)stiodata(bp));
		bnastptdb(bn, bc);
		freeb(bp);	/* IOCACK when the port finishes */
		return;

	case ENIOADDR:
		bcopy(bn->myaddr, stiodata(bp), sizeof(bn->myaddr));
		bp->wptr = bp->rptr + sizeof(bn->myaddr) + STIOCHDR;
		break;

	default:
		bp->type = M_IOCNAK;
		break;
	}
	qreply(q, bp);
}

bnastptdb(bn, bc)
register struct bnactl *bn;
register struct bnachan *bc;
{
	register struct mst *dp;
	register int s;

	s = spl6();
	if ((dp = (struct mst *)bn->xfree) == NULL) {
		bc->needst = 1;
		bn->needst = 1;
		splx(s);
		return;
	}
	bn->xfree = (struct dgi *)(dp->h.q.head);
	bc->needst = 0;
	splx(s);
	bzero((caddr_t)dp, sizeof(*dp));
	dp->h.bd_opc = BVPSNDMSG;
	dp->h.bd_flag = BVPRSP;
	dp->h.bm_len = sizeof(*dp) - ((char *)&dp->h.bm_opc - (char *)dp);
	dp->h.bm_opc = NISTPTDB;
	dp->p.pt_proto = bc->proto;
	dp->p.pt_fqi = PFREQ;
	dp->p.pt_flag = PTABM|PTAAM;
	dp->p.pt_id = bc - bn->chan;
	if (insqti((caddr_t)dp, &bn->bvp.d->p.p_cmdq[PCMDQ]))
		bvpcomm(bn->bvp.r, PCOWN|PCCMDQ|(PCMDQ<<PCDS));
}

/*
 * here on an interrupt
 */
bna0int(unit)
int unit;
{
	register struct bvpdata *b;
	register struct bnactl *bn;
	register struct dgi *dp;
	register struct bnabuf *bf;	/* just for safety check */
	register int i;

	if ((unsigned)unit >= bnacnt)
		panic("bna0int");
	bn = &bna[unit];
	if (bn->bvp.r == NULL) {
		printf("debna %d stray intr\n", unit);
		return;
	}
	bn->bvp.rsave = *bn->bvp.r;	/* copy all, for debugging */
	if (bn->bvp.rsave.stat & PSOWN)
		bn->bvp.r->stat &=~ PSOWN;	/* allow fresh status */
	if ((bn->bvp.rsave.stat & PSSTAT) != SENAB) {
		printf("debna %d: ps %x pe %x pd %x\n", unit,
			bn->bvp.rsave.stat, bn->bvp.rsave.err, bn->bvp.rsave.data);
		return;
	}
	b = bn->bvp.d;
	bf = &bnabuf[unit];
	while ((dp = (struct dgi *)remqhi(&b->p.p_rspq)) != NULL) {
		if (dp->h.bd_sts != BVPSUC)
			printf("debna %d opc %x sts %x\n", unit, dp->h.bd_opc, dp->h.bd_sts);
		switch (dp->h.bd_opc) {
		case BVPSNDDGI:		/* datagram sent */
			for (i = ETHERALEN-1; i >= 0; --i)
				if (((struct etherpup *)dp->d)->dhost[i] != 0xff
				&&  ((struct etherpup *)dp->d)->dhost[i] != bn->myaddr[i])
					goto notecho;
			/*
			 * packet sent to ourselves;
			 * port won't echo it back
			 */
			dp->h.bd_len += ETHERCKSUM;	/* banrdg will trim */
			bcopy(bn->myaddr, ((struct etherpup *)dp->d)->shost, sizeof(bn->myaddr));
			bnardg(bn, dp);
notecho:
			/* fall through */
		case BVPSNDMSG:		/* message sent */
			if ((char *)dp < bf->xbuf
			||  (char *)dp >= &bf->xbuf[sizeof(bf->xbuf)])
				panic("bna0int sent");
			bnaxbuf(bn, dp);
			continue;

		case BVPRCVDGI:		/* datagram received */
			if ((char *)dp < bf->rbuf
			||  (char *)dp >= &bf->rbuf[sizeof(bf->rbuf)])
				panic("bna0int rcv dg");
			bnardg(bn, dp);
			bnarbuf(bn, dp);
			continue;

		case BVPRCVMSG:		/* message received */
			if ((char *)dp < bf->rbuf
			||  (char *)dp >= &bf->rbuf[sizeof(bf->rbuf)])
				panic("bna0int rcv msg");
			bnarmsg(bn, (struct bvpmsg *)dp);
			bnarbuf(bn, dp);
			continue;

		default:
			printf("debna %d: %x: opc %x\n", unit, dp, dp->h.bd_opc);
			continue;	/* leaving buffer tied up */
		}
	}
	while (bnasend(unit))
		;
}

/*
 * here is a received datagram;
 * find the right channel,
 * and send it up the queue
 */
bnardg(bn, dp)
register struct bnactl *bn;
register struct dgi *dp;
{
	register int n;
	register struct block *bp;
	register char *p;
	register struct queue *q;
	int len;
	short proto;

	if (dp->h.bd_dgsts != NISUC) {
		printf("debna %d dg sts %x\n", bn - bna, dp->h.bd_dgsts);
		return;
	}
	len = (dp->h.bd_len+BVPHSIZE)-sizeof(struct bvpdg)-ETHERCKSUM;
	proto = (short)((struct etherpup *)dp->d)->type;
	for (n = 0; n < BNACHAN; n++)
		if (bn->chan[n].proto == proto)
			break;
	if (n >= BNACHAN)
		return;		/* unexpected proto */
	q = bn->chan[n].rq;
	if (q == NULL)
		return;		/* snh */
	if (q->next->flag & QFULL)
		return;		/* no room */
	p = dp->d;
	while (len > 0) {
		if ((bp = allocb(len)) == NULL)
			return;		/* wrong, but hard */
		n = bp->lim - bp->wptr;
		if (len < n)
			n = len;
		bcopy(p, bp->wptr, n);
		bp->wptr += n;
		p += n;
		len -= n;
		if (len <= 0)
			bp->class |= S_DELIM;
		(*q->next->qinfo->putp)(q->next, bp);
	}
}

/*
 * here is a message
 */

bnarmsg(bn, dp)
register struct bnactl *bn;
register struct bvpmsg *dp;
{
	register struct bnachan *bc;
	register struct mst *sdp;

	switch (dp->bm_opc) {
	case NISTPTDB:	/* ptdb set, sent ioctl back */
		sdp = (struct mst *)dp;
		if (sdp->p.pt_id < 0 || sdp->p.pt_id >= BNACHAN)
			panic("bnastptdb");
		bc = &bn->chan[sdp->p.pt_id];
		if (bc->rq == NULL || bc->proto != sdp->p.pt_proto)
			return;	/* early close? */
		if (sdp->h.bm_nists != NISUC)
			printf("debna %d %d stptdb sts %x\n", bn - bna,
				sdp->p.pt_id, sdp->h.bm_nists);
		putctl(bc->rq->next, M_IOCACK);
		return;

	default:
		printf("debna %d msg opc %x sts %x\n", dp->bm_opc, dp->bm_nists);
		return;
	}
}

/*
 * find a channel with a packet,
 * find a buffer,
 * and send
 */
bnasend(unit)
int unit;
{
	register struct bnactl *bn;
	register struct bnachan *bc;
	register struct dgi *dp;
	register char *p;
	register struct block *bp;
	register int n;
	char *ep;

	bn = &bna[unit];
	n = bn->lastx;
	while (bn->chan[n].dcnt == 0) {
		if (++n >= BNACHAN)
			n = 0;
		if (n == bn->lastx)
			return (0);		/* nothing to send */
	}
	bn->lastx = n;
	bc = &bn->chan[n];
	if ((dp = bn->xfree) == NULL)
		return (0);			/* no buffer */
	bn->xfree = (struct dgi *)dp->h.q.head;
	bzero((caddr_t)&dp->h, sizeof(dp->h));
	p = dp->d;
	ep = p + sizeof(dp->d);
	while ((bp = getq(WR(bc->rq))) != NULL) {
		n = bp->wptr - bp->rptr;
		if (p + n > ep)
			n = ep - p;	/* quietly truncate */
		if (n) {
			bcopy(bp->rptr, p, n);
			p += n;
		}
		if (bp->class & S_DELIM) {
			bc->dcnt--;
			freeb(bp);
			break;
		}
		freeb(bp);
	}
	if (bp == NULL) {
		printf("debna %d no delim\n", unit);
		bc->dcnt = 0;
		dp->h.q.head = (quadque *)bn->xfree;
		bn->xfree = dp;
		return (1);	/* ok to try another channel */
	}
	if (p - dp->d < ETHERMINTU)
		p = dp->d + ETHERMINTU;	/* quietly extend */
	dp->h.bd_opc = BVPSNDDGI;
	dp->h.bd_flag = BVPRSP;		/* so it will appear on rsp queue */
	dp->h.bd_len = p - &dp->h.bd_dgsts;
	dp->h.bd_ptdb = unit;	/* some valid number */
	/* probably should zero some other fields */
	if (insqti((caddr_t)dp, &bn->bvp.d->p.p_cmdq[PCMDQ]))
		bvpcomm(bn->bvp.r, PCOWN|PCCMDQ|(PCMDQ<<PCDS));
	return (1);
}

/*
 * free a buffer for the port to fill in
 * -- port fusses if buffer bigger than an Ethernet packet + BVP header
 */
bnarbuf(bn, dp)
register struct bnactl *bn;
register struct dgi *dp;
{

	dp->h.bd_sts = 0;	/* just in case */
	dp->h.bd_opc = BVPRCVDGI;
	dp->h.bd_len = sizeof(struct bvpdg) + NIRCVBUF - BVPHSIZE;
	if (insqti((caddr_t)dp, &bn->bvp.d->f[PFREQ].f_q))
		bvpcomm(bn->bvp.r, PCOWN|PCFREQ|(PFREQ<<PCDS));
}

bnaxbuf(bn, dp)
register struct bnactl *bn;
register struct dgi *dp;
{
	register int i;

	dp->h.q.head = (quadque *)bn->xfree;
	bn->xfree = dp;
	if (bn->needst) {
		bn->needst = 0;
		for (i = 0; i < BNACHAN; i++)
			if (bn->chan[i].needst)
				bnastptdb(bn, &bn->chan[i]);
	}
}

/*
 * (fairly) general BVP routines
 */
bvpinit(bv, ap, roff)
register struct bvp *bv;
struct biaddr *ap;
int roff;
{
	register struct bvpregs *r;
	register struct biic *rb;
	register long ps;	/* most recent status */

	if ((rb = bv->rb) == NULL) {
		if ((rb = (struct biic *)biaddr(ap)) == NULL
		||  badaddr(rb, sizeof(long))) {
			printf("bvp absent\n");
			return (0);
		}
		bv->rb = rb;
	}
	if (bv->r == NULL) {
		r = (struct bvpregs *)((char *)rb + roff);
		if (badaddr(r, sizeof(long))) {
			printf("bvp off %x absent\n", roff);
			return (0);
		}
		bv->r = r;
	}
	if (bv->dbuf == NULL) {
		bv->dbuf = geteblk();
		if (bv->dbuf->b_bcount < sizeof(struct bvpdata)) {
			printf("bcount too small\n");
			brelse(bv->dbuf);
			return (0);
		}
		clrbuf(bv->dbuf);
		bv->d = (struct bvpdata *)bv->dbuf->b_un.b_addr;
	}
	r = bv->r;
	ps = r->stat;
	if ((ps & PSSTAT) == SUNDEF) {
		biinit(ap, 0);
		bvpdatinit(bv->d);
		bv->d->p.p_nodmsk = rb->biintr;
		bv->d->p.p_vector = (ap->vec<<2) | 5;	/* vec, br */
		bvpcomm(r, physadr(&bv->d->p)|PCOWN|PCINIT);
		ps = bvpstat(r);
	}
	if ((ps & PSSTAT) == SINIT) {
		bvpcomm(r, PCOWN|PCENAB);
		ps = bvpstat(r);
	}
	if ((ps & PSSTAT) != SENAB) {
		printf("ps %x pe %x pd %x\n", r->stat, r->err, r->data);
		return (0);
	}
	return (1);
}

bvpdatinit(b)
register struct bvpdata *b;
{
	bzero((char *)b, sizeof(*b));
	b->p.p_nfreeq = NFREEQ;
	b->p.p_fqb = b->f;
	b->p.p_bvplvl = 1;
	b->p.p_pqb = &b->p;
	b->p.p_bdt = b->b;
	b->p.p_bdtlen = NBDT;	/* -1? */
	b->p.p_spt = (struct pte *)physadr(Sysmap);
	b->p.p_sptlen = 0x100000;	/* huge */
	b->p.p_gpt = b->p.p_spt;
	b->p.p_gptlen = b->p.p_sptlen;
}

/*
 * send a port command
 */
#define	COMMTIME	1000000

bvpcomm(r, c)
register struct bvpregs *r;
long c;
{
	register int i;

	i = COMMTIME;
	while (r->ctrl & PCOWN)
		if (--i <= 0) {
			printf("bvpcomm timeout c %x\n", c);
			return;	/* what else to do? */
		}
	r->stat &=~ PSOWN;
	r->ctrl = c;
}

/*
 * wait for status in init
 */

#define	STATTIME	1000000

int
bvpstat(r)
register struct bvpregs *r;
{
	register long ps;
	register int i;

	i = STATTIME;
	while (((ps = r->stat) & PSOWN) == 0)
		if (--i <= 0) {
			printf("bvpstat timeout, ps %x\n", ps);
			return (0);
		}
	return (ps);
}

/*
 * VAX queue primitives
 *
 * -- to set up empty queue, just zero both longwords
 */

/*
 * remove entry from head;
 * return entry, or 0 if none
 */
caddr_t
remqhi(q)
quadque *q;
{
	asm("0: remqhi *4(ap),r0");
	asm("bcs 0b");		/* couldn't interlock; try again */
	asm("bvc 1f");		/* br if removed something */
	asm("clrl r0");
	asm("1:");
}

/*
 * insert entry to tail
 * return 1 if this was the first entry
 */
insqti(e, q)
caddr_t e;
quadque *q;
{
	asm("clrl r0");
	asm("0: insqti *4(ap),*8(ap)");
	asm("bcs 0b");
	asm("bneq 1f");
	asm("incl r0");
	asm("1:");
}
