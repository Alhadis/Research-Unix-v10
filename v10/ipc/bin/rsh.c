#ifndef lint
static char sccsid[] = "@(#)rsh.c	4.8 83/06/10";
#endif

#include <sys/param.h>
#include <sys/file.h>

#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <pwd.h>
#include "config.h"

/*
 * rsh - remote shell
 */
/* VARARGS */
int	error();
char	*index(), *rindex(), *malloc(), *getpass(), *sprintf(), *strcpy();

struct	passwd *getpwuid();

int	errno;
int	sendsig();

#define	mask(s)	(1 << ((s) - 1))

main(argc, argv0)
	int argc;
	char **argv0;
{
	fd_set rdfds, fds;
	int rem;
	char *host, *cp, **ap, buf[BUFSIZ], *args, **argv = argv0, *user = 0;
	register int n, wn;
	register int cc;
	int asrsh = 0;
	struct passwd *pwd;

	host = rindex(argv[0], '/');
	if (host)
		host++;
	else
		host = argv[0];
	argv++, --argc;
	if (!strcmp(host, "rsh")) {
		host = *argv++, --argc;
		asrsh = 1;
	}
another:
	if (argc > 0 && !strcmp(*argv, "-l")) {
		argv++, argc--;
		if (argc > 0)
			user = *argv++, argc--;
		goto another;
	}
	if (argc > 0 && !strcmp(*argv, "-n")) {
		argv++, argc--;
		(void) close(0);
		(void) open("/dev/null", 0);
		goto another;
	}
	/*
	 * Ignore the -e flag to allow aliases with rogin
	 * to work
	 */
	if (argc > 0 && !strncmp(*argv, "-e", 2)) {
		argv++, argc--;
		goto another;
	}
	if (host == 0)
		goto usage;
#ifdef broken
	if (argv[0] == 0) {
		if (asrsh)
			*argv0 = "rogin";
		execv(RLOGIN, argv0);
		perror(RLOGIN);
		exit(1);
	}
#endif
	pwd = getpwuid(getuid());
	if (pwd == 0) {
		fprintf(stderr, "who are you?\n");
		exit(1);
	}
	cc = 0;
	for (ap = argv; *ap; ap++)
		cc += strlen(*ap) + 1;
	cp = args = malloc(cc);
	for (ap = argv; *ap; ap++) {
		(void) strcpy(cp, *ap);
		while (*cp)
			cp++;
		if (ap[1])
			*cp++ = ' ';
	}
fprintf(stderr, "remote user = %s\n", user ? user : pwd->pw_name);
	rem = tcp_rcmd(host, "shell", pwd->pw_name,
	    user ? user : pwd->pw_name, args, 0);
	if (rem < 0)
		exit(1);
	setuid(getuid());
	FD_ZERO(fds);
	FD_SET(0, fds);
	FD_SET(rem, fds);
	while(1){
		rdfds = fds;
		if(select(NOFILE, &rdfds, 0, 20000) < 0){
			if(errno == EINTR)
				continue;
			else {
				perror("select");
				exit(1);
			}
		}
		if(FD_ISSET(0, rdfds)){
			n = read(0, buf, sizeof(buf));
			if(n < 0)
				break;
			if(n == 0){
				FD_CLR(0, fds);
				write(rem, "", 0);
			} else {
				if(write(rem, buf, n) != n)
					break;
			}
		}
		if(FD_ISSET(rem, rdfds)){
			n = read(rem, buf, sizeof(buf));
			if(n <= 0) {
				break;
			}
			wn = write(1, buf, n);
			if (wn != n) {
				break;
			}
		}
	}
	exit(0);
usage:
	fprintf(stderr,
	    "usage: rsh host [ -l login ] [ -p passwd ] command\n");
	exit(1);
}
