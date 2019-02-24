#include <stdio.h>
#include <dk.h>
#include <string.h>

extern int ipcdebug;


dk_connect(dest, param)
char *dest, *param;
{
	int fd;
	char *bp;
	extern int dk_verbose, dk_errno;
	extern char *dkerr();
	static short timeout=500;	/* read timeout in ms */
#ifdef DKR_BLOCK
	static short dkrmode[3] = {DKR_BLOCK|DKR_TIME, 0, 0};
#endif

	if (ipcdebug)
		fprintf(stderr, "dk_connect %s\n", dest);
	if ((bp = strchr(dest, '!')) != NULL)
		*bp = '.';

	if (ipcdebug)
		fprintf(stderr, "dk_connect %s\n", dest);
	dk_verbose = 0;
	if ((fd = dkdial(dest)) < 0) {
		ipcseterror(-fd, dkerr(dk_errno), "Can't connect");
	}
#ifdef 	TCDKITIME
	ioctl(fd, TCDKITIME, &timeout);
	ioctl(fd, DIOCRMODE, dkrmode);
#endif
	return(fd);
}

int
dk_close(fd)
int fd;
{
	write(fd, &fd, 0);	/* stub - is this useful or necessary? */
	return close(fd);
}
