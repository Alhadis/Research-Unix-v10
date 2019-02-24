
#include <nlist.h>
#include <sys/param.h>
#include <ctype.h>
#include <sys/stream.h>
#include <sys/inode.h>
#include <sys/conf.h>
#include <sys/dkp.h>
#include <sys/dz.h>

struct nlist nl[] ={
#define	QNL	0
	{"_queue"},
#define	BLNL	1
	{"_cblock"},
#define	STNL	2
	{"_streams"},
#define	QFNL	3
	{"_qfreelist"},
#define DKPNL	4
	{ "_dkpstream" },
#define	NBLOCK	5
	{ "_blkcnt"},
#define	NQUEUE	6
	{ "_queuecnt"},
#define	NSTREAM	7
	{ "_streamcnt"},
#define	DZNL	8
	{"_dzinfo"},
#define CDKPNL	9
	{ "_cdkpstream" },
#define	BLKALL	10
	{ "_blkall" },
#define	BLKMAX	11
	{ "_blkmax" },
#define	BLKFREE	12
	{ "_blkfree" },
#define	STWNL	13
	{ "_stwdata" },
#define	BLKOWN	14
	{ "_blkowner" },
#define	STRDATA	15
	{ "_strdata" },
#define	STREAMSTART	16
	{ "_bufldstream" },
	{ "_rdkstream" },
	{ "_connstream" },
	{ "_connginfo" },
	{ "_kcinfo" },
	{ "_rcureinfo" },
	{ "_qesinfo" },
	{ "_dhvinfo" },
	{ "_dkinfo" },
	{ "_dkpstream" },
	{ "_cdkpstream" },
	{ "_dzinfo" },
	{ "_kbinfo" },
	{ "_kdiinfo" },
	{ "_msgstream" },
	{ "_rmsgstream" },
	{ "_mginfo" },
	{ "_ilsinfo" },
	{ "_nttystream" },
	{ "_spinfo" },
	{ "_ttystream" },
	{ "_xpstream" },
	{ "_ipdinfo" },
	{ "_ipstream" },
	{ "_tcpdinfo" },
	{ "_tcpstream" },
	{ "_udpdinfo" },
	{ "_udpstream" },
	{ "_nilinfo" },
	{ "_cnstream" },
	{ "" },
};

struct	infoinfo {
	char	*name;
	struct	streamtab inf;
} inf[50];

struct	qinit **qall;
char	*ball;
char	*strall;
char	*core = "/dev/mem";
char	*sys  = "/unix";
int	mem;
int	blkcnt;
int	queuecnt;
int	streamcnt;
short	blkall[6];
short	blkmax[6];
short	blkfree[6];

#define	DK_OPEN	01
#define	DK_LCLOSE	02
#define	DK_RCLOSE	04
#define	DK_XCHARMODE	010
#define	DK_RCHARMODE	0200
#define	DK_OPENING	020

#define	getme(s, kind, off, type)  _get(kind, off, sizeof(type), &s)

struct flags {
	int	flag;
	char	*name;
};

struct flags stflags[] = {
	IOCWAIT,	"IOC",
	RSLEEP,		"RSLEEP",
	WSLEEP,		"WSLEEP",
	HUNGUP,		"HUNGUP",
	RSEL,		"RSEL",
	WSEL,		"WSEL",
	EXCL,		"EXCL",
	STWOPEN,	"WOPEN",
	0
};

struct flags qflags[] = {
	QUSE,		"use",
	QREADR,		"read",
	QNOENB,		"noenb",
	QENAB,		"enab",
	QWANTR,		"wantr",
	QWANTW,		"wantw",
	QFULL,		"full",
	QDELIM,		"delim",
	QBIGB,		"bigb",
	0
};

struct	flags dkpflags[] = {
	DK_OPEN,	"open",
	DK_LCLOSE,	"lclose",
	DK_RCLOSE,	"rclose",
	DK_XCHARMODE,	"xcharmode",
	DK_RCHARMODE,	"rcharmode",
	DK_OPENING,	"opening",
	0
};

#define	DZEXIST	01
#define	DZISOPEN 02
#define	DZWOPEN	04
#define	DZTIME	010
#define	DZCARR	020
#define	DZSTOP	040
#define	DZHUPCL	0100

