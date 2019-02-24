/*
 * DSA port driver for uda50
 *
 * this just passes packets around;
 * it makes calls to the routines in ra.c
 * to get the packets filled in
 */

#include "sys/param.h"
#include "sys/buf.h"
#include "sys/ubaddr.h"
#include "sys/map.h"
#include "sys/uba.h"
#include "sys/uda.h"
#include "sys/mscp.h"

/*
 * ONEPATH tries to use only one data path per controller;
 * a nice idea, but a disaster on the DW780 --
 * the UDA50 will cheerfully run transfers in parallel from
 * two drives on the same data path, which confuses the
 * DW780 and mixes up the data
 */
/* #define	ONEPATH	1	/* don't */

#define	hiword(x)	(((long)(x)>>16)&0177777)
#define	loword(x)	((long)(x)&0177777)

extern struct uba uba[];
extern struct ubaddr udaddr[];
extern struct ud ud[];
extern int udcnt;

struct ctab {
	int (*c_seql)();
	int (*c_dg)();
	int c_ctype;
} udctab[MSMAXID];

struct device {
	short udip;
	short udsa;
};

/*
 * bits in udsa
 */

#define	ERR	0100000
#define	STEP4	040000
#define	STEP3	020000
#define	STEP2	010000
#define	STEP1	04000
#define	STEPS	(STEP1|STEP2|STEP3|STEP4)

#define	IE	0200		/* step1 interrupt enable */
#define	PI	01		/* step2 purge intr enab */
#define	GO	01		/* step4 ok */

/*
 * uda communication area
 * ring sizes are chosen so that,
 * with 4K byte buffers,
 * one udcomm + CSIZE-sized command packets will
 * fit in one buffer;
 * RSIZE-sized response packets
 * will fit in another
 *
 * ring sizes must be powers of 2
 * (they are passed as such to the port)
 *
 * a delicacy:
 * command packets should not cross 64Kb boundaries.
 * it is believed that the KDB50 gets it wrong if they do.
 * hence, make CSIZE+4 evenly divide a page,
 * and get it aligned sensibly in bdreset
 */

#define	NCP2	5
#define	NRP2	5
#define	NCMD	(1<<NCP2)
#define	NRSP	(1<<NRP2)

struct udcomm {
	short	ud__r0;		/* reserved (ugh) */
	char	ud__r1;
	char	ud_bdp;		/* path to purge */
	short	ud_cmdint;	/* flag for command interrupt */
	short	ud_rspint;	/* flag for response interrupt */
	long	ud_rsp[NRSP];	/* response pointer ring */
	long	ud_cmd[NCMD];	/* command pointer ring */
};

/*
 * bits in ring pointers
 */

#define	DPOWN	0x80000000	/* port owns descriptor */
#define	DIE	0x40000000	/* ring transition intr enab */

#define	CSIZE	60	/* max size of command packet */
#define	RSIZE	60	/* max size of response packet */
#define	HDRSIZE	4	/* size of the header */

struct udcmd {
	short	uc_len;		/* length of message */
	char	uc_tc;		/* type, credits */
	char	uc_cid;		/* connection id */
	char	uc_data[CSIZE];
};

struct udrsp {
	short	ur_len;		/* length of message */
	char	ur_tc;		/* type, credits */
	char	ur_cid;		/* connection id */
	char	ur_data[RSIZE];
};

/*
 * message types
 */

#define	MTYPE	0xf0		/* where type lives */
#define	MTSEQL	0x00		/* sequential message */
#define	MTDG	0x10		/* datagram */
#define	MTCR	0x20		/* credit notification */

#define	MTNC	0xf		/* credits in tc */

/*
 * etc
 */

#define	PRIINI	(PZERO-3)
#define	PRIPKT	(PZERO-2)
#define	PRICRED	(PZERO-1)

/*
 * command packet to index
 */
#define	udmptoi(up, mp)	((struct udcmd *)((char *)(mp) - HDRSIZE) - (up)->ud_cpkt)

#define	TIMEOUT	15		/* time between checks */

