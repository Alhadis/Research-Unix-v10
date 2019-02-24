static	char *sccsid = "@(#)kill.c	4.2 (Berkeley) 10/9/80";
/*
 * kill - send signal to process
 */

#include <signal.h>
#include <ctype.h>

char *signm[NSIG+1] = { 0,
"SIGHUP", "SIGINT", "SIGQUIT", "SIGILL", "SIGTRAP", "SIGIOT", "SIGEMT", "SIGFPE",	/* 1-8 */
"SIGKILL", "SIGBUS", "SIGSEGV", "SIGSYS", "SIGPIPE", "SIGALRM", "SIGTERM", 0,	/* 9-16 */
"SIGSTOP", "SIGTSTP", "SIGCONT", "SIGCHLD", "SIGTTIN", "SIGTTOU", "SIGTINT", "SIGXCPU",	/* 17-24 */
"SIGXFSZ"				/* 25- */
};

main(argc, argv)
char **argv;
{
	register signo, pid, res;
	int errlev;
	extern char *sys_errlist[];
	extern errno;

	errlev = 0;
	if (argc <= 1) {
	usage:
		printf("usage: kill [ -sig ] pid ...\n");
		printf("for a list of signals: kill -l\n");
		exit(2);
	}
	if (*argv[1] == '-') {
		if (argv[1][1] == 'l') {
			int i = 0;
			for (signo = 1; signo <= NSIG; signo++)
				if (signm[signo]) {
					printf("%s ", signm[signo]);
					if (++i%8 == 0)
						printf("\n");
				}
			if(i%8 !=0)
				printf("\n");
			exit(0);
		} else if (isdigit(argv[1][1])) {
			signo = atoi(argv[1]+1);
			if (signo < 0 || signo > NSIG) {
				printf("kill: %s: number out of range\n",
				    argv[1]);
				exit(1);
			}
		} else {
			char *name = argv[1]+1;
			for (signo = 1; signo <= NSIG; signo++)
				if (signm[signo] && (
				    !strcmp(signm[signo], name)||
				    !strcmp(signm[signo]+3, name)))
					goto foundsig;
			printf("kill: %s: unknown signal; kill -l lists signals\n", name);
			exit(1);
foundsig:
			;
		}
		argc--;
		argv++;
	} else
		signo = SIGTERM;
	argv++;
	while (argc > 1) {
		if ((**argv<'0' || **argv>'9') && **argv!='-')
			goto usage;
		res = kill(pid = atoi(*argv), signo);
		if (res<0) {
			printf("%u: %s\n", pid, sys_errlist[errno]);
			errlev = 1;
		}
		argc--;
		argv++;
	}
	return(errlev);
}
