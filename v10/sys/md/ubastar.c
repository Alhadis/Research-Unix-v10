/*
 * unibus adapter routines for DW780
 */

#include "sys/param.h"
#include "sys/ubaddr.h"
#include "sys/nexus.h"
#include "sys/nxaddr.h"
#include "sys/map.h"
#include "sys/uba.h"
#include "sys/pte.h"
#include "sys/vmparam.h"

extern struct uba uba[];
extern long ubavoff[];
extern long *ubavreg[];
extern long ubazvec[];
extern struct nxaddr ubaaddr[];
extern int ubacnt;

/*
 * 0x2000 bytes of nexus space, followed by the address window
 */

#define	NMAP	496
#define	NBDP	15

struct ubadev {
	long cnfgr;		/* configuration register */
	long uacr;		/* adapter control */
	long uasr;		/* adapter status */
	long dcr;		/* diag stuff */
	long fmer;		/* failed map entry */
	long fubar;		/* failed unibus addr */
	long junk0[2];
	long bufsel[4];
	long brrvr[4];		/* vectors for each BR */
	long dpr[16];		/* data path registers */
	long junk1[0x1e0];
	long mreg[512];		/* unibus map registers; last 16 unused */
	long junk2[0x400];
	char ubspace[NXWSIZE];
};

/*
 * bits in uacr
 */
#define	ADINIT	0x1		/* adapter init */
#define	CNFIE	0x4		/* config IE */
#define	SUEFIE	0x8		/* SBI to UNIBUS error IE */
#define	USEFIE	0x10		/* UNIBUS to SBI error IE */
#define	BRIE	0x20		/* BR interrupt enable */
#define	IFS	0x40		/* pass UNIBUS interrupts to SBI */

#define	UBAIE	(IFS|BRIE|USEFIE|SUEFIE|CNFIE)

/*
 * bits in cnfgr
 */
#define	PARFLT	0x80000000	/* SBI Parity Fault */
#define	WSQFLT	0x40000000	/* SBI Write Sequence Fault */
#define	URDFLT	0x20000000	/* SBI Unexpected Read Fault */
#define	ISQFLT	0x10000000	/* SBI Interlock Sequence Fault */
#define	MXTFLT	0x8000000	/* SBI Multiple Transmitter Fault */
#define	XMTFLT	0x4000000	/* UBA is transmit faulter */
#define	UBIC	0x10000		/* init complete */

#define	CFGFLT	(PARFLT|WSQFLT|URDFLT|ISQFLT|MXTFLT|XMTFLT)

/*
 * bits in uasr
 */
#define	DPPE	0x20		/* Data Path Parity Error */
#define	IVMR	0x10		/* Invalid Map Register */
#define	MRPF	0x8		/* Map Register Parity Failure */
#define	UBSSYNTO 0x1		/* unibus SSYN timeout */

#define	BADMAP	(MRPF|IVMR|DPPE)	/* bits indicating mapping reg problem */

/*
 * bits in the data path register
 */

#define	BNE	0x80000000	/* purge datapath */
#define	BTE	0x40000000	/* bdp transfer error */

/*
 * bits in map register
 */

#define	MPAGE	0x7ffff		/* physical page number */
#define	BO	0x2000000	/* offset address by one */
#define	MRV	0x80000000	/* map register valid */
#define	PSHIFT	21		/* shift data path number this much */

#define	INITTIME 50000		/* time to stall waiting for UNIBUS init */
				/* 25ms nominal; 50000 for VAX-11/785 */

/*
 * return the register address for a unibus device
 */

caddr_t
ubaddr(up)
register struct ubaddr *up;
{
	register struct uba *ub;

	if (up->ubno < 0 || up->ubno > ubacnt) {
		printf("bad ubano %d\n", up->ubno);
		return (0);
	}
	ub = &uba[up->ubno];
	if ((ub->flags & UBINIT) == 0)
		if (ubstart(up->ubno) == 0)
			return (0);
	return ((caddr_t)&ub->addr->ubspace[up->uboff]);
}

/*
 * init the unibus adapter
 */

ubstart(u)
int u;
{
	register struct uba *ub;

	ub = &uba[u];
	if ((ub->addr = (struct ubadev *)nxaddr(&ubaaddr[u])) == 0)
		return (0);
	if (badaddr(&ub->addr->cnfgr, sizeof(long))) {
		printf("ub%d not responding\n", u);
		return (0);
	}
	rminit(ub->map, UBNMAP, NMAP-1, 1);	/* NMAP-1 because can't alloc 0 */
	ub->path = ((1<<NBDP)-1)<<1;
	if (ubmstart(u) == 0)
		return (0);
	ubinit(u);
	ub->flags |= UBINIT;
	return (1);
}

/*
 * init the unibus adapter hardware
 */

