/*
 * DSA port driver for KDB50 (aka BDA)
 * called by, e.g., ra.c
 */

#include "sys/param.h"
#include "sys/buf.h"
#include "sys/biic.h"
#include "sys/biaddr.h"
#include "sys/map.h"
#include "sys/bda.h"
#include "sys/mscp.h"
#include "sys/pte.h"

#define	hiword(x)	(((long)(x)>>16)&0177777)
#define	loword(x)	((long)(x)&0177777)

extern struct biaddr bdaddr[];
extern struct bd bd[];
extern int bdcnt;
struct ctab {
	int (*c_seql)();
	int (*c_dg)();
	int c_ctype;
} bdctab[MSMAXID];

/*
 * bd_flags
 */

#define	UINIT	01		/* already did trivial init */
#define	UIDONE	02		/* initialization all done */
#define	UPWAIT	04		/* waiting for command packet */
#define	UFIRST	010		/* let first packet go even if no credits */
#define	UTIMER	020		/* timer will kick on next go */
#define	UCWAIT	040		/* waiting for credits */
#define	UPMWAIT	0100		/* waiting for map */

#define	NOBACK	(-1)

/*
 * bd_cbusy
 */

#define	FREE	0
#define	NABBED	01
#define	SENT	02
#define	MAPPED	04

/*
 * bdip is really bigpr0 in struct biic
 */

struct device {
	short	bdxx;	/* unused */
	short	bdip;
	short	bdsar;
	short	bdsaw;
};

#define	bdaregs(p) ((struct device *)&((p)->bigpr0))

/*
 * bits in bdsa (variously read and write half)
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
 * bda communication area
 * ring sizes are chosen so that,
 * with 4K byte buffers,
 * one bdcomm + CSIZE-sized command packets will
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

struct bdcomm {
	short	bd__r0;		/* reserved (ugh) */
	char	bd__r1;
	char	bd_bdp;		/* path to purge */
	short	bd_cmdint;	/* flag for command interrupt */
	short	bd_rspint;	/* flag for response interrupt */
	long	bd_rsp[NRSP];	/* response pointer ring */
	long	bd_cmd[NCMD];	/* command pointer ring */
};

/*
 * bits in ring pointers
 */

#define	DPOWN	0x80000000	/* port owns descriptor */
#define	DIE	0x40000000	/* ring transition intr enab */

#define	BDAVIRT	0x80000000	/* in m_buff: mapped transfer */
#define	V	0x80000000	/* valid bit in page map */

#define	CSIZE	60	/* max size of command packet */
#define	RSIZE	60	/* max size of response packet */
#define	HDRSIZE	4	/* size of the header */

struct bdcmd {
	short	uc_len;		/* length of message */
	char	uc_tc;		/* type, credits */
	char	uc_cid;		/* connection id */
	char	uc_data[CSIZE];
};

