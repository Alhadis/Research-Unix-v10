#include <stdio.h>
#include <sys/types.h>
#include <sys/timeb.h>

/*
 *	convert a pt number into a pt name 
 */
#define PTSIZE sizeof("/dev/pt/ptxxx")
static char *devname = "/dev/pt/ptxxx";

char *
ptname(pt)
int pt;
{
	register char *pp;

	pp = devname + sizeof("/dev/pt/pt") - 1;
	if (pt/100 > 0)
		*pp++ = '0' + ((pt/100)%10);
	*pp++ = '0' + ((pt/10)%10);
	*pp++ = '0' + (pt%10);
	*pp++ = '\0';
	
	return devname;
}

/*
 *	Returns an fd for an open odd numbered pt.
 */
int
ptopen(ptp)
int *ptp;		/* the opened pt */
{
	int pt, hpt, hash, fd;
	char *name;
	struct timeb tb;
	extern char *ptname();

	/* find highest pt number */
	if ((hpt = highpair()) < 0)
		return -1;

	/* search for an unused pt */
	ftime(&tb);
	pt = hash = (tb.millitm/10) % hpt & ~1;
	do {
		if ((fd = open(ptname(pt+1), 2)) >= 0)
			break;
		pt = (pt + 2) % hpt;
	} while (pt != hash);
	*ptp = pt;
	return fd; 
}

/*
 *	read the directory and return the highest numbered pt
 */

#define PTDIR "/dev/pt"
static int highest = -1;

static
highpair()
{
#	include <sys/types.h>
#	include <sys/dir.h>
	FILE *dir;
	struct direct dbuf;
	int pt;

	if (highest > -1)
		return highest;

	if ((dir = fopen(PTDIR, "r")) == NULL)
		return -1;
	while (fread(&dbuf, sizeof(dbuf), 1, dir) != 0) {
		if (dbuf.d_ino != 0 && strncmp(dbuf.d_name, "pt", 2) == 0) {
			pt = atoi(&dbuf.d_name[2]);
			if (pt > highest)
				highest = pt;
		}
	}

	/* point to the first of an even odd pair */
	if (highest & 1)
		highest--;
	else
		highest -= 2;
	return highest;
}

/*
 *	Like pipe(2) except that a pt is opened.  Returns the number of
 *	the slave pt or -1.
 */
int
ptpipe(pfd)
	int *pfd;
{
	int slave;

	pfd[0] = ptopen(&slave);
	if (pfd[0] < 0)
		return -1;
	pfd[1] = open(ptname(slave), 2);
	if (pfd[1] < 0) {
		close(pfd[0]);
		return -1;
	}
	return slave;
}
char *
whoami()
{
	static char name[128];
	int fd, n;
	char *cp;

	fd = open("/etc/whoami", 0);
	if (fd < 0)
		return ("Kremvax");
	n = read(fd, name, sizeof(name)-1);
	if (n <= 0)
		return ("Kremvax");
	name[n] = '\0';
	for (cp=name; *cp; cp++)
		if (*cp == '\n') {
			*cp = '\0';
			break;
		}
	return name;
}

