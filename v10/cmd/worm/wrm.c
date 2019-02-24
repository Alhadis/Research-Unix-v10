#include	<libc.h>
#include	<fio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<signal.h>
#include	"worm.h"

static int bad = 0;
static long nbytes;
static long nfiles;
char *argout;

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
	if(e = openinode(&s, DO_INODE|SPIN_DOWN)){
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
	inwrite(&s);
	if(bad)
		exit(1);
	exit(0);
}

usage()
{
	fprint(2, "Usage: worm rm [-fdevice] vol_id [files]\n");
	exit(1);
}

proc(s, file)
	Superblock *s;
	char *file;
{
	Inode i;

	if(inodeof(file) == 0){
		fprint(2, "%s: not on worm\n", file);
		return;
	}
	i.magic = DMAGIC;
	i.block = -1;
	i.name.n = file;
	if(inadd(s, &i))
		bad = 1;
}

static Inode *inodes;
static long ip;
static long ninodes = 0;
static char *nameb;
static long np;
static long nnameb = 0;
static long nblocks;
#define		IINC		1024
#define		NINC		(64*IINC)

ininit()
{
	if(nnameb == 0){
		nameb = malloc((unsigned)(nnameb = NINC));
		if(nameb == 0){
			fprint(2, "wrm: malloc fail, %d bytes\n", nnameb);
			exit(1);
		}
	}
	np = 0;
	if(ninodes == 0){
		inodes = (Inode *)malloc(sizeof(Inode)*(unsigned)(ninodes = IINC));
		if(inodes == 0){
			fprint(2, "wrm: malloc fail, %d inodes %d bytes\n", ninodes, ninodes*sizeof(Inode));
			exit(1);
		}
	}
	ip = 0;
}

inadd(s, i)
	Superblock *s;
	register Inode *i;
{
	register long len;

	len = strlen(i->name.n)+1;
	if(np+len > nnameb){
		while(np+len > nnameb)
			nnameb += NINC;
		nameb = realloc(nameb, (unsigned)nnameb);
		if(nameb == 0){
			fprint(2, "wrm: realloc fail, %d bytes\n", nnameb);
			exit(1);
		}
	}
	strcpy(nameb+np, i->name.n);
	i->name.o = np;
	np += len;
	if(ip == ninodes){
		ninodes += IINC;
		inodes = (Inode *)realloc((char *)inodes, (unsigned)ninodes*sizeof(Inode));
		if(inodes == 0){
			fprint(2, "wrm: realloc fail, %d inodes %d bytes\n", ninodes, ninodes*sizeof(Inode));
			exit(1);
		}
	}
	inodes[ip++] = *i;
	return(0);
}

inwrite(s)
	Superblock *s;
{
	char *e;

	if(e = lkwri(s, inodes, ip, nameb, np, 0L)){
		fprint(2, "%s\n", e);
		bad = 1;
		return;
	}
}
