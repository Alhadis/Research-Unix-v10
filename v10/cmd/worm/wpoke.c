#include	<libc.h>
#include	"worm.h"

main(argc, argv)
	char **argv;
{
	Superblock s;
	char *e, *vol_id = 0;
	char *dev = "/dev/worm0";
	int c;
	long nf = 0;
	int vflag = 0;
	Superblock ss;
	extern char *optarg;
	extern int optind;
	extern long atol();

	while((c = getopt(argc, argv, "vF:f:")) != -1)
		switch(c)
		{
		case 'f':	dev = optarg; break;
		case 'F':	nf = atol(optarg); break;
		case 'v':	vflag = 1; break;
		case '?':	usage();
		}
	if(optind < argc){
		vol_id = argv[optind++];
		if(optind != argc)
			usage();
	}
	dev = mapdev(dev);
	if((s.fd = open(dev, 0)) < 0){
		perror(dev);
		exit(2);
	}
	if(read(s.fd, &ss, sizeof ss) != sizeof ss){
		if(errno == ENXIO)
			print("unwritten block zero\n");
		else
			perror("block zero");
	} else {
		if(ss.magic == SMAGIC){
			print("appears to be a good superblock at zero\n");
			exit(0);
		} else if(ss.magic == 0){
			print("appears to be a zero'ed block at zero.\n");
		} else
			print("ignoring bogus block at zero\n");
	}
	vlink(s.fd, 1);
	exit(0);
}

usage()
{
	print("Usage: worm poke [-v] [-fdevice] [-Fnfree] [vol_id]\n");
	exit(2);
}

vlink(fd, blk)
{
	Superblock s;
	int i, n;

	while(blk < 1650000){
loop:
print("reading sb at %d\n", blk);
		bigseek(fd, blk, 1024, 0);
		if(read(fd, &s, sizeof s) == sizeof s){
			if(s.magic == SMAGIC){
				blk = s.nextsb;
				continue;
			}
			print("apparent garbage at supposed superblock@%ld\n", blk);
		} else {
			print("bad read at blk %ld, errno=%d\n", blk, errno);
			lseek(fd, 1024, 1);
			blk++;
		}
		for(i = 0; i < 50; i++){
			n = read(fd, &s, sizeof s);
			if(n < 0){
				lseek(fd, 1024, 1);
				continue;
			}
			if((n == sizeof s) && (s.magic == SMAGIC)){
				blk += i;
				print("after error, skipped %d blocks to apparent superblock at %ld\n", i, blk);
				goto loop;
			}
		}
		print("after error, no superblock after %d tries\n", i);
		blk += i;
	}
}
