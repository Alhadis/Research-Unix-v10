#include	<libc.h>
#include	"worm.h"
#include	<sys/types.h>
#include	<sys/udaioc.h>

main(argc, argv)
	char **argv;
{
	Superblock s;
	char *e, *vol_id = 0;
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
	/*
		normally, you have to read to bring the drive online.
		however, when you are likely to call reset, the drive
		is online and blocked so skip the read
	*/
	/*lseek(s.fd, 1024L, 0);
	read(s.fd, buf, sizeof buf);/**/
	if(ioctl(s.fd, UIORST) < 0)
		perror("reset ioctl");
	exit(0);
}

usage()
{
	print("Usage: worm reset [-fdevice]\n");
	exit(2);
}
