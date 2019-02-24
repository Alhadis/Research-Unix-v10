#include <stdio.h>
#include <sgtty.h>
#include <signal.h>

extern int tcp_ld;

main(argc, argv)
char *argv[];
{
	int fd, ai;
	int maxseg = 1024;

	switch(argc) {
	case 2:
		break;
	case 3:
		maxseg = atoi(argv[2]);
		break;
	default:
		fprintf(stderr, "Usage: %s device [maxseg]\n", argv[0]);
		exit(1);
	}
	signal(SIGHUP, SIG_IGN);
	fd = open(argv[1], 2);
	if(fd < 0){
		perror(argv[1]);
		exit(1);
	}
	if(ioctl(fd, FIOPUSHLD, &tcp_ld) < 0){
		perror("PUSHLD tcp");
		exit(1);
	}
	if(argc == 3)
		if(ioctl(fd, TCPIOMAXSEG, &maxseg) < 0) {
			perror("TCPIOMAXSEG tcp");
			exit(1);
		}
	pause();
}
