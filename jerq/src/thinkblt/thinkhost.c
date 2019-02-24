#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include "/usr/jerq/include/jioctl.h"
#include "thinkblt.h"

#define Signal(s, f)	((signal(s, SIG_IGN) == SIG_IGN) ? SIG_IGN : signal(s, f))

#define PARGVAL	((*argv)[2] ? (*argv)+2 : --argc ? *++argv : (char *)0)

#define putch(c)	(cbuf[0]=(c), write(1, cbuf, 1))

char *jpgm = { "/usr/jerq/mbin/thinkblt.m" }; int zflag, vopt = 1;

char *linkname, *namelink(), *namept();
unsigned char cbuf[240];

int sighup();

main(argc,argv)
int argc; char **argv;
{
	int n;
	signal(SIGHUP, sighup);
	signal(SIGTERM, SIG_IGN);
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

	if (!linkname && !(linkname=namelink("THINK"))) {
		fprintf(stderr, "no name for THINK\n");
		exit(1);
	}
	if (namept(0, linkname, 0222) < 0) {
		fprintf(stderr, "cannot mount on %s\n", linkname);
		exit(1);
	}
	if (jload(jpgm, zflag, (vopt ? "-v1" : "-v0")))
		Exit(1);
	rawtty(1);
	putch(0); putch(255);
	write(1, linkname, strlen(linkname)+1);
	while (read(0, cbuf, sizeof cbuf) > 0) switch (*cbuf) {
	case EXIT:
		Exit(0);
	}
}

Exit(n)
{
	namept(-1, linkname, 0);
	rawtty(0);
	ioctl(0, JTERM, (char *)0);
	exit(n);
}

sighup(s)
{
	namept(-1, linkname, 0);
	_exit(1);
}

jload(prog, zflag, arg)
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
