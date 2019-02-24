#include  <stdio.h>
#include  <pwd.h>
#include  <signal.h>
#include  <sys/types.h>
#include  <sys/ioctl.h>
#include  <errno.h>
#include  <utmp.h>

#include  <sys/wait.h>
#include  <netdb.h>
#include  <sys/inet/in.h>
#include  <sys/socket.h>

extern	errno;
extern	char **environ;

#define	PATH	"PATH=/usr/fbin:/bin:/usr/bin:/usr/usgbin"
char	home[128], mail[128], user[32], shell[128];

char *envinit[] = {
	home, PATH, mail, user, shell,
	NULL};

int     debug;
int     nfd;
char 	*cmdname;
struct	sockaddr_in sin = { AF_INET }, from;


main(argc, argv)
int     argc;
char    *argv[];
{
	struct servent *sp;
	register int c, i;
	int f;
	short lport = -1;
	char *p, *rindex();
	int reapchild();

	cmdname = argv[0];
	p = rindex(cmdname, '/');
	if (p != NULL) cmdname = p+1;

	for(i = 1; i < argc; i++) {
		if(argv[i][0] == '-') switch(argv[i][1]) {
		case 'd':
			debug++;
			continue;
		default:
			fprintf(stderr, "Usage: %s [-d] [port]\n", cmdname);
			exit(1);
		} else
			lport = atoi(argv[i]);
	}

	if (lport > 0) sin.sin_port = htons(lport);
	else {
		sp = getservbyname("uucp", "tcp");
		if (sp == 0) {
			fprintf(stderr, "uucpsrv: tcp/rlogin: unknown service\n");
			exit(1);
		}
		sin.sin_port = sp->s_port;
	}

	if (!debug) {

		if (fork())
			exit(0);
		for (f = 0; f < 10; f++)
			(void) close(f);
		(void) open("/", 0);
		(void) dup2(0, 1);
		(void) dup2(0, 2);
		{ int tt = open("/dev/tty", 2);
		  if (tt > 0) {
			ioctl(tt, TIOCNOTTY, 0);
			close(tt);
		  }
		}
	}
	if (debug < 2) {
		f = socket(AF_INET, SOCK_STREAM, 0, 0);
		if (f < 0) {
			perror("rlogind: socket");
			exit(1);
		}
		if (bind(f, &sin, sizeof (sin), 0) < 0) {
			perror("rlogind: bind");
			exit(1);
		}
		signal(SIGCHLD, reapchild);
		listen(f, 10);
		for (;;) {
			int len = sizeof (from);

			nfd = accept(f, &from, &len, 0);
			if (nfd < 0) {
				if (errno == EINTR)
					continue;
				perror("rlogind: accept");
				continue;
			}
			if (fork() == 0) {
				signal(SIGCHLD, SIG_IGN);
				signal(SIGPIPE, SIG_IGN);
				setbuf(stdin, NULL);
				if (nfd != 0) close(0);
				if (nfd != 1) close(1);
				if (!debug) {
					if (nfd != 2) close(2);
				}
				dup(nfd); dup(nfd); dup(nfd);
				for (i=3; i<25; i++) close(i);
				break;
			}
			close(nfd);
		}
	}
	else {
		fprintf(stderr, "pseudo-connect port %d\n", lport);
		nfd = 2;
	}
	login();
	if (debug < 2) {
		fprintf(stderr, "%s: no shell!\n", cmdname);
		exit(1);
	}
	else exit(0);
}

reapchild()
{
	union wait status;

	while (wait3(&status, WNOHANG, 0) > 0)
		;
}

#define	NMAX	(sizeof utmp.ut_name + 1)
#define	PWMAX	20
char *nolog = "/etc/nologin";
struct utmp utmp;

