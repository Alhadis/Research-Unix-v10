/*	%W%
*/
#include "uucp.h"
VERSION(%W%);

#include <dk.h>

#define XBUFSIZ 4096
time_t time();
static jmp_buf Dfailbuf;

/*
 * Datakit protocol
 */
static dalarm() {longjmp(Dfailbuf);}
static int (*dsig)();
#define	EOFCTL	106

#ifndef V8
static short dkrmode[3] = { DKR_BLOCK, 0, 0 };
static short dkeof[3] = { EOFCTL, 0, 0 };	/* End of File signal */
#define	MSGread	read
#define	MSGwrite write
#else
#include <sys/stream.h>
#include <sys/filio.h>
#include <sys/dkio.h>
static struct	mesg	rhdr;
static struct	mesg	whdr = { M_DATA, MSGMAGIC, 0, 0 };
static struct	mesg	delim = { M_DELIM, MSGMAGIC, 0, 0 };
/*struct	mesg	eofmsg = { M_CTL, MSGMAGIC, 1, 0 };*/
static u_char	eofmsg[] = { M_CTL, MSGMAGIC, 1, 0, EOFCTL };
extern	int	mesg_ld;
static	rcount = 0;
static	int	usemesg;
static	char	ctlmsg[12];	/* sizeof struct sgttyb -- hack */
#endif

/*
 * turn on protocol
 */
dturnon()
{
	int ret; extern int errno;

	dsig=signal(SIGALRM, dalarm);
#ifndef V8
	if(ioctl(Ofn, DIOCRMODE, dkrmode) < 0) {
	    ret=ioctl(Ofn, DIOCRMODE, dkrmode);
	    DEBUG(4, "dturnon: ret=%d, ", ret);
	    DEBUG(4, "Ofn=%d, ", Ofn);
	    DEBUG(4, "errno=%d\n", errno);
	    return(-1);
	}
#else
	ctlmsg[0] = 0;
	if (ioctl(Ofn, DIOCSCTL, ctlmsg) >= 0) {
		DEBUG(4, "can send ctls\n", 0);
		usemesg = 0;
	} else {
		DEBUG(4, "must use mesgld\n", 0);
		usemesg = 1;
		if ((ret=ioctl(Ofn, FIOPUSHLD, &mesg_ld)) < 0) {
			DEBUG(4, "pushld: ret=%d, ", ret);
			DEBUG(4, "Ofn=%d, ", Ofn);
			DEBUG(4, "errno=%d\n", errno);
			return(-1);
		}
	}
	rcount = 0;
#endif
	return(0);
}
dturnoff()
{
	(void) signal(SIGALRM, dsig);
#ifdef V8
	if (usemesg)
		ioctl(Ofn, FIOPOPLD, 0);
#endif
	return(0);
}

/*
 * write message across Datakit link
 *	type	-> message type
 *	str	-> message body (ascii string)
 *	fn	-> Datakit file descriptor
 * return
 *	SUCCESS	-> message sent
 *	FAIL	-> write failed
 */
dwrmsg(type, str, fn)
register char *str;
int fn;
char type;
{
	register char *s;
	char bufr[XBUFSIZ];

	bufr[0] = type;
	s = &bufr[1];
	while (*str)
		*s++ = *str++;
	*s = '\0';
	if (*(--s) == '\n')
		*s = '\0';
	return(MSGwrite(fn, bufr, (unsigned) strlen(bufr) + 1) < 0 ? FAIL : SUCCESS);
}

/*
 * read message from Datakit link
 *	str	-> message buffer
 *	fn	-> Datakit file descriptor
 * return
 *	FAIL	-> send timed out
 *	SUCCESS	-> ok message in str
 */
drdmsg(str, fn)
register char *str;
{

	register int len;

	if(setjmp(Dfailbuf))
		return(FAIL);

	(void) alarm(300);
	for (;;) {
		if( (len = MSGread(fn, str, XBUFSIZ)) <= 0) {
			(void) alarm(0);
			return(FAIL);
		}
		str += len;
		if (*(str - 1) == '\0')
			break;
	}
	(void) alarm(0);
	return(SUCCESS);
}

/*
 * read data from file fp1 and write
 * on Datakit link
 *	fp1	-> file descriptor
 *	fn	-> Datakit descriptor
 * returns:
 *	FAIL	->failure in Datakit link
 *	SUCCESS	-> ok
 */
