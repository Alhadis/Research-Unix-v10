/*
 * RLOGIN
 * Connect terminal to TCP/IP network
 * Operation is line-at-a-time with remote echo.
 */
#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sgtty.h>
#include <signal.h>
#include <errno.h>
#include <pwd.h>

#define	CEOT	04
#define	NULL	0


struct	sgttyb	locvec, savloc;
struct tchars savechars;
struct ltchars lsavechars;
struct ltchars nlchars = { -1, -1, -1, -1, -1, -1 };

int	rem;	/* remote descriptor */
int	rembit;	/* 1<<rem; for select */

fd_set	rdfd_set;
#define	NSELFD	(rem+1)

int	sigint();

char *mytty;
SIG_TYP savint, savquit;
char term[64] = "network";

intcatch(){	/* catch interrupts, turn them into rubouts */
	signal(SIGINT, intcatch);
	ioctl(rem, TIOCFLUSH, 0);
	write(rem, &savechars.t_intrc, 1);
	ioctl(rem, TIOCFLUSH, 0);
	ioctl(0, TIOCFLUSH, 0);
	FD_ZERO(rdfd_set);
}

quitcatch(){	/* catch quits, turn them into FS's */
	ioctl(0, TIOCFLUSH, 0);
	signal(SIGQUIT, quitcatch);
	write(rem, &savechars.t_quitc, 1);
}

main(argc,argv)
	char **argv;
{
	char *name=0, *host, *cp;
	struct passwd *pwd;
	extern struct passwd *getpwuid();

	ioctl(0, TIOCGETC, &savechars);
	ioctl(0, TIOCGLTC, &lsavechars);
	ioctl(0, TIOCGETP, &savloc);
	locvec = savloc;

	argv++, --argc;
	host = *argv++, --argc;
another:
	if (argc > 0 && !strcmp(*argv, "-l")) {
		argv++, argc--;
		if (argc == 0)
			goto usage;
		name = *argv++; argc--;
		goto another;
	}
	if (host == 0)
		goto usage;
	if (argc > 0)
		goto usage;
	pwd = getpwuid(getuid());
	if (pwd == 0) {
		fprintf(stderr, "Who are you?\n");
		exit(1);
	}
	cp = (char *)getenv("TERM");
	if (cp)
		strcpy(term, cp);


	/*
	 * request circuit to host.
	 */
	rem = tcprcmd(host, "login", pwd->pw_name,
		name ? name : pwd->pw_name, term, 0);
	if (rem < 0) 
		exit(1);
	if(setuid(getuid()) < 0){
		perror("setuid");
		exit(1);
	}

	savint = signal(SIGINT, intcatch);
	savquit = signal(SIGQUIT, quitcatch);

	locvec.sg_flags &= ~(CRMOD|ECHO|XTABS);
	locvec.sg_flags |= CBREAK;
	ioctl(0, TIOCSETP, &locvec);
	ioctl(0, TIOCSLTC, &nlchars);


	/*
	 * main loop.
	 */
	do; while(scan() != -1);
	quit("select failed");
	/*NOTREACHED*/
usage:
	fprintf(stderr, "Usage: rogin host [-l user]\n");
	exit(1);
}
scan()
{
	extern errno;

	FD_ZERO(rdfd_set);
    Loop:
	FD_SET(0, rdfd_set);
	FD_SET(rem, rdfd_set);
	if(select(NSELFD, &rdfd_set, (fd_set *)0, 2000) == -1)
		if(errno == EINTR)
			goto Loop;
		else
			return -1;
	if(FD_ISSET(0, rdfd_set))
		keyboard();
	if(FD_ISSET(rem, rdfd_set))
		remote();
	return 0;
}

quit(s)
	char *s;
{
	printf("rogin: %s\n\r", s);
	ioctl(0, TIOCSETP, &savloc);
	ioctl(0, TIOCSLTC, &lsavechars);
	signal(SIGINT, SIG_DFL);
	ioctl(rem, TIOCFLUSH, 0);
	savloc.sg_ispeed = savloc.sg_ospeed = 0;	/* hangup */
	ioctl(rem, TIOCSETP, &savloc);
	close(rem);
	exit(strcmp(s, "eof"));
}

/*
 * Scan data from keyboard, looking for escape lines.
 */
