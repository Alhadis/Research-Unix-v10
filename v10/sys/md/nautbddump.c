/*
 * code to take a crash dump
 * on a disk attached to a KDB50 on some VAXBI machine
 * assume buses are, in general, already initialized
 */

#include "sys/param.h"
#include "sys/mscp.h"
#include "sys/biic.h"

/*
 * pieces of unit number
 */

#define	UNIT(d)	((d)&0xff)
#define	NODE(d)	(((d)>>8)&0xf)
#define	BI(d)	(((d)>>12)&0xf)

/*
 * hardware parameters
 */

#define	PHYS(x)	((long)(x)&~KSTART)

struct device {
	short	bdxx;	/* unused */
	short	bdip;
	short	bdsar;
	short	bdsaw;
};

/*
 * bits in bdsa
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

#define	BDACSIZE 50		/* size of a command packet, says KDB50 */

struct bdcmd {
	short	uc_len;		/* length of message */
	char	uc_tc;		/* type, credits */
	char	uc_cid;		/* connection id */
	struct mscmd uc_p;
	char junk[BDACSIZE-sizeof(struct mscmd)];
};

struct bdrsp {
	short	ur_len;		/* length of message */
	char	ur_tc;		/* type, credits */
	char	ur_cid;		/* connection id */
	struct msend ur_p;
};

static struct bdx {
	short	ud__r0;		/* reserved (ugh) */
	char	ud__r1;
	char	ud_bdp;		/* path to purge */
	short	ud_cmdint;	/* flag for command interrupt */
	short	ud_rspint;	/* flag for response interrupt */
	long	ud_rsp;		/* response pointer ring */
	long	ud_cmd;		/* command pointer ring */
	struct bdcmd ud_cp;	/* the only command packet */
	struct bdrsp ud_rp;	/* the only response packet */
} bdx;

#define	CHUNK	20	/* number of blocks to write at once */

static struct biic *bdxaddr();
static bdxinit(), bdxsend();

bddump(unit, low, size)
int unit;
daddr_t low, size;
{
	register struct bdx *up;
	extern int physmem;
	struct biic *addr;

	if (size > physmem)
		size = physmem;
	addr = bdxaddr(BI(unit), NODE(unit));
	if (bdxinit(addr))
		return (1);
	unit = UNIT(unit);
	up = (struct bdx *)PHYS(&bdx);
	up->ud_cp.uc_p.m_unit = unit;
	up->ud_cp.uc_p.m_opcd = OPONL;
	if (bdxsend(addr))
		return (1);
printf("onl ok\n");
	up->ud_cp.uc_p.m_opcd = OPWR;
	up->ud_cp.uc_p.m_bcnt = size * NBPG;
	*(long *)&up->ud_cp.uc_p.m_buff = 0;
	up->ud_cp.uc_p.m_lbn = low;
	return (bdxsend(addr));
}

static struct biic *
bdxaddr(bi, node)
{
	return ((struct biic *)(0x20000000 + (bi * 0x2000000) + (node * 0x2000)));
}

static
bdxinit(bp)
register struct biic *bp;
{
	register struct bdx *up;
	register struct device *rp;

	up = (struct bdx *)PHYS(&bdx);
	up->ud_cmd = 0;
	up->ud_rsp = 0;
	bp->biintr = 0;		/* no interrupts */
	bp->bicsr |= BINRST;
	rp = (struct device *)&bp->bigpr0;
	while ((rp->bdsar & (ERR|STEP1)) == 0)
		;
	if (rp->bdsar & ERR)
		return (1);
	rp->bdsaw = ERR;		/* no vector, no IE, ring size 1, 1 */
	while ((rp->bdsar & (ERR|STEP2)) == 0)
		;
	if (rp->bdsar & ERR)
		return (1);
	rp->bdsaw = (short)&up->ud_rsp;		/* low order part */
	while ((rp->bdsar & (ERR|STEP3)) == 0)
		;
	if (rp->bdsar & ERR)
		return (1);
	rp->bdsaw = ((long)&up->ud_rsp)>>16;	/* high order part */
	while ((rp->bdsar & (ERR|STEP4)) == 0)
		;
	if (rp->bdsar & ERR)
		return (1);
	rp->bdsaw = GO;
	return (0);
}

static
bdxsend(bp)
struct biic *bp;
{
	register struct bdx *up;
	register int x;
	struct device *rp;
	static int ref;

	rp = (struct device *)&bp->bigpr0;
	up = (struct bdx *)PHYS(&bdx);
	up->ud_cp.uc_p.m_crf = ++ref;
	up->ud_cp.uc_len = BDACSIZE;
	up->ud_rp.ur_len = sizeof(struct msend);
	up->ud_rsp = (long)&up->ud_rp.ur_p|DPOWN;
	up->ud_cmd = (long)&up->ud_cp.uc_p|DPOWN;
	x = rp->bdip;
	for (;;) {
		while (up->ud_rsp & DPOWN)
			;
		if (up->ud_rp.ur_p.m_crf == ref)
			break;
		up->ud_rsp |= DPOWN;
	}
	if ((up->ud_rp.ur_p.m_sts & STMSK) != STSUC) {
		printf("bd err %o opc %o\n", up->ud_rp.ur_p.m_sts,
			up->ud_cp.uc_p.m_opcd);
		return (1);
	}
	return (0);
}
