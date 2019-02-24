#ifndef lint
static char sccsid[] = "@(#)telnetd.c	4.26 (Berkeley) 83/08/06";
#endif

/*
 * Stripped-down telnet server.
 */
#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <sgtty.h>
#include <stdio.h>
#include <wait.h>
#include <sys/ttyld.h>
#include <sys/stream.h>
#include "telnet.h"

#define	BELL	'\07'
#define DEBUG if (debug)

/* option settings (remote and local) */
static char	hisopts[256];
static char	myopts[256];

/* formats for option messages */
static char	doopt[] = { IAC, DO, '%', 'c', 0 };
static char	dont[] = { IAC, DONT, '%', 'c', 0 };
static char	will[] = { IAC, WILL, '%', 'c', 0 };
static char	wont[] = { IAC, WONT, '%', 'c', 0 };

static int 	ptfd, netfd;	/* fd's to pt and tcp */
static int	done;		/* true if session is to be ended */
static int	debug;		/* true if debugging is to be output */
static struct sgttyb ptb;	/* result of an IOCGETP on the pt */
static struct tchars ptt;	/* result of an IOCGETC on the pt */

/* predefined */
static 	reapchild();
static	catchint();	

/* imported */
extern	char **environ;
extern	int errno;
extern	char *strrchr(), *strchr();
 

/*
 *	The following macros and routines are used to
 *	manage the I/O buffers.  They're a cross between
 *	stream buffers and standard I/O.
 */
struct buffer {
	char *b_rp;		/* read pointer */
	char *b_wp;		/* write pointer */
	char b_buf[BUFSIZ];	/* the buffer */
};
struct buffer ptibuf, *ptin = &ptibuf;
struct buffer ptobuf, *ptout = &ptobuf;
struct buffer netibuf, *netin = &netibuf;
struct buffer netobuf, *netout = &netobuf;

#define binit(bp) (bp->b_rp = bp->b_wp = bp->b_buf)
#define bytes_filled(bp) (bp->b_wp - bp->b_rp)
#define space_left(bp) (bp->b_buf+sizeof(bp->b_buf) - bp->b_wp)
#define bput(bp, c) (*(bp->b_wp++) = c)
#define bget(bp) (*(bp->b_rp++) & 0377)

/* read whatever the buffer can take */
static int
bread(bp, fd)
	struct buffer *bp;
	int fd;
{
	int cc;

	/* normalize the buffer */
	if (bytes_filled(bp) == 0)
		bp->b_rp = bp->b_wp = bp->b_buf;

	/* fill it */
	cc = read(fd, bp->b_wp, space_left(bp));
	if (cc > 0)
		bp->b_wp += cc;
	return cc;
}

/* read at most n bytes */
static int
breadn(bp, fd, n)
	struct buffer *bp;
	int fd, n;
{
	int cc;

	/* normalize the buffer */
	if (bytes_filled(bp) == 0)
		bp->b_rp = bp->b_wp = bp->b_buf;
	if (n > space_left(bp))
		n = space_left(bp);

	/* fill it */
	cc = read(fd, bp->b_wp, n);
	if (cc > 0)
		bp->b_wp += cc;
	return cc;
}

/* empty the buffer */
static int
bwrite(bp, fd)
	struct buffer *bp;
	int fd;
{
	int cc;

	
	cc = write(fd, bp->b_rp, bytes_filled(bp));

	/* normalize the buffer */
	if (cc == bytes_filled(bp))
		binit(bp);
	return cc;
}
static int
bputs(bp, s)
	struct buffer *bp;
	char *s;
{
	while (*s)
		bput(bp, *s++);
}

/*
 *	Establish a tcp socket and fork off a process for each connection.
 */
main(argc, argv)
	char *argv[];
{
	int finish();
	int f;

	doit(0);
	rmut();
	/*NOTREACHED*/
}

/*
 *	Get a pt.  Put a login on one side and a telnet receiver on
 *	the other.
 */
