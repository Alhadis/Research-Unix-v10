#define DKMSGS 1
#include <dk.h>
#include <dkerr.h>
#include <sgtty.h>
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include "dkdial.h"

#define SUBR "/etc/dkdialsub"

int	dkverbose;
int	dkrchan;
int	dkerrno;
char	*dkerror;

static	char *msgs[] = {
	NULL,					/* NORM */
	"bad call to dk dialler",		/* ILLEG */
	"can't tell who you are",		/* NOUID */
	"remote system doesn't respond",	/* NOREP */
	"can't find dial program",		/* NODIAL */
	"can't find outgoing channel",		/* NOOCHAN */
	"can't open manager channel",		/* NOMGR */
	"can't get dial tone",			/* NODIALT */
	"DK controller system error",
	"destination busy",
	"remote node not answering",
	"destination not answering",
	"unassigned destination",
	"DK system overload",
	"server already exists",
	"permission denied by destination",
};

static char	*argbuf;
static char	**bufptr;
extern	int	rdk_ld, dkp_ld;
extern 	int	errno ;

int
tdkdial(dialstr, traffic)
int traffic ;
char * dialstr ;
{
	return (_tdkdial(1, traffic, dialstr)) ;
}

int
tdkserv(srvname, maxtraf)
int maxtraf ;
char * srvname ;
{
	return (_tdkdial(2, maxtraf, srvname)) ;
}


int
_tdkdial (type, traffic, dialstr)
int type ;
int traffic ;
char * dialstr ;
{
	register int fd, pid, rc;
	char args[100];
	char *argptrs[10];
	int status;
	struct dialout reply;
	int pipefd[2];
	register i;
	char outname[64];
	int popld ;
	int oldchdie ;
	int n3dig = 0, n2dig = 0;
	extern errno;
	static char alph[] = "0123456789abcdefghijklmnopqrstuvwxyz";

	/* establish the initial connection */
	if (traffic)
		traffic = 2;
	for (i=3; i<256; i+=2) {
		if (n3dig<3) {
			sprintf(outname, "/dev/dk/dk%d%c%d", traffic,
			   alph[i/10], i%10);
			if ((fd = open(outname, 2)) >= 0)
				break;
			if (errno==ENOENT)
				n3dig++;
		}
		if (n2dig < 5) {
			sprintf(outname, "/dev/dk/dk%c%d", alph[i/10], i%10);
			if ((fd = open(outname, 2)) >= 0)
				break;
			if (errno==ENOENT)
				++n2dig;
		}
	}
	if (fd < 0) {
		dkerrno = NOOCHAN ;
		dkerror = msgs[NOOCHAN];
		return fd;
	}
	/* Install URP now if traffic-type is 2, to permit */
	/*  window-size setting in dkdialsub */
	if (traffic==2)
		dkproto(fd, dkp_ld);
	ioctl(fd, FIOPUSHLD, &rdk_ld) ;

	/* create a pipe to the sub-process */
	if (pipe (pipefd) < 0) {
		dkerrno = NODIALT ;
		dkerror =  "Can't pipe to dialler";
		close (fd);
		return -1;
	}

	/* build the arg list for the sub-process */
	arginit (args, argptrs);
	strarg ("tdkdial");
	intarg (fd);
	intarg (pipefd[1]);
	intarg (type);
	intarg(traffic) ;
	strarg(dialstr) ;
	argend();
	oldchdie = (int)signal(SIGCHLD, SIG_IGN) ;

	/* start up a sub-process */
	switch (pid = fork()) {

		/* child */
	case 0:
		close (pipefd[0]);
		setuid (geteuid());
		execv (SUBR, argptrs);
		exit (NODIAL);

		/* error */
	case -1:
		close (pipefd[0]);
		close (pipefd[1]);
		close(fd);
		dkerrno = NODIAL ;
		dkerror = "DK dialler can't fork";
		signal(SIGCHLD, oldchdie) ;
		return -1;

		/* parent */
	default:
		close (pipefd[1]);

		/* wait for the child to complete */
		do 
			rc = wait (&status);
		while (rc > 0 && rc != pid);
		signal(SIGCHLD, oldchdie) ;
		/* figure out what happened */
		if (rc < 0 || status&0xff) {
			dkerrno = NODIAL ;
			dkerror = "DK dialler exited abnormally";
			goto bad;
		}
		/* read the reply */
		rc = read (pipefd[0], &reply, sizeof (reply));
		if (rc != sizeof (reply)) {
			dkerrno = NODIAL ;
			dkerror = "Bad reply from DK dialler";
			goto bad;
		}
		close (pipefd[0]);
		if ((status & 0xFF00) == 0) {
			ioctl(fd, FIOPOPLD, 0) ;
			dkerrno = 0 ;
			dkerror = NULL ;
			return fd;
		}
		status >>= 8;
		dkerrno = status ;
		if (status < sizeof(msgs)/sizeof(msgs[0]))
			dkerror = msgs[status];
		else {
			static char msg[32];
			sprintf(msg, "DK dial error %d", status-8);
			dkerror = msg;
		}
		goto bad;
	}
bad:
	close (pipefd[0]);
	close (fd);
	return -1;
}


tdkerrmess (s)
	register char	*s;
{
	if (dkverbose)
		write (2, s, strlen(s));
}


/*
 *	The following subroutines help build argument lists
 */

static
arginit (x, y)
	char	*x;
	char	**y;
{
	argbuf = x;
	bufptr = y;
}


static
strarg (x)
	register char	*x;
{
	*bufptr++ = x;
}


static
intarg (x)
	int	x;
{
	*bufptr++ = argbuf;
	if (x < 0)
		*argbuf++ = '-';
	else
		x = -x;
	convint (x);
	*argbuf++ = '\0';
}


static
convint (x)
	register int	x;
{
	if (x <= -10)
		convint (x / 10);
	*argbuf++ = '0' - x % 10;
}


static
argend()
{
	*bufptr++ = 0;
}


