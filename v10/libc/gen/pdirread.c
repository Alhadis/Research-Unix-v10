/* simulate new system call */
/* output is up to cnt bytes (up to some max)
 * each directory entry gets a string which is
 * the ascii representation of the inode number, a tab, and the
 * zero-terminated file name.  these zero-terminated strings are packed together
 * This implementation freely returns short reads.  End of directory
 * is a 0 length read, -1 and ENOSPC indicate that cnt couldn't hold
 * even the next entry
 */
#include "sys/types.h"
#include "sys/dir.h"
#include "errno.h"

char mb[4096], ncnv[23];
pdirread(fd, buf, cnt)
char *buf;
{	int n, m, j, loc;
	char *p, *lp;
	struct direct *d;

	loc = lseek(fd, 0, 1);
	n = read(fd, mb, sizeof(mb));
	if(n <= 0)
		return(n);
	lp = p = buf;
	d = (struct direct *) mb;
loop:
	if(d->d_ino == 0)
		goto incr;
	for(m = d->d_ino, j = 22; m; j--) {
		ncnv[j] = m % 10 + '0';
		m /= 10;
	}
	for(++j; j < 23 && p < buf + cnt; )
		*p++ = ncnv[j++];
	if(j != 23 || p >= buf + cnt)
		goto early;
	*p++ = '\t';
	for(j = 0; d->d_name[j] && j < DIRSIZ && p < buf + cnt; j++)
		*p++ = d->d_name[j];
	if(p >= buf + cnt)
		goto early;
	*p++ = 0;
incr:
	lp = p;
	n -= sizeof(*d);
	d++;
	if(lp < buf + cnt && n > 0)
		goto loop;
done:
	lseek(fd, loc + (char *)d - mb, 0);
	return(lp - buf);
early:
	*lp = 0;
	if((char *)d != mb)
		goto done;
	errno = ENOSPC;
	return(-1);
}