struct flags dzflags[] = {
	DZEXIST,	"exists",
	DZISOPEN,	"open",
	DZWOPEN,	"wopen",
	DZTIME,		"time",
	DZCARR,		"carrier",
	DZSTOP,		"stop",
	DZHUPCL,	"hupcl",
	0
};

int	vflag;
int	sflag;
int	mflag;
char	*infstr();

main(argc, argv)
char **argv;
{
	struct queue q;
	struct block b;
	struct stdata s;
	int i;
	int nq = 0;

	for (;;argc--, argv++) {
		if (argc>1 && strcmp(argv[1], "-v")==0) {
			vflag++;
			continue;
		}
		if (argc>1 && strcmp(argv[1], "-m")==0) {
			mflag++;
			continue;
		}
		if (argc>1 && strcmp(argv[1], "-V")==0) {
			vflag = 2;
			continue;
		}
		if (argc>1 && strcmp(argv[1], "-s")==0) {
			sflag++;
			continue;
		}
		break;
	}
	if (argc>1)
		sys = argv[1];
	if (argc>2)
		core = argv[2];
	nlist(sys, nl);
	mem = open(core, 0);
	if (mem<0) {
		printf("can't open %s\n", core);
		exit(1);
	}
	getme(queuecnt, NQUEUE, 0, int);
	getme(streamcnt, NSTREAM, 0, int);
	getme(blkcnt, NBLOCK, 0, int);
	getme(blkmax[0], BLKMAX, 0, blkmax);
	getme(blkall[0], BLKALL, 0, blkall);
	getme(blkfree[0], BLKFREE, 0, blkfree);
	printf("blocks max: (%d,%d,%d,%d,%d,%d)\n",
		blkmax[0], blkmax[1], blkmax[2], blkmax[3], blkmax[4], blkmax[5]);
	printf("blocks cur: (%d,%d,%d,%d,%d,%d)\n",
		blkall[0], blkall[1], blkall[2], blkall[3], blkall[4], blkall[5]);
	printf("blocks free: (%d,%d,%d,%d,%d,%d)\n",
		blkfree[0], blkfree[1], blkfree[2], blkfree[3], blkfree[4], blkfree[5]);
	initinfo();
	ball = (char *)malloc(blkcnt*sizeof(char));
	strall = (char *)malloc(streamcnt*sizeof(char));
	qall = (struct qinit **)malloc(queuecnt*sizeof(struct queue *));
	for (i=0; i<blkcnt; i++)
		ball[i] = 0;
	for (i=0; i<streamcnt; i++)
		strall[i] = 0;
	for (i=0; i<queuecnt; i++) {
		getme(q, QNL, i, struct queue);
		if (q.flag==0)
			qall[i] = 0;
		else {
			qall[i] = q.qinfo;
			nq++;
		}
	}
	for (i=0; i<streamcnt; i++)
		pstream(i);
	for (i=0; i<queuecnt; i++) {
		if (qall[i])
			printf("Loose queue %d, info #%x\n", i, qall[i]);
	}
	printf("%d queues in use\n", nq);
	bfreecount();
	nq = 0;
	for (i=0; i<blkcnt; i++)
		if (ball[i]==0) {
			struct block b;
			long owner;

			if (mflag) {
				getme(b, BLNL, i, struct block);
#if BLKOWNER
				getme(owner, BLKOWN, i, long);
				printf("   owner=##%x\n", owner);
#endif
				pblock(&b);
			}
			nq++;
		}
	if (!sflag)
		printf("%d blocks missing\n", nq);
	nq = 0;
	for (i=0; i<blkcnt; i++) {
		if (ball[i])
			continue;
		if (nq >= 5 && vflag < 2)
			continue;
		printf("#%x\n", (struct block *)nl[BLNL].n_value+i);
		nq++;
	}
	exit(0);
}

