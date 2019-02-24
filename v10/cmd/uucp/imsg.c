/*	/sccs/src/cmd/uucp/s.imsg.c
	imsg.c	1.1	8/30/84 17:37:29
*/
#include "uucp.h"
VERSION(@(#)imsg.c	1.1);

#define MSYNC	'\020'
/* maximum likely message - make sure you don't get run away input */
#define MAXIMSG	256

/*
 * read message routine used before a
 * protocol is agreed upon.
 *	msg	-> address of input buffer
 *	fn	-> input file descriptor 
 * returns:
 *	EOF	-> no more messages
 *	0	-> message returned
 */
imsg(msg, fn)
register char *msg;
register int fn;
{
	register char c;
	short fndsync;
	char *bmsg;
	extern jmp_buf Sjbuf;
	extern int errno;

	fndsync = 0;
	bmsg = msg;
	CDEBUG(7, "imsg %s>", "");
	errno = 0;	/* only for debugging */
	while (read(fn, msg, sizeof(char)) == sizeof(char)) {
		*msg &= 0177;
		c = *msg;
		CDEBUG(7, "%s", c < 040 ? "^" : "");
		CDEBUG(7, "%c", c < 040 ? c | 0100 : c);
		if (c == MSYNC) { /* look for sync character */
			msg = bmsg;
			fndsync = 1;
			continue;
		}
		if (!fndsync)
			continue;

		if (c == '\0' || c == '\n') {
			*msg = '\0';
			return(0);
		}
		else
			msg++;

		if (msg - bmsg > MAXIMSG)	/* unlikely */
			return(FAIL);
	}
	CDEBUG(7, "\nread failed (%d)\n", errno);
	/* have not found sync or end of message */
	longjmp(Sjbuf, 1);
}

/*
 * initial write message routine -
 * used before a protocol is agreed upon.
 *	type	-> message type
 *	msg	-> message body address
 *	fn	-> file descriptor
 * return: 
 *	Must always return 0 - wmesg (WMESG) looks for zero
 */
omsg(type, msg, fn)
register char *msg;
register char type;
int fn;
{
	char buf[BUFSIZ];

	(void) sprintf(buf, "%c%c%s", MSYNC, type, msg);
	CDEBUG(7, "omsg >%s<\n", buf);
	write(fn, buf, strlen(buf) + 1);
	return(0);
}
