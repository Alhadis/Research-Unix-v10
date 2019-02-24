/*
 *	iread - insistent read.  Same usage as read, but will always return
 *	either the exact number of bytes requested, 0, or -1, unless a call
 *	to read gets some data and a subsequent call returns 0.
 */
int
iread (fd, buf, n)
	int fd, n;
	register char *buf;
{
	register int r, offset;

	r = read (fd, buf, n);
	if (r <= 0)
		return r;
	offset = 0;
	while (r != n - offset && r > 0) {
		offset += r;
		r = read (fd, buf + offset, n - offset);
	}
	return r < 0? r: r + offset;
}
