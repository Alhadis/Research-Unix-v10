/*	/sccs/src/cmd/uucp/s.shio.c
	shio.c	1.1	8/30/84 17:37:41
*/
#include "uucp.h"

VERSION(@(#)shio.c	1.1);

/*
 * use shell to execute command with
 * fi, fo, and fe as standard input/output/error
 *	cmd 	-> command to execute
 *	fi 	-> standard input
 *	fo 	-> standard output
 *	fe 	-> standard error
 * return:
 *	0		-> success 
 *	non zero	-> failure  -  status from child
			(Note - -1 means the fork failed)
 */
shio(cmd, fi, fo, fe)
char *cmd, *fi, *fo, *fe;
{
	register int pid, ret;
	int status;

	if (fi == NULL)
		fi = "/dev/null";
	if (fo == NULL)
		fo = "/dev/null";
	if (fe == NULL)
		fe = "/dev/null";

	DEBUG(3, "shio - %s\n", cmd);
	if ((pid = fork()) == 0) {
		(void) signal(SIGINT, SIG_IGN);
		(void) signal(SIGHUP, SIG_IGN);
		(void) signal(SIGQUIT, SIG_IGN);
		closelog();
		(void) close(Ifn);	/* close connection fd's */
		(void) close(Ofn);
		(void) close(0);	/* get stdin from file fi */
		if (open(fi, 0) != 0)
			exit(errno);
		(void) close(1);	/* divert stdout to fo */
		if (creat(fo, 0666) != 1)
			exit(errno);
		(void) close(2);	/* divert stderr to fe */
		if (creat(fe, 0666) != 2)
			exit(errno);
#ifdef V8
		close(3);		/* get rid of /dev/tty */
#endif
		(void) execle(SHELL, "sh", "-c", cmd, 0, Env);
		exit(100);
	}

	/*
	 * the status returned from wait can never be -1
	 * see man page wait(2)
	 * So we use the -1 value to indicate fork failed
	 */
	if (pid == -1)
		return(-1);
	

	while ((ret = wait(&status)) != pid && ret != -1);
	DEBUG(3, "status %d\n", status);
	return(status);
}