static
doit(f)
	int f;
{
	int pfd[2];
#	define TERMEND pfd[0]
#	define PROCEND pfd[1]
	extern int tty_ld, mesg_ld;

	/* get a pt pair */
	if (pipe(pfd) < 0)
		fatalperror(f, "out of pipes", errno);

	/* make it really look like a terminal */
	if (ioctl(TERMEND, FIOPUSHLD, (struct sgttyb *)&mesg_ld)<0)
		fatalperror(f, "can't push mesgld", errno);
	if (ioctl(PROCEND, FIOPUSHLD, (struct sgttyb *)&tty_ld)<0)
		fatalperror(f, "can't push ttyld", errno);

	/* prepare for the death of a child */
	signal(SIGCHLD, catchint);
	signal(SIGHUP, catchint);
	signal(SIGPIPE, catchint);
	switch (fork()) {
		case -1:
			fatalperror(f, "fork", errno);
		case 0:
			/* a process which is the remote login */
			getty(PROCEND);
		default:
			/* the protocol process */
			close(PROCEND);
			netfd = f;
			ptfd = TERMEND;
			telnet();
	}
	/*NOTREACHED*/
}

static
fatal(f, msg)
	int f;
	char *msg;
{
	char buf[BUFSIZ];

	(void) sprintf(buf, "telnetd: %s.\n", msg);
	(void) write(f, buf, strlen(buf));
	rmut();
	exit(1);
}
static
finish()
{
	if (debug) fprintf(stderr, "pipe closed\n");
	rmut();
	exit(1);
}

static
fatalperror(f, msg, errno)
	int f;
	char *msg;
	int errno;
{
	char buf[BUFSIZ];
	extern char *sys_errlist[];

	(void) sprintf(buf, "%s: %s", msg, sys_errlist[errno]);
	fatal(f, buf);
}

static
terminate(s)
	char *s;
{
	DEBUG fprintf(stderr, "session on terminated because of %s\n", s);
	done = 1;
}

/* loop on input from the pt and the network */
static
telnet()
{
	int n;
	fd_set ibits, obits;

	binit(ptin);
	binit(ptout);
	binit(netin);
	binit(netout);

	/* Request to do remote echo. */
	dooption(TELOPT_ECHO);
	myopts[TELOPT_ECHO] = 1;

	while(!done) {
		FD_ZERO(ibits);
		FD_ZERO(obits);

		/* process anything sitting in the input buffers */
		if (bytes_filled(ptin) > 0)
			ptprocess();
		if (bytes_filled(netin) > 0)
			netprocess();

		/* select for read only if there's room to read into */
		if (space_left(netin) > 0)
			FD_SET(netfd, ibits);
		if (space_left(ptin) > 0)
			FD_SET(ptfd, ibits);

		/* select for write only if there's something to write */
		if (bytes_filled(ptout) > 0)
			FD_SET(ptfd, obits);
		if (bytes_filled(netout) > 0)
			FD_SET(netfd, obits);

		n = select(NOFILE, &ibits, &obits, 100000);
		if (n < 0)
			break;
		else if (n == 0)
			continue;

		/* fill input buffers */
		if (FD_ISSET(netfd, ibits))
			netrcv();
		if (FD_ISSET(ptfd, ibits))
			ptrcv();

		/* flush output buffers */
		if (FD_ISSET(netfd, obits) && bytes_filled(netout) > 0)
			netflush();
		if (FD_ISSET(ptfd, obits) && bytes_filled(ptout) > 0)
			ptflush();
	}
	rmut();
	exit(0);
}


/* read from the pt */
static
ptrcv()
{
	static struct mesg m;
	static int size = 0;

	/* get the header if we don't already have one */
	if (size <= 0) {
		if (read(ptfd, (char *)&m, MSGHLEN) != MSGHLEN) {
			terminate("pt read");
			return;
		}
		size = (m.losize & 0377) + ((m.hisize & 0377) << 8);
	}
	switch(m.type) {
	case M_HANGUP:
		terminate("pt hangup");
		size=0;
		break;
	case M_DATA:
		size=datamesg(size);
		break;
	case M_IOCTL:
		ioctlmesg(size);
		size=0;
		break;
	case M_IOCACK:
		fprintf(stderr, "IOCACK\n");
		flushmesg(size);
		size=0;
		break;
	case M_IOCNAK:
		fprintf(stderr, "IOCNAK\n");
		flushmesg(size);
		size=0;
		break;
	default:
		othermesg(m, size);
		size=0;
		break;
	}
}

