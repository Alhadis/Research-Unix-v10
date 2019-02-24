#include	<libc.h>
#include	"worm.h"
#include	"sym.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<pwd.h>
#include	<grp.h>
#include	<signal.h>

int lflag = 0;
int bflag = 0;

main(argc, argv)
	char **argv;
{
	Superblock s;
	register Inode *i;
	Inode newi;
	char *e;
	char *dev = "/dev/worm0";
	int c;
	extern char *optarg;
	extern int optind;
	void pr();

	while((c = getopt(argc, argv, "lbf:")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case 'l':	lflag = 1; break;
		case 'b':	bflag = 1; break;
		case '?':	usage();
		}
	dev = mapdev(dev);
	if((s.fd = open(dev, 2)) < 0){
		perror(dev);
		exit(1);
	}
	if(e = openinode(&s, DO_INODE|SPIN_DOWN)){
		fprint(2, "%s: %s\n", dev, e);
		exit(1);
	}
	if(optind != argc-3)
		usage();
	if(strcmp(s.vol_id, argv[optind])){
		fprint(2, "worm mv: vol_id mismatch: wanted %s, got %s\n", argv[optind], s.vol_id);
		exit(1);
	}
	optind++;
	if((i = inodeof(argv[optind])) == 0){
		Fprint(2, "%s not found\n", argv[optind]);
		exit(1);
	}
	optind++;
	if(strlen(argv[optind]) < 1){
		Fprint(2, "worm mv: destination name is null\n");
		exit(1);
	}
	for(c = 1; c <= NSIG; c++)
		signal(c, SIG_IGN);
	ininit();
	newi = *i;
	i->block = -1;
	inadd(&s, i);
	newi.name.n = argv[optind];
	inadd(&s, &newi);
	if(inwrite(&s))
		exit(1);
	exit(0);
}

usage()
{
	fprint(2, "Usage: worm mv [-fdevice] vol_id from to\n");
	exit(2);
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
			fprint(2, "wmv: malloc fail, %d bytes\n", nnameb);
			exit(1);
		}
	}
	np = 0;
	if(ninodes == 0){
		inodes = (Inode *)malloc(sizeof(Inode)*(unsigned)(ninodes = IINC));
		if(inodes == 0){
			fprint(2, "wmv: malloc fail, %d inodes %d bytes\n", ninodes, ninodes*sizeof(Inode));
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
			fprint(2, "wmv: realloc fail, %d bytes\n", nnameb);
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
			fprint(2, "wmv: realloc fail, %d inodes %d bytes\n", ninodes, ninodes*sizeof(Inode));
			exit(1);
		}
	}
	inodes[ip++] = *i;
}

inwrite(s)
	Superblock *s;
{
	char *e;

	if(e = lkwri(s, inodes, ip, nameb, np, 0L)){
		fprint(2, "%s\n", e);
		return(1);
	}
	return(0);
}
