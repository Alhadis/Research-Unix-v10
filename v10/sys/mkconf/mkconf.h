/*
 * global definitions for config
 */

#define	NULL	0
#define	NONUM	(-1)
#define	STDERR	2


/*
 * an entry from the master devices file
 */

#define	NSTR	10

typedef struct mdev {
	struct mdev *next;
	char *name;
	char *tag;
	char *strs[NSTR];
	char fstrs[NSTR];
	short rept;	/* repeat count; e.g. 8 dz11s per controller */
	char atype;	/* type of address */
	char nvec;	/* unibus only; number of vector entries */
	char adptype;	/* if i'm an adapter, which kind */
	char flags;
} Mdev;

extern Mdev *mlist;

/*
 * flags
 */
#define	FTBDONE	01	/* already marked as used in all tables */
#define	FDUPOK	02	/* duplicate devices supercede */
#define	FRAWVEC	04	/* raw vector, whatever that may mean */

/*
 * address type
 * these are indices into acode; see main.c
 */

#define	ANONE	0	/* no address */
#define	ACNT	1	/* unaddressed device, but print the count */
#define	ASBI	2	/* sbi-style nexus */
#define	AMBA	3	/* dev on massbus */
#define	AUBA	4	/* dev on unibus */
#define	ASUB	5	/* subdevice */
#define	AVBI	6	/* VAXBI nexus */
#define	APARAM	7	/* unaddressed; print count without `cnt' */
#define	AMSCP	8	/* MSCP device */
#define	ANOBUS	9	/* device adapter attached to no bus */

struct acode {
	int (*syntax)();	/* check Dev when conf file is read */
	int (*fixup)();		/* fix entry before printing address */
	int (*putaddr)();	/* put address stuff */
	int (*putvec)();	/* put vector interrupt routine */
} acode[];

/*
 * an entry in the conf file
 */

#define	NADDR	4

typedef struct dev {
	struct dev *next;
	struct dev *parent;	/* bus adapter */
	Mdev *mdev;
	int id;		/* 0 in `dz11 0' */
	int vecoff;	/* vector offset if bus adapter */
	long addr[NADDR];	/* used in mysterious ways */
} Dev;

extern Dev *dlist;

/*
 * an entry in the scb/trap vector list
 */

typedef struct scb {
	struct scb *next;
	int offset;
	int type;
	char *name;
} Scb;

extern Scb *llist;

/*
 * scb entry types
 */

#define	STRAP	0	/* trap onto kernel stack */
#define	SINTR	1	/* trap onto interrupt stack */

/*
 * device tables
 */

typedef struct devtab {
	Mdev *dev;
	int used;
} Devtab;

extern Devtab bdevsw[], cdevsw[], fstypsw[], ldispsw[], mscpsw[];;
extern int nblk, nchr, nfs, nld, nmscp;

typedef struct include {
	struct include *next;
	Mdev *dev;
	char *incl;
	char inuse;
} Include;

extern Include *inclist;

typedef struct swapdev {
	Mdev *mp;
	short major, minor;
	long size;
} Swapdev;

extern Swapdev swaptab[];
extern int nswapdev;
extern Mdev *rootfmp, *rootdmp;
extern int rootfs, rootmaj, rootmin;
extern int swapmaj, swapmin;
extern char *dumprout;
extern int dumpunit;
extern long dumplow, dumpsize;

char *ealloc();
char *estrdup();
Dev *advtype();
Mdev *mdlook();
long nconv();
char *keyread();

extern int errs;
