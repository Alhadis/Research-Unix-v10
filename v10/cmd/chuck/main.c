#include "fs.h"
#include "sys/ttyio.h"
#include "ctype.h"

char *options = "ivwI:B:b:M:";
int bsize = 4096;
main(argc, argv)
char **argv;
{	int c;
	extern char *optarg;
	extern int optind;

	timer(0);
	while((c = getopt(argc, argv, options)) != -1)
	switch(c) {
	default:
		flags[c]++;
		continue;
	case '?':
		fatal("unknown arg %c (%s)\n", c, options);
	case 'b':
		bsize = atoi(optarg);
		continue;
	case 'M':
		flags[c]++;
		pblk = atoi(optarg);
		continue;
	case 'I':
		for(optind--; optind < argc && isdigit(argv[optind][0]); optind++)
			addi(atoi(argv[optind]));
		continue;
	case 'B':
		for(optind--; optind < argc && isdigit(argv[optind][0]); optind++)
			addb(atoi(argv[optind]));
		continue;
	}
	if(optind >= argc)
		fatal("no file?\n");
	if(optind < argc-1)
		fatal("too many files %s\n", argv[optind]);
	if(flags['i'])
		flags['w']++;
	file = argv[optind];
	if(flags['M'])
		mknew();
	doit(file);
}

doit(file)
{
	fd = -1;
	if(flags['w']) {
		fd = open(file, 2);
		if(fd < 0) {
			pmesg("no write\n");
			flags['w'] = 0;
		}
	}
	if(fd < 0)
		fd = open(file, 0);
	if(fd < 0) {
		perror(file);
		fatal("can't open file system\n");
	}
	firstsuper();	/* read it, sanity check, allocate imap, bmap */
	timer("read");
	scaninodes();	/* read all the inode blocks */
	timer("inodes");
	if(blkcnts[Ind3])
		indblocks(Ind3, Ind2);
	if(blkcnts[Ind2])	/* cope with indirect blocks */
		indblocks(Ind2, Ind);
	if(blkcnts[Ind])
		indblocks(Ind, Data);
	timer("indir");
	dirblocks();	/* read all the directory blocks */
	timer("dirs");
	dochecks();	/* tree structure etc */
	checksuper();	/* is the super block ok? */
	repairs();
	report();	/* say what happened somewhere */
	timer("done");
	if (exitcode)
		pmesg("exit %d\n", exitcode);
	exit(exitcode);
}

pmesg(s, a, b, c, d, e, f, g, h, i)
char *s;
{	static char *p;
	if(!p)
		p = mbuf;
	if(p == mbuf && file) {
		sprintf(mbuf, "%s: ", file);
		p = mbuf + strlen(mbuf);
	}
	sprintf(p, s, a, b, c, d, e, f, g, h, i);
	write(2, mbuf, strlen(mbuf));
}

fatal(s, a, b, c, d, e, f, g, h, i)
char *s;
{
	pmesg(s, a, b, c, d, e, f, g, h, i);
	exit(1);
}

bread(loc, buf, cnt)
char *buf;
{	int i;
	i = bsize *cnt;
	if(lseek(fd, bsize*loc, 0) < 0 || read(fd, buf, i) != i)
		return(-1);
	return(0);
}

bwrite(loc, buf, cnt)
char *buf;
{	int i = bsize * cnt;
	if(lseek(fd, bsize*loc, 0) < 0 || write(fd, buf, i) != i)
		return(-1);
	return(0);
}

char *
itype(n)
{
	switch(n) {
	default:
		sprintf(xbuf, "?%d?", n);
		return(xbuf);
	case Unalloc:	return("Unalloc");
	case Dir:	return("Dir");
	case Lnk:	return("Lnk");
	case Chr:	return("Chr");
	case Blk:	return("Blk");
	case Reg:	return("Reg");
	case Weird:	return("Weird");
	}
}

char *
btype(n)
{
	switch(n) {
	default:
		sprintf(xbuf, "?%d?", n);
		return(xbuf);
	case Unk:	return("Unk");
	case Sblock:	return("Sblock");
	case Inode:	return("Inode");
	case Free:	return("Free");
	case Data:	return("Data");
	case Ind:	return("Ind");
	case Ind2:	return("Ind2");
	case Ind3:	return("Ind3");
	case First:	return("First");
	case Other:	return("Otherdir");
	case Bits:	return("Bits");
	case Ioerr:	return("Ioerr");
	case Boot:	return("Boot");
	case Bad:	return("Bad");
	}
}

addi(n)
{
	if(!ilen)
		iarg = (int *) malloc((ilen = 10) * sizeof(int));
	else if(iptr >= ilen) {
		ilen *= 3;
		iarg = (int *) realloc((char *)iarg, ilen * sizeof(int));
	}
	if(!iarg) {
		pmesg("can't alloc iarg %d\n", n);
		return;
	}
	iarg[iptr++] = n;
}

addb(n)
{
	if(!blen)
		barg = (int *) malloc((blen = 10) * sizeof(int));
	else if(bptr >= blen) {
		blen *= 3;
		barg = (int *) realloc((char *)barg, blen * sizeof(int));
	}
	if(!barg) {
		pmesg("can't alloc barg %d\n", n);
		return;
	}
	barg[bptr++] = n;
}

/* how do we flush stdin? */
qry(s)
char *s;
{	int n;
	pmesg(s);
	ioctl(0, TIOCFLUSH, 0);
	n = read(0, mbuf+40, 128-40);
	if(n <= 0)
		return(n);
	return(mbuf[40]);
}
#include "sys/times.h"
long ot, nt;
struct tms otm, ntm;
timer(s)
char *s;
{
	if(ot == nt && nt == 0 || !flags['v']) {
		(void) time(&ot);
		times(&otm);
		return;
	}
	(void) time(&nt);
	times(&ntm);
	pmesg("(time)%s %d %.3fu %.3fs\n", s, nt-ot,
		(ntm.tms_utime-otm.tms_utime)/60.,
		(ntm.tms_stime-otm.tms_stime)/60.);
	ot=nt;
	otm = ntm;
}