int udinit(), udsend(), udmap(), udunmap();
struct mscmd *udgpkt();
struct msportsw udport = {
	udinit, udgpkt, udmap, udsend, udunmap
};

/*
 * init the port
 * returns nonzero if probably ok
 * allowed to sleep
 */

#define	UDCOFF	(((sizeof(struct udcomm)/sizeof(struct udcmd))+1)*sizeof(struct udcmd))

udinit(dev, type, force, cid, seql, dg)
unsigned int dev;
unsigned int cid;
int force;
int (*seql)(), (*dg)();
{
	register struct ud *up;
	register int ubno;
	struct buf *geteblk();
	extern udtimer();

	if (dev >= udcnt)
		return (0);
	if (cid >= MSMAXID)
		return (0);
	udctab[cid].c_seql = seql;
	udctab[cid].c_dg = dg;
	udctab[cid].c_ctype = type;
	up = &ud[dev];
	ubno = udaddr[dev].ubno;
	if (up->ud_flags & UISTART && force == 0)
		return (1);
	if ((up->ud_addr = (struct device *)ubaddr(&udaddr[dev])) == 0)
		return (0);
	if (ubbadaddr(ubno, &up->ud_addr->udsa, sizeof(short))) {
		printf("ud%d not present\n", dev);
		return (0);
	}
	udrundown(dev);
	if ((up->ud_flags & UINIT) == 0) {
		up->ud_cbuf = geteblk();
		clrbuf(up->ud_cbuf);
		up->ud_rbuf = geteblk();
		clrbuf(up->ud_rbuf);
		up->ud_cbm = ubmbuf(ubno, up->ud_cbuf, 0);
		up->ud_rbm = ubmbuf(ubno, up->ud_cbuf, 0);
#if ONEPATH
		up->ud_bdpno = ubmapath(ubno);
#endif
		up->ud_comm = (struct udcomm *)up->ud_cbuf->b_un.b_addr;
		up->ud_cpkt = (struct udcmd *)(up->ud_cbuf->b_un.b_addr + UDCOFF);
		up->ud_rpkt = (struct udrsp *)up->ud_rbuf->b_un.b_addr;
		up->ud_flags |= UINIT;
		timeout(udtimer, (caddr_t)dev, TIMEOUT * HZ);
	}
	return (udreset(dev));
}

/*
 * reset device
 * initially or after error or power fail
 * -- empirically, INITSTALL must be 200000 or so
 * to work with TD Systems SCSI adapter on 8550
 */

#define	INITSTALL	200000	/* >>1200us */

udreset(dev)
int dev;
{
	register struct ud *up;
	register struct device *rp;
	register uaddr_t pa;
	register int i;

	up = &ud[dev];
	rp = up->ud_addr;
	up->ud_flags &=~ UIDONE;
	up->ud_flags |= UISTART;
	rp->udip = 0;		/* hard reset */
	pa = ubadbuf(udaddr[dev].ubno, up->ud_cbuf, up->ud_cbm);
	up->ud_pcomm = pa + (4 * sizeof(short));	/* -> ud_rsp */
	up->ud_pcpkt = pa + UDCOFF;
	up->ud_cnext = 0;
	up->ud_rnext = 0;
	up->ud_credits = 0;
	for (i = 0; (rp->udsa & STEP1) == 0 && i < INITSTALL; i++)
		;
	if ((rp->udsa & STEP1) == 0) {
		up->ud_flags &=~ UISTART;
		printf("ud%d won't init\n", dev);
		return (0);
	}
	rp->udsa = ERR | IE | (NCP2<<11) | (NRP2<<8) | (udaddr[dev].vec>>2);
	return (1);
}

/*
 * finish up init, step by step
 * called from interrupt code
 */

