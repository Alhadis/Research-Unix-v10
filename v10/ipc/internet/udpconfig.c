#include <stdio.h>
#include <sys/filio.h>
#include <signal.h>

extern int udp_ld;

main(argc, argv)
char *argv[];
{
	int fd;

	if(argc < 2){
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}
	signal(SIGHUP, SIG_IGN);
	fd = open(argv[1], 2);
	if(fd < 0){
		perror(argv[1]);
		exit(1);
	}
	if(ioctl(fd, FIOPUSHLD, &udp_ld) < 0){
		perror("PUSHLD udp");
		exit(1);
	}
	pause();
}
