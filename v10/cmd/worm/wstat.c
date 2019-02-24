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
		if(!vol_id && !nf)
			perror(dev);
		exit(2);
	}
	if(e = openinode(&s, SPIN_DOWN)){
		if(!vol_id && !nf)
			fprint(2, "%s: %s\n", dev, e);
		exit(2);
	}
	if(nf){
		if(vol_id)
			if(strcmp(vol_id, s.vol_id) != 0)
				exit(1);
		exit(s.nfree >= nf? 0:3);
	}
	if(vol_id)
		exit(strcmp(vol_id, s.vol_id) != 0);
	if(vflag){
		Fprint(1, "%s: %s, %s\t%ldx%uhd blocks, %ld (=%.1fMB) free, zero=%ld this_sb@%ld next_sb@%ld\n",
			s.vol_id, s.comment, ctime(&s.ctime), s.nblocks, s.blocksize,
			s.nfree, s.nfree*(double)s.blocksize*1e-6, s.zero, s.myblock, s.nextsb);
	} else
		Fprint(1, "%s: %.0f%% used (%.1fMB free)\n", s.vol_id,
			s.nextffree*100.0/s.nblocks, s.nfree*(double)s.blocksize*1e-6);
	exit(0);
}

usage()
{
	print("Usage: worm stat [-v] [-fdevice] [-Fnfree] [vol_id]\n");
	exit(2);
}