udinintr(dev)
int dev;
{
	register struct ud *up;
	register struct device *rp;
	register int i;
	register uaddr_t pa;

	up = &ud[dev];
	rp = up->ud_addr;
	if (up->ud_flags & UIDONE) {
		printf("ud%d: unexpected init: sa %o\n", dev, rp->udsa);
		return;
	}
	switch (rp->udsa & STEPS) {
	case STEP1:
		udreset(dev);
		return;

	case STEP2:
		/* need to set PI for 780 type unibus. */
		rp->udsa = loword(up->ud_pcomm)|((uba[udaddr[dev].ubno].flags&UBQBUS)?0:PI);
		return;

	case STEP3:
		rp->udsa = hiword(up->ud_pcomm);
		return;

	case STEP4:
		rp->udsa = GO;
		for (i = 0; i < NCMD; i++) {
			up->ud_comm->ud_cmd[i] = 0;	/* unnecessary */
			up->ud_cpkt[i].uc_len = CSIZE;
			up->ud_cbusy[i] = FREE;
			up->ud_back[i] = NOBACK;
		}
		pa = ubadbuf(udaddr[dev].ubno, up->ud_rbuf, up->ud_rbm);
		pa += 2 * sizeof(short);
		for (i = 0; i < NRSP; i++, pa += sizeof(struct udrsp)) {
			up->ud_comm->ud_rsp[i] = pa | DIE | DPOWN;
			up->ud_rpkt[i].ur_len = RSIZE;
		}
		up->ud_flags |= UIDONE | UFIRST;
		wakeup((caddr_t)up);
		return;

	default:
		printf("ud%d init bad: sa %o\n", dev, rp->udsa);
		return;
	}
}

/*
 * tell the class drivers that the controller was reset
 * so they can clean up
 * called after controller is stopped (so it's safe to unmap things)
 */
udrundown(dev)
int dev;
{
	static struct msend me;
	register int i;

	me.m_sts = STRST;	/* magic */
	for (i = 0; i < MSMAXID; i++)
		if (udctab[i].c_seql)
			(*udctab[i].c_seql)(dev, udctab[i].c_ctype, &me);
}

/*
 * allocate a packet
 * and sufficient resources to send it
 * eg credits
 * may sleep
 *
 * somewhat silly assumption:
 * class driver will allocate a packet, and send it right away or nearly so
 */

struct mscmd *
udgpkt(dev)
int dev;
{
	register struct ud *up;
	register int i;
	int s;

	up = &ud[dev];
	if (up->ud_addr->udsa & STEP1)	/* was reset somehow */
		udreset(dev);
	s = spl6();
	while ((up->ud_flags & UIDONE) == 0)
		sleep((caddr_t)up, PRIINI);
	while (up->ud_credits < 2 && (up->ud_flags & UFIRST) == 0) {
		if (udpkscan(dev, 1))
			continue;
		up->ud_flags |= UCWAIT;
		sleep((caddr_t)&up->ud_credits, PRICRED);
	}
	if ((up->ud_flags & UFIRST) == 0)
		up->ud_credits--;
	for (;;) {
		for (i = 0; i < NCMD; i++)
			if (up->ud_cbusy[i] == FREE)
				break;
		if (i < NCMD)
			break;
		if (udpkscan(dev, 1) == 0 && udcmdscan(dev) == 0) {	/* kludge for hung controller */
			up->ud_flags |= UPWAIT;
			sleep((caddr_t)up->ud_cbusy, PRIPKT);
		}
	}
	up->ud_cbusy[i] = NABBED;
	splx(s);
	return ((struct mscmd *)up->ud_cpkt[i].uc_data);
}

/*
 * map a transfer into unibus space
 * and record in the buffer descriptor of a packet
 * may sleep
 */

udmap(dev, mp, bp)
int dev;
struct mscmd *mp;
register struct buf *bp;
{
	register struct ud *up;
	register int i;

	up = &ud[dev];
	i = udmptoi(up, mp);
#if ONEPATH
	if (up->ud_cmap[i] == 0)
		up->ud_cmap[i] = ubmbuf(udaddr[dev].ubno, bp, USLP);
	up->ud_cmap[i] = ubinspath(up->ud_bdpno, up->ud_cmap[i]);
#else
	if (up->ud_cmap[i] == 0)
		up->ud_cmap[i] = ubmbuf(udaddr[dev].ubno, bp, UBDP|USLP);
#endif
	up->ud_cbusy[i] |= MAPPED;
	*(long *)&mp->m_buff = ubadbuf(udaddr[dev].ubno, bp, up->ud_cmap[i]);
#if ONEPATH
	*(long *)&mp->m_buff |= up->ud_bdpno << 24;
#else
	*(long *)&mp->m_buff |= ubmpath(up->ud_cmap[i]) << 24;
#endif
}

