#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <ipc.h>
#include <errno.h>
#include <libc.h>
#include <pwd.h>
#include <grp.h>

typedef struct {
	char *address;
	char *arg;
	char *mpoint;
	char *callusr;
	int rootino;
	dev_t rootdev;
	char proto;
	char stat;
	char devnum;
	char debug;
	int retry;
} Friend;

#define	SOK	01	/* filesystem running ok */
#define	SFOUND	02	/* found in most recent friends pass */

#define	MAXRETRY	60	/* slow down after this many retries */
#define	LONGRETRY	20	/* after slowdown, retry after this many cycles */

#define	MAXFRIENDS 128
Friend friends[MAXFRIENDS];	/* should probably be dynamic */

#define	MAXMSG	5	/* largest message 5*MSGBLOCK -- known to kernel too */
#define	MSGBLOCK 1024

#define	NBFS	4	/* our filesystem type */

char frfile[] = "/usr/netb/friends";	/* should be an argument */

char *estr();
Friend *findmpoint();

main(argc, argv)
int argc;
char **argv;
{
	Friend f;
	int fd;

	if (argc <= 1) {
		mkdaemon();
		rundaemon();
	}
	if (argc < 6 || argc > 8) {
		fprintf(stderr, "usage: setup netaddr arg mountpoint protocol devnum [debug userid]\n");
		exit(1);
	}
	f.address = argv[1];
	f.arg = argv[2];
	f.mpoint = argv[3];
	f.proto = argv[4][0];
	f.devnum = atoi(argv[5]);
	if (argc <= 6)
		f.debug = 0;
	else
		f.debug = atoi(argv[6]);
	if (argc <= 7)
		f.callusr = "daemon";
	else
		f.callusr = argv[7];
	if ((fd = callsys(&f)) < 0) {
		fprintf(stderr, "%s %s %c %s: %s\n", f.address, f.arg, f.proto, f.callusr, errstr);
		exit(1);
	}
	if (setup(fd, &f) < 0)
		exit(1);
	exit(0);
}

nulltrap(s)
{
	signal(s, nulltrap);
}

/*
 * prepare to be a daemon
 */

mkdaemon()
{
	switch (fork()) {
	case -1:
		perror("fork");
		exit(1);
	default:	/* parent */
		exit(0);
	case 0:		/* child */
		break;
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);	/* needed? */
	signal(SIGALRM, nulltrap);	/* miscellaneous alarms */
}

/*
 * the daemon:
 * once a minute,
 *	see if the filesystems we've already mounted are ok
 *	see if the friends file has changed, and read it if so
 *	try to mount any filesystem that isn't
 */

rundaemon()
{
	static time_t fmtime;
	struct stat st;
	register Friend *fp;
	int fd;

	for (;; sleep(60)) {
		for (fp = friends; fp < &friends[MAXFRIENDS]; fp++) {
			if (fp->address == NULL || (fp->stat & SOK) == 0)
				continue;
			alarm(60);
			if (stat(fp->mpoint, &st) < 0) {
				plog("stat %s: %s\n", fp->mpoint, estr());
				fp->stat &=~ SOK;
			} else if (st.st_dev != fp->rootdev
			       ||  st.st_ino != fp->rootino) {
				plog("%s: not root: want 0x%x:%d, have 0x%x:%d\n",
				  fp->mpoint, fp->rootdev, fp->rootino,
				  st.st_dev, st.st_ino);
				fp->stat &=~ SOK;
			}
			alarm(0);
		}
		if (stat(frfile, &st) < 0)
			plog("stat %s: %s\n", frfile, estr());
		else if (fmtime != st.st_mtime) {
			fmtime = st.st_mtime;
			newfriends();
		}
		for (fp = friends; fp < &friends[MAXFRIENDS]; fp++) {
			if (fp->address == NULL || fp->stat & SOK)
				continue;
			if (fp->retry > MAXRETRY) {
				if (fp->retry < MAXRETRY+LONGRETRY) {
					fp->retry++;
					continue;	/* skip this time */
				}
				fp->retry = MAXRETRY;	/* do this one, then skip */
			}
			plog("%s: calling %s %s %c %s\n",
				fp->mpoint, fp->address, fp->arg, fp->proto, fp->callusr);
			if ((fd = callsys(fp)) < 0) {
				plog("%s: %s\n", fp->address, errstr);
				fp->retry++;
				continue;
			}
			if (setup(fd, fp) < 0) {
				plog("%s: setup failed\n", fp->mpoint);
				alarm(30);
				close(fd);
				alarm(0);
				fp->retry++;
				continue;
			}
			close(fd);
			plog("%s started\n", fp->mpoint);
			fp->stat |= SOK;
			fp->retry = 0;
		}
	}
}