ubinit(u)
int u;
{
	register struct uba *ub;
	register struct ubadev *up;
	register int i;
	register long *p;

	ub = &uba[u];
	up = ub->addr;
	up->uacr = ADINIT;
	for (i = 0; i < INITTIME; i++)
		if (up->cnfgr & UBIC)
			break;
	if ((up->cnfgr & UBIC) == 0) {
		printf("ub%d not ready\n", u);
		return (0);
	}
	ubavoff[u] = ubaaddr[u].voff;
	ubavreg[u] = &up->brrvr[0];
	up->cnfgr = up->cnfgr;		/* clear power-up latches */
	up->uacr = UBAIE;
	for (i = 0, p = ub->addr->mreg; i < NMAP; i++)
		*p++ = 0;
	ubminit(u);
	return (1);
}

/*
 * determine whether a particular address,
 * which happens to be in UNIBUS space,
 * exists
 */
ubbadaddr(u, a, s)
int u, s;
caddr_t a;
{
	register int p;
	register struct ubadev *up;

	if (u < 0 || u >= ubacnt)
		return (1);
	up = uba[u].addr;
	p = spl7();
	/* perhaps disable SBI-to-UNIBUS interrupts? */
	up->uasr = up->uasr;	/* clear errors */
	s = badaddr(a, s);
	if (up->uasr & UBSSYNTO)	/* or perhaps any error? */
		s = 1;
	up->uasr = up->uasr;	/* clear errors again */
	splx(p);
	return (s);
}

/*
 * get/put a single byte to a particular unibus address
 * intended for use by ECC code
 * work it out from first principles,
 * because UNIBUS adapter may loop and hang otherwise
 */

static long
ubphys(ubno, addr)
unsigned int ubno;
uaddr_t addr;
{
	register struct uba *ub;
	register int pg;
	register long m;

	if (ubno >= ubacnt)
		panic("ubphys");
	ub = &uba[ubno];
	if (ub->addr == NULL)
		panic("ubphys");
	pg = addr / NBPG;
	if (pg >= NMAP)
		return (-1);	/* addr too big */
	m = ub->addr->mreg[pg];
	if ((m & MRV) == 0)
		return (-1);	/* invalid */
	return ((m & MPAGE) * NBPG + addr % NBPG);
}

int
ubgetc(ubno, addr)
unsigned int ubno;
uaddr_t addr;
{
	register long phys;

	phys = ubphys(ubno, addr);
	if (phys < 0)
		return (-1);
	return (phgetc(phys));
}

int
ubputc(ubno, addr, c)
unsigned int ubno;
uaddr_t addr;
char c;
{
	register long phys;

	phys = ubphys(ubno, addr);
	if (phys < 0)
		return (-1);
	return (phputc(phys, c));
}

/*
 * unibus adapter interrupts
 * here only on non-device interrupts:
 * passive release, or adapter has something to say
 *
 * -- assertion:
 * to get here, we must read the BRRVR,
 * which means ubavreg is set up,
 * which means uba.addr is set up
 */

uba0int(u, vec)
int u;
{
	register struct ubadev *up;
	long cnfgr, uasr, fubar, fmer;

	if (vec == 0) {			/* passive release */
		ubazvec[u]++;
		return;
	}
	up = uba[u].addr;
	cnfgr = up->cnfgr;
	uasr = up->uasr;
	fubar = up->fubar<<2;
	fmer = up->fmer;
	up->uasr = uasr;
	up->cnfgr = cnfgr;
	if (cnfgr & CFGFLT) {	/* does this really happen? */
		printf("ub%d: SBI fault cnfgr%x sr%x\n", u, cnfgr, uasr);
		return;
	}
	printf("ub%d: cfg%x sr%x fubar %o\n", u, cnfgr, uasr, fubar);
	if (uasr & BADMAP)
		printf("fmer %x\n", fmer);
}

/*
 * allocate a buffered data path
 * return the ddp if none available
 */
int ubnopath;

int
ubmapath(u)
int u;
{
	register struct uba *ub;
	register int path;
	register int s;

	ub = &uba[u];
	s = spl6();
	for (path = NBDP; path > 0; path--)
		if (ub->path & (1<<path)) {
			ub->path &=~ (1<<path);
			break;
		}
	splx(s);
	if (path == 0)
		ubnopath++;
	return (path);
}

ubmflush(u, path)
int u;
int path;
{
	register long *reg;

	if (path == 0)
		return;
	reg = &uba[u].addr->dpr[path];
	*reg |= BNE;
	DELAY(2);
	if (*reg & BTE) {
		printf("ub%d bdp%d err %x\n", u, path, *reg);
		*reg = *reg;
	}
}

/*
 * fill in a piece of unibus map
 * return the address of the base
 */

uaddr_t
ubmsetmap(u, p, nreg, um)
int u;
register struct pte *p;
register int nreg;
ubm_t um;
{
	register long *m;
	register long x;

	m = &uba[u].addr->mreg[ubmfirst(um)];
	x = (ubmpath(um)<<PSHIFT)|MRV;
	if (nreg > ubmsize(um)-1)
		panic("ubmsetmap");
	while (--nreg >= 0)
		*m++ = p++->pg_pfnum | x;
	*m = 0;
	return (ctob(ubmfirst(um)));
}