dwrdata(fp1, fn)
register FILE *fp1;
{
	register int len, ret;
	long bytes;
	char bufr[XBUFSIZ];
	char text[128];
	time_t	ticks;
	int fd;

	bytes = 0L;
	fd = fileno(fp1);
	(void) millitick();	/* set msec timer */
	while ((len = read(fd, bufr, XBUFSIZ)) > 0) {
		bytes += len;
		ret = MSGwrite(fn, bufr, (unsigned) len);
		if (ret != len) {
			return(FAIL);
		}
		if (len != XBUFSIZ)
			break;
	}
#ifndef V8
	ioctl(fn, DIOCXCTL, dkeof);
#else
	MSGeof(fn);
#endif
	ticks = millitick();
	(void) sprintf(text, "-> %ld / %ld.%.3d secs",
		bytes, ticks / 1000, ticks % 1000);
	DEBUG(4, "%s\n", text);
	syslog(text);
	return(SUCCESS);
}


/*
 * read data from Datakit link and
 * write into file
 *	fp2	-> file descriptor
 *	fn	-> Datakit descriptor
 * returns:
 *	SUCCESS	-> ok
 *	FAIL	-> failure on Datakit link
 */
drddata(fn, fp2)
register FILE *fp2;
{
	register int len;
	long bytes;
	char text[128];
	char bufr[XBUFSIZ];
	time_t ticks;
	int fd;

	bytes = 0L;
	fd = fileno(fp2);
	(void) millitick();	/* set msec timer */
	for (;;) {
		len = drdblk(bufr, XBUFSIZ, fn);
		if (len < 0)
			return(FAIL);
		bytes += len;
		if (write(fd, bufr, len) != len)
			return (FAIL);
		if (len < XBUFSIZ)
			break;
	}
	/* should check for control character here */
	ticks = millitick();
	(void) sprintf(text, "<- %ld / %ld.%.3d secs",
		bytes, ticks / 1000, ticks % 1000);
	DEBUG(4, "%s\n", text);
	syslog(text);
	return(SUCCESS);
}

/*
 * read block from Datakit link
 * reads are timed
 *	blk	-> address of buffer
 *	len	-> size to read
 *	fn	-> Datakit descriptor
 * returns:
 *	FAIL	-> link error timeout on link
 *	i	-> # of bytes read
 */
drdblk(blk, len,  fn)
register char *blk;
{
	register int i, ret;

	if(setjmp(Dfailbuf))
		return(FAIL);

	for (i = 0; i < len; i += ret) {
		(void) alarm(300);
		if ((ret = MSGread(fn, blk, (unsigned) len - i)) < 0) {
			(void) alarm(0);
			return(FAIL);
		}
		blk += ret;
		if (ret == 0)	/* zero length block contains only EOF signal */
			break;
	}
	(void) alarm(0);
	return(i);
}

#ifdef V8
MSGread(fd, buf, count)
char *buf;
{
	int mycount = 0, i;
	char junk[256];

	if (usemesg == 0)
		return (read(fd, buf, count));
more:
	if (rcount==0) {
		if ((i = read(fd, (char *)&rhdr, sizeof(rhdr))) != sizeof(rhdr)) {
			DEBUG(4, "DK msg bad read: %d\n", i);
			return(-1);
		}
		if (rhdr.magic != MSGMAGIC) {
			DEBUG(1, "Bad magic %o on DK message read\n", rhdr.magic);
			return(-1);
		}
		rcount = rhdr.losize + (rhdr.hisize<<8);
	}
	switch (rhdr.type) {

	case M_DELIM:
	case M_CTL:
	default:
		DEBUG(5, "Got ctl %o\n", rhdr.type + (rcount<<6));
		while (rcount>0) {
			i = read(fd, junk, min(rcount, sizeof(junk)));
			if (i<0)
				break;
			rcount -= i;
		}
		rcount = 0;
		if (rhdr.type != M_DELIM)
			goto more;
		return (mycount);

	case M_DATA:
		DEBUG(5, "Got data rcount %d\n", rcount);
		while (count > 0 && rcount > 0) {
			i = read(fd, buf, min(count, rcount));
			if (i <= 0)
				break;
			buf += i;
			mycount += i;
			count -= i;
			rcount -= i;
		}
		if (rcount == 0)
			goto more;
		return (mycount);
	}
}

MSGwrite(fd, buf, count)
char *buf;
{
	if (usemesg == 0)
		return (write(fd, buf, count));
	whdr.hisize = count>>8;
	whdr.losize = count;
	if (write(fd, (char *)&whdr, sizeof(whdr)) != sizeof(whdr))
		return(-1);
	if (write(fd, buf, count) != count)
		return(-1);
	if (write(fd, (char *)&delim, sizeof(delim)) != sizeof(delim))
		return(-1);
	return(count);
}

MSGeof(fn)
int fn;
{
	if (usemesg)
		write(fn, eofmsg, sizeof(eofmsg));
	else {
		ctlmsg[0] = EOFCTL;
		ioctl(fn, DIOCSCTL, ctlmsg);
	}
}
#endif
