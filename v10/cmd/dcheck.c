/*
 * dcheck - check directory consistency
 */
#define	NI	16
#define	NB	10

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
int	bigdev;

int	fi;
long	ino;
char	*ecount;
int	headpr;
long nfiles;
int	bigflag;

int	nerror;
daddr_t	bmap();
long	atol();
char	*malloc();

main(argc, argv)
char *argv[];
{
	register i;
	long n;

	setbuf(stdout, NULL);
	while (--argc) {
		argv++;
		if (**argv=='-')
		switch ((*argv)[1]) {

		case 'i':
			for(i=0; i<NB; i++) {
				n = atol(argv[1]);
				if(n == 0)
					break;
				ilist[i] = n;
				argv++;
				argc--;
			}
			ilist[i] = 0;
			continue;

		case 'B':
			bigflag = BITFSBIT;
			continue;

		default:
			fprintf(stderr, "Bad flag %c\n", (*argv)[1]);
			nerror++;
		}
		check(*argv);
	}
	return(nerror);
}

check(file)
char *file;
{
	register i;
	register j;
	register nchunk;

	fi = open(file, 0);
	if(fi < 0) {
		perror(file);
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
	headpr = 0;
	printf("%s:\n", file);
	sync();
	bread((daddr_t)1, (char *)&sblock, sizeof(sblock));
	nfiles = (sblock.s_isize-2)*INOPB(dev);
	nchunk = INOPB(dev)*NI;
	ecount = malloc(nfiles+1);
	if (ecount==NULL) {
		fprintf(stderr, "Not enough core\n");
		exit(04);
	}
	for (i=0; i<=nfiles; i++)
		ecount[i] = 0;
	ino = 0;
	for(i=2;; i+=NI) {
		if(ino >= nfiles)
			break;
		bread((daddr_t)i, (char *)itab, BSIZE(dev)*NI);
		for(j=0; j<nchunk; j++) {
			if(ino >= nfiles)
				break;
			ino++;
			pass1(&itab[j]);
		}
	}
	ino = 0;
	for(i=2;; i+=NI) {
		if(ino >= nfiles)
			break;
		bread((daddr_t)i, (char *)itab, BSIZE(dev)*NI);
		for(j=0; j<nchunk; j++) {
			if(ino >= nfiles)
				break;
			ino++;
			pass2(&itab[j]);
		}
	}
	free(ecount);
}

pass1(ip)
register struct dinode *ip;
{
	struct direct dbuf[BIGNDIR];
	off_t doff;
	struct direct *dp;
	register i, j;
	int k;
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
			dp = &dbuf[j];
			kno = dp->d_ino;
			if(kno == 0)
				continue;
			if (kno > nfiles) {
				printf("%5u bad; %u/%.14s\n", kno, ino, dp->d_name);
				nerror++;
				continue;
			}
			for (k=0; ilist[k] != 0; k++)
				if (ilist[k]==kno) {
					printf("%5u arg; %u/%.14s\n", kno, ino, dp->d_name);
					nerror++;
				}
			ecount[kno]++;
			if (ecount[kno] == 0)
				ecount[kno] = 0377;
		}
	}
}

pass2(ip)
register struct dinode *ip;
{
	register i;

	i = ino;
	if ((ip->di_mode&IFMT)==0 && ecount[i]==0)
		return;
	if (ip->di_nlink==((ecount[i])&0377) && ip->di_nlink!=0)
		return;
	if (ino <= 0 && ip->di_nlink==0 && ecount[i]==0)
		return;
	if (headpr==0) {
		printf("     entries  link cnt\n");
		headpr++;
	}
	printf("%u	%d	%d\n", ino,
	    ecount[i]&0377, ip->di_nlink);
}

bread(bno, buf, cnt)
daddr_t bno;
char *buf;
{
	register i;

	lseek(fi, bno*BSIZE(dev), 0);
	if (read(fi, buf, cnt) != cnt) {
		fprintf(stderr, "read error %ld\n", bno);
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
		fprintf(stderr, "%u - huge directory\n", ino);
		return((daddr_t)0);
	}
	bread(iaddr[NADDR-3], (char *)ibuf, sizeof(daddr_t)*NINDIR(dev));
	return(ibuf[i]);
}