keyboard()
{
	register c;
	register cc;
	register char *bp;
	register char *be, *obp;
	char buf[1024];
	static char line[128];
	static char *linep = &line[0];
	static col = 0;
	long time();

	cc = read(0, buf, sizeof buf);
	if(cc <0){
		if(errno == EINTR)
			return;
	}
	if(cc <= 0)
		quit("read error on file descriptor 0");
	be = buf+cc;
	bp = obp = buf;
	while(bp < be) {
		c = *bp++;
		if (col==0 && c=='~') {
			*linep++ = c;
			col = 1;
			locvec.sg_flags |= savloc.sg_flags&(CRMOD|ECHO);
			ioctl(0, TIOCSETP, &locvec);
			write(1, linep-1, 1);
			continue;
		}
		col++;
		if (c=='\r')
			c = '\n';
		if (linep>line) {
			*linep++ = c;
			if (c==savloc.sg_kill)
				write(1, "\n", 1);
			if (c==savloc.sg_erase)
				linep -= 2;
			if (c==savloc.sg_kill || linep<=line) {
				linep = line;
				locvec.sg_flags &= ~(CRMOD|ECHO);
				ioctl(0, TIOCSETP, &locvec);
				col = 0;
				obp = bp;
			}
		}
		if (c=='\n') {
			col = 0;
			if (linep > line) {
				*linep = '\0';
				if (escape(line+1))
					write(rem, line+1, linep-line-1);
				obp = bp;
				linep = line;
				locvec.sg_flags &= ~(CRMOD|ECHO);
				ioctl(0, TIOCSETP, &locvec);
			}
		}
	}
	if (bp>obp && linep==line)
		if(write(rem, obp, bp-obp) <= 0)
			quit("Eof");
}

/*
 * Send data from remote machine to standard output (trivial)
 */
remote(){
	char buf[1024];
	register n;

	n = read(rem, buf, sizeof buf);
	if(n < 0 && errno == EINTR)
		return;
	if(n <= 0)
		quit("Eof");
	write(1, buf, n);
}

escape(line)
	register char *line;
{

	switch(*line++) {
	case '!':
		cunix(line);
		return(0);

	case '.':
	case CEOT:
		quit("eof");

	case 'b':
		ioctl(rem, TIOCSBRK, 0);
		return(0);
	default:
		return(1);
	}
}

cunix(prog)
char *prog;
{
	register int upid;
	int retcode;

	if ((upid = fork()) == 0) {
		signal(SIGINT, savint);
		signal(SIGQUIT, savquit);
		ioctl(0, TIOCSETN, &savloc);
		ioctl(0, TIOCSLTC, &lsavechars);
		if (*prog == '\n')
			execl("/bin/sh", "sh", "-i", 0);
		else
			execl("/bin/sh","sh","-c",prog,0);
		exit(0100);
	}
	if (upid < 0) {
		printf("can't fork\n");
	} else {
		while((wait(&retcode) !=upid))
			;
	}
	signal(SIGINT, intcatch);
	signal(SIGQUIT, quitcatch);
	ioctl(0, TIOCSETN, &locvec);
	ioctl(0, TIOCSLTC, &nlchars);
	printf("!!\n");
}

tcprcmd(host, port, locuser, remuser, cmd, fd2p)
char *host, *port, *locuser, *remuser, *cmd;
int *fd2p;
{
	char buf[1];
	int fd;
	char dialstr[128];

	sprintf(dialstr, "/cs/in!%s!%s", host, port);
	fd = ipcopen(dialstr, "light");
	if (fd<0) {
		fprintf(stderr, "%s: connection failed\n", host);
		goto bad;
	}

	if(fd2p == 0)
		write(fd, "", 1);
	else
		goto bad;	/* later */

	write(fd, locuser, strlen(locuser)+1);
	write(fd, remuser, strlen(remuser)+1);
	write(fd, cmd, strlen(cmd)+1);

	if(read(fd, buf, 1) != 1){
		perror(host);
		goto bad;
	}
	if(buf[0] != 0){
		while(read(fd, buf, 1) == 1){
			write(2, buf, 1);
			if(buf[0] == '\n')
				break;
		}
		goto bad;
	}
	return(fd);
bad:
	close(fd);
	return(-1);
}
