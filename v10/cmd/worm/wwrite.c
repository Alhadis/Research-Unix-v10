#include	<libc.h>
#include	<fio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<signal.h>
#include	"worm.h"

main(argc, argv)
	char **argv;
{
	Superblock s;
	char *e;
	char buf[4096];
	int n;
	int c;
	char *dev = "/dev/worm0";
	extern char *optarg;
	extern int optind;

	argout = argv[0];
	while((c = getopt(argc, argv, "f:")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case '?':	usage();
		}

	if(optind >= argc)
		usage();
	dev = mapdev(dev);
	if((s.fd = open(dev, 2)) < 0){
		perror(*argv);
		exit(1);
	}
	if(e = openinode(&s, SPIN_DOWN)){
		fprint(2, "%s: %s\n", *argv, e);
		exit(1);
	}
	if(strcmp(s.vol_id, argv[optind])){
		fprint(2, "vol_id mismatch: wanted %s, got %s\n", argv[optind], s.vol_id);
		exit(1);
	}
	if(s.nfree == 0){
		fprint(2, "%s: can't write any more!\n", dev);
		exit(1);
	}
	if(s.version != VLINK){
		fprint(2, "%s: can't write on a b-tree disk\n", s.vol_id);
		exit(1);
	}
	for(n = 1; n <= NSIG; n++)
		signal(n, SIG_IGN);
	ininit();
	if(++optind < argc)
		while(optind < argc)
			proc(&s, argv[optind++]);
	else
		while(e = Frdline(0))
			proc(&s, e);
	if(bad)
		exit(1);
	nfiles = nbytes = 0;
	inwrite(&s, (void *)0);
	if(bad)
		exit(1);
	fprint(1, "%d files, %.6fMb\n", nfiles, nbytes/1e6);
	exit(0);
}

usage()
{
	fprint(2, "Usage: worm write [-fdevice] vol_id [files]\n");
	exit(1);
}

proc(s, file)
	Superblock *s;
	char *file;
{
	struct stat sbuf;
	unsigned short mode;
	Inode i;

	memset((char *)&i, 0, sizeof(i));
	if(stat(file, &sbuf) < 0){
		perror(file);
		return;
	}
	mode = sbuf.st_mode&S_IFMT;
	if((mode == S_IFREG) || (mode == S_IFDIR)){
		i.magic = DMAGIC;
		i.block = 0;
		i.nbytes = sbuf.st_size;
		nbytes += i.nbytes;
		i.ctime = sbuf.st_ctime;
		i.name.n = file;
		i.mode = sbuf.st_mode;
		i.uid = sbuf.st_uid;
		i.gid = sbuf.st_gid;
		if(inadd(s, &i))
			bad = 1;
	} else
		fprint(2, "%s is not a file\n", file);
}

writeout(s, i, blk)
	Superblock *s;
	Inode *i;
	long *blk;
{
	char b[63*1024L];
	int fd;
	long n, len, blen;
	char *name;

	n = (i->nbytes+s->blocksize-1)/s->blocksize;
	*blk += n;
	blkdone += n;
	blen = sizeof b/s->blocksize;
	len = blen*s->blocksize;
	nbytes += i->nbytes;
	nfiles++;
	name = i->name.n;
	if((fd = open(name, 0)) < 0)
		goto out;
	for(n = i->nbytes; n > len; n -= len){
		if(read(fd, (char *)b, (int)len) != len){
	out:
			perror(name);
			bad = 1;
			return;
		}
		if(Write(s, b, blen)){
fprint(2, "nb=%d, n=%d len=%d blen=%d\n", i->nbytes, n, len, blen);
			perror("data write");
			exit(1);
		}
	}
	if(n){
		memset(b, 0, sizeof b);
		if(read(fd, (char *)b, (int)n) != n)
			goto out;
		n += s->blocksize-1;
		n /= s->blocksize;
		if(Write(s, b, n)){
			perror("data write");
			exit(1);
		}
	}
	close(fd);
}
