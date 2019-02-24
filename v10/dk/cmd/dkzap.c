#include <sys/ioctl.h>

main()
{
	register fd = dkctlchan(2);
	register r;

	if (fd < 0) {
		printf("Can't open ctlchan to kmc\n");
		exit(1);
	}
	r = ioctl(fd, KIOCSHUT, (char *)0);
	if (r < 0)
		printf("Couldn't shut down kdi\n");
	return(0);
}
