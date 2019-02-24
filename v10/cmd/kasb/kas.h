/* @(#)kas.h	1.2 */
#define	NINST	100
#define	NSYM	500
#define	NHASH	(NSYM+NINST)
#define	NCPS	8
#define	NKMCI	4096
#define	NKMCD	4096

/*
 * Symbol types
 */
#define	XUNDEF	0x0
#define	XABS	0x1000
#define	XTEXT	0x2000
#define	XDATA	0x3000
#define	XREG	0x4000
#define	XTAG	0x8000
#define	XTYPE	0x7000

#define	DSTEP	1
#define	DSTOP	4
#define	DRESET	5
#define	DRUN	6
#define	DWCSR	8
#define	DCLEAR	9
#define	DREGS	10
#define	DLOAD	11
#define	DDUMP	12

#define	TBRG	01
#define	TMAR	06

#define	RLCMV	0
#define	RLCPOP	020000
#define	RLCPG	040000
#define	RLCBR	060000
#define	RLCMASK	070000
#define	RLUSE	0100000
#define	RLBUP	077777
#define	RLNEXT	07777

#define	ERR	(-1)

#define	SRCREGL	(1<<13)
#define	SRCMEM	(2<<13)
#define	SRCBRG	(3<<13)
#define	SRCREGH	(5<<13)
#define	SRCPCL	(10)
#define	SRCPCH	(11)
#define	SRCMARL	(12)
#define	SRCMARH	(13)

#define	SELA	(8<<4)
#define	SELB	(9<<4)

#define	DSTBRG	(1<<8)
#define	DSTREGH	(2<<8)
#define	DSTBGRS	(3<<8)
#define	DSTREGL	(4<<8)
#define	DSTMEM	(5<<8)
#define	DSTREG	(6<<8)
#define	DSTBREG	(7<<8)
#define	DSTMARP	(1<<11)
#define	DSTMAR	(2<<11)
#define	DSTMARI	(3<<11)
#define	DSTPCH	(11)

struct	symtab {
	char	name[NCPS];
	int	type;
	int	value;
};

struct	exp {
	int	xtype;
	int	xvalue;
};

extern struct	symtab	symtab[NSYM];
extern struct	symtab	*hshtab[NHASH];
extern struct	symtab	loclab[10];
extern struct	symtab	*dot;
extern	lineno;
extern	tsize;
extern	dsize;
extern	anyerrs;
extern	textsv, datasv;
extern char	dbmode;
extern char	*outfile, *dbfile, *infile;
extern	dbfd;
extern	dbtab[3];
extern char	dbstr[50], instr[50];
extern struct	symtab	instab[];
extern short	ispace[NKMCI], reloc[NKMCI];
extern char	dspace[NKMCD];
struct	symtab	**lookup();

struct ir {
	char	csr[8];
	char	lur[8];
	char	reg[16];
	char	io[8];
	char	npr[2];
	char	brg;
	char	mem;
	char	fill[4];
};
extern struct ir ir;
