#include <stdio.h>
#include <ctype.h>
#include <sys/param.h>
#include <sys/types.h>
#include <pwd.h>
#include <signal.h>
#include <sgtty.h>
#include "defs.h"
#include "load.h"
#include "msg.h"
#include "scanmail.h"

/* structures */
struct machdef {
	struct ld	m_old;	/* previous set of poal numbers */
	struct ld	m_new;	/* current set of load numbers */
	bool	m_cpnew;	/* TRUE-> a new cpu percentage exists */
	bool	m_rqnew;	/* TRUE-> a new run queue exists */
	bool	m_init;		/* first sample should not be displayed */
	char	*m_name;	/* name of this node */
	int	m_fd;		/* stream for connection to remote machine */
};

#define MAGIC '_'	/* message introduction */

/* globals */
static int vismon;
static struct scanmail sm;		/* mail for local machine */
static struct machdef md[MAXMACHS];	/* connections are fd limited */
static int local;			/* offset into md of local machine */
static int nm;				/* the number of machines */
static fd_set g_fd;			/* select set for the generators */
static char *blitfile;			/* binary file to load into blit */
static int deathknell;			/* true if we should die */
#define DEBUG if (debug)

/* exported */
int debug = 0;
int onlymail = 0;
int lucaversion = 0;
char *thissys;

int sendjerq();

/* imports */
extern int errno;

extern int getopt();
extern char * getsysname();
extern int initload();
extern int genload();
extern void sminit();
extern char *smnext();
extern char *strrchr(), *strncpy(), *strcpy();
extern long lseek();
extern long time();
extern char *malloc();
extern unsigned int sleep();
extern int getutmp();
extern int setutmp();
extern void resetutmp();
extern char *getlogin(), *getenv();

/* predefined */
static void die();
static int catchint();
static int forkexec();
static void error();

/*
	"user",
	"nice",
	"sys",
	"queue",
	"idle",
*/

main(ac, av)
int ac;
char *av[];
{
	int ticks;
	int per15secs = 15;
	int permin = 60;
	int per2mins = 120;
	char *p;

	/* get the info for a utmp entry */
	if (getutmp() < 0)
		error("can't get login or terminal name");

	/* make sure we clean up the utmp entry on termination */
	signal (SIGPIPE, catchint);
	signal (SIGHUP, catchint);
	signal (SIGINT, catchint);
	signal (SIGQUIT, catchint);
	signal (SIGKILL, catchint);

	/* put an entry into utmp */
	if (setutmp() < 0)
		error("can't set utmp entry");

	/* initialize load gathering */
	initload();

	/* initialize mail spying */
	thissys = getsysname();
	p = getenv ("MAIL");
	sminit (&sm, p, getlogin());

	ticks = getargs (ac, av);
	dotime();
	while (TRUE) {
		getinfo();
		docpu();
		if(per2mins++ >= 120/ticks){
			doconnect();
			per2mins = 0;
		}
		if(permin++ >= 60/ticks){
			dotime();
			doload();
			permin = 0;
		}
		if(per15secs++ >= 15/ticks){
			domail();
			per15secs = 0;
		}
		scanjerq(0);
		sleep(ticks);
	}
}

/*
 *	Get a request from the jerq.  Return
 *		0 	if no request or a request other than R or A
 *		1	if request is A
 *		-1	if request is R
 */
