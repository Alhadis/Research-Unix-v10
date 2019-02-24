#include	<libc.h>
#include	"worm.h"
#include	"sym.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<pwd.h>
#include	<grp.h>

Inode *inodebase, *inext;
char *namebase, *cnext;

main(argc, argv)
	char **argv;
{
	Superblock s;
	char *e;
	char *dev = "/dev/worm0";
	register c, j;
	register Inode *from, *to;
	extern char *optarg;
	extern int optind;
	char *vlk();
	int cmp();
	int fd;
	int verbose = 0;
	char buf[512];
	long ni, nc;

	while((c = getopt(argc, argv, "vf:")) != -1)
		switch(c)
		{
		case 'v':	verbose = 1; break;
		case 'f':	dev = optarg; break;
		case '?':	usage();
		}
	dev = mapdev(dev);
	if((s.fd = open(dev, 0)) < 0){
		perror(dev);
		exit(1);
	}
	if(e = openinode(&s, SPIN_DOWN)){
		fprint(2, "%s: %s\n", dev, e);
		exit(1);
	}
	if(s.version != VLINK){
		fprint(2, "%s: not a vlink disk, no action taken.\n", s.vol_id);
		exit(1);
	}
	if(optind != argc-1)
		usage();
	if(strcmp(argv[optind], s.vol_id)){
		fprint(2, "wanted volid '%s'; got '%s'\n", argv[optind], s.vol_id);
		exit(1);
	}
	if(verbose)
		print("%d inodes\n", numinodes);
	if((inodebase = (Inode *)malloc(s.blocksize+(int)numinodes*sizeof(Inode))) == 0){
		fprint(2, "malloc of %ld inodes failed\n", numinodes);
		exit(2);
	}
	if((namebase = malloc(s.blocksize+(int)numnamechars+(int)numinodes)) == 0){
		fprint(2, "malloc of %ld chars failed\n", numnamechars);
		exit(2);
	}
	inext = inodebase;
	cnext = namebase;
	s.ninodes = 0;
	s.nextsb = 2;
	if(e = vlk(&s)){
		fprint(2, "%s: %s\n", dev, e);
		exit(1);
	}
	j = inext-inodebase;
	if(verbose)
		print("%d in base\n", j);
	qsort((char *)inodebase, j, sizeof(*inodebase), cmp);
	for(to = inodebase, from = inodebase+1; from < inext; from++){
		/* find the next different one */
		while(strcmp(from->name.o+namebase, to->name.o+namebase) == 0){
			if(to->block < from->block)
				*to = *from;		/* want the last one */
			if(++from >= inext)
				goto done;
		}
		*++to = *from;
	}
done:
	inext = to;
	j = inext-inodebase;
	sprint(buf, "/usr/worm/tmp/%s", s.vol_id);
	if((fd = creat(buf, 0666)) < 0){
		perror(buf);
		exit(1);
	}
	ni = j;
	nc = cnext-namebase;
	write(fd, (char *)&s.ctime, 4);
	write(fd, (char *)&ni, 4);
	write(fd, (char *)inodebase, (int)ni*sizeof(Inode));
	write(fd, (char *)&nc, 4);
	write(fd, namebase, (int)nc);
	exit(0);
}

usage()
{
	fprint(2, "Usage: worm tmpdir [-fdevice] vol_id\n");
	exit(2);
}

cmp(a, b)
	Inode *a, *b;
{
	return(strcmp(namebase+a->name.o, namebase+b->name.o));
}

char *
vlk(s)
	register Superblock *s;
{
	register Inode *i;
	short fd = s->fd;
	register long j;
	long blk = -1;		/* shouldn't be accessed first time through */
	char *b;
	long nb;
	Inode *iend;
	static char buf[64];

	if((b = malloc(s->blocksize)) == 0){
		sprint(buf, "couldn't malloc buffer (%d bytes)", s->blocksize);
		return(buf);
	}
	for(;;){
		if(s->magic != SMAGIC){
			fprint(2, "bad Superblock at %ld\n", blk);
			exit(1);
		}
		if(s->ninodes){
			nb = (s->ninodes+IPERB-1)/IPERB;
			Seek(s, s->binodes);
			if(Read(s, (char *)inext, nb))
				goto skip;
			j = cnext-namebase;
			for(i = inext, iend = i+s->ninodes; i < iend; i++)
				i->name.o += j;
			inext += s->ninodes;
			nb = (s->ninochars+s->blocksize-1)/s->blocksize;
			if(Read(s, cnext, nb))
				goto skip;
			cnext += (s->ninochars+1)&~1;
		}
	skip:
		blk = s->nextsb;
		Seek(s, blk);
		if(Read(s, b, 1L))
			break;
		*s = *((Superblock *)b);
		s->fd = fd;
		if(s->myblock == 0)
			s->myblock = blk;
	}
	free(b);
	return((char *)0);
}