struct bdrsp {
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
#define	bdmptoi(up, mp)	((struct bdcmd *)((char *)(mp) - HDRSIZE) - (up)->bd_cpkt)

#define	TIMEOUT	10		/* time between checks */

int bdinit(), bdsend(), bdmap(), bdunmap();
struct mscmd *bdgpkt();
struct msportsw bdport = {
	bdinit, bdgpkt, bdmap, bdsend, bdunmap
};

/*
 * init the port
 * called once only
 * returns nonzero if probably ok
 * allowed to sleep
 */

bdinit(dev, type, force, cid, seql, dg)
unsigned int dev;
unsigned int cid;
int force;
int (*seql)(), (*dg)();
{
	register struct bd *up;
	struct buf *geteblk();
	extern bdtimer();

	if (dev >= bdcnt)
		return (0);
	if (cid >= MSMAXID)
		return (0);
	bdctab[cid].c_seql = seql;
	bdctab[cid].c_dg = dg;
	bdctab[cid].c_ctype = type;
	up = &bd[dev];
	if (up->bd_flags & UINIT && force == 0)
		return (1);
	if ((up->bd_addr = (struct biic *)biaddr(&bdaddr[dev])) == 0) {
		printf("bd%d absent\n", dev);
		return (0);
	}
	bdrundown(dev);
	if ((up->bd_flags & UINIT) == 0) {
		up->bd_cbuf = geteblk();
		clrbuf(up->bd_cbuf);
		up->bd_rbuf = geteblk();
		clrbuf(up->bd_rbuf);
		up->bd_mbuf = geteblk();
		clrbuf(up->bd_mbuf);
		up->bd_comm = (struct bdcomm *)up->bd_cbuf->b_un.b_addr;
#define	BDCOFF	(((sizeof(struct bdcomm)/sizeof(struct bdcmd))+1)*sizeof(struct bdcmd))
		up->bd_cpkt = (struct bdcmd *)(up->bd_cbuf->b_un.b_addr + BDCOFF);
		up->bd_rpkt = (struct bdrsp *)up->bd_rbuf->b_un.b_addr;
		up->bd_pmap = (long *)up->bd_mbuf->b_un.b_addr;
		up->bd_flags |= UINIT;
		timeout(bdtimer, (caddr_t)dev, TIMEOUT * HZ);
	}
	bdreset(dev);
	return (1);
}

/*
 * reset device
 * initially or after error or power fail
 *
 * just kick the device here;
 * ideally we would get an interrupt when self-test finishes
 * (step 1 starts) but we don't, so let the timer routine catch it
 */

bdreset(dev)
register int dev;
{
	register struct bd *up;

	up = &bd[dev];
	up->bd_flags &=~ UIDONE;
	up->bd_addr->bicsr |= BINRST;	/* hard reset */
}

/*
 * finish up init, step by step
 * called from interrupt code
 */

bdinintr(dev)
register int dev;
{
	register struct bd *up;
	register struct device *rp;
	register struct biic *bi;

	up = &bd[dev];
	rp = bdaregs(up->bd_addr);
	if (up->bd_flags & UIDONE)
		printf("bd%d: unexpected init: sa %o\n", dev, rp->bdsar);
	switch (rp->bdsar & STEPS) {
	case STEP1:
		up->bd_cnext = 0;
		up->bd_rnext = 0;
		up->bd_credits = 0;
		bi = up->bd_addr;
		biinit(&bdaddr[dev], 0);
		bi->biuir = bdaddr[dev].vec;
		bi->bieir = (bdaddr[dev].vec + sizeof(long))|EIBR5;
		rp->bdsaw = ERR | IE | (NCP2<<11) | (NRP2<<8);
		return;

	case STEP2:
		rp->bdsaw = PI | loword(physadr(up->bd_comm->bd_rsp));
		return;

	case STEP3:
		rp->bdsaw = hiword(physadr(up->bd_comm->bd_rsp));
		return;

	case STEP4:
		rp->bdsaw = GO;
		for (dev = 0; dev < NCMD; dev++) {
			up->bd_comm->bd_cmd[dev] = 0;	/* unnecessary */
			up->bd_cpkt[dev].uc_len = CSIZE;
			up->bd_back[dev] = NOBACK;
		}
		for (dev = 0; dev < NRSP; dev++) {
			up->bd_rpkt[dev].ur_len = RSIZE;
			up->bd_comm->bd_rsp[dev] = physadr(up->bd_rpkt[dev].ur_data) | DIE | DPOWN;
		}
		up->bd_flags |= UIDONE | UFIRST;
		rminit(up->bd_map, BDANMAP, (up->bd_mbuf->b_bcount/sizeof(long))-1, 1);
		wakeup((caddr_t)up);
		return;

	default:
		printf("bd%d init bad: sar %o\n", dev, rp->bdsar);
		return;
	}
}

/*
 * tell the class drivers that the controller was reset
 * so they can clean up
 * called after controller is stopped (so it's safe to unmap things)
 */
bdrundown(dev)
int dev;
{
	static struct msend me;
	register int i;

	me.m_sts = STRST;	/* magic */
	for (i = 0; i < MSMAXID; i++)
		if (bdctab[i].c_seql)
			(*bdctab[i].c_seql)(dev, bdctab[i].c_ctype, &me);
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
bdgpkt(dev)
int dev;
{
	register struct bd *up;
	register int i;
	int s;

	up = &bd[dev];
	s = spl6();
	while ((up->bd_flags & UIDONE) == 0)
		sleep((caddr_t)up, PRIINI);
	while (up->bd_credits < 2 && (up->bd_flags & UFIRST) == 0) {
		if (bdpkscan(dev, 1))
			continue;
		up->bd_flags |= UCWAIT;
		sleep((caddr_t)&up->bd_credits, PRICRED);
	}
	if ((up->bd_flags & UFIRST) == 0)
		up->bd_credits--;
	for (;;) {
		for (i = 0; i < NCMD; i++)
			if (up->bd_cbusy[i] == FREE)
				break;
		if (i < NCMD)
			break;
		if (bdpkscan(dev, 1) == 0 && bdcmdscan(dev) == 0) {	/* kludge for hung controller */
			up->bd_flags |= UPWAIT;
			sleep((caddr_t)up->bd_cbusy, PRIPKT);
		}
	}
	up->bd_cbusy[i] = NABBED;
	splx(s);
	return ((struct mscmd *)up->bd_cpkt[i].uc_data);
}

/*
 * map a transfer if need be
 * and record in the buffer descriptor of a packet
 * may sleep
 *
 * BDA use of m_buff:
 *	first longword is buffer address;
 *		if BDAVIRT clear, physical addr, else virtual
 *	second longword is phys addr of base of page table if virtual
 *
 * subtlety: pretend we mapped it even if we didn't,
 * so it won't be unmapped until someone calls bdunmap
 * else they may be confused when they call it anyway
 */

bdmap(dev, mp, bp)
int dev;
struct mscmd *mp;
register struct buf *bp;
{
	register int i;
	register long *b;
	register struct pte *pt;
	int s;
	int base;
	register int size;
	register struct bd *up;

	up = &bd[dev];
	i = bdmptoi(up, mp);
	up->bd_cbusy[i] |= MAPPED;
	if ((bp->b_flags & (B_PHYS|B_UAREA|B_PAGET|B_DIRTY)) == 0) {
		b = (long *)&mp->m_buff;
		*b++ = (long)bp->b_un.b_addr|BDAVIRT;
		*b++ = physadr(Sysmap);
		*b = 0;
		return;
	}
	size = btoc(bp->b_bcount);
	if ((long)bp->b_un.b_addr & PGOFSET)
		size++;			/* unaligned */
	s = spl6();
	while ((base = rmalloc(up->bd_map, size)) == 0) {
		up->bd_flags |= UPMWAIT;
		sleep((caddr_t)up->bd_map, PRIPKT);
	}
	splx(s);
	up->bd_mbase[i] = base;
	up->bd_msize[i] = size;
	b = &up->bd_pmap[base];
	pt = btopte(bp);
	while (--size >= 0)
		*b++ = pt++->pg_pfnum | V;
	b = (long *)&mp->m_buff;
	*b++ = ctob(base)|((long)bp->b_un.b_addr&PGOFSET)|BDAVIRT;
	*b++ = physadr(up->bd_pmap);
	*b = 0;
}

/*
 * free a mapped packet
 */

bdunmap(dev, mp)
int dev;
struct mscmd *mp;
{
	register struct bd *up;
	register int i;

	up = &bd[dev];
	i = bdmptoi(up, mp);
	if (up->bd_cbusy[i] == FREE)
		return;		/* wasn't mapped, and already freed */
	if (up->bd_msize[i]) {
		rmfree(up->bd_map, up->bd_msize[i], up->bd_mbase[i]);
		up->bd_msize[i] = 0;
		if (up->bd_flags & UPMWAIT) {
			up->bd_flags &=~ UPMWAIT;
			wakeup((caddr_t)up->bd_map);
		}
	}
	up->bd_cbusy[i] = FREE;
	if (up->bd_flags & UPWAIT) {
		up->bd_flags &=~ UPWAIT;
		wakeup((caddr_t)up->bd_cbusy);
	}
}

/*
 * send a packet
 * may not sleep
 * call at spl5
 */

bdsend(dev, cid, mp)
int dev;
int cid;
struct mscmd *mp;
{
	register struct bd *up;
	register int i;
	register int j;

	up = &bd[dev];
	up->bd_flags &=~ UFIRST;
	i = bdmptoi(up, mp);
	up->bd_cpkt[i].uc_cid = cid;
	j = up->bd_cnext++;
	if (up->bd_cnext >= NCMD)
		up->bd_cnext = 0;
	if (up->bd_comm->bd_cmd[j] & DPOWN)
		panic("bdsend");
	if (up->bd_back[j] >= 0) {	/* ran for a while with no free */
		bdcmdscan(dev);
		if (up->bd_back[j] >= 0)
			panic("bdsend");
	}
	up->bd_back[j] = i;
	up->bd_comm->bd_cmd[j] = DPOWN | DIE | physadr(mp);
	up->bd_cbusy[i] |= SENT;
	up->bd_cbusy[i] &=~ NABBED;
	i = bdaregs(up->bd_addr)->bdip;		/* initiate polling */
}

/*
 * interrupt routine
 */

long bd_spur;
int bdspl;

bd0int(dev)
int dev;
{
	register struct bd *up;
	register struct device *rp;

	bdspl = mfpr(0x12);
	up = &bd[dev];
	if (dev >= bdcnt || (up->bd_flags & UINIT) == 0) {
		printf("bd%d: stray intr\n", dev);
		return;
	}
	rp = bdaregs(up->bd_addr);
	if (rp->bdsar & ERR) {
		printf("bd%d: hard error %o\n", dev, rp->bdsar & 0177777);
		bdreset(dev);
		return;
	}
	if (rp->bdsar & STEPS) {
		bdinintr(dev);
		return;
	}
	if (up->bd_comm->bd_cmdint == 0
	&&  up->bd_comm->bd_rspint == 0)
		bd_spur++;
	while (up->bd_comm->bd_cmdint) {
		up->bd_comm->bd_cmdint = 0;
		bdcmdscan(dev);
	}
	while (up->bd_comm->bd_rspint) {
		up->bd_comm->bd_rspint = 0;
		if (bdpkscan(dev, 0))
			up->bd_flags &=~ UTIMER;
	}
}

bd1int(dev)
int dev;
{
	printf("bd%d: error intr\n", dev);
}

/*
 * free packets which are completely sent
 * (and which don't have associated map)
 */

bdcmdscan(dev)
int dev;
{
	register struct bd *up;
	register int i, j;
	register int freed;
	register struct bdcomm *bdc;

	up = &bd[dev];
	bdc = up->bd_comm;
	freed = 0;
	for (j = 0; j < NCMD; j++)
		if (up->bd_back[j] >= 0
		&&  (bdc->bd_cmd[j] & DPOWN) == 0) {
			i = up->bd_back[j];
			if ((up->bd_cbusy[i] & (SENT|MAPPED)) == SENT) {
				up->bd_cbusy[i] = FREE;
				freed++;
			}
			up->bd_back[j] = NOBACK;
		}
	if (freed && up->bd_flags & UPWAIT)
		wakeup((caddr_t)up->bd_cbusy);
	return (freed);
}

/*
 * check for responses from the controller
 * and deal with them
 * return a count for debugging purposes
 */

int
bdpkscan(dev, doall)
int dev;
int doall;
{
	register struct bd *up;
	register int i;
	int nf;
	register struct bdrsp *pk;
	register struct ctab *cp;
	register struct bdcomm *bdc;	/* speed */
	int s;

	up = &bd[dev];
	bdc = up->bd_comm;
	nf = 0;
	s = spl6();
	for (i = up->bd_rnext; ; i < NRSP-1 ? i++ : (i = 0)) {
		if (bdc->bd_rsp[i] & DPOWN) {
			up->bd_rnext = i;
			/* don't stop if doall? */
			break;
		}
		nf++;
		pk = &up->bd_rpkt[i];
		up->bd_credits += pk->ur_tc & MTNC;
		if (up->bd_flags & UCWAIT) {
			wakeup((caddr_t)&up->bd_credits);
			up->bd_flags &=~ UCWAIT;
		}
		if (pk->ur_cid > MSMAXID)
			printf("bd%d msg id %d\n", dev, pk->ur_cid);
		else {
			cp = &bdctab[pk->ur_cid];
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
		bdc->bd_rsp[i] |= DPOWN | DIE;
	}
	splx(s);
	return (nf);
}

/*
 * timeout routine
 * return any waiting packets
 * -- callout routines don't necessarily run at high priority.
 * hence the spl6, so bdpkscan won't be reentered
 */

int bd_kicked;

bdtimer(i)
int i;
{
	register struct bd *up;
	register int s;

	up = &bd[i];
	if ((up->bd_flags & UINIT) == 0)
		return;
	if ((up->bd_flags & UIDONE) == 0) {
		if ((bdaregs(up->bd_addr)->bdsar & STEPS) == STEP1)
			bdinintr(i);
	}
	else if ((up->bd_flags & UTIMER) == 0)
		up->bd_flags |= UTIMER;
	else {
		s = spl6();
		if (bdpkscan(i, 1) && up->bd_flags & UPWAIT) {
			wakeup((caddr_t)up->bd_cbusy);
			bd_kicked++;
		}
		splx(s);
		up->bd_flags &=~ UTIMER;
	}
	timeout(bdtimer, (caddr_t)i, TIMEOUT * HZ);
}
