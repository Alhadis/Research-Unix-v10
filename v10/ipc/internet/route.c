#include <stdio.h>
#include <sys/param.h>
#include <sys/stream.h>
#include <sys/inio.h>

struct route{
	int dst;
	int gate;
};

main(argc, argv)
char *argv[];
{
	struct route r;
	int fd, usage = 1;

	r.gate = 1;
	if(argc > 1){
		if(strcmp(argv[1], "delete") == 0) {
			r.gate = 0;
			usage = argc != 3;
		} else if(strcmp(argv[1], "add") == 0)
			usage = argc != 4;
	}
	if(usage){
		fprintf(stderr, "Usage: %s add dest gateway\n", argv[0]);
		fprintf(stderr, "       %s delete dest\n", argv[0]);
		exit(1);
	}
	if(strcmp(argv[2], "*") == 0) {
		/* the default gateway */
		r.dst = 0;
	} else {
		r.dst = in_address(argv[2]);
		if(r.dst == 0 && argv[2][0] != '0'){
			fprintf(stderr, "%s: unknown host/net\n", argv[2]);
			exit(1);
		}
	}
	if(r.gate != 0) {
		r.gate = in_address(argv[3]);
		if(r.gate == 0){
			fprintf(stderr, "%s: unknown host/net\n", argv[3]);
			exit(1);
		}
	}
	fd = open("/dev/ip0", 2);
	if(fd < 0){
		perror("/dev/ip0");
		exit(1);
	}
	if(ioctl(fd, IPIOROUTE, &r) < 0)
		perror("IPIOROUTE");
}
