#include <stdio.h>
#include <fio.h>
#include <ctype.h>
#include <sys/param.h>
#include <pwd.h>
#include <sys/ttyio.h>
#include <ipc.h>
#include <sys/stat.h>
#include <string.h>
#include "defs.h"
#include "load.h"
#include "msg.h"

/* structures */
struct machdef {
	struct ld	m_old;	/* previous set of load numbers */
	struct ld	m_new;	/* current set of load numbers */
	bool	m_cpnew;	/* TRUE-> a new cpu percentage exists */
	bool	m_rqnew;	/* TRUE-> a new run queue exists */
	bool	m_init;		/* first sample should not be displayed */
	char	*m_label;	/* label for this node */
	char	*m_name;	/* name of this node */
	int	m_fd;		/* stream for connection to remote machine */
};

#define MAGIC '_'	/* message introduction */

/* globals */
static int vismon;
int greyicon;
static struct machdef md[MAXMACHS];	/* connections are fd limited */
static int nm;				/* the number of machines */
static fd_set g_fd;			/* select set for the generators */
static char *blitfile;			/* binary file to load into blit */
static int deathknell;			/* true if we should die */
static char *vtty;			/* tty for writes */
static char *mailname;
#define DEBUG if (debug)

/* exported */
int debug = 0;
char *thissys;

int sendjerq();
int remote=0;

/* imports */
extern int errno;

extern int getopt();
extern char * getsysname();
extern int initload();
extern long lseek();
extern long time();
extern char *malloc();
extern unsigned int sleep();
extern char *mytty();
extern char *getlogin(), *getenv();