static int
scanjerq(replyflag)
	int replyflag;		/* non-zero to indicate waiting for reply */
{
	int i;
	fd_set fds;
	char name[64];
	char *np, *np2;

	FD_ZERO(fds);
	FD_SET(0, fds);
	if (select(NOFILE, (int *)&fds, (int *)0, 0) != 0) {

		/* read the command */
		if ((i = read(0, name, sizeof(name)))<=0)
			die();

		/* switch on command type (first letter) */
		name[i-1] = 0;
		switch (name[0]) {
		case 'A':
			/* accept */
			return 1;

		case 'R':
			/* refuse */
			return -1;

		case 'Q':
			/* quit */
			die();

		case 'M':
			/* monitor request, are we already monitoring it ? */
			if (replyflag)
				break;
			np = strrchr(name, '/');
			np = np==NULL ? name+1 : np+1;
			for (i=0; i<nm; i++) {
				np2 = strrchr(md[i].m_name, '/');
				np2 = np2==NULL ? md[i].m_name : np2+1;
				if (strcmp(np, np2)==0)
					break;
			}
			if (i < nm) {
				/* remove the system */
				remove_sys(i);
			} else {
				/* add the system */
				add_sys(name+1);
			}

		default:
			/* Oh well */
			break;
		}
	}
	return 0;
}

/* try to connect unconnected generators */
static int
doconnect ()
{
	register int i;

	/* don't connect to local system (i = 0) */
	for (i = 0; i < nm; i++)
		if (md[i].m_fd < 0 && i != local)
			connectto(i);
}

/* look for new load information */
static int
getinfo()
{
	register int i, fd;
	fd_set rdfd;
	int len;
	char buf[128];
	char noise[10], noise2[10], noise3[10];

	/* look for waiting input */
	rdfd = g_fd;
	while (select (NOFILE, (int *)&rdfd, (int *)0, 0) > 0) {

		/* find out which machine it's from */
		for (i = 0; i < nm; i++) {
			fd = md[i].m_fd;
			if (fd < 0 || !FD_ISSET(fd, rdfd))
				continue;

			/* read the message */
			DEBUG fprintf(stderr,"info for %s\n", md[i].m_name);
			if ((len = read(fd, buf, sizeof(buf))) <= 0) {
				DEBUG fprintf (stderr, "closing\n");
				close (fd);
				FD_CLR(fd, g_fd);
				md[i].m_fd = -1;
				continue;
			}
			if (buf[len-1] == '\n')
				buf[len-1] = '\0';

			/* switch on message type */
			switch (buf[0]) {
			case 'l':
				sscanf(buf,"%s%f%s%d%s%d%d%d%d%d",
				  noise, &md[i].m_new.l_runq,
				  noise2, &md[i].m_new.l_time,
				  noise3, &md[i].m_new.l_cp[0],
				  &md[i].m_new.l_cp[1],
				  &md[i].m_new.l_cp[2],
				  &md[i].m_new.l_cp[3],
				  &md[i].m_new.l_cp[4]);
				md[i].m_cpnew = md[i].m_rqnew = TRUE;
				break;
			case 'f':
				sendmail (buf+5, i);
				break;
			}
		}
		rdfd = g_fd;
	}

	/* get local info */
	if (local >= 0) {
		genload();
		md[local].m_new = load;
		md[local].m_cpnew = md[local].m_rqnew = TRUE;
	}
}

/* display CPU percentages */
char vec[NOFILE*6 + 3];
static int
docpu()
{
	register int i, j, sum;
	char *vp = vec;
	int diff[5];
	long l;

	for (i = 0; i < (onlymail ? 1 : nm); i++) {
		if (md[i].m_cpnew) {
			l = md[i].m_new.l_cp[3];
			md[i].m_new.l_cp[3] = md[i].m_new.l_cp[4];
			md[i].m_new.l_cp[4] = l;
			*vp++ = i + '0';
			for (sum=j=0; j<5; j++)
				sum += (diff[j] = (md[i].m_new.l_cp[j] - md[i].m_old.l_cp[j]));
			sum = sum ? sum : 1;
			for (j=0; j<5; j++) {
				*vp++ = (diff[j] * 100) / sum;
				md[i].m_old.l_cp[j] = md[i].m_new.l_cp[j];
			}
			if (md[i].m_init) {
				md[i].m_init = FALSE;
				vp -= 6;
			}
			md[i].m_cpnew = FALSE;
		}
	}
	if (vp != vec)
		sendjerq('V', vec, vp - vec);
}


