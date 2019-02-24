#include <stdio.h>
#include <sgtty.h>
#include <assert.h>

main()
{
	struct sgttyb sbuf;
	char dbuf[512];
	int fd, n;

	fd = open ("/dev/weather", 0);
	assert (fd >= 0);
	assert (gtty(fd, &sbuf) >= 0);
	sbuf.sg_ispeed = sbuf.sg_ospeed = B1200;
	sbuf.sg_flags &= ~(ECHO|RAW);
	assert (stty(fd, &sbuf) >= 0);
	ioctl(fd, TIOCEXCL, 0);

	for(;;) {
		n = read (fd, dbuf, 512);
		write (1, dbuf, n);
	}
}
