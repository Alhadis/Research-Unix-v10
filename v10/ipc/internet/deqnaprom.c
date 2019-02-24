#include <stdio.h>
#include <sys/types.h>
#include <sys/enio.h>

main(ac, av)
char *av[];
{
	int fd;
	unsigned int cmd;

	fd = open("/dev/qe03", 2);
	if (fd<0) {
		perror("opening /dev/qe03");
		exit(1);
	}
	if (ioctl(fd, ENIOPROM, &cmd) < 0) {
		perror("ioctl");
		exit(1);
	}
	return 0;
}