/*
 * reread the friends file
 * the algorithm is quadratic,
 * but we don't do it often
 */

#define	ASIZE	8
newfriends()
{
	FILE *f;
	char line[512];		/* huge */
	char *args[ASIZE];
	char proto, devnum, debug;
	char *callusr;
	register Friend *fp;

	if ((f = fopen(frfile, "r")) == NULL) {
		plog("can't open %s: %s\n", frfile, estr());
		return;
	}
	setfields(" \t\n");
	for (fp = friends; fp < &friends[MAXFRIENDS]; fp++)
		fp->stat &=~ SFOUND;
	while (fgets(line, sizeof(line), f)) {
		if (line[0] == '#')
			continue;
		/*
		 * backward compatibility:
		 * login name may be omitted
		 */
		switch (getmfields(line, args, ASIZE)) {
		default:
			continue;

		case 6:
			callusr = "daemon";
			break;

		case 7:
			callusr = args[6];
			break;
		}
		proto = args[3][0];
		devnum = atoi(args[4]);
		debug = atoi(args[5]);
		fp = findmpoint(args[2]);
		if (fp->address			/* didn't return an empty */
		&&  strcmp(fp->address, args[0]) == 0
		&&  strcmp(fp->arg, args[1]) == 0
		/* mount point already checked */
		&&  strcmp(fp->callusr, callusr) == 0
		&&  fp->proto == proto
		&&  fp->devnum == devnum
		&&  fp->debug == debug) {
			fp->stat |= SFOUND;
			fp->retry = 0;
			continue;		/* it hasn't changed */
		}
		if (fp->address) {		/* was already there, clear it */
			plog("changing %s\n", fp->mpoint);
			free(fp->address);
			free(fp->arg);
			free(fp->mpoint);
			free(fp->callusr);
		}
		fp->address = strdup(args[0]);
		fp->arg = strdup(args[1]);
		fp->mpoint = strdup(args[2]);
		fp->callusr = strdup(callusr);
		fp->proto = proto;
		fp->devnum = devnum;
		fp->debug = debug;
		fp->stat = SFOUND;	/* nb OK not set yet */
		fp->retry = 0;
	}
	fclose(f);
	for (fp = friends; fp < &friends[MAXFRIENDS]; fp++) {
		if (fp->address == NULL || fp->stat & SFOUND)
			continue;
		plog("dropping %s\n", fp->mpoint);
		free(fp->address);
		free(fp->arg);
		free(fp->mpoint);
		fp->address = NULL;
	}
}

Friend *
findmpoint(mp)
char *mp;
{
	register Friend *fp, *ep;
	static Friend junkfriend;

	ep = NULL;
	for (fp = friends; fp < &friends[MAXFRIENDS]; fp++) {
		if (fp->address == NULL) {
			if (ep == NULL)
				ep = fp;
			continue;
		}
		if (strcmp(fp->mpoint, mp) == 0)
			return (fp);
	}
	if (ep)
		return (ep);
	plog("too many friends; no room for %s\n", mp);
	return (&junkfriend);
}

