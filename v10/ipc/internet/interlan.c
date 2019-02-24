#include <stdio.h>
#include <sys/types.h>
#include <sys/ill_reg.h>
#include <sys/ioctl.h>

main(ac, av)
char *av[];
{
	int fd;
	unsigned int cmd;

	fd = open("/dev/il02", 2);
	if (fd<0) {
		perror("opening /dev/il02");
		exit(1);
	}
	cmd = atoi(av[1])<<8;
	if (ioctl(fd, ENIOCMD, &cmd) < 0) {
		perror("ioctl");
		exit(1);
	}
	printf("%x\n", cmd);
	return 0;
}
