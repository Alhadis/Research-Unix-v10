#include	<libc.h>
#include	"worm.h"
#include	"sym.h"
#include	<sys/types.h>
#include	<sys/stat.h>

char *dumpdir();
int verbose = 0;

main(argc, argv)
	char **argv;
{
	Superblock s, news;
	char *e;
	char *dev = "/dev/worm0";
	int update = 0;
	int c;
	char buf[1024];
	extern char *optarg;
	extern int optind;
	void blkfn();

	while((c = getopt(argc, argv, "f:vu")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case 'u':	update = 1; break;
		case 'v':	verbose = 1; break;
		case '?':	usage();
		}
	dev = mapdev(dev);
	if(optind != argc-1)
		usage();
	if((s.fd = open(dev, 2)) < 0){
		perror(dev);
		exit(1);
	}
	if(e = openinode(&s, SPIN_DOWN)){
		fprint(2, "%s: %s\n", dev, e);
		exit(2);
	}
	if(s.version != VBTREE){
		fprint(2, "%s is not a btree!\n", s.vol_id);
		exit(2);
	}
	if(strcmp(argv[optind], s.vol_id)){
		fprint(2, "wanted volid '%s'; got '%s'\n", argv[optind], s.vol_id);
		exit(1);
	}
	if(e = dumpdir(&s, update)){
		fprint(2, "%s: %s\n", dev, e);
		exit(2);
	}
	sprint(buf, "/usr/worm/tmp/%s", s.vol_id);
	unlink(buf);
	exit(0);
}

usage()
{
	fprint(2, "Usage: dir [-fdevice] -v] [-u] vol_id\n");
	exit(2);
}

char *
dumpdir(s, update)
	register Superblock *s;
{
	char *b;
	static char buf[64];
	char name[256], buf1[256];

	if((b = malloc(s->blocksize)) == 0){
		sprint(buf, "couldn't malloc buffer (%d bytes)", s->blocksize);
		return(buf);
	}
	numinodes = s->ninodes;
	sprint(name, "/usr/worm/dirs/%s", s->vol_id);
	Seek(s, s->binodes);
	sprint(buf1, "%s.F", name);
	copyout(s, buf1, s->nF, update, verbose);
	sprint(buf1, "%s.T", name);
	copyout(s, buf1, s->nT, update, verbose);
	sprint(buf1, "%s.I", name);
	copyout(s, buf1, s->ninochars, update, verbose);
	free(b);
	return((char *)0);
}
