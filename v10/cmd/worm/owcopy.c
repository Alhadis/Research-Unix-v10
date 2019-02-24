#include	<libc.h>
#include	"worm.h"
#include	"sym.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<pwd.h>
#include	<grp.h>

char *timenow();
long minfree = 40000;
int verbose = 0;
static char *copy();

main(argc, argv)
	char **argv;
{
	char *e;
	char *dev = "/dev/worm0";
	char *dest = 0;
	Superblock in, out;
	long start = 1;
	long count = 1000000L;
	int c;
	extern char *optarg;
	extern int optind;
	void pr();

	while((c = getopt(argc, argv, "vc:m:s:f:")) != -1)
		switch(c)
		{
		case 'c':	count = atol(optarg); break;
		case 'f':	dev = optarg; break;
		case 'm':	minfree = atol(optarg); break;
		case 's':	start = atol(optarg); break;
		case 'v':	verbose = 1; break;
		case '?':	usage();
		}
	dev = mapdev(dev);
	if((in.fd = open(dev, 0)) < 0){
		perror(dev);
		exit(1);
	}
	if(optind+3 != argc)
		usage();

	if(e = openinode(&in, SPIN_DOWN)){
		fprint(2, "worm copy: %s: %s\n", dev, e);
		exit(1);
	}
	if(strcmp(argv[optind], in.vol_id)){
		fprint(2, "src vol_id mismatch: expected %s, got %s\n",
			argv[optind], in.vol_id);
		exit(1);
	}
	dest = mapdev(argv[optind+1]);
	if((out.fd = open(dest, 2)) < 0){
		perror(dest);
		exit(1);
	}
	if(e = openinode(&out, SPIN_DOWN)){
		fprint(2, "worm copy: %s: %s\n", dest, e);
		exit(1);
	}
	if(strcmp(argv[optind+2], out.vol_id)){
		fprint(2, "destination vol_id mismatch: expected %s, got %s\n",
			argv[optind+2], out.vol_id);
		exit(1);
	}
	if(e = copy(&in, start, count, &out)){
		fprint(2, "worm copy: %s: %s\n", dest, e);
		exit(1);
	}
	exit(0);
}

usage()
{
	fprint(2, "Usage: worm copy [-v] [-fdevice] [-s startblk] [-c count] [-m minfree] srcvolid destdev destvolid\n");
	exit(2);
}

static char *
copy(in, blk, count, out)
	register Superblock *in, *out;
	long blk, count;
{
	register Inode *i;
	short fd = in->fd;
	char *b, *err;
	long nb, ndata;
	char *nameb;
	Inode *inodes = 0;
	Inode *iend;
	long delta;
	static char buf[64];
	extern char *lkwri();

	in->blocksize = 1024;
	if((b = malloc(in->blocksize)) == 0){
		sprint(buf, "couldn't malloc buffer (%d bytes)", in->blocksize);
		return(buf);
	}
	Seek(in, blk);
	if(Read(in, b, 1)){
		fprint(2, "worm copy: unexpected read error\n");
		exit(1);
	}
	*in = *((Superblock *)b);
	in->fd = fd;
	while(count-- > 0){
		if(in->magic != SMAGIC){
			fprint(2, "bad Superblock at %ld\n", blk);
			exit(1);
		}
		if(in->ninodes){
			if(verbose)
				fprint(2, "%s reading chunk %s:%ld (%.1f%%)\n",
					timenow(), in->vol_id, in->myblock,
					in->myblock*100./(float)in->nblocks);
			if(out->nfree < minfree){
				fprint(2, "worm copy: disk %s has < %d free at input %d\n",
					out->vol_id, minfree, in->myblock);
				exit(1);
			}
			nb = (in->ninodes+(in->blocksize/sizeof(Inode))-1)/(in->blocksize/sizeof(Inode));
			inodes = (Inode *)malloc((unsigned)(in->blocksize*nb));
			if(inodes == 0){
				sprint(buf, "inode malloc(%d) fail, sbrk=%d\n",
					(in->blocksize*nb), sbrk(0));
				return(buf);
			}
			Seek(in, in->binodes);
			if(Read(in, (char *)inodes, nb))
				goto skip;
			delta = out->nextffree - inodes->block;
print("%d inodes; delta=%d\n", in->ninodes, delta);
			for(i = inodes, iend = inodes+in->ninodes; i < iend; i++)
				if(i->block > 0)
					i->block += delta;
			nb = (in->ninochars+in->blocksize-1)/in->blocksize;
			nameb = malloc((unsigned)(in->blocksize*nb));
			if(nameb == 0){
				sprint(buf, "name buffer malloc(%d) fail, sbrk=%d\n",
					(in->blocksize*nb), sbrk(0));
				return(buf);
			}
			if(Read(in, nameb, nb))
				goto skip;
			ndata = in->binodes - in->myblock - 1;
			if(err = lkwri(out, inodes, in->ninodes, nameb, in->ninochars, ndata)){
				fprint(2, "wcopy: lkwri: %s at input %d\n", err, in->myblock);
				exit(1);
			}
			dcopy(in, out, ndata);
		}
	skip:
		blk = in->nextsb;
		Seek(in, blk);
		if(Read(in, b, 1L))
			break;
		*in = *((Superblock *)b);
		in->fd = fd;
		if(in->myblock == 0)
			in->myblock = blk;
	}
	free(b);
	if(verbose)
		fprint(2, "%s copy done, %s is %.1f%% full\n", timenow(), out->vol_id,
			out->myblock*100.0/(float)out->nblocks);
	return((char *)0);
}

dcopy(in, out, n)
	Superblock *in, *out;
	long n;
{
	char buf[BIGBLOCK];
	long bs;

	bs = BIGBLOCK/in->blocksize;
	Seek(in, in->myblock+1);
	Seek(out, out->myblock+1);
	while(n > 0){
		if(bs > n) bs = n;
		if(Read(in, buf, bs)){
			fprint(2, "wcopy: warning: data read error at %d\n", in->myblock);
		}
		if(Write(out, buf, bs)){
			fprint(2, "wcopy: dcopy write error\n");
			exit(1);
		}
		n -= bs;
	}
}

char *
timenow()
{
	long tim;
	char *tims;

	time(&tim);
	tims = ctime(&tim);
	tims[19] = 0;
	return(tims);
}