/*
 * free a mapped packet
 * flush the bdp;
 * change the path in the map to the DDP before freeing (ugh)
 * so we won't free our reserved BDP too
 * it is OK to free an already freed packet, mostly to help reset code
 */

udunmap(dev, mp)
int dev;
struct mscmd *mp;
{
	register struct ud *up;
	register int i;

	up = &ud[dev];
	i = udmptoi(up, mp);
	if (up->ud_cmap[i]) {
#if ONEPATH
		ubmflush(udaddr[dev].ubno, ubmpath(up->ud_cmap[i]));
		ubmfree(udaddr[dev].ubno, ubinspath(0, up->ud_cmap[i]));
#else
		ubmfree(udaddr[dev].ubno, up->ud_cmap[i]);
#endif
		up->ud_cmap[i] = 0;
	}
	up->ud_cbusy[i] = FREE;
	if (up->ud_flags & UPWAIT) {
		up->ud_flags &=~ UPWAIT;
		wakeup((caddr_t)up->ud_cbusy);
	}
}


/*
 * send a packet
 * may not sleep
 * call at spl5
 */
udsend(dev, cid, mp)
int dev;
int cid;
struct mscmd *mp;
{
	register struct ud *up;
	register int i;
	register int j;
	register struct device *rp;

	up = &ud[dev];
	up->ud_flags &=~ UFIRST;
	i = udmptoi(up, mp);
	up->ud_cpkt[i].uc_cid = cid;
	j = up->ud_cnext++;
	if (up->ud_cnext >= NCMD)
		up->ud_cnext = 0;
	if (up->ud_comm->ud_cmd[j] & DPOWN)
		panic("udsend");
	if (up->ud_back[j] >= 0) {	/* ran for a while with no free */
		udcmdscan(dev);
		if (up->ud_back[j] >= 0)
			panic("udsend");
	}
	rp = up->ud_addr;
	if (rp->udsa & ERR) {	/* and now we lose a packet */
		printf("ud%d: hard error %o\n", dev, rp->udsa & 0177777);
		udreset(dev);
		return;
	}
	up->ud_back[j] = i;
	up->ud_comm->ud_cmd[j] = DPOWN | DIE |
		up->ud_pcpkt + (i * sizeof(struct udcmd)) + (2 * sizeof(short));
	up->ud_cbusy[i] |= SENT;
	up->ud_cbusy[i] &=~ NABBED;
	i = rp->udip;		/* initiate polling */
}

/*
 * interrupt routine
 */

long ud_spur;
long ud_npr;
#define	INIRETRY 5	/* TQK50 botch -- needs time to settle */

ud0int(dev)
int dev;
{
	register struct ud *up;
	register struct device *rp;
	register int i;

	up = &ud[dev];
	if (dev >= udcnt || (up->ud_flags & UINIT) == 0) {
		printf("ud%d: stray intr\n", dev);
		return;
	}
	rp = up->ud_addr;
	if ((up->ud_flags & UIDONE) == 0) {
		for (i = 0; i < INIRETRY; i++)
			if (rp->udsa & (STEPS|ERR))
				break;
		if ((rp->udsa & (STEPS|ERR)) == 0) {
			printf("ud%d: init lost; sa 0%o\n", dev, rp->udsa);
			return;
		}
	}
	if (rp->udsa & ERR) {
		printf("ud%d: hard error %o\n", dev, rp->udsa & 0177777);
		udreset(dev);
		return;
	}
	if (rp->udsa & STEPS) {
		udinintr(dev);
		return;
	}
	if (up->ud_comm->ud_bdp == 0
	&&  up->ud_comm->ud_cmdint == 0
	&&  up->ud_comm->ud_rspint == 0)
		ud_spur++;
	if (up->ud_comm->ud_bdp) {
		ubmflush(udaddr[dev].ubno, up->ud_comm->ud_bdp);
		up->ud_comm->ud_bdp = 0;
		rp->udsa = 0;		/* ack purge */
		ud_npr++;
	}
	while (up->ud_comm->ud_cmdint) {
		up->ud_comm->ud_cmdint = 0;
		udcmdscan(dev);
	}
	while (up->ud_comm->ud_rspint) {
		up->ud_comm->ud_rspint = 0;
		if (udpkscan(dev, 0))
			up->ud_flags &=~ UTIMER;
	}
}