login()
{
	int c;
	char lname[NMAX];
	char pwt[PWMAX];
	struct passwd *pwd, *getpwnam();
	static struct passwd nouser = {"none", "nope"};
	FILE *nlfd;
	char *crypt();

	do {
		instr("login:", lname, NMAX);
	} while (lname[0] == '\0');

	if ((pwd = getpwnam(lname)) == NULL) {
		pwd = &nouser;
	}
	if (*pwd->pw_passwd != '\0') {
		instr("Password:", pwt, PWMAX);
		if (strcmp(crypt(pwt, pwd->pw_passwd), pwd->pw_passwd)) {
			fprintf(stderr, "Login incorrect\n");
			exit(1);
		}
	}
	if ((nlfd = fopen(nolog, "r")) > 0) {
		/* logins are disabled */
		while ((c = getc(nlfd)) != EOF) {
			putc(c, stderr);
		}
		exit(0);
	}
	if (*pwd->pw_shell == '\0')
		pwd->pw_shell = "/bin/sh";
	if (chdir(pwd->pw_dir) < 0) {
		if (chdir("/") < 0) {
			fprintf(stderr, "No directory!\n");
			exit(1);
		} else {
			fprintf(stderr, "No directory!  Logging in with home=/\n");
			pwd->pw_dir = "/";
		}
	}
	strncpy(utmp.ut_name, lname, sizeof utmp.ut_name);
	logit();
	setgid(pwd->pw_gid);
	setuid(pwd->pw_uid);
	signal(SIGHUP, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	envset(pwd);
	dosh(pwd->pw_shell);
}

instr(prompt, buf, len) char *buf; int len;
{
	register char *p, c;

	p = buf;
	fputs(prompt, stdout);
	fflush(stdout);
	while ((c = getchar()) != '\n') {
		if (c == '\r') break;
		if (c == ' ')
			c = '_';
		if (c == EOF)
			exit(0);
		if (p < buf+len-1)
			*p++ = c;
	}
	*p = '\0';
}

envset(pwd) struct passwd *pwd;
{

	sprintf(home, "HOME=%s", pwd->pw_dir);
	sprintf(shell, "SHELL=%s", pwd->pw_shell);
	sprintf(mail, "MAIL=/usr/spool/mail/%s", pwd->pw_name);
	sprintf(user, "USER=%s", pwd->pw_name);
	environ = envinit;
}

dosh(shellp) char *shellp;
{
	char shn[300], *p, *rindex();

	p = rindex(shellp, '/');
	sprintf(shn, "-%s", (p == NULL ? shellp : p+1));
	if (debug > 1)
		fprintf(stderr, "exec %s %s\n", shellp, shn);
	else execl(shellp, shn, 0);
}

logit()
{
	setpgrp();
#if 0==1
	/* What we do here is create a utmp entry, then fork and wait */
	/* for the child to terminate so we can record the new entry. */
	/* The main process never returns from this subroutine, only  */
	/* the child. */
	struct stat statb;
	int pid, status;
	char tmp[100];

	if ((pid = fork()) <= 0) {	/* If fork fails -- no entry made */
		setpgrp();
		return;
	}

	if (fstat(0, &statb) == 0)
		sprintf(tmp, "%*.*d", sizeof utmp.ut_id,
		    sizeof utmp.ut_id, statb.st_dev & 0xff);
	else strcpy(tmp, "unet");
	strncpy(utmp.ut_id, tmp, sizeof utmp.ut_id);

	ut.ut_pid = pid;
	ut.ut_type = USER_PROCESS;
	utmp.ut_exit.e_termination = 0;
	utmp.ut_exit.e_exit = 0;
	time(&utmp.ut_time);
	pututline(&utmp);
	endutent();

	while ((i = wait(&status)) != pid)
		if (i == ENOCHILD) break;
	
	ut.ut_type = DEAD_PROCESS;
	ut.ut_exit.e_termination = status & 0xff;
	ut.ut_exit.e_exit = (status >> 8) & 0xff;
#endif
}
