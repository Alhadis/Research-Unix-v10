#include	<libc.h>
#include	"worm.h"

main(argc, argv)
	char **argv;
{
	Superblock s;
	char *e, *vol_id = 0, *vol;
	char *dev = "/dev/worm0";
	int c;
	long nf = 0;
	char wflag[512];
	char buf[512];
	int i;
	extern char *optarg;
	extern int optind;
	extern long atol();

	wflag[0] = 0;
	while((c = getopt(argc, argv, "w:")) != -1)
		switch(c)
		{
		case 'w':	sprint(wflag, "-w%s", optarg); break;
		case '?':	usage();
		}
	if(optind < argc){
		vol_id = argv[optind++];
		if(optind != argc)
			usage();
	}
	if(vol_id == 0){
		for(i = 0; ; i++){
			sprint(buf, "%d", i);
			dev = mapdev(buf);
			if((s.fd = open(dev, 0)) < 0){
				if(errno == ENOENT)
					break;
				if(errno == ENXIO)
					continue;
				perror(dev);
				exit(2);
			}
			if(e = openinode(&s, SPIN_DOWN)){
				fprint(2, "%s: %s\n", dev, e);
				exit(2);
			}
			print("%s: %s\n", dev, s.vol_id);
			close(s.fd);
		}
		exit(0);
	}
	if(isjukebox()){
		jload(vol_id, wflag);
		exit(0);
	}
	{
		for(i = 0; ; i++){
			sprint(buf, "%d", i);
			dev = mapdev(buf);
			if((s.fd = open(dev, 0)) < 0){
				if(errno == ENOENT)
					break;
				if(errno == ENXIO)
					continue;
				perror(dev);
				exit(2);
			}
			if(e = openinode(&s, SPIN_DOWN)){
				fprint(2, "%s: %s\n", dev, e);
				exit(2);
			}
			if(strcmp(vol_id, s.vol_id) == 0){
				print("%s\n", buf);
				exit(0);
			}
			close(s.fd);
		}
	}
	fprint(2, "worm mount: couldn't find %s\n", vol_id);
	exit(1);
}

usage()
{
	print("Usage: worm mount [-wsecs] [vol_id]\n");
	exit(2);
}

/*
	return zero if there isn't a jukebox
*/
isjukebox()
{
	return(access("/dev/scsi", 6) == 0);
}

/*
	secs is the number of seconds to wait
*/
jload(vol, secs)
	char *vol, *secs;
{
	if(*secs)
		execlp("/usr/lib/worm/jukebox", "jukebox", secs, "-Rm", vol, (char *)0);
	else
		execlp("/usr/lib/worm/jukebox", "jukebox", "-Rm", vol, (char *)0);
	perror("execlp(/usr/lib/worm/jukebox)");
	exit(1);
}
