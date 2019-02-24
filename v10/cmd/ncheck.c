static char *sccsid = "@(#)ncheck.c	4.1 (Berkeley) 10/1/80";
/*
 * ncheck -- obtain file names from reading filesystem
 */

#define	NI	4
#define	NB	1000
#define	HSIZE	8209

#include <stdio.h>
#include <sys/param.h>
#include <sys/inode.h>
#include <sys/ino.h>
#include <sys/dir.h>
#include <sys/filsys.h>
#include <sys/fblk.h>
#include <sys/stat.h>

#define	BITFSBIT	64	/* should be in param.h */
#define	BIGINOPB	INOPB(BITFSBIT)
#define	BIGBSIZE	BSIZE(BITFSBIT)
#define	BIGNINDIR	NINDIR(BITFSBIT)
#define	BIGNDIR	(BIGBSIZE/sizeof(struct direct))

#define	NDIR(dev)	(BSIZE(dev)/sizeof(struct direct))

struct	filsys	sblock;
struct	stat	status;
#define	dev	status.st_rdev
struct	dinode	itab[BIGINOPB*NI];
daddr_t	iaddr[NADDR];
ino_t	ilist[NB];
unsigned short	imodes[NB];
struct	htab
{
	ino_t	h_ino;
	ino_t	h_pino;
	char	h_name[DIRSIZ];
} htab[HSIZE];

int	aflg;
int	sflg;
int	fi;
long ino;
int	nhent;
int	nxfile;
int	bigflag;

int	nerror;
daddr_t	bmap();
long	atol();
struct htab *lookup();

main(argc, argv)
char *argv[];
{
	register i;
	long n;

	setbuf(stdout, NULL);
	nochk(1,0);	/* secure unix */
	nochk(2,0);
	while (--argc) {
		argv++;
		if (**argv=='-')
		switch ((*argv)[1]) {

		case 'a':
			aflg++;
			continue;

		case 'i':
			for(i=0; i<NB; i++) {
				n = atol(argv[1]);
				if(n == 0)
					break;
				ilist[i] = n;
				argv++;
				argc--;
			}
			nxfile = i;
			ilist[nxfile] = 0;
			continue;

		case 's':
			sflg++;
			continue;

		case 'B':
			bigflag = BITFSBIT;
			continue;

		default:
			fprintf(stderr, "ncheck: bad flag %c\n", (*argv)[1]);
			nerror++;
		}
		check(*argv);
		if(sflg) {
			nxfile = 0;
			ilist[nxfile] = 0;
		}
	}
	return(nerror);
}

check(file)
char *file;
{
	register i, j;
	long mino;
	register nchunk;

	fi = open(file, 0);
	if(fi < 0) {
		fprintf(stderr, "ncheck: cannot open %s\n", file);
		nerror++;
		return;
	}
	if (fstat(fi, &status) < 0) {
		fprintf(stderr, "ncheck: cannot stat %s\n", file);
		nerror++;
		close(fi);
		return;
	}
	if ((status.st_mode & S_IFMT) == S_IFREG)
		dev = makedev(0, bigflag);
	nhent = 0;
	printf("%s:\n", file);
	sync();
	bread((daddr_t)1, (char *)&sblock, BSIZE(dev));
	mino = (sblock.s_isize-2) * INOPB(dev);
	nchunk = INOPB(dev)*NI;
	ino = 0;
	for(i=2;; i+=NI) {
		if(ino >= mino)
			break;
		bread((daddr_t)i, (char *)itab, BSIZE(dev)*NI);
		for(j=0; j<nchunk; j++) {
			if(ino >= mino)
				break;
			ino++;
			pass1(&itab[j]);
		}
	}
	ilist[nxfile] = 0;
	ino = 0;
	for(i=2;; i+=NI) {
		if(ino >= mino)
			break;
		bread((daddr_t)i, (char *)itab, BSIZE(dev)*NI);
		for(j=0; j<nchunk; j++) {
			if(ino >= mino)
				break;
			ino++;
			pass2(&itab[j]);
		}
	}
	ino = 0;
	for(i=2;; i+=NI) {
		if(ino >= mino)
			break;
		bread((daddr_t)i, (char *)itab, BSIZE(dev)*NI);
		for(j=0; j<nchunk; j++) {
			if(ino >= mino)
				break;
			ino++;
			pass3(&itab[j]);
		}
	}
}

pass1(ip)
register struct dinode *ip;
{
	register struct htab *	hp;
	int filtyp = ip->di_mode & IFMT;

	if(filtyp != IFDIR) {
		if (sflg==0 || nxfile>=NB)
			return;
		if(filtyp==IFBLK || filtyp==IFCHR || 
		   ip->di_mode&(ISUID|ISGID) && !(ip->di_mode&ISYNC) 
#ifdef lb_t
		   || ip->di_label.lb_t	/* secure unix */
#endif
		  ) {
			ilist[nxfile] = ino;
			imodes[nxfile++] = ip->di_mode;
		}
		return;
	}
	lookup(ino, 1);
}