/* flush a message from the pt */
static
flushmesg(size)
{
	char buf[64];
	int cc;
	/* flush it */
	while (size > 0) {
		fprintf(stderr, "flush\n");
		cc = size > sizeof(buf) ? sizeof(buf) : size;
		cc = read(ptfd, buf, cc);
		if (cc < 0) {
			terminate("pt read");
			return;
		}
		size -= cc;
	}
}

/* handle an ioctl message from the pt */
static
ioctlmesg(size)
{
	struct mesg rm;
	union stmsg s;
	int cc;

	if (size > 0) {
		cc = read(ptfd, (char *)&s, size > sizeof(s) ? sizeof(s) : size);
		if (cc < 0) {
			terminate("pt read");
			return;
		}
		if (size > cc)
			flushmesg(size - cc);
	}
	rm.type = M_IOCACK;
	switch (s.ioc0.com) {

	case TIOCSETN:
	case TIOCSETP:
		size = 0;
		break;

	case TIOCGETP:
		s.ioc1.sb.sg_ispeed = B9600;
		s.ioc1.sb.sg_ospeed = B9600;
		ptb = s.ioc1.sb;	/* sic; remember what ttyld said */
		/* leave size as it was */
		break;

	default:
		rm.type = M_IOCNAK;
		size = 0;
		break;
	}
	rm.magic = MSGMAGIC;
	rm.losize = size;
	rm.hisize = size>>8;
	if (write(ptfd, (char *)&rm, MSGHLEN) != MSGHLEN) {
		terminate("pt write");
		return;
	}
	if (size > 0)
		if (write(ptfd, (char *)&s, size) != size) {
			terminate("pt write");
			return;
		}
}

/* read bytes from the pt and write to the network connection */
static
datamesg(size)
{
	int cc;

	cc = breadn(ptin, ptfd, size);
	if (cc < 0)
		terminate("pt read");
	else
		size -= cc;
	return (size);
}

/* handle an unrecognized type of message */
static
othermesg(m, size)
struct mesg m;
{
	char buf[132];
	int rcc, wcc;

	wcc = write(ptfd, (char *)&m, MSGHLEN);
	if (wcc != MSGHLEN) {
		terminate("pt write");
		return;
	}
	while (size > 0) {
		rcc = read(ptfd, buf, size > sizeof(buf) ? sizeof(buf) : size);
		if (rcc <= 0) {
			terminate("pt read");
			return;
		}
		wcc = write(ptfd, buf, rcc);
		if (wcc != rcc) {
			terminate("pt write");
			return;
		}
		size -= rcc;
	}
}

/* set pt mode */
static
mode(on, off)
	int on, off;
{
	ptflush();
	ptb.sg_flags |= on;
	ptb.sg_flags &= ~off;
	if(ioctl(ptfd, TIOCSETP, &ptb)<0)
		terminate("setting mode of pt");
}

/* flush the pt's output buffer */
ptflush()
{
	struct mesg rm;
	int size;

	if ((size = bytes_filled(ptout)) > 0) {
		rm.type = M_DATA;
		rm.magic = MSGMAGIC;
		rm.losize = size;
		rm.hisize = size>>8;
		if (write(ptfd, (char *)&rm, MSGHLEN) < MSGHLEN)
			terminate("pt write");
		else if (bwrite(ptout, ptfd) <= 0)
			terminate("pt write");
	}
}

/* process bytes from the pt */
static
ptprocess()
{
	register int c;

	while(bytes_filled(ptin) && space_left(netout) > 1) {
		c = bget(ptin);
		if (c == IAC)
			bput(netout, c);
		bput(netout, c);
	}
}

/* read bytes from the net */
static
netrcv()
{
	if (bread(netin, netfd) < 0)
		terminate("net read");
}

/* flush the netwrk's output buffer */
netflush()
{
	if (bwrite(netout, netfd) < 0)	
		terminate("net write");
}
	