/*
 * call a server
 * according to network protocol
 */

callsys(fp)
register Friend *fp;
{
	register int fd;

	alarm(120);
	setlogname(fp->callusr);	/* who the network thinks we are */
	switch (fp->proto) {
	case 'd':	/* `datakit' -- general connection */
		fd = ipcopen(ipcpath(fp->address, "dk", "fsb"), "heavy hup");
		break;

	case 'e':	/* same, but use rexec -- why bother? */
		fd = ipcexec(ipcpath(fp->address, "dk", "exec"), "heavy hup", fp->arg);
		break;

	case 't':
		fd = ipcrexec(fp->address, "heavy hup", fp->arg);
		break;

	default:
		errstr = "unknown setup protocol";
		fd = -1;
		break;
	}
	alarm(0);
	return (fd);
}

/*
 * initial protocol with server:
 * client sends 16 bytes:
 *	max buffer size in 1024-byte units
 *	device number in use
 *	protocol letter
 *	debugging flag
 *	12 unused
 * server sends one byte: 1
 * client sends a list of userids: maxsize bytes
 *	if list won't fit in one buffer, last username is `:'
 *	server acks with one byte 012; client sends more
 * server sends one byte: 2
 * client sends a list of groupids: maxsize bytes
 *	if list won't fit in one buffer, last groupname is `:'
 *	server acks with one byte 013; client sends more
 * server sends one byte: 3
 * then we mount it, and further messages come from the kernel
 */

#define	M1LEN	16

setup(fd, fp)
int fd;
register Friend *fp;
{
	char m[M1LEN];
	register int n;
	struct stat st;

	memset(m, 0, M1LEN);
	m[0] = MAXMSG;
	m[1] = fp->devnum;
	m[2] = fp->proto;
	if (m[2] == 'e')
		m[2] = 'd';	/* hack */
	m[3] = fp->debug;
	alarm(600);
	if ((n = write(fd, m, M1LEN)) != M1LEN) {
		plog("%s setup 1: write returned %d; %s\n", fp->mpoint, n, estr());
		alarm(0);
		return (-1);
	}
	if (getresp(fd, 1, fp->mpoint) == 0)
		return (-1);
	alarm(600);
	if (senduid(fd, fp->mpoint) < 0) {
		alarm(0);
		return (-1);
	}
	alarm(600);
	if (sendgid(fd, fp->mpoint) < 0) {
		alarm(0);
		return (-1);
	}
	alarm(60);
	funmount(fp->mpoint);	/* bad -- might unmount wrong fs */
	if (fmount(NBFS, fd, fp->mpoint, makedev(fp->devnum, 0)) < 0) {
		plog("%s: fmount: %s\n", fp->mpoint, estr());
		alarm(0);
		return (-1);
	}
	alarm(60);
	if (stat(fp->mpoint, &st) < 0) {
		plog("%s: initial root stat: %s\n", fp->mpoint, estr());
		alarm(0);
		return (-1);
	}
	alarm(0);
	fp->rootino = st.st_ino;
	fp->rootdev = st.st_dev;
	return (0);
}

/*
 * get a single-byte response from server
 * try to cope with unexpected noise,
 * usually something like /etc/motd or `You have mail.'
 * fortunately server responses are all characters
 * unlikely to appear in ordinary ASCII text (viz. octal 1 2 3).
 * it is more important to copy the junk to the logfile
 * (so someone can get rid of it) than to survive it
 */

getresp(fd, id, fs)
int fd, id;
char *fs;
{
	register int i, n;
	char m[400];
	int tries, tmo;

	tmo = 600;		/* timeout */
	for (tries = 0; tries < 5 && tmo > 0; tries++) {
		alarm(tmo);
		n = read(fd, m, sizeof(m)-1);
		tmo = alarm(0);		/* assume errno untouched */
		m[n] = 0;
		if (n <= 0) {
			plog("%s setup %d: read returned %d: %s\n", fs, id, n, estr());
			return (0);
		}
		for (i = 0; i < n; i++)
			if (m[i] == id)
				break;
		if (i) {
			m[i] = 0;
			plog("%s setup %d: read junk: %s\n", fs, id, m);
		}
		if (i < n)		/* found it */
			return (1);
	}
	plog("%s setup %d: no response\n", fs, id);
	return (0);
}