/*
 * free packets which are completely sent
 * (and which don't have associated map)
 */

udcmdscan(dev)
int dev;
{
	register struct ud *up;
	register int i, j;
	register int freed;
	register struct udcomm *udc;

	up = &ud[dev];
	udc = up->ud_comm;
	freed = 0;
	for (j = 0; j < NCMD; j++)
		if (up->ud_back[j] >= 0
		&&  (udc->ud_cmd[j] & DPOWN) == 0) {
			i = up->ud_back[j];
			if ((up->ud_cbusy[i] & (SENT|MAPPED)) == SENT) {
				up->ud_cbusy[i] = FREE;
				freed++;
			}
			up->ud_back[j] = NOBACK;
		}
	if (freed && up->ud_flags & UPWAIT)
		wakeup((caddr_t)up->ud_cbusy);
	return (freed);
}

/*
 * check for responses from the controller
 * and deal with them
 * return a count for debugging purposes
 * called at spl6 (because of udtimer)
 */

int
udpkscan(dev, doall)
int dev;
int doall;
{
	register struct ud *up;
	register int i;
	int nf;
	register struct udrsp *pk;
	register struct ctab *cp;
	register struct udcomm *udc;	/* speed */

	up = &ud[dev];
	udc = up->ud_comm;
	nf = 0;
	for (i = up->ud_rnext; ; i < NRSP-1 ? i++ : (i=0)) {
		if (udc->ud_rsp[i] & DPOWN) {
			up->ud_rnext = i;
			/* don't stop if doall? */
			break;
		}
		nf++;
		pk = &up->ud_rpkt[i];
		up->ud_credits += pk->ur_tc & MTNC;
		if (up->ud_flags & UCWAIT) {
			wakeup((caddr_t)&up->ud_credits);
			up->ud_flags &=~ UCWAIT;
		}
		if (pk->ur_cid > MSMAXID)
			printf("ud%d msg id %d\n", dev, pk->ur_cid);
		else {
			cp = &udctab[pk->ur_cid];
			switch (pk->ur_tc & MTYPE) {
			case MTSEQL:
				if (cp->c_seql)
					(*cp->c_seql)(dev, cp->c_ctype, (struct msend *)pk->ur_data);
				break;
		
			case MTDG:
				if (cp->c_dg)
					(*cp->c_dg)(dev, cp->c_ctype, pk->ur_data);
				break;
		
			/* default: ignore */
			}
		}
		pk->ur_len = RSIZE;
		udc->ud_rsp[i] |= DPOWN | DIE;
	}
	return (nf);
}

/*
 * timeout routine
 * return any waiting packets
 * -- callout routines don't necessarily run at high priority.
 * hence the spl6, so udpkscan won't be reentered
 */

int ud_kicked;

udtimer(i)
int i;
{
	register struct ud *up;
	register int s;

	up = &ud[i];
	if ((up->ud_flags & UINIT) == 0)
		return;
	if (up->ud_flags & UIDONE) {
		if ((up->ud_flags & UTIMER) == 0)
			up->ud_flags |= UTIMER;
		else {
			s = spl6();
			if (udpkscan(i, 1) && up->ud_flags & UPWAIT) {
				wakeup((caddr_t)up->ud_cbusy);
				ud_kicked++;
			}
			splx(s);
			up->ud_flags &=~ UTIMER;
		}
	}
	timeout(udtimer, (caddr_t)i, TIMEOUT * HZ);
}
