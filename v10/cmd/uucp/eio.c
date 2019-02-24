/*	/sccs/src/cmd/uucp/s.eio.c
	eio.c	1.4	8/30/84 17:37:17
*/

#include "uucp.h"
VERSION(@(#)eio.c	1.4);

#ifdef ATTSV
#define     MIN(a,b) (((a)<(b))?(a):(b))
#endif

#define	XBUFSIZ	4096
#define	CMLEN	20

static jmp_buf Failbuf;

/*
 * error-free channel protocol
 */
static
ealarm() {
	DEBUG(4, "read timeout\n", "");
	longjmp(Failbuf, 1);
}
static int (*esig)();

/*
 * turn on protocol timer
 */
eturnon()
{
	esig=signal(SIGALRM, ealarm);
	return(0);
}

eturnoff()
{
	signal(SIGALRM, esig);
	return(0);
}

/*
 * write message across link
 *	type	-> message type
 *	str	-> message body (ascii string)
 *	fn	-> link file descriptor
 * return
 *	FAIL	-> write failed
 *	0	-> write succeeded
 */
ewrmsg(type, str, fn)
register char *str;
int fn;
char type;
{
	register char *s;
	char bufr[BUFSIZ];
	int	s1, s2;

	bufr[0] = type;
	s = &bufr[1];
	while (*str)
		*s++ = *str++;
	*s = '\0';
	if (*(--s) == '\n')
		*s = '\0';
	s1 = strlen(bufr) + 1;
	if (setjmp(Failbuf)) {
		DEBUG(7, "ewrmsg write failed\n", "");
		return(FAIL);
	}
	alarm(120);
	s2 = write(fn, bufr, (unsigned) s1);
	alarm(0);
	if (s1 != s2)
		return(FAIL);
	return(0);
}

/*
 * read message from link
 *	str	-> message buffer
 *	fn	-> file descriptor
 * return
 *	FAIL	-> read timed out
 *	0	-> ok message in str
 */
erdmsg(str, fn)
register char *str;
{
	register int i;
	register unsigned len;

	if(setjmp(Failbuf)) {
		DEBUG(7, "erdmsg read failed\n", "");
		return(FAIL);
	}

	i = BUFSIZ;
	for (;;) {
		alarm(120);
		if ((len = read(fn, str, i)) == 0)
			continue;	/* Perhaps should be FAIL, but the */
					/* timeout will get it (skip alarm(0) */
		alarm(0);
		if (len < 0) { 
			DEBUG(4, "read fail errno %d", errno);
			return(FAIL);
		}
		str += len; i -= len;
		if (*(str - 1) == '\0')
			break;
	}
	return(0);
}

/*
 * read data from file fp1 and write
 * on link
 *	fp1	-> file descriptor
 *	fn	-> link descriptor
 * returns:
 *	FAIL	->failure in link
 *	0	-> ok
 */
ewrdata(fp1, fn)
int	fn;
register FILE *fp1;
{
	register int ret;
	int len;
	long bytes;
	char bufr[XBUFSIZ];
	char text[BUFSIZ];
	time_t	ticks;
	int	mil;
	struct stat	statbuf;
	off_t	msglen;
	char	cmsglen[CMLEN];
	int fd;

	if (setjmp(Failbuf)) {
		DEBUG(7, "ewrdata failed\n", "");
		return(FAIL);
	}
	bytes = 0L;
	fd = fileno(fp1);
	fstat(fd, &statbuf);
	bytes = msglen = statbuf.st_size;
	(void) millitick();
	sprintf(cmsglen, "%ld", (long) msglen);
	alarm(120);
	ret = write(fn, cmsglen, sizeof(cmsglen));
	DEBUG(7, "ewrmsg write %d\n", statbuf.st_size);
	while ((len = read(fd, bufr, XBUFSIZ)) > 0) {
		alarm(120);
		ret = write(fn, bufr, (unsigned) len);
		alarm(0);
		DEBUG(7, "ewrmsg ret %d\n", ret);
		if (ret != len)
			return(FAIL);
		if ((msglen -= len) <= 0)
			break;
	}
	if (len < 0 || (len == 0 && msglen != 0)) return(FAIL);
	ticks = millitick();
	sprintf(text, "-> %ld / %ld.%.3d secs", bytes, ticks / 1000, ticks % 1000);
	DEBUG(4, "%s\n", text);
	syslog(text);
	return(0);
}

/*
 * read data from link and
 * write into file
 *	fp2	-> file descriptor
 *	fn	-> link descriptor
 * returns:
 *	0	-> ok
 *	FAIL	-> failure on link
 */
erddata(fn, fp2)
register FILE *fp2;
{
	register int len;
	long bytes;
	char text[BUFSIZ];
	char bufr[XBUFSIZ];
	time_t	ticks;
	int	mil;
	long	msglen;
	char	cmsglen[CMLEN];
	int fd;

	bytes = 0L;
	fd = fileno(fp2);
	(void) millitick();
	len = erdblk(cmsglen, sizeof(cmsglen), fn);
	if (len < 0) return(FAIL);
	sscanf(cmsglen, "%ld", &msglen);
	bytes = msglen;
	DEBUG(7, "erdblk msglen %d\n", msglen);
	for (;;) {
		len = erdblk(bufr, MIN(msglen, XBUFSIZ), fn);
		DEBUG(7, "erdblk ret %d\n", len);
		if (len < 0) {
			DEBUG(7, "erdblk failed\n", "");
			return(FAIL);
		}
		if ((msglen -= len) < 0) {
			DEBUG(7, "erdblk read too much\n", "");
			return(FAIL);
		}
		if (write(fd, bufr, len) != len) {
			DEBUG(7, "fs write failed %d", errno);
			return (FAIL);
		}
		if (msglen == 0)
			break;
	}
	ticks = millitick();
	sprintf(text, "<- %ld / %ld.%.3d secs", bytes, ticks / 1000, ticks % 1000);
	DEBUG(4, "%s\n", text);
	syslog(text);
	return(0);
}

/*
 * read block from link
 * reads are timed
 *	blk	-> address of buffer
 *	len	-> size to read
 *	fn	-> link descriptor
 * returns:
 *	FAIL	-> link error timeout on link
 *	i	-> # of bytes read
 */
erdblk(blk, len,  fn)
register char *blk;
{
	register int i, ret;

	if(setjmp(Failbuf)) {
		DEBUG(7, "erdblk timeout\n", "");
		return(FAIL);
	}

	for (i = 0; i < len; i += ret) {
		alarm(120);
		DEBUG(7, "ask %d ", len - i);
		if ((ret = read(fn, blk, (unsigned) len - i)) < 0) {
			alarm(0);
			DEBUG(7, "read failed\n", "");
			return(FAIL);
		}
		DEBUG(7, "got %d\n", ret);
		blk += ret;
		if (ret == 0)
			break;
	}
	alarm(0);
	return(i);
}
