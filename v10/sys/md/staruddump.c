/*
 * code to take a crash dump
 * on a disk attached to a UDA50 on a 780
 * your choice of a couple of unibus adapters,
 * a couple of common uda addresses
 * (wretched nonstandard alice)
 */

#include "sys/param.h"
#include "sys/mscp.h"

/*
 * hardware parameters
 */

#define	PHYS(x)	((long)(x)&~KSTART)

/*
 * `unit' number is really unit, controller, unibus adapter
 * one hopes that controller and adapter will usually be 0
 */
#define	UUNIT(d)	((d)&0xff)
#define	UCTL(d)		(((d)>>8)&0xf)
#define	UUBA(d)		(((d)>>12)&0xf)

#define	MAXUBA	2
static struct {
	long reg;			/* addr 0 in unibus space */
	long adp;			/* adapter registers */
} udxuba[MAXUBA] = {
	{0x20100000, 0x20006000},	/* first unibus */
	{0x20140000, 0x20008000},	/* second unibus */
};
#define	UBMAP	(0x800/4)		/* offset to map regs */
#define	CNFGR	0
#define	UACR	1
#define	ADINIT	01			/* UACR - adapter init */
#define	UBIC	0x10000			/* CNFGR - init complete */

#define	MAXUDA	2
static long udxaddr[MAXUDA] = {
	0772150, 0772160
};

#define	MRV	0x80000000		/* map register valid bit */

#define	CHUNK	8			/* write this many sectors at once */

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

#define	GO	01		/* step4 ok */

/*
 * bits in ring pointers
 */

#define	DPOWN	0x80000000	/* port owns descriptor */

struct udcmd {
	short	uc_len;		/* length of message */
	char	uc_tc;		/* type, credits */
	char	uc_cid;		/* connection id */
	struct mscmd uc_p;
};

struct udrsp {
	short	ur_len;		/* length of message */
	char	ur_tc;		/* type, credits */
	char	ur_cid;		/* connection id */
	struct msend ur_p;
};

static struct udx {
	short	ud__r0;		/* reserved (ugh) */
	char	ud__r1;
	char	ud_bdp;		/* path to purge */
	short	ud_cmdint;	/* flag for command interrupt */
	short	ud_rspint;	/* flag for response interrupt */
	long	ud_rsp;		/* response pointer ring */
	long	ud_cmd;		/* command pointer ring */
	struct udcmd ud_cp;	/* the only command packet */
	struct udrsp ud_rp;	/* the only response packet */
} udx;
static long udxbase;
static udxinit(), udxsend();

/*
 * struct udx (above) is mapped into pages 0 and 1 of unibus space
 * the memory we want to transfer goes into page 2 and above
 */

uddump(unit, low, size)
int unit;
daddr_t low, size;
{
	register struct udx *up;
	register long p;
	register int i;
	register long *ubmap;
	register struct device *udaddr;
	extern int physmem;

	if (size > physmem)
		size = physmem;
	if ((i = UUBA(unit)) >= MAXUBA)
		return (1);
	ubmap = (long *)udxuba[i].adp;
	ubmap[UACR] = ADINIT;
	while ((ubmap[CNFGR] & UBIC) == 0)
		;
	ubmap += UBMAP;
	if (UCTL(unit) >= MAXUDA)
		return (1);
	udaddr = (struct device *)(udxuba[i].reg + udxaddr[UCTL(unit)]);
	unit = UUNIT(unit);
	if (udxinit(udaddr, ubmap))
		return (1);
	up = (struct udx *)PHYS(&udx);
	up->ud_cp.uc_p.m_unit = unit;
	up->ud_cp.uc_p.m_opcd = OPONL;
	if (udxsend(udaddr))
		return (1);
	up->ud_cp.uc_p.m_opcd = OPWR;
	up->ud_cp.uc_p.m_bcnt = NBPG*CHUNK;
	*(long *)&up->ud_cp.uc_p.m_buff = (2*NBPG);
	for (p = 0; p < size; p += CHUNK) {
		for (i = 0; i < CHUNK; i++)
			ubmap[i+2] = MRV|(p+i);
		up->ud_cp.uc_p.m_lbn = low + p;
		if (udxsend(udaddr))
			return (1);
	}
	return (0);
}

static
udxinit(rp, map)
register struct device *rp;
register long *map;
{
	register struct udx *up;

	up = (struct udx *)PHYS(&udx);
	map[0] = ((long)up/NBPG)|MRV;
	map[1] = (((long)up/NBPG)+1)|MRV;
	udxbase = (long)up & ~PGOFSET;
	up->ud_cmd = 0;
	up->ud_rsp = 0;
	rp->udip = 0;		/* reset */
	while ((rp->udsa & (ERR|STEP1)) == 0)
		;
	if (rp->udsa & ERR)
		goto bad;
	rp->udsa = ERR;		/* no vector, no IE, ring size 1, 1 */
	while ((rp->udsa & (ERR|STEP2)) == 0)
		;
	if (rp->udsa & ERR)
		goto bad;
	rp->udsa = ((long)&up->ud_rsp - udxbase);	/* low order part */
	while ((rp->udsa & (ERR|STEP3)) == 0)
		;
	if (rp->udsa & ERR)
		goto bad;
	rp->udsa = 0;		/* high order part */
	while ((rp->udsa & (ERR|STEP4)) == 0)
		;
	if (rp->udsa & ERR)
		goto bad;
	rp->udsa = GO;
	return (0);
bad:
	printf("sa %o\n", rp->udsa & 0xffff);
	return (1);
}

static
udxsend(rp)
struct device *rp;
{
	register struct udx *up;
	register int x;
	static int ref;

	up = (struct udx *)PHYS(&udx);
	up->ud_cp.uc_p.m_crf = ++ref;
	up->ud_cp.uc_len = sizeof(struct mscmd);
	up->ud_rp.ur_len = sizeof(struct msend);
	up->ud_rsp = ((long)&up->ud_rp.ur_p - udxbase)|DPOWN;
	up->ud_cmd = ((long)&up->ud_cp.uc_p - udxbase)|DPOWN;
	x = rp->udip;
	for (;;) {
		while (up->ud_rsp & DPOWN)
			;
		if (up->ud_rp.ur_p.m_crf == ref)
			break;
		up->ud_rsp |= DPOWN;
	}
	if ((up->ud_rp.ur_p.m_sts & STMSK) != STSUC) {
		printf("ud err %o opc %o\n", up->ud_rp.ur_p.m_sts,
			up->ud_cp.uc_p.m_opcd);
		return (1);
	}
	return (0);
}
