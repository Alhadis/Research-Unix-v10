#include	<libc.h>
#include	"sym.h"
#include	"worm.h"

main(argc, argv)
	char **argv;
{
	Superblock s;
	char buf[1024];
	char *e;
	Inode *i;
	int c;
	char *com = 0;
	char *dev = "/dev/worm0";
	extern char *optarg;
	extern int optind;

	while((c = getopt(argc, argv, "f:c:")) != -1)
		switch(c)
		{
		case 'c':	com = optarg; break;
		case 'f':	dev = optarg; break;
		case '?':	usage();
		}
	if(optind+2 != argc)
		usage();
	dev = mapdev(dev);
	if((s.fd = open(dev, 2)) < 0){
		perror(dev);
		exit(1);
	}
	if(e = openinode(&s, SPIN_DOWN)){
		fprint(2, "%s: %s\n", dev, e);
		exit(1);
	}
	if(strcmp(s.vol_id, argv[optind])){
		fprint(2, "vol_id mismatch: wanted %s, got %s\n", argv[optind], s.vol_id);
		exit(1);
	}
	if(s.version != VBTREE){
		fprint(2, "poot: %s must be a btree disk\n", s.vol_id);
		exit(1);
	}
	memset(buf, sizeof buf, 0);
	memset(s.vol_id, sizeof s.vol_id, 0);
	strcpy(s.vol_id, argv[++optind]);
	if(com){
		memset(s.comment, sizeof s.comment, 0);
		strcpy(s.comment, com);
	}
	memcpy(buf, &s, 1024);
	lseek(s.fd, 0, 0);
	if(write(s.fd, buf, 1024) != 1024){
		perror("write of new superblock");
		exit(1);
	}
	exit(0);
}

usage()
{
	fprint(2, "Usage: poot [-fdevice] [-ccnewcomment] vol_id new_vol_id\n");
	exit(1);
}