#define	SLOP	50

senduid(fd, mpoint)
int fd;
char *mpoint;
{
	char idbuf[MAXMSG*MSGBLOCK+SLOP];
	char onebuf[SLOP];
	register char *p;
	register struct passwd *pw;
	register int n, wn;

	setpwent();
	p = idbuf;
	while ((pw = getpwent()) != NULL) {
		sprintf(onebuf, "%s %d\n", pw->pw_name, pw->pw_uid);
		n = strlen(onebuf);
		if (n > &idbuf[MAXMSG*MSGBLOCK] - p - 4) { /* room for ": 1\n" */
			strcpy(p, ": 1\n");	/* `more coming' */
			if ((wn = write(fd, idbuf, MAXMSG*MSGBLOCK)) != MAXMSG*MSGBLOCK) {
				plog("%s: write uid: %d; %s\n", mpoint, wn, estr());
				return (-1);
			}
			if (getresp(fd, 012, mpoint) == 0)
				return (-1);
			p = idbuf;
		}
		strcpy(p, onebuf);
		p += n;
	}
	endpwent();
	if ((wn = write(fd, idbuf, MAXMSG*MSGBLOCK)) != MAXMSG*MSGBLOCK) {
		plog("%s: write uid: %d; %s\n", mpoint, wn, estr());
		return (-1);
	}
	if (getresp(fd, 2, mpoint) == 0)
		return (-1);
	return (0);
}

sendgid(fd, mpoint)
int fd;
char *mpoint;
{
	char idbuf[MAXMSG*MSGBLOCK+SLOP];
	char onebuf[SLOP];
	register char *p;
	register struct group *gr;
	register int n, wn;

	setgrent();
	p = idbuf;
	while ((gr = getgrent()) != NULL) {
		sprintf(onebuf, "%s %d\n", gr->gr_name, gr->gr_gid);
		n = strlen(onebuf);
		if (n > &idbuf[MAXMSG*MSGBLOCK] - p - 4) { /* room for ": 1\n" */
			strcpy(p, ": 1\n");	/* `more coming' */
			if ((wn = write(fd, idbuf, MAXMSG*MSGBLOCK)) != MAXMSG*MSGBLOCK) {
				plog("%s: write gid: %d; %s\n", mpoint, wn, estr());
				return (-1);
			}
			if (getresp(fd, 013, mpoint) == 0)
				return (-1);
			p = idbuf;
		}
		strcpy(p, onebuf);
		p += n;
	}
	endgrent();
	if ((wn = write(fd, idbuf, MAXMSG*MSGBLOCK)) != MAXMSG*MSGBLOCK) {
		plog("%s: write gid: %d; %s\n", mpoint, wn, estr());
		return (-1);
	}
	if (getresp(fd, 3, mpoint) == 0)
		return (-1);
	return (0);
}

/*
 * error logging stuff
 * just print to stderr for now
 */

/* PRINTFLIKE1 */
plog(s, a, b, c, d, e, f)
char *s;
{
	long now;

	time(&now);
	fseek(stderr, 0L, 2);
	fprintf(stderr, "%.15s ", ctime(&now)+4);	/* trim day of week, year */
	fprintf(stderr, s, a, b, c, d, e, f);
	fflush(stderr);
}

char *
estr()
{
	static char buf[] = "Error -2147483648";

	if (errno < 0 || errno >= sys_nerr) {
		sprintf(buf, "Error %d", errno);
		return (buf);
	}
	return (sys_errlist[errno]);
}