/* predefined */
static void die();
static int forkexec();
static void error();
static void warn();

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
	char mach[100];
	int fd;
	struct stat st, sv;

	/* nlink is a pipe aliasing hack */
	if ((p = mytty()) == NULL) {
		warn("can't find my tty\n");
		sleep(5);
	}
	else if (stat(p, &sv) < 0 || fstat(3, &st) < 0)
		warn("can't stat my tty\n");
	else if (sv.st_ino != st.st_ino || sv.st_dev != st.st_dev ||
			sv.st_nlink != st.st_nlink) {
		if (nametty(3, p) == 0)
			error("can't set my tty\n");
		/* make sure people can write there */
		chmod(p, 0222);
		vtty = p;
	}
	siginit();

	/* find my machine name */
	if ((fd=open("/etc/whoami", 0))<0 || read(fd, mach, sizeof mach - 1)<=0)
		thissys="kremvax\n";
	close(fd);
	thissys=mach;
	mach[sizeof mach - 1]='\n';
	for (p=mach; *p!='\n'; p++)
		;
	*p=0;

	ticks = getargs (ac, av);
	if (!remote)
		loadmenus();
	dotime();
	while (TRUE) {
		getinfo();
		docpu();
		if(per2mins++ >= 120/ticks){
			doconnect();
			per2mins = 0;
		}
		if(per15secs++ >= 15/ticks){
			dotime();
			doload();
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
	char buf[250];
	char *this, *next;

	FD_ZERO(fds);
	FD_SET(0, fds);
	if (select(NOFILE, (int *)&fds, (int *)0, 0) != 0) {

		/* read the command */
		if ((i = read(0, buf, sizeof(buf)))<=0)
			die();

		/* switch on command type (first letter) */
		buf[i-1] = 0;
/*		DEBUG fprintf(stderr, "command back '%s'\n", buf);
*/
		switch (buf[0]) {
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
			this = buf+1;
			do {
				char *cp, *name, *label, tbuf[250];

				/* jerqmon may return multiple 'M's if things are slow */
				if ((next=strchr(this, '\n')) != (char *)0) {
					*next++ = '\0';
					if (*next++ != 'M')
						next = (char *)0;
				}
				strcpy(tbuf, this);
				if (cp=strchr(tbuf, '|')) {
					label = tbuf;
					*cp++ = '\0';
					name = cp;
				} else
					label = name = tbuf;
				DEBUG fprintf(stderr, "Monitor request %s %s %s\n",
					this, label, name);
				for (i=0; i<nm; i++) {
					if (strcmp(name, md[i].m_name)==0 ||
					   (label && strcmp(label, md[i].m_label)==0))
						break;
				}
				if (i < nm) {
					/* remove the system */
					remove_sys(i);
				} else {
					/* add the system */
					add_sys(this);
				}
				this = next;
			} while (this);
			break;

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

	for (i = 0; i < nm; i++)
		if(md[i].m_fd<0)
			connectto(i);
}

/* look for new load information */
static int
getinfo()
{
	register int i, j, fd;
	fd_set rdfd;
	int len;
	char buf[128], *bp, *nbp;
	char noise[10], noise2[10], noise3[10];

	/* look for waiting input */
	rdfd = g_fd;
	while (select (NOFILE, (int *)&rdfd, (int *)0, 0) > 0) {

		/* find out which machine it's from */
		for (i = 0; i < nm; i++) {
			char *bp=buf, *eb=bp+sizeof(buf);

			fd = md[i].m_fd;
			if (fd < 0 || !FD_ISSET(fd, rdfd))
				continue;

			/* read the message(s) */
			DEBUG fprintf(stderr,"info for %s\n", md[i].m_name);

			if ((len = read(fd, buf, sizeof(buf)-1)) <= 0) {
				DEBUG fprintf (stderr,
					"read error %d, len=%d, closing\n",
					errno, len);
				close (fd);
				FD_CLR(fd, g_fd);
				md[i].m_fd = -1;
				continue;
			}
			/* this stuff is not robust and is not guaranteed to
			 * work over a network that does not have delimiters
			 * (such as TCP/IP). It is hoped that a read contains
			 * a full message or set of full messages.
			 */ 
			do {
				bp=strchr(buf, '\n');
				if (bp != (char *)0)
					*bp++ = '\0';
				DEBUG fprintf(stderr, "	text: %s\n", buf);
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
				strcpy(buf,bp);		/* hope this works! */
				len -= bp-buf;
			} while (len>0);
		}
		rdfd = g_fd;
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

	for (i = 0; i < nm; i++) {
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
	char *myname;

	myname = strrchr(av[0], '/');
	myname = myname == NULL ? av[0] : myname+1;
	mailname = getlogin();

	/* read the tick value and arguments */
	while (ac > 1 && av[1][0] == '-') {
		switch (av[1][1] & 0xff) {
		case 'f':
			blitfile = av[2];
			av += 2, ac -= 2;
			break;
		case 'd':
			debug = 1;
			av++, ac--;
			break;
		case 'm':
			mailname = av[2];
			av += 2, ac -= 2;
			break;
		case 'G':
			greyicon = 1;
		case 'R':
			remote = 1;
			av++, ac--;
			break;
		default:
			ticks = -atoi(av[1]);
			if (ticks < 2)
				ticks = 2;
			av++, ac--;
		}
	}

	/* initialize entries */
	for (i = 0; i<MAXMACHS; i++)
		md[i].m_fd = -1;

	/* set up the communications */
	if (!remote)
		(void)startblit();

	/* are we vismon? */
	if (strcmp(myname, "vismon") == 0) {
		sendjerq('Q', (char *)0, 0);
/*		while(!(i=scanjerq(1)))
			sleep(1);*/
		if (i > 0)
			vismon++;
	}

	/* who to watch */
	nm = 0;
	add_sys("");
	for (av++; ac > 1; ac--)
		add_sys (*av++);

	/* get symbols and things */
	return (ticks);
}

getmenufile()
{
	char *home, *menu;
	char buf[256];

	int fd;

	if ((menu = getenv("VISMON")) != NULL) {
		if((fd = open(menu, 0)) > 0) 
			return(fd);
	}
	DEBUG fprintf (stderr, "No VISMON\n");
	if ((home = getenv("HOME")) != NULL) {
		strcpy(buf, home);
		strcat(buf, "/lib/vismon");
		if((fd = open(buf, 0)) > 0) 
			return(fd);
	}
	DEBUG fprintf (stderr, "No HOME/lib/vismon\n");
	if((fd = open("/usr/jerq/lib/vismon", 0)) > 0) 
		return(fd);
	return(-1);
}

loadmenus()
{
	int fd, i, n=0;
	char buf[sizeof(Fbuffer)];
	char *p;

	if((fd=getmenufile()) < 0) {
		fprintf (stderr, "couldn't find menu file\n");
		die();
	}
	Finit(fd, buf);
	while((p=Frdline(fd)) != (char *)0) {
		DEBUG fprintf (stderr, "Menu: %s\n", p);
		sendjerq('E', p, strlen(p));
		while(!(i=scanjerq(1)));
		if (i < 0)
			break;
		n++;
	}
	Fclose(fd);
	if(n==0) {
		fprintf (stderr, "no menus in file\n");
		die();
	}

}

static int
add_sys(p)
char *p;
{
	int i;
	char *cp;

	for (i = 0; i < 5; i++)
		md[nm].m_old.l_cp[i] = 0;
	md[nm].m_cpnew = FALSE;
	md[nm].m_rqnew = FALSE;
	md[nm].m_init = TRUE;
	md[nm].m_fd = -1;
	DEBUG fprintf (stderr, "adding system %s\n", p);
	if (cp=strchr(p, '|')) {
		*cp++ = '\0';
		md[nm].m_label = (char *)strcpy(malloc(strlen(p)+1), p);
		md[nm].m_name = (char *)strcpy(malloc(strlen(cp)+1), cp);
	} else if (cp=strrchr(p, '/')) {
		md[nm].m_name = (char *)strcpy(malloc(strlen(p)+1), p);
		md[nm].m_label = (char *)strcpy(malloc(strlen(cp+1)+1), cp+1);
	} else if (cp=strchr(p, '.')) {
		md[nm].m_name = (char *)strcpy(malloc(strlen(p)+1), p);
		*cp = '\0';
		if (cp=strrchr(p, '!'))
			md[nm].m_label = (char *)strcpy(malloc(strlen(cp+1)+1), cp+1);
		else
			md[nm].m_label = md[nm].m_name;
	} else
		md[nm].m_label = md[nm].m_name = (char *)strcpy(malloc(strlen(p)+1), p);
	DEBUG fprintf (stderr, "label=%s name=%s\n", md[nm].m_label, md[nm].m_name);
	sendjerq('N', md[nm].m_label, strlen(md[nm].m_label));
	while(!(i=scanjerq(1)));
	if (i < 0)
		return;
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

	if (term != 0 && strcmp(term, "630") == 0) {
		if (blitfile == 0)
			blitfile =  "/usr/630/mbin/sysmon.m";
		DEBUG fprintf(stderr, "Downloading %s\n", blitfile);
		pid = forkexec("/usr/630/bin/dmdld", "dmdld", blitfile, (char *)0);
	} else {
		if (blitfile == 0)
			blitfile =  "/usr/jerq/mbin/sysmon.m";
		DEBUG fprintf(stderr, "Downloading %s\n", blitfile);
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
	char *path;
	char *network="dk";
	char *service="sysmon";

	senddiag("dialing", i);
	DEBUG fprintf(stderr, "Connecting to \"%s\"\n", md[i].m_name);
	if ((fd = ipcopen(path=ipcpath(md[i].m_name, network, service), "light")) > 0) {
		DEBUG fprintf (stderr, "connected to %s via %s on %d\n", md[i].m_name, path, fd);
		md[i].m_fd = fd;
		FD_SET(fd, g_fd);
		write(fd, mailname, strlen(mailname)+1);
		senddiag("connected", i);
		return;
	} else {
		md[i].m_fd = -1;
		senddiag("failed", i);
		return;
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
	char buf[1024];
	char *bp = buf;
	double fabs();

	for (i = 0; i < nm; i++) {
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


static int
sendmail(p, i)
	char *p;
	int i;
{
	char sender[64];
	char machine[64];
	char buf[132];
	char *cp;

	from (p, machine, sender);
	if (vismon)
		sendicon(machine, sender);
	buf[0] = '0' + i;
	buf[1] = '\0';
	if (*machine != '\0') {
		/* look for domain address in machine */
		if ((cp = strchr(machine, '.'))!=NULL)
			*cp = '\0';
		strcat(buf+1, machine);
		strcat(buf+1, "!");
	}
	strcat(buf+1, sender);
	buf[MAXMAIL+1] = '\0';
	sendjerq ('M', buf, strlen(buf));
}

static int
senddiag(p, i)
	char *p;
	int i;
{
	char buf[MAXMAIL+3];

	sprintf(buf, "%c%.13s\t", '0' + i, p);
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
	char buf[600];
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
/*	DEBUG fprintf(stderr, "sending -> ");
	DEBUG fwrite(buf, 1, bp-buf, stderr);
*/
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

/* cleanup and exit */
static void
die()
{
	if (vtty)
		funmount(vtty);
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
static void
warn (s)
char *s;
{
	fprintf(stderr, "sysmon: %s\n", s);
}

/*
 * not really needed, save for close bug on old systems, to unmount
 */

siginit()
{
	signal(SIGHUP, die);
	signal(SIGTERM, die);
	signal(SIGPIPE, die);
}

