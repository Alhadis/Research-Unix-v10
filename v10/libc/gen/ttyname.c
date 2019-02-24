/*
 * on success:
 *	returns the pathname ("/dev/...") of the terminal
 *	with file descriptor "fd".
 *	bug: returns pointer to static area.
 * on failure:
 *	returns 0.
 */

#include <sys/types.h>
#include <ndir.h>
#include <sys/stat.h>
#include <string.h>

#define	ARB	64	/* arbitrary and not checked safely */

char	*_ttyname();
static char ttybuf[ARB];

char *
ttyname(fd)
	int fd;
{
	return(_ttyname(&ttybuf[0], fd));
}

/*
 * on success:
 *	stores at "s" the pathname of the terminal with file descriptor fd,
 *	and returns "s".
 * on failure:
 *	leaves "s" unchanged,
 *	and returns 0.
 */

static char *ttymatch();

char *
_ttyname(s, fd)
	char *s;
	int fd;
{
	int ff;
	register char *p, *q;
	register int n;
	struct stat fstb;
	char buf[ARB+1];

	if (fstat(fd, &fstb) < 0)
		return(0);
	if ((ff = open("/lib/ttydevs", 0)) < 0)
		return (ttymatch(&fstb, "/dev", s));
	while ((n = read(ff, buf, ARB)) > 0) {
		buf[n] = 0;
		for (q = buf; p = strchr(q, '\n'); q = p) {
			*p++ = 0;
			if ((q = ttymatch(&fstb, q, s)) != NULL) {
				close(ff);
				return (q);
			}
		}
		if (q < &buf[n])
			memcpy(buf, q, n - (q - buf));
	}
	close(ff);
	return (NULL);
}

static char *
ttymatch(stp, dp, s)
register struct stat *stp;
char *dp;
char *s;
{
	struct stat tsb;
	DIR *dfp;
	register struct direct *dir;
	char tmps[ARB];

	if ((dfp = opendir(dp)) == NULL)
		return (NULL);
	while ((dir = readdir(dfp)) != NULL) {
		if (dir->d_ino != stp->st_ino)
			continue;
		strcpy(tmps, dp);
		strcat(tmps, "/");
		strcat(tmps, dir->d_name);
		if (stat(tmps, &tsb) < 0)
			continue;
		if (stp->st_dev != tsb.st_dev || stp->st_ino != tsb.st_ino)
			continue;
		closedir(dfp);
		strcpy(s, tmps);
		return(s);
	}
	closedir(dfp);
	return(0);
}