/* read the arguments */
static int
getargs (ac, av)
int ac;
char *av[];
{
	int i;
	int ticks = 5;
	char *myname, *p;

	myname = strrchr(av[0], '/');
	myname = myname == NULL ? av[0] : myname+1;

	/* read the tick value and arguments */
	if (ac > 1 && av[1][0] == '-') {
		switch (av[1][1] & 0xff) {
		case 'f':
			blitfile = av[2];
			av++, ac--;
			break;
		case 'd':
			debug = 1;
			break;
		case 'm':
			onlymail = 1;
			break;
		case 'l':
			lucaversion = 1;
			break;
		default:
			ticks = -atoi(av[1]);
			if (ticks < 2)
				ticks = 2;
		}
		av++, ac--;
	}

	/* initialize entries */
	for (i = 0; i<MAXMACHS; i++)
		md[i].m_fd = -1;

	/* set up the communications */
	if (!debug)
		(void)startblit();

	/* are we vismon? */
	if (strcmp(myname, "vismon") == 0) {
		sendjerq('Q', (char *)0, 0);
		while(!(i=scanjerq(1)));
		if (i > 0)
			vismon++;
	}

	/* who to watch */
	for (nm = 0, p = getsysname(); ac > 0; av++, ac--, p = *av) {
		add_sys (p);
	}

	/* get symbols and things */
	return (ticks);
}

static int
add_sys(p)
char *p;
{
	int i;

	md[nm].m_name = (char *)strcpy(malloc(strlen(p)+1), p);
	for (i = 0; i < 5; i++)
		md[nm].m_old.l_cp[i] = 0;
	md[nm].m_cpnew = FALSE;
	md[nm].m_rqnew = FALSE;
	md[nm].m_init = TRUE;
	md[nm].m_fd = -1;
	if (!onlymail || nm == 0) {
		p = strrchr(p, '/');
		p = p==NULL ? md[nm].m_name : p+1;
		sendjerq('N', p, strlen(p));
		while(!(i=scanjerq(1)));
		if (i < 0)
			return;
	}
	if (nm != local)
		connectto(nm);
	nm++;
}

static int
remove_sys(i)
int i;
{
	char which;

	/* tell jerq to forget it */
	which = i + '0';
	sendjerq('R', &which, sizeof(which));

	/* special case for local machine */
	if (local == i)
		local = -1;

	/* close connection to server */
	if (md[i].m_fd >= 0) {
		close(md[i].m_fd);
		FD_CLR(md[i].m_fd, g_fd);
	}

	/* compact what's left */
	for (; i < nm; i++)
		md[i] = md[i+1];
	--nm;
}

static int
startblit()
{
	char *getenv();
	char *term = getenv("TERM");
	int status;
	int pid;
	struct sgttyb sttybuf, savebuf;

	ioctl(0, TIOCGETP, &savebuf);
	sttybuf = savebuf;
	sttybuf.sg_flags|=RAW;
	sttybuf.sg_flags&=~ECHO;
	ioctl(0, TIOCSETP, &sttybuf);

	if (term != 0 && strcmp(term, "blit") == 0) {
		if (blitfile == 0)
			blitfile = lucaversion ? "/usr/blit/mbin/lucamon.m"
					: "/usr/blit/mbin/sysmon.m";
		pid = forkexec("/usr/blit/bin/68ld", "68ld", blitfile, (char *)0);
	} else {
		if (blitfile == 0)
			blitfile = lucaversion ? "/usr/jerq/mbin/lucamon.m"
					: "/usr/jerq/mbin/sysmon.m";
		pid = forkexec("/usr/jerq/bin/32ld", "32ld", blitfile, (char *)0);
	}
	if (pid == -1) {
		fprintf (stderr, "couldn't load jerq\n", status>>8);
		die();
	}

	/* wait for loading to finish */
	while (wait(&status) != pid)
		;
	if (status != 0) {
		ioctl(0, TIOCSETP, &savebuf);
		error ("jerq load returns error status");
	}
}

