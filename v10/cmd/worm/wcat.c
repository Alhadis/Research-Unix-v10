#include	<libc.h>
#include	"sym.h"
#include	"worm.h"

main(argc, argv)
	char **argv;
{
	Superblock s;
	char *e;
	Inode *i;
	int c;
	char *dev = "/dev/worm0";
	extern char *optarg;
	extern int optind;

	while((c = getopt(argc, argv, "f:")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case '?':	usage();
		}
	if(optind+2 != argc)
		usage();
	dev = mapdev(dev);
	if((s.fd = open(dev, 0)) < 0){
		perror(dev);
		exit(1);
	}
	if(e = openinode(&s, DO_INODE|SPIN_DOWN)){
		fprint(2, "%s: %s\n", dev, e);
		exit(1);
	}
	if(strcmp(s.vol_id, argv[optind])){
		fprint(2, "vol_id mismatch: wanted %s, got %s\n", argv[optind], s.vol_id);
		exit(1);
	}
	if(i = inodeof(argv[++optind]))
		c = pr(&s, i);
	else {
		fprint(2, "wcat: can't find %s\n", argv[optind]);
		c = 1;
	}
	exit(c);
}

usage()
{
	fprint(2, "Usage: worm cat [-fdevice] vol_id file\n");
	exit(1);
}

pr(s, i)
	Superblock *s;
	register Inode *i;
{
	char b[BIGBLOCK];
	register long len, n;
	long nb;
	int fd;

	fd = 1;
	nb = sizeof b / s->blocksize;
	Seek(s, i->block);
	for(n = i->nbytes, len = nb*s->blocksize; n > 0;){
		if(len > n){
			len = n;
			nb = (len+s->blocksize-1)/s->blocksize;
		}
		Read(s, b, nb);
		if(write(fd, b, (int)len) != len){
			perror("write");
			return(1);
		}
		n -= len;
	}
	close(fd);
	return(0);
}