pstream(i)
{
	struct stdata s;
	struct queue rq, wq;
	struct inode ino;
	struct queue *rqp, *wqp;
	struct stdata *sp;
	static struct streamtab dkpinfo, dzinfo, cdkpinfo;
	static int stread = 0;
	int d;
	int forward = 1;

	if (strall[i])
		return;
	strall[i]++;
	sp = (struct stdata *)getme(s, STNL, i, struct stdata);
	if (s.wrq==NULL)
		return;
	if (!sflag)
		printf("\nstream %d: ", i);
	getme(ino, s.inode, 0, struct inode);
	d = ino.i_un.i_rdev;
	if (!sflag) {
		printf("dev %d,%d  ", major(d), minor(d));
		printf("ino %d  ", ino.i_number);
		printf("count %d ", s.count);
		flags(stflags, s.flag);
		if (vflag)
			printf(" pgrp %d", s.pgrp);
		printf("\n");
	}
	wqp = (struct queue *)getme(wq, s.wrq, 0, struct queue);
	rqp = (struct queue *)getme(rq, RD(s.wrq), 0, struct queue);
	if ((struct stdata *)rq.ptr != sp)
		printf("rq ptr? = #%x\n", rq.ptr);
	if (stread == 0) {
		getme(dkpinfo, DKPNL, 0, struct streamtab);
		getme(cdkpinfo, CDKPNL, 0, struct streamtab);
		getme(dzinfo, DZNL, 0, struct streamtab);
		stread++;
	}
	for (;;) {
		d = rqp - (struct queue *)nl[QNL].n_value;
		if (d<0 || d>=queuecnt)
			printf("?");
		else
			qall[d] = 0;
		d = wqp - (struct queue *)nl[QNL].n_value;
		if (d<0 || d>=queuecnt)
			printf("?");
		else
			qall[d] = 0;
		if (!sflag) {
			printf("   RQ #%x: ", rqp);
			printf("%s; ", infstr(rq.qinfo));
			flags(qflags, rq.flag);
			printf(". ptr #%x", rq.ptr);
			printf("\n");
			pblocks(&rq);
			printf("   WQ #%x:   ", wqp);
			flags(qflags, wq.flag);
			printf(". ptr #%x", wq.ptr);
			printf("\n");
			pblocks(&wq);
			if ((long)wq.qinfo == nl[STWNL].n_value)
				strfacts(&wq);
			if (wq.qinfo == dkpinfo.wrinit || wq.qinfo == cdkpinfo.wrinit)
				dkpline(&wq);
			if (wq.qinfo == dzinfo.wrinit)
				dzfacts(&wq);
		}
		if (forward) {
			if (wq.next == NULL)
				break;
			wqp = (struct queue *)
			     getme(wq, wq.next, 0, struct queue);
			if (wq.flag&QREADR) {
				printf("   <>\n");
				forward = 0;
				rqp = wqp;
				rq = wq;
				wqp = (struct queue *)
				     getme(wq, WR(rqp), 0, struct queue);
			} else
				rqp = (struct queue *)
				     getme(rq, RD(wqp), 0, struct queue);
		} else {
			if (rq.next == NULL)
				break;
			rqp = (struct queue *)
			     getme(rq, rq.next, 0, struct queue);
			if ((rq.flag&QREADR) == 0) {
				printf("   <>\n");
				forward = 1;
				wqp = rqp;
				wq = rq;
				rqp = (struct queue *)
				     getme(rq, RD(wqp), 0, struct queue);
			} else
				wqp = (struct queue *)
				     getme(wq, WR(rqp), 0, struct queue);
		}
	}
}

strfacts(q)
register struct queue *q;
{
	register int i;
	struct stdata s;
	dev_t d;
	struct inode ino;

	i = (struct stdata *)q->ptr - (struct stdata *)nl[STNL].n_value;
	if (i < 0 || i > streamcnt)
		return;
	if (strall[i])
		return;
	strall[i]++;
	getme(s, STNL, i, struct stdata);
	if (s.wrq==NULL)
		return;
	if (!sflag)
		printf("stream %d: ", i);
	getme(ino, s.inode, 0, struct inode);
	d = ino.i_un.i_rdev;
	if (!sflag) {
		printf("dev %d,%d  ", major(d), minor(d));
		printf("ino %d  ", ino.i_number);
		printf("count %d ", s.count);
		flags(stflags, s.flag);
		if (vflag)
			printf(" pgrp %d", s.pgrp);
		printf("\n");
	}
}

