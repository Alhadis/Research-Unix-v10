/*	/sccs/src/cmd/uucp/s.chremdir.c
	chremdir.c	1.1	8/30/84 17:37:02
*/
#include "uucp.h"
VERSION(@(#)chremdir.c	1.1);

/*
 * chremdir(sys)
 * char	*sys;
 *
 * create SPOOL/sys directory and chdir to it
 * side effect: set RemSpool
 */
void
chremdir(sys)
char	*sys;
{
	int	ret;

	mkremdir(sys);	/* set RemSpool, makes sure it exists */
	DEBUG(6, "chdir(%s)\n", RemSpool);
	ret = chdir(RemSpool);
	ASSERT(ret == 0, Ct_CHDIR, RemSpool, errno);
	(void) strcpy(Wrkdir, RemSpool);
	return;
}

/*
 * mkremdir(sys)
 * char	*sys;
 *
 * create SPOOL/sys directory
 */

void
mkremdir(sys)
char	*sys;
{
	(void) sprintf(RemSpool, "%s/%s", SPOOL, sys);
	do {
		(void) mkdirs2(RemSpool, DIRMASK);
	} while (mkdirlock() == 0);
	return;
}

/*
 * put a file in the spool directory
 * so it won't be empty, so rm won't rm it
 * it's not really a lockfile, but we have good ways of cleaning up
 */

#define	HUGE	10000

static int
mkdirlock()
{
	char buf[BUFSIZ];
	extern int errno;

	sprintf(buf, "%s/L.%d", RemSpool, getpid());
	if (access(buf, 0) >= 0)
		return (1);
	if (ulockf(buf, HUGE) != FAIL)
		return (1);
	ASSERT(errno == ENOENT, "spool dir screwup", buf, errno);
	return (0);
}
