/*
 * gather data that may come in dribs and drabs:
 * read size bytes into buf,
 * but keep looking until at least minsize have arrived
 */
#include <errno.h>	/* just for EINTR */

int
_rfgread(fd, buf, size, minsize)
int fd;
char *buf;
int size;
register int minsize;
{
	register int n, tot;
	extern int errno;

	tot = 0;
	while (minsize > 0) {
		if ((n = read(fd, buf, size)) <= 0) {
			if (errno == EINTR)
				continue;
			break;
		}
		buf += n;
		size -= n;
		minsize -= n;
		tot += n;
	}
	if (tot)
		return (tot);
	return (n);
}
