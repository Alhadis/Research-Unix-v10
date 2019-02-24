/*
 * run and mount a netb file server,
 * including the uid-login name handshake protocol
 */
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>	/* for makedev */

#define	NBFS	4	/* for fmount */

/*
 * setup handshake parameters
 */
#define	MAXMSG	5	/* largest message 5*MSGBLOCK -- known to kernel too */
#define	MSGBLOCK 1024

int debug;	/* debug level, for server */

char *estr();

main(argc, argv)
int argc;
char **argv;
{
	int pfd[2];
	int pid;
	int sts;

	while (--argc > 0 && **++argv == '-') {
		switch (argv[0][1]) {
		case 'd':
			debug = atoi(&argv[0][2]);
			break;

		default:
			fprintf(stderr, "unknown flag %s ignored\n", argv[0]);
			break;
		}
	}
	if (argc < 2) {
		fprintf(stderr, "usage: runfs [-dlevel] mpoint server arg ...\n");
		exit(1);
	}
	if (pipe(pfd) < 0) {
		perror("pipe");
		exit(1);
	}
	if ((pid = fork()) < 0) {
		perror("fork");
		exit(1);
	}
	/*
	 * child: run setup handshake, mount filesystem
	 */
	if (pid == 0) {
		close(pfd[1]);	/* for clean error */
		sts = setup(pfd[0], argv[0]);
		exit(sts);
	}
	/*
	 * parent: exec server
	 */
	close(pfd[0]);
	dup2(pfd[1], 0);
	dup2(pfd[1], 1);
	close(pfd[1]);
	execv(argv[1], &argv[1]);
	fprintf(stderr, "exec %s: %s\n", argv[1], estr());
	exit(1);
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

setup(fd, mpoint)
int fd;
char *mpoint;
{
	char m[M1LEN];
	register int n;

	signal(SIGPIPE, SIG_DFL);
	memset(m, 0, M1LEN);
	m[0] = MAXMSG;
/* 	m[1] = fp->devnum;. */
	m[1] = 0;	/* will this work? */
	m[2] = 'd';	/* ignored by server? */
	m[3] = debug;
	alarm(600);
	if ((n = write(fd, m, M1LEN)) != M1LEN) {
		fprintf(stderr, "setup 1: write returned %d; %s\n", n, estr());
		alarm(0);
		return (-1);
	}
	if (getresp(fd, 1, mpoint) == 0)
		return (-1);
	alarm(600);
	if (senduid(fd, mpoint) < 0) {
		alarm(0);
		return (-1);
	}
	alarm(600);
	if (sendgid(fd, mpoint) < 0) {
		alarm(0);
		return (-1);
	}
	alarm(60);
	alarm(60);
	funmount(mpoint);	/* bad -- might unmount wrong fs */
	if (trymount(fd, mpoint) < 0) {
		fprintf(stderr, "fmount: %s: %s\n", mpoint, estr());
		alarm(0);
		return (-1);
	}
	alarm(0);
	return (0);
}

/*
 * fmount, but try to guess unique device number
 */

int
trymount(fd, mpoint)
int fd;
char *mpoint;
{
	register int dev;

	for (dev = 200; dev >= 100; --dev) {
		if (fmount(NBFS, fd, mpoint, makedev(dev, 0)) >= 0)
			return (0);
		if (errno != EBUSY)
			return (-1);
	}
	return (-1);
}

getresp(fd, id, mpoint)
int fd, id;
{
	register int n;
	char m;

	alarm(600);
	n = read(fd, &m, 1);
	alarm(0);	/* assume errno untouched */
	if (n != 1) {
		fprintf(stderr, "setup %d: %s: read returned %d: %s\n", id, mpoint, n, estr());
		return (0);
	}
	if (m != id) {
		fprintf(stderr, "setup %d: %s: received %d (%c)\n", id, mpoint, (unsigned char)m, m);
		return (0);
	}
	return (1);
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
				fprintf(stderr, "%s: write uid: %d; %s\n", mpoint, wn, estr());
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
		fprintf(stderr, "%s: write uid: %d; %s\n", mpoint, wn, estr());
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
				fprintf(stderr, "%s: write gid: %d; %s\n", mpoint, wn, estr());
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
		fprintf(stderr, "%s: write gid: %d; %s\n", mpoint, wn, estr());
		return (-1);
	}
	if (getresp(fd, 3, mpoint) == 0)
		return (-1);
	return (0);
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
