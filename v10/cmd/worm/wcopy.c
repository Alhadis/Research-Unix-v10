#include	<libc.h>
#include	<fio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<signal.h>
#include	"worm.h"

long minfree = 40000;
int verbose = 0;

main(argc, argv)
	char **argv;
{
	Superblock in, out;
	char *e;
	char buf[4096];
	int n;
	long lineno;
	int c;
	char *dev = "/dev/worm0";
	long tfiles, tbytes;
	int eof;
	char first[4096];
	extern char *optarg;
	extern int optind;

	argout = argv[0];
	while((c = getopt(argc, argv, "vm:f:")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case 'm':	minfree = atol(optarg); break;
		case 'v':	verbose = 1; break;
		case '?':	usage();
		}

	if(optind+3 != argc)
		usage();
	e = mapdev(argv[optind+1]);
	if((out.fd = open(e, 2)) < 0){
		perror(e);
		exit(1);
	}
	if(e = openinode(&out, SPIN_DOWN)){
		fprint(2, "%s: %s\n", *argv, e);
		exit(1);
	}
	if(strcmp(out.vol_id, argv[optind+2])){
		fprint(2, "vol_id mismatch: wanted %s, got %s\n", argv[optind+2], out.vol_id);
		exit(1);
	}
	if(out.version != VLINK){
		fprint(2, "%s: can't write on a b-tree disk\n", out.vol_id);
		exit(1);
	}
	dev = mapdev(dev);
	if((in.fd = open(dev, 2)) < 0){
		perror(*argv);
		exit(1);
	}
	if(e = openinode(&in, DO_INODE|SPIN_DOWN)){
		fprint(2, "%s: %s\n", *argv, e);
		exit(1);
	}
	if(strcmp(in.vol_id, argv[optind])){
		fprint(2, "vol_id mismatch: wanted %s, got %s\n", argv[optind], in.vol_id);
		exit(1);
	}
	for(n = 1; n <= NSIG; n++)
		signal(n, SIG_IGN);
	eof = 0;
	tfiles = tbytes = 0;
	lineno = 0;
	while(!eof){
		/* flush seperater lines */
		while(e = Frdline(0)){
			lineno++;
			if(*e)
				break;
		}
		if(e == 0)
			break;
		ininit();
		proc(&out, e);
		strncpy(first, e, sizeof first);
		if(out.nfree < minfree){
			fprint(2, "wcopy: disk %s full before copying group '%s' line %d\n",
				out.vol_id, first, lineno);
			exit(1);
		}
		while(e = Frdline(0)){
			lineno++;
			if(*e == 0)
				break;
			proc(&out, e);
		}
		if(e == 0)
			eof = 1;
		if(bad)
			exit(1);
		nfiles = nbytes = 0;
		blkdone = 0;
		inwrite(&out, &in);
		if(bad)
			exit(1);
		if(verbose)
			fprint(1, "%s group('%s' %d files, %.6fMB) done\n",
				timenow(), first, nfiles, nbytes/1e6);
		tfiles += nfiles;
		tbytes += nbytes;
	}
	if(verbose)
		fprint(1, "%s total: %d files, %.6fMB\n", timenow(), tfiles, tbytes/1e6);
	exit(0);
}

usage()
{
	fprint(2, "Usage: worm copy [-v] [-m minfree] [-f src_dev] src_id dest_dev dest_id < files\n");
	exit(1);
}

proc(s, file)
	Superblock *s;
	char *file;
{
	struct stat sbuf;
	unsigned short mode;
	Inode i;
	Inode *srci;

	if((srci = inodeof(file)) == 0){
		fprint(2, "can't find file '%s'\n", file);
		return;
	}
	memset((char *)&i, 0, sizeof(i));
	i = *srci;
	i.block = 0;
	nbytes += i.nbytes;
	if(inadd(s, &i))
		bad = 1;
}

writeout(dest, i, blk, src)
	Superblock *dest, *src;
	Inode *i;
	long *blk;
{
	char b[BIGBLOCK];
	Inode *srci;
	long n, len, blen;
	char *name;

	n = (i->nbytes+dest->blocksize-1)/dest->blocksize;
	*blk += n;
	blkdone += n;
	blen = sizeof b/dest->blocksize;
	len = blen*dest->blocksize;
	nbytes += i->nbytes;
	nfiles++;
	name = i->name.n;
	srci = inodeof(name);
	Seek(src, srci->block);
	for(n = i->nbytes; n > len; n -= len){
		if(Read(src, b, blen)){
	out:
			fprint(2, "read problem: seek=%d n=%d blen=%d len=%d; ",
				srci->block, n, blen, len);
			perror(name);
			bad = 1;
			return;
		}
		if(Write(dest, b, blen)){
fprint(2, "nb=%d, n=%d len=%d blen=%d\n", i->nbytes, n, len, blen);
			perror("data write");
			exit(1);
		}
	}
	if(n){
		n += dest->blocksize-1;
		n /= dest->blocksize;
		if(Read(src, b, n))
			goto out;
		if(Write(dest, b, n)){
			perror("data write");
			exit(1);
		}
	}
}