/*
 * State for recv fsm
 */
#define	TS_DATA		0	/* base state */
#define	TS_IAC		1	/* look for double IAC's */
#define	TS_CR		2	/* CR-LF ->'s CR */
#define	TS_BEGINNEG	3	/* throw away begin's... */
#define	TS_ENDNEG	4	/* ...end's (suboption negotiation) */
#define	TS_WILL		5	/* will option negotiation */
#define	TS_WONT		6	/* wont " */
#define	TS_DO		7	/* do " */
#define	TS_DONT		8	/* dont " */

static
netprocess()
{
	static int state = TS_DATA;
	register int c;
	char buf[10];

	while (bytes_filled(netin)) {
		c = bget(netin);
		switch (state) {

		case TS_DATA:
			if (c == IAC) {
				state = TS_IAC;
				break;
			}
			bput(ptout, c);
			if (!myopts[TELOPT_BINARY] && c == '\r')
				state = TS_CR;
			break;

		case TS_CR:
			if (c && c != '\n')
				bput(ptout, c);
			state = TS_DATA;
			break;

		case TS_IAC:
			switch (c) {

			/*
			 * Send the process on the pty side an
			 * interrupt.  Do this with a NULL or
			 * interrupt char; depending on the tty mode.
			 */
			case BREAK:
			case IP:
				interrupt();
				break;

			/*
			 * Are You There?
			 */
			case AYT:
				bput(ptout, BELL);
				break;

			/*
			 * Erase Character and
			 * Erase Line
			 */
			case EC:
			case EL:
				ptflush();
				bput(ptout, (c == EC) ? ptb.sg_erase : ptb.sg_kill);
				break;

			/*
			 * Check for urgent data...
			 */
			case DM:
				break;

			/*
			 * Begin option subnegotiation...
			 */
			case SB:
				state = TS_BEGINNEG;
				continue;

			case WILL:
			case WONT:
			case DO:
			case DONT:
				state = TS_WILL + (c - WILL);
				continue;

			case IAC:
				bput(ptout, c);
				break;
			}
			state = TS_DATA;
			break;

		case TS_BEGINNEG:
			if (c == IAC)
				state = TS_ENDNEG;
			break;

		case TS_ENDNEG:
			state = c == SE ? TS_DATA : TS_BEGINNEG;
			break;

		case TS_WILL:
			if (!hisopts[c])
				willoption(c);
			state = TS_DATA;
			continue;

		case TS_WONT:
			if (hisopts[c])
				wontoption(c);
			state = TS_DATA;
			continue;

		case TS_DO:
			if (!myopts[c])
				dooption(c);
			state = TS_DATA;
			continue;

		case TS_DONT:
			if (myopts[c]) {
				myopts[c] = 0;
				sprintf(buf, wont, c);
				bputs(netout, buf);
			}
			state = TS_DATA;
			continue;

		default:
			printf("telnetd: panic state=%d\n", state);
			exit(1);
		}
	}
}

static
willoption(option)
	int option;
{
	char *fmt;
	char buf[10];

	switch (option) {

	case TELOPT_BINARY:
		mode(RAW, 0);
		goto common;

	case TELOPT_ECHO:
		mode(0, ECHO|CRMOD);
		/*FALL THRU*/

	case TELOPT_SGA:
	common:
		hisopts[option] = 1;
		fmt = doopt;
		break;

	case TELOPT_TM:
		fmt = dont;
		break;

	default:
		fmt = dont;
		break;
	}
	sprintf(buf, fmt, option);
	bputs(netout, buf);
}

static
wontoption(option)
	int option;
{
	char *fmt;
	char buf[10];

	switch (option) {

	case TELOPT_ECHO:
		mode(ECHO|CRMOD, 0);
		goto common;

	case TELOPT_BINARY:
		mode(0, RAW);
		/*FALL THRU*/

	case TELOPT_SGA:
	common:
		hisopts[option] = 0;
		fmt = dont;
		break;

	default:
		fmt = dont;
	}
	sprintf(buf, fmt, option);
	bputs(netout, buf);
}