pass2(ip)
register struct dinode *ip;
{
	struct direct dbuf[BIGNDIR];
	long doff;
	struct direct *dp;
	register i, j;
	int k;
	struct htab *hp;
	daddr_t d;
	ino_t kno;

	if((ip->di_mode&IFMT) != IFDIR)
		return;
	l3tol(iaddr, ip->di_addr, NADDR);
	doff = 0;
	for(i=0;; i++) {
		if(doff >= ip->di_size)
			break;
		d = bmap(i);
		if(d == 0)
			break;
		bread(d, (char *)dbuf, BSIZE(dev));
		for(j=0; j<NDIR(dev); j++) {
			if(doff >= ip->di_size)
				break;
			doff += sizeof(struct direct);
			dp = dbuf+j;
			kno = dp->d_ino;
			if(kno == 0)
				continue;
			hp = lookup(kno, 0);
			if(hp == 0)
				continue;
			if(dotname(dp))
				continue;
			hp->h_pino = ino;
			for(k=0; k<DIRSIZ; k++)
				hp->h_name[k] = dp->d_name[k];
		}
	}
}

pass3(ip)
register struct dinode *ip;
{
	struct direct dbuf[BIGNDIR];
	long doff;
	register struct direct *dp;
	register i, j;
	int k;
	daddr_t d;
	ino_t kno;
	struct htab *	hp;

	if((ip->di_mode&IFMT) != IFDIR)
		return;
	l3tol(iaddr, ip->di_addr, NADDR);
	doff = 0;
	for(i=0;; i++) {
		if(doff >= ip->di_size)
			break;
		d = bmap(i);
		if(d == 0)
			break;
		bread(d, (char *)dbuf, BSIZE(dev));
		for(j=0; j<NDIR(dev); j++) {
			if(doff >= ip->di_size)
				break;
			doff += sizeof(struct direct);
			dp = dbuf+j;
			kno = dp->d_ino;
			if(kno == 0)
				continue;
			if(aflg==0 && dotname(dp))
				continue;
			if(ilist[0] || sflg) {
				for(k=0; ilist[k] != 0; k++)
					if(ilist[k] == kno)
						goto pr;
				continue;
			}
		pr:
			hp = lookup(kno, 0);
			printf("%u\t", kno);
			if (sflg && kno == ilist[k])
				printf("%o\t", imodes[k]);
			pname(ino, 0);
			printf("/%.14s", dp->d_name);
			if (hp)
				printf("/.");
			printf("\n");
		}
	}
}

dotname(dp)
register struct direct *dp;
{

	if (dp->d_name[0]=='.')
		if (dp->d_name[1]==0 || (dp->d_name[1]=='.' && dp->d_name[2]==0))
			return(1);
	return(0);
}

pname(i, lev)
ino_t i;
{
	register struct htab *hp;

	if (i==ROOTINO)
		return;
	if ((hp = lookup(i, 0)) == 0) {
		printf("???");
		return;
	}
	if (lev > 10) {
		printf("...");
		return;
	}
	pname(hp->h_pino, ++lev);
	printf("/%.14s", hp->h_name);
}

struct htab *
lookup(i, ef)
ino_t i;
{
	register struct htab *hp;

	for (hp = &htab[i%HSIZE]; hp->h_ino;) {
		if (hp->h_ino==i)
			return(hp);
		if (++hp >= &htab[HSIZE])
			hp = htab;
	}
	if (ef==0)
		return(0);
	if (++nhent >= HSIZE) {
		fprintf(stderr, "ncheck: out of core-- increase HSIZE\n");
		exit(1);
	}
	hp->h_ino = i;
	return(hp);
}

bread(bno, buf, cnt)
daddr_t bno;
char *buf;
{
	register i;

	lseek(fi, bno*BSIZE(dev), 0);
	if (read(fi, buf, cnt) != cnt) {
		fprintf(stderr, "ncheck: read error %d\n", bno);
		for(i=0; i<BSIZE(dev); i++)
			buf[i] = 0;
	}
}

daddr_t
bmap(i)
{
	daddr_t ibuf[BIGNINDIR];

	if(i < NADDR-3)
		return(iaddr[i]);
	i -= NADDR-3;
	if(i > NINDIR(dev)) {
		fprintf(stderr, "ncheck: %u - huge directory\n", ino);
		return((daddr_t)0);
	}
	bread(iaddr[NADDR-3], (char *)ibuf, BSIZE(dev));
	return(ibuf[i]);
}
