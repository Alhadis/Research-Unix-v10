/* @(#)popen.c	4.1 (Berkeley) 12/21/80 */
#include <stdio.h>
#include <signal.h>
#include <sys/param.h>

#define MAXFORKS	20
#define	tst(a,b)	(*mode == 'r'? (b) : (a))
#define	RDR	0
#define	WTR	1

struct a_fork {
	short	done;
	short	fd;
	int	pid;
	int	status;
};
static struct a_fork the_fork[MAXFORKS];

static FILE *
_popen(cmd,mode,paranoid,args,env)
char	*cmd;
char	*mode;
int	paranoid;
char	**args, **env;
{
	int p[2];
	register myside, hisside, pid;
	register i, ind;

	for (ind = 0; ind < MAXFORKS; ind++)
		if (the_fork[ind].pid == 0)
			break;
	if (ind == MAXFORKS)
		return NULL;
	if(pipe(p) < 0)
		return NULL;
	myside = tst(p[WTR], p[RDR]);
	hisside = tst(p[RDR], p[WTR]);
	switch (pid = fork()) {
	case -1:
		return NULL;
	case 0:
		/* myside and hisside reverse roles in child */
		close(myside);
		dup2(hisside, tst(0, 1));
		if (paranoid!=2) {
			/* a hack for "safety" against incautious SUIDs */
			setuid(getuid());
			setgid(getgid());
		}
		for (i=NSYSFILE; i<_NFILE; i++)
			close(i);
		if (paranoid==2)
			execve(cmd, args, env);
		else if (paranoid==1)
			execl("/bin/sh", "sh", "-c", "-p", cmd, 0);
		else
			execl("/bin/sh", "sh", "-c", cmd, 0);
		_exit(1);
	default:
		the_fork[ind].pid = pid;
		the_fork[ind].fd = myside;
		the_fork[ind].done = 0;
		close(hisside);
		return(fdopen(myside, mode));
	}
}

FILE *
popen(cmd,mode)
char	*cmd;
char	*mode;
{
	return _popen(cmd, mode, 0, (char **)NULL, (char **)NULL);
}

FILE *
ppopen(cmd,mode)
char	*cmd;
char	*mode;
{
	return _popen(cmd, mode, 1, (char **)NULL, (char **)NULL);
}

vepopen(cmd,mode,args,env)
char	*cmd;
char	*mode;
char	**args;
char	**env;
{
	return(_popen(cmd, mode, 2, args, env));
}

pclose(ptr)
FILE *ptr;
{
	register f, r, ind, (*hstat)(), (*istat)(), (*qstat)();
	int status;

	f = fileno(ptr);
	fclose(ptr);
	for (ind = 0; ind < MAXFORKS; ind++)
		if (the_fork[ind].fd == f && the_fork[ind].pid != 0)
			break;
	if (ind == MAXFORKS)
		return 0;
	if (!the_fork[ind].done) {
		istat = signal(SIGINT, SIG_IGN);
		qstat = signal(SIGQUIT, SIG_IGN);
		hstat = signal(SIGHUP, SIG_IGN);
		do {
			r = wait(&status);
			for (f = 0; f < MAXFORKS; f++)
				if (the_fork[f].pid == r) {
					the_fork[f].done = 1;
					the_fork[f].status = status;
					break;
				}
		} while(r != the_fork[ind].pid && r != -1);
		the_fork[ind].status = r == -1 ? -1 : status;
		signal(SIGINT, istat);
		signal(SIGQUIT, qstat);
		signal(SIGHUP, hstat);
	}
	the_fork[ind].pid = 0;
	return (the_fork[ind].status);
}
