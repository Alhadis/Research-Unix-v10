/*
 *	sysem
 *
 *	wa: error code number on input
 *	xr: pointer to scblk with message text on output
 */

#include "inter.h"
#define MSGFILE "/usr/lib/vaxspitv35.err"
#define MSGCHUNK 49

sysem()
{
	long msg;
	static int fd = -1;
	register char *p;

	msg = WA(long);

	/* any premature return gives a null result */
	SET_XR(&tempstr);
	tempstr.len = 0;

	/* we keep a file descriptor open after the first message */
	if (fd < 0) {
		fd = open (MSGFILE, 0);
		if (fd < 0)
			return 0;
	}

	/* calculate how far into the file to go and go there */
	if (lseek (fd, (msg - 1) * MSGCHUNK, 0) < 0)
		return 0;

	/* read the message, check for success */
	if (read (fd, tempstr.s, MSGCHUNK - 1) != MSGCHUNK - 1)
		return 0;

	/* cut trailing blanks */
	p = tempstr.s + MSGCHUNK - 2;
	while (*p == ' ' && p > tempstr.s)
		p--;

	/* insert message length and return */
	tempstr.len = p - tempstr.s + 1;
	return 0;
}
