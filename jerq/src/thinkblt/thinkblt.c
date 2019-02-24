#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include "/usr/jerq/include/jioctl.h"
#include "thinkblt.h"

#define Signal(s, f)	((signal(s, SIG_IGN) == SIG_IGN) ? SIG_IGN : signal(s, f))

#define SELTIMO	0x7fff
#define PARGVAL	((*argv)[2] ? (*argv)+2 : --argc ? *++argv : (char *)0)

#define putch(c)	(cbuf[0]=(c), write(1, cbuf, 1))

char *jpgm = { "/usr/jerq/mbin/thinkblt.m" }; int zflag, vopt = 1;

extern int errno;

char ptname[32], linkbuf[64], *linkname = 0;
char *getenv(), *strcat(), *strcpy();
unsigned char cbuf[240];

fd_set fdset, fdbits;
int infd, keepfd, ready, sighup();

main(argc,argv)
int argc; char **argv;
{
	int n;
	Signal(SIGHUP, sighup);
	while (--argc > 0) {
		if ((*++argv)[0] == '-') switch ((*argv)[1]) {
		case 'j':
			jpgm = PARGVAL; break;
		case 'v':
			vopt = atoi(PARGVAL); break;
		case 'z':
			zflag++; break;
		default:
			fprintf(stderr,"unknown option %s\n",*argv);
			exit(1);
		} else {
			if (linkname == 0)
				linkname = *argv;
		}
	}

	ptinit();
	if (jload(jpgm, zflag, (vopt ? "-v1" : "-v0")))
		Exit(1);
	rawtty(1);
	putch(0); putch(255);
	n = strlen(linkname);
	cbuf[0] = n>>8;
	cbuf[1] = n;
	strcpy(&cbuf[2], linkname);
	cbuf[n+2] = 0;
	cbuf[n+3] = 0;
	write(1, cbuf, n+4);
	ready = 1;
	n = 0;
	FD_SET(0, fdset);
	for (;;) {
		fdbits = fdset;
		if (ready)
			FD_SET(infd, fdbits);
		while (select(infd + 1, &fdbits, (fd_set *)0, SELTIMO) == -1)
			if (errno != EINTR)
				Exit(1);
		if (FD_ISSET(0, fdbits)) {
			read(0, cbuf, sizeof cbuf);
			switch (*cbuf) {
			case EXIT:
				Exit(0);
			case ABORT:
				ptinit();
				if (n > 0) {
					cbuf[0] = cbuf[1] = 0;
					write(1, cbuf, 2);
				}
				ready = 1;
				continue;
			case READY:
				ready = 1;
				break;
			}
		}
		if (FD_ISSET(infd, fdbits)) {
			n = read(infd, &cbuf[2], (sizeof cbuf)-2);
			if (n < 0)
				n = 0;
			cbuf[0] = n>>8;
			cbuf[1] = n;
			write(1, cbuf, n+2);
			if (n > 0)
				ready = 0;
		}
	}
}

ptinit()
{
	int ptfd;
	if ((ptfd = ptopen(ptname)) < 0) {
		fprintf(stderr, "no streams\n");
		Exit(1);
	}
	if (keepfd)
		close(keepfd);
	if (infd)
		close(infd);
	infd = ptfd;
	if ((keepfd = open(ptname, 2)) < 0) {
		fprintf(stderr, "cannot open %s\n", ptname);
		Exit(1);
	}
	if (linkname == 0)
		linkname = getenv("THINK");
	if (linkname == 0 && (linkname = getenv("HOME")))
		linkname = strcat(strcpy(linkbuf, linkname), "/.THINK");
	if (linkname) {
		unlink(linkname);
		if (symlink(ptname, linkname) < 0) {
			fprintf(stderr, "cannot symlink %s to %s\n",
			    ptname, linkname);
			Exit(1);
		}
	} else
		linkname = ptname;
}

Exit(n)
{
	if (linkname && linkname != ptname)
		unlink(linkname);
	rawtty(0);
	ioctl(0, JTERM, (char *)0);
	exit(n);
}

sighup(s)
{
	if (linkname && linkname != ptname)
		unlink(linkname);
	_exit(1);
}

jload(prog ,zflag, arg)
char *prog; int zflag; char *arg;
{
	register i = 0;
	char *cmd[5];
	cmd[i++] = "32ld";
	if (zflag)
		cmd[i++] = "-z";
	cmd[i++] = prog;
	cmd[i++] = arg;
	cmd[i++] = 0;
	if (systemv("/usr/jerq/bin/32ld",cmd))
		return 1;
	sleep(2);
	return 0;
}

systemv(name,argv)
char *name, **argv;
{
	int status, pid, l;
	if ((pid = fork()) == 0) { execv(name,argv); _exit(127); }
	while ((l = wait(&status)) != pid && l != -1);
	return (l == -1) ? -1 : status;
}

#include <sgtty.h>

rawtty(flag)
{
	static struct sgttyb ttyb, ttybsave;
	static struct ttydevb ttyd, ttydsave;
	static int tty_raw = 0, was_ld;
	extern int tty_ld;

	if (flag == tty_raw) return;

	if (flag) {
		ioctl(0, TIOCGETP, &ttyb);
		ioctl(0, TIOCGDEV, &ttyd);
		ttybsave = ttyb;
		ttydsave = ttyd;
		ttyb.sg_flags |= RAW;
		ttyd.flags |= F8BIT;
		ioctl(0, TIOCSDEV, &ttyd);
		ioctl(0, TIOCSETP, &ttyb);
		if ((was_ld = ioctl(0, FIOLOOKLD, 0)) == tty_ld)
			ioctl(0, FIOPOPLD, 0);
	} else {
		if (was_ld == tty_ld)
			ioctl(0, FIOPUSHLD, &tty_ld);
		ioctl(0, TIOCSDEV, &ttydsave);
		ioctl(0, TIOCSETP, &ttybsave);
	}
	tty_raw = flag;
}
