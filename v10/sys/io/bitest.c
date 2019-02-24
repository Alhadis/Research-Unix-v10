#include "sys/param.h"
#include "sys/biaddr.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/pte.h"

extern caddr_t bitest[];
extern struct biaddr bitstaddr[];
extern int bitstcnt;

static bitstopen();
static bitstread(), bitstwrite();
struct cdevsw bitstcdev = cdinit(bitstopen, nulldev, bitstread, bitstwrite, nodev);

char biscratch[4096];

/*
 * BVP data structures
 */

typedef struct quadque {
	struct quadque *head, *tail;
} quadque;

struct pqb {
	quadque p_cmdq[4];	/* command queues */
	quadque p_rspq;		/* response queue */
	short p_vector;		/* interrupt vector, BR */
	short p_nodmsk;		/* interrupt node mask */
	long p_nfreeq;		/* number of free queues */
	struct fqb *p_fqb;	/* fqp virtual address */
	char pp_junk0[156];
	long p_bvplvl;		/* BVP protocol version */
	struct pqb *p_pqb;	/* pqb virtual self-pointer */
	struct bdt *p_bdt;	/* bdt virtual address */
	long p_bdtlen;		/* and size */
	struct pte *p_spt;	/* system page table phys addr */
	long p_sptlen;		/* and size */
	struct pte *p_gpt;	/* global page pable phys addr */
	long p_gptlen;		/* and size */
	long p_funcmask;	/* ?? */
	char pp_junk1[24];
	/* stuff filled in by port after init */
	short p_maxdg;		/* max datagram size */
	short p_maxmsg;		/* max message size */
	long p_ucodetype;
	long p_ucodevers;
	long p_hwtype;
	long p_vers[3];
	char p_qelogo[216];	/* `queue entry logout area' */
};

/*
 * free queue block
 */
struct fqb {
	long f_size;		/* max size of things in this queue */
	long f_junk;		/* reserved, apparently */
	quadque f_q;		/* the free queue itself */
};

#define	NFREEQ	1

/*
 * buffer descriptor
 */
struct bdt {
	short b_flags;
	short b_key;
	long b_len;		/* length of buffer */
	struct pte *b_pte;	/* relevant page table */
	long b_sw;		/* reserved for software */
};

#define	NBDT	1

/*
 * bundle of data for one port
 * the pqb must be page-aligned;
 * stick this in a buffer,
 * which means it must all fit in one
 */
struct bvpdata {
	struct pqb p;
	struct fqb f[NFREEQ];
	struct bdt b[NBDT];
};

/*
 * port registers
 */
struct regs {
	long ctrl;
	long stat;
	long err;
	long data;
};

/*
 * control bits
 */
#define	PCOWN	0x80	/* port own -- here is a command */
#define	PCINIT	1
#define	PCENAB	2
#define	PCCMDQ	6	/* command queue not empty */
#define	PCFREQ	7	/* free queue not empty */

/*
 * status bits
 */
#define	PSOWN	0x80000000	/* port own -- status valid */
#define	PSSTD	0x20000000	/* self test done */
#define	PSACC	0x10000000	/* adapter can communicate */
#define	PSSTAT	0x70000		/* port state: */
#define	SUNDEF	0x10000		/* undefined */
#define	SINIT	0x20000		/* initialized */
#define	SENAB	0x40000		/* enabled */
#define	PSERR	0x40		/* error summary */

/*
 * to a header later
 */
struct bvp {
	struct bvpdata *d;
	struct regs *r;
	struct buf *dbuf;
};

char *bitsnd, *bitrcv;		/* places to put packets */

struct bvp bvp;

static
bitstopen(dev, flag)
int dev, flag;
{

	if (minor(dev)) {
		u.u_error = ENXIO;
		return;
	}
	if (bitest[0] == 0) {	/* wrong */
		if ((bitest[0] = biaddr(&bitstaddr[0])) == 0
		||   badaddr(bitest[0], sizeof(long))) {
			printf("bitest absent\n");
			u.u_error = ENXIO;
			return;
		}
		bvp.r = (struct regs *)(bitest[0] + 0x204);	/* cheat */
		bitsnd = biscratch+7;
		bitrcv = bitsnd + 2048;
		bitsnd -= (long)bitsnd & 07;	/* quad align */
		bitrcv -= (long)bitrcv & 07;
	}
	if (bvpinit(&bvp) == 0) {
		u.u_error = ENXIO;
		return;
	}
}

/*
 * read routine:
 * nab a packet
 */
static
bitstread(dev)
{
	register struct bvpdata *b;

	b = bvp.d;
	b->f[0].f_size = 2048;
	if (insqti(bitrcv, &b->p.p_fqb[0].f_q))
		bvpcomm(bvp.r, PCOWN|PCFREQ);
	/* let someone else wait for now */
}

/*
 * write routine:
 * send a packet
 */
static
bitstwrite(dev)
{
	register struct bvpdata *b;

	b = bvp.d;
	if (insqti(bitsnd, &b->p.p_cmdq[0]))
		bvpcomm(bvp.r, PCOWN|PCCMDQ);
}

bvpinit(bv)
register struct bvp *bv;
{
	register struct bvpdata *b;
	register struct regs *r;

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
	if ((r->stat & PSSTAT) == SUNDEF) {
		bvpdatinit(bv->d);
		bvpcomm(r, physadr(&bv->d->p)|PCOWN|PCINIT);
		bvpstat(r);
	}
	if ((r->stat & PSSTAT) == SINIT) {
		bvpcomm(r, PCOWN|PCENAB);
		bvpstat(r);
	}
	if ((r->stat & PSSTAT) != SENAB) {
		printf("ps %x pe %x pd %x\n", r->stat, r->err, r->data);
		return (0);
	}
	return (1);
}

bvpdatinit(b)
register struct bvpdata *b;
{
	/*
	 * just enough to make hardware happy
	 */
	b->p.p_nfreeq = NFREEQ;
	b->p.p_fqb = b->f;
	b->p.p_bvplvl = 1;
	b->p.p_pqb = &b->p;
	b->p.p_bdt = b->b;
	b->p.p_bdtlen = NBDT;
	b->p.p_spt = (struct pte *)physadr(Sysmap);
	b->p.p_sptlen = 0x100000;	/* huge */
	b->p.p_gpt = b->p.p_spt;
	b->p.p_gptlen = b->p.p_sptlen;
}

/*
 * send a port command
 * need a timeout
 */
bvpcomm(r, c)
register struct regs *r;
long c;
{
	while (r->ctrl & PCOWN)
		;
	r->stat &=~ PSOWN;
	r->ctrl = c;
}

/*
 * wait for status in init
 * needs timeout
 */

long
bvpstat(r)
register struct regs *r;
{
	while ((r->stat & PSOWN) == 0)
		;
	return (r->stat);
}

/*
 * VAX queue primitives
 */

/*
 * make an empty queue
 */
initvq(q)
register quadque *q;
{
	q->head = q->tail = 0;
}

/*
 * remove entry from head;
 * return entry, or 0 if none
 */
caddr_t
remqhi(q)
quadque *q;
{
	asm("clrl r0");
	asm("0: remqhi *4(ap),r0");
	asm("bcs 0b");		/* couldn't interlock; try again */
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