static
dooption(option)
	int option;
{
	char *fmt;
	char buf[10];

	switch (option) {

	case TELOPT_TM:
		fmt = wont;
		break;

	case TELOPT_ECHO:
		mode(ECHO|CRMOD, 0);
		goto common;

	case TELOPT_BINARY:
		mode(RAW, 0);
		/*FALL THRU*/

	case TELOPT_SGA:
	common:
		fmt = will;
		break;

	default:
		fmt = wont;
		break;
	}
	sprintf(buf, fmt, option);
	bputs(netout, buf);
}

/*
 * Send interrupt to process on other side of pty.
 * If it is in raw mode, just write NULL;
 * otherwise, write intr char.
 */
static
interrupt()
{
	ptflush();
	ioctl(ptfd, TIOCGETC, &ptt);
	bput(ptout, ptb.sg_flags & RAW ? '\0' : ptt.t_intrc);
}

static
catchint()
{
	terminate("child death");
}

#include <utmp.h>

struct	utmp wtmp;
char	wtmpf[]	= "/usr/adm/wtmp";
char	utmp[] = "/etc/utmp";
#define SCPYN(a, b)	strncpy(a, b, sizeof (a))
#define SCMPN(a, b)	strncmp(a, b, sizeof (a))

static
rmut()
{
	register f;
	int found = 0;
	char *line, *dev;
	long time();

	dev = NULL;
	if (dev == NULL)
		return;
	line = dev + 5;

	DEBUG fprintf(stderr, "closing connection on %s\n", line);

	f = open(utmp, 2);
	if (f >= 0) {
		while(read(f, (char *)&wtmp, sizeof (wtmp)) == sizeof (wtmp)) {
			if (SCMPN(wtmp.ut_line, line) || wtmp.ut_name[0]==0)
				continue;
			lseek(f, -(long)sizeof (wtmp), 1);
			SCPYN(wtmp.ut_name, "");
			time(&wtmp.ut_time);
			write(f, (char *)&wtmp, sizeof (wtmp));
			found++;
		}
		close(f);
	}
	if (found) {
		f = open(wtmpf, 1);
		if (f >= 0) {
			SCPYN(wtmp.ut_line, line);
			SCPYN(wtmp.ut_name, "");
			time(&wtmp.ut_time);
			lseek(f, (long)0, 2);
			write(f, (char *)&wtmp, sizeof (wtmp));
			close(f);
		}
	}
	chown(dev, 0, 0);
	chmod(dev, 0666);
}


static
getty(f)
{
	int i;
	struct sgttyb b;
	char banner[128];
	char *envp[4];
	char **p, **passenv();
	char *whoami();

	ioctl(f, TIOCSPGRP, 0);
	signal(SIGTERM, SIG_DFL) ;
	signal(SIGPIPE, SIG_DFL) ;
	signal(SIGQUIT, SIG_DFL) ;
	signal(SIGINT, SIG_DFL) ;
	signal(SIGALRM, SIG_DFL) ;
	signal(SIGHUP, SIG_DFL) ;
	signal(SIGCHLD, SIG_DFL) ;
	for (i = 0; i < NSYSFILE; i++)
		if (i != f)
			close(i);
	for (i = 0; i < NSYSFILE; i++)
		dup(f);
	for (i=NSYSFILE; i<NOFILE; i++)
		close(i) ;
	ioctl(0, TIOCGETP, &b);
	b.sg_flags |= CRMOD|XTABS|ANYP|ECHO;
	b.sg_erase = '#';
	b.sg_kill = '@';
	ioctl(0, TIOCSETP, &b);
	p = envp;
	p = passenv(p, "CSOURCE");
	*p = (char *)0;
	sprintf(banner, "%s\n", whoami());
	write (netfd, banner, strlen(banner));
	execle("/etc/login", "login", 0, envp);
	execle("/etc/login", "login", 0, envp);
	fatalperror(2, "/etc/login", errno);
	exit(1);
}

char**
passenv(ep, var)
	char **ep;
{
	char *cp;
	char *getenv();

	cp = getenv(var);
	if(cp){
		*ep = cp-strlen(var)-1;
		ep++;
	}
	return ep;
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
