/*
 * BVP (BI VAX port) definitions
 * used by the NI (DEBNA) driver,
 * and perhaps eventually by the BVP MSCP port
 */

typedef struct quadque {
	struct quadque *head, *tail;
} quadque;

/*
 * port queue block
 * one per port;
 * must be page-aligned
 */
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
	struct pte *p_gpt;	/* global page table phys addr */
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
	struct fqb f[NFREEQ+1];	/* +1 acct off-by-one mystery in controller */
	struct bdt b[NBDT];
};

/*
 * port registers
 */
struct bvpregs {
	long ctrl;
	long stat;
	long err;
	long data;
	/* on NI port only: */
	long pudr;
	unsigned char addr[6];	/* ethernet address */
};

#define	MSREGS	0xf0	/* offset to storage port registers */
#define	NIREGS	0x204	/* offset to NI port registers */

/*
 * control bits
 */
#define	PCOWN	0x80	/* port own -- here is a command */
#define	PCDS	8	/* shift data by eight bits */
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
 * BVP packet headers
 */

/*
 * datagram
 */
struct bvpdg {
	quadque q;	/* queue linkage */
	long bd_sw0;	/* for software */
	char bd_mbz;	/* must be zero */
	char bd_sts;	/* BVP status */
	char bd_opc;	/* BVP opcode */
	char bd_flag;	/* BVP flags */
	/* datagram header */
	short bd_len;	/* length, starting at next byte */
	char bd_dgsts;	/* datagram status */
	char bd_sw1;	/* software */
	long bd_sw2;	/* software */
	long bd_ptdb;	/* proto type block index */
};

/*
 * message
 */
struct bvpmsg {
	quadque q;	/* queue linkage */
	long bd_sw0;	/* for software */
	char bd_mbz;	/* must be zero */
	char bd_sts;	/* BVP status */
	char bd_opc;	/* BVP opcode */
	char bd_flag;	/* BVP flags */
	/* message header */
	short bm_len;	/* length, from next byte */
	char bm_opc;	/* message opcode */
	char bm_nists;	/* message/NI status */
	long bm_sw1;	/* software */
};

#define	BVPHSIZE 18	/* sizeof(whole packet) == bd_len + BVPHSIZE */

/*
 * BVP opcodes
 */
#define	BVPSNDDG	1	/* send datagram (or ack) */
#define	BVPSNDMSG	2	/* send message */
#define	BVPSNDDGI	3	/* send immediate datagram (or ack) */
#define	BVPRCVDG	33	/* received datagram */
#define	BVPRCVMSG	34	/* received message */
#define	BVPRCVDGI	35	/* received immediate datagram */

/*
 * BVP flags
 */
#define	BVPRSP	01		/* response required */

/*
 * NI message opcodes
 */
#define	NIWPARAM	3	/* write device parameters */
#define	NISTPTDB	7	/* set proto data block */
#define	NICLPTDB	8	/* clear proto data block */


/*
 * status codes
 */
#define	BVPSUC	0	/* BVP success */
#define	NISUC	0	/* NI success */

/*
 * data for STPTDB
 */
struct stptdb {
	short pt_proto;		/* protocol number */
	char pt_fqi;		/* free queue index */
	char pt_flag;		/* flags */
	long pt_id;		/* id number for this ptdb */
	short pt_mcalen;
	short pt_mcamax;
	char pt_mcalist[8];
};

/*
 * pt_flag
 */
#define	PTABM	0x20	/* accept broadcast messages */
#define	PTAUP	0x40	/* accept unknown protocols */
#define	PTAAM	0x80	/* accept all multicasts */

/*
 * software data structures
 */
struct bvp {
	struct bvpdata *d;
	struct bvpregs *r;
	struct biic *rb;
	struct buf *dbuf;
	struct bvpregs rsave;	/* for debugging */
};