static int
connectto (i)
int i;
{
	int fd;

	senddiag("dialing", i);
	if ((fd = calldaemon (md[i].m_name, "mon")) <= 0) {
		md[i].m_fd = -1;
		senddiag("failed", i);
	} else {
		DEBUG fprintf (stderr, "connected to %s on %d\n", md[i].m_name, fd);
		md[i].m_fd = fd;
		FD_SET(fd, g_fd);
		senddiag("connected", i);
	}
}

char timevec[]="T01234\n";
static int
dotime(){
	char *p, *ctime();
	long l;

	l = time ((long *)0);
	p = ctime(&l);
	sendjerq ('T', p+11, 5);
}


/* send the new run queue to the blit */
static int
doload()
{
	register int i;
	char buf[132];
	char *bp = buf;
	double fabs();

	for (i = 0; i < (onlymail ? 1 : nm); i++) {
		if (md[i].m_rqnew) {
			sprintf(bp, "%c %.2f %c%.2f\t", i + '0', md[i].m_new.l_runq,
					"-+"[md[i].m_new.l_runq>md[i].m_old.l_runq],
					fabs(md[i].m_new.l_runq-md[i].m_old.l_runq));
			bp += strlen(bp);
			md[i].m_old.l_runq = md[i].m_new.l_runq;
			md[i].m_rqnew = FALSE;
		}
	}
	if (bp != buf)
		sendjerq ('L', buf, bp-buf);
}

/*
 *	mail stuff
 */
static int
domail ()
{
	char *p;

	if (local < 0)
		return;

	/* check local mail */
	while ((p = smnext (&sm, (FILE *)NULL)) != NULL)
		sendmail (p, local);
}

static int
sendmail(p, i)
	char *p;
	int i;
{
	char sender[64];
	char machine[64];
	char buf[132];

	from (p, machine, sender);
	buf[0] = '0' + (onlymail ? 0 : i);
	buf[1] = '\0';
	if (*machine != '\0') {
		strcat(buf+1, machine);
		strcat(buf+1, "!");
	}
	strcat(buf+1, sender);
	buf[MAXMAIL+1] = '\0';
	sendjerq ('M', buf, strlen(buf));
	if (vismon)
		sendicon(machine, sender);
}

static int
senddiag(p, i)
	char *p;
	int i;
{
	char buf[MAXMAIL+3];

	buf[0] = '0' + (onlymail ? 0 : i);
	strncpy(buf+1, p, MAXMAIL);
	buf[MAXMAIL+1] = '\0';
	sendjerq ('L', buf, strlen(buf));
}

/*
 *	send a message to the jerq
 */
extern int
sendjerq(type, msg, len)
char type;		/* message type */
char *msg;		/* a null terminated message */
{
	char buf[300];
	char *bp = buf;
	int n;

	*bp++ = MAGIC;
	*bp++ = type;
	for (; len > 0; len--) {
		if (*msg == '\n' || *msg == '\\')
			*bp++ = '\\';
		*bp++ = *msg++;
	}
	*bp++ = '\n';
	n = write (1, buf, bp-buf);
	if (n != bp-buf)
		error("display terminated abnormally");
}

static int
forkexec(a1, a2, a3, a4)
char *a1, *a2, *a3, *a4;
{
	int pid;

	switch (pid = fork()) {
	case 0:
			execl (a1, a2, a3, a4);
			error ("can't execl");
	case -1:
			fprintf(stderr, "out of processes");
	}
	return pid;
}

/* catch interrupts and exit */
static int
catchint()
{
	die();
}

/* cleanup and exit */
static void
die()
{
	signal (SIGPIPE, SIG_IGN);
	signal (SIGALRM, SIG_IGN);
	signal (SIGHUP, SIG_IGN);
	signal (SIGINT, SIG_IGN);
	signal (SIGQUIT, SIG_IGN);
	signal (SIGKILL, SIG_IGN);
	resetutmp();
	exit (0);
}

/* error messges */
static void
error (s)
char *s;
{
	fprintf(stderr, "sysmon: %s\n", s);
	die();
}
