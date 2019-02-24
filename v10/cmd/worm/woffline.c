#include	<libc.h>
#include	"worm.h"
#include	<sys/types.h>
#include	<sys/udaioc.h>

main(argc, argv)
	char **argv;
{
	Superblock s;
	char *vol_id = 0;
	char buf[1024];
	char *dev = "/dev/worm0";
	int c;
	extern char *optarg;
	extern int optind;

	while((c = getopt(argc, argv, "f:")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case '?':	usage();
		}
	if(optind != argc)
		usage();
	dev = mapdev(dev);
	if((s.fd = open(dev, 2)) < 0){
		if(!vol_id)
			perror(dev);
		exit(2);
	}
	/* have to read to make ioctl work */
	lseek(s.fd, 1024L, 0);
	read(s.fd, buf, sizeof buf);
	if(ioctl(s.fd, UIOSPDW) < 0){
		perror("ioctl");
		exit(2);
	}
	exit(0);
}

usage()
{
	print("Usage: worm offline [-fdevice]\n");
	exit(2);
}