dzfacts(q)
register struct queue *q;
{
	struct dz dz;

	getme(dz, q->ptr, 0, struct dz);
	printf("     DZ: ");
	flags(dzflags, dz.state);
	printf("\n");
}

dkpline(q)
register struct queue *q;
{
	struct dkp dkp;
	register i;

	getme(dkp, q->ptr, 0, struct dkp);
	printf("	WS %o WACK %o WNX %o iseq %o; ", dkp.WS, dkp.WACK,
	    dkp.WNX, dkp.iseq);
	printf("window %d x %d; ", dkp.XW, dkp.xsize);
	flags(dkpflags, dkp.state);
	printf("\n	");
	for (i=0; i<8; i++)
		printf(" #%x", dkp.xb[i]);
	printf("\n");
}

pblocks(q)
register struct queue *q;
{
	struct block b, *bp;
	int nb = 0;
	u_char buf[512];
	register i, bno;

	bp = q->first;
	for (i = 0; i < 1000; i++) {
		if (bp==NULL)
			break;
		bno = bp - (struct block *)nl[BLNL].n_value;
		nb++;
		baccount(bno);
		getme(b, bp, 0, struct block);
		if (vflag && (i<10 || vflag>1))
			pblock(&b);
		bp = b.next;
	}
	if (vflag && i>=10)
		printf("(gave up) ");
	if (nb > 0)
		printf("       %d blocks\n", nb);
}

pblock(bp)
register struct block *bp;
{
	register u_char *cp;
	register i = 0;
	u_char buf[4096];

	printf("       %c%.3o:", bp->class&0200?'D':' ', bp->type);
	cp = buf;
	getme(buf[0], bp->base, 0, buf);
	printf("%3d:", bp->wptr-bp->base);
	while (bp->base < bp->wptr) {
		if (i >= 16) {
			printf("\n	        ");
			i = 0;
		}
		if (*cp<040 || *cp>=0177)
			printf(" %.3o", *cp);
		else
			printf("  %c ", *cp);
		cp++;
		bp->base++;
		i++;
	}
	printf("\n");
}

flags(fp, w)
register struct flags *fp;
{
	int any = 0;
	while (fp->name) {
		if (fp->flag&w) {
			if (any)
				printf(", ");
			any++;
			printf("%s", fp->name);
		}
		fp++;
	}
}

_get(addr, off, size, loc)
unsigned long addr;
char *loc;
{

	if (addr < 20)
		addr = nl[addr].n_value;
	addr += off*size;
	lseek(mem, addr&0x7fffffff, 0);
	read(mem, loc, size);
	return(addr);
}

bfreecount()
{
	struct block b;
	struct block *bp;
	register i;

	for (i=0; i<6; i++) {
		getme(bp, QFNL, i, struct block *);
		while (bp) {
			baccount(bp - (struct block *)nl[BLNL].n_value);
			getme(b, bp, 0, struct block);
			bp = b.next;
		}
	}
}

baccount(bno)
{
	if (bno < 0 || bno >= blkcnt)
		printf("funny block %d\n", bno);
	else if (ball[bno])
		printf("dup block %d\n", bno);
	else
		ball[bno]++;
}

initinfo()
{
	register i, j;

	inf[0].name = "head";
	inf[0].inf.rdinit = (struct qinit *)nl[STRDATA].n_value;
	for (j=1, i=STREAMSTART; nl[i].n_name[0]; i++) {
		if (nl[i].n_type!=0) {
			getme(inf[j].inf, nl[i].n_value, 0, struct streamtab);
			inf[j].name = nl[i].n_name;
			if (inf[j].name[0]=='_')
				inf[j].name++;
			j++;
		}
	}
}

char *
infstr(s)
long s;
{
	register i;
	static char buf[64];

	for (i=0; inf[i].name; i++) {
		if (inf[i].inf.rdinit == (struct qinit *)s
		 || inf[i].inf.wrinit == (struct qinit *)s) {
			sprintf(buf, "%s", inf[i].name);
			break;
		}
	}
	if (inf[i].name == NULL)
		sprintf(buf, "qinfo #%x", s);
	return(buf);
}
