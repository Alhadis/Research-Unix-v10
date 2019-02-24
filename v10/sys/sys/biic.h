/*
 * registers in the VAXBI interface chip
 * used by all VAXBI nodes
 */

struct biic {
	long bitype;	/* device type */
	long bicsr;
	long biber;
	long bieir;	/* error interrupt -- used by biic */
	long biintr;	/* mask of interrupt destination bits */
	long biipmsk;
	long bifipdes;
	long biipsrc;
	long bisadr;	/* some starting address */
	long bieadr;	/* some ending address */
	long bibci;	/* bci csr */
	long biwstat;
	long bifipscmd;
	long bi__pad1[3];
	long biuir;	/* user device interrupt -- sometimes used */
	long bi__pad2[43];
	long bigpr0;
	long bigpr1;
	long bigpr2;
	long bigpr3;
};
/* 0x100 bytes long */

/*
 * some bits
 * only put them here if needed, please
 */

/*
 * bicsr
 */

#define	BIHES	0x8000		/* hard error summary */
#define	BISES	0x4000		/* soft error summary */
#define	BIBROKE	0x1000		/* device is broken */
#define	BISTS	0x800		/* BIIC passed self-test */
#define	BINRST	0x400		/* node, reset thyself */
#define	BIHEIE	0x80		/* hard error interrupt enable */
#define	BISEIE	0x40		/* soft error interrupt enable */
#define	BIHIARB	0x10		/* fixed-high priority for this node */
#define	BINODEID 0xf		/* number of this node in this BI */

/*
 * biber
 */

#define	BIBERCLR 0x3fff0007	/* clear all the error bits */

/*
 * bieir
 */

#define	EIFORCE	0x100000	/* force interrupt if set */
#define	EIBR7	0x80000		/* error interrupts at BR7 */
#define	EIBR6	0x40000
#define	EIBR5	0x20000
#define	EIBR4	0x10000

/*
 * bibci
 */

#define	BCINTREN 0x40		/* enable received interrupts */
#define	BCRTOEN	0x2		/* enable retry event */

/*
 * biuir
 */

#define	UIEXVEC	0x8000		/* external vector */
