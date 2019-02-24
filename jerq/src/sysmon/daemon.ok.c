#include <stdio.h>
/*
 *	The following code implements a daemon that "feeds" information to
 *	other processes over datakit.  	It handles the multiplexing
 *	and expects the following routines to perform the data collection
 *	and distribution:
 *		init() - initialize data collectin
 *		generate() - generate a new set of data
 *		sendinfo(fd) - send the information on the given fd
 *		myname() - return the name this daemon should use
 *		add(fd, who) - tell application that a new cleint exists
 *		drop(fd) - tell it that that client has disappeared
 */		
#include <errno.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dkmgr.h>
#include <ctype.h>
#include <sys/stat.h>
#include <signal.h>
#include "defs.h"

/* globals */
#define LIFETIME 24*60*60	/* secs to live since first started */
#define LIFESECS 1800		/* secs of lifetime while no clients exist */
#define RESENDSECS  5		/* secs between resends */
#define RESTART 20		/* minutes between restarts */
#define DEBUG ;
#define LOGDIR "/tmp/"

char *sysname;			/* name of system */
char *oursrv;			/* name of this server */
int cfd_owait[NOFILE];		/* TRUE if waiting for output to complete */
fd_set cfds;			/* set of client fd's */
char errbuf[128];
bool solo;			/* TRUE if started by a shell */
int clients;			/* number of clients */

/* imports */
extern int errno;
extern int dkp_ld;
extern int dkmgropen;
extern int dkmgrreply;
char * malloc();
char * realloc();
char * strcpy();

/* predefined */
char * ealloc();
char * sname();
char * getsysname();
int    ding();

/* specific to generator */
int generate();			/* generate information */
int sendinfo();
char *myname();
int init();
void add();
void drop();

paranoia()
{
	exit(1);
}

main (argc, argv)
int argc;
char *argv[];
{
	struct mgrmsg *dkmgr(), *mp;
	fd_set rfds, wfds;
	int fds, fd, mask;
	long deathtime, lonelytime, now;
	int (*sigfunc)();

	FD_ZERO(rfds); FD_ZERO(wfds);
	doargs(argc, argv);
	detach();
	init();			/* application dependent init */
	lonelytime = time((long *)0) + 2*LIFESECS;
	deathtime = time((long *)0) + LIFETIME;

	while (1) {

		/* generate the new information */
		generate();

		/* only read if there is a request waiting */
		if (FD_ISSET(dkmgropen, rfds)) {
			mp = dkmgr(oursrv, 0);
			if (mp != NULL)
				newclient (mp->m_chan, mp->m_uid);
		}

		/* send information to clients (if any) */
		sigfunc = signal(SIGALRM, paranoia);
		alarm(60);
		for (fd = 0; fd < NOFILE; fd++) {
			if (FD_ISSET(fd, wfds))
				cfd_owait[fd] = FALSE;
			if (FD_ISSET(fd, cfds) && !cfd_owait[fd]) {
				if (sendinfo (fd) < 0)
					dropclient (fd);
				else
					cfd_owait[fd] = TRUE;
			}
		}
		now = time((long *)0);
		alarm(0);
		signal(SIGALRM, sigfunc);

		/* is anyone out there? */
		if (clients != 0)
				lonelytime = time((long *)0) + LIFESECS;
		else if (now > lonelytime)
				die("life ain't worth livin");

		/* time to reset? */
		if (now > deathtime)
			die("reached retirement age");

		/* scan for request from dk or child termination */
		FD_SET(dkmgropen, rfds);
		if (mp != NULL) {
			wfds = cfds;
			fds = select (20, &rfds, &wfds, 0);
		} else {
			FD_ZERO(wfds);
		}

		/* don't hog the CPU */
		sleep (RESENDSECS);
	}
}

/* scan the arguments and pick a service name */
doargs (argc, argv)
	int argc;
	char *argv[];
{
	int fd;

	if (argc > 1) {
		solo = FALSE;
		oursrv = argv[1];
	} else if ((sysname = getsysname()) != NULL) {
		solo = TRUE;
		oursrv = sname(sysname, myname());
		oursrv = strcpy (ealloc (strlen (oursrv) + 1), oursrv);
	} else {
		sprintf (errbuf, "%s: cant's find /etc/whoami", oursrv);
		perror (errbuf);
		exit (-1);
	}
}

/* detach from our environment */
detach ()
{
	int i;
	char logfile[64];

	/* detach from the process group */	
	setpgrp (0, 0);

	/* ignore some signals */
	signal(SIGPIPE, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGTERM, SIG_IGN);

	/* detach from old i/o */
	switch (fork()) {
	case -1:
		perror ("gen: couldn't fork");
		exit (-1);
	case 0:
		for (i = 0; i < NOFILE; i++)
			close (i);
		i = open ("/dev/null", 0);
		strcpy (logfile, LOGDIR);
		strcat (logfile, myname());
		strcat (logfile, ".log");
		i = creat (logfile, 0666);
		i = dup (1);
		break;
	default:
		_exit (0);
	}
}

/* add client to the list */
newclient(chan, who)
int chan;		/* dkchannel */
char * who;		/* client */
{
	int fd, i, rv;
	char *devname, *dkfilename();

	/* open the line to the client */
	devname = dkfilename(chan);
	fd = open (devname, 2);
	if (fd < 0) {
		perror (oursrv);
		dkmgrnak (chan);
		return;
	}
	if (dkproto(fd, dkp_ld) < 0) {
		(void)close (fd);
		perror (oursrv);
		dkmgrnak (chan);
		return;
	}
	dkmgrack(chan);

	/* channel is open, add client to fdlist */
	FD_SET(fd, cfds);

	/* tell application about it */
	add (fd, who);
	clients++;

	log("new client %s %d", who, fd);
}


dropclient (fd)
int fd;
{
	/* do our bookkeeping */
	FD_CLR(fd, cfds);
	close (fd);

	/* tell application about it */
	drop (fd);
	clients--;

	log("drop client %d", fd);
}

die(s)
char *s;
{
	log(s);
	exit(0);
}

log(p1, p2, p3, p4, p5)
int p1, p2, p3, p4, p5;
{
	fprintf(stderr, "[%d] ", time((long *)0));
	fprintf(stderr, p1, p2, p3, p4, p5);
	fprintf(stderr, "\n");
}
