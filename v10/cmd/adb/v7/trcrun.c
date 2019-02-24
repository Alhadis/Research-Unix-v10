/*
 * machine-specific functions for running the debugged process
 * v7-style (ptrace)
 */

#include "defs.h"
#include "regs.h"
#include "ptrace.h"
#include <sys/param.h>
#include <signal.h>

extern char lastc, peekc;
extern ADDR txtsize;

/*
 * kill process
 */

killpcs()
{

	ptrace(P_KILL, pid, 0, 0);
}

/*
 * grab the process already opened (but not traced);
 * stop it so we can look at it
 */

grab()
{

	error("antique system, can't grab");
}

/*
 * turn off tracing & let it go
 */

ungrab()
{

	error("antique system, can't ungrab");
}

/*
 * get the program to be debugged ready to run
 * program is left stopped at the beginning (so we can poke in breakpoints)
 */

extern int (*sigint)(), (*sigqit)();

startpcs()
{

	if ((pid = fork()) == 0) {
		close(fsym);
		close(fcor);
		signal(SIGINT, sigint);
		signal(SIGQUIT, sigqit);
		doexec();
		exit(0);
	}
	if (pid == -1)
		error("cannot fork");
	bpwait();
	if (adrflg)
		rput(PC, wtoa(adrval));
	while (rdc() != EOR)
		;
	reread();
}

/*
 * set process running, single-stepped
 */

runstep(keepsig)
int keepsig;
{

	delbp();
	ptrace(P_STEP, pid, CONTNEXT, keepsig ? signo : 0);
}

/*
 * set process running
 */

runrun(keepsig)
int keepsig;
{

	ptrace(P_CONT, pid, CONTNEXT, keepsig ? signo : 0);
}

/*
 * exec the program to be debugged
 * opening standard input and output as requested
 */

extern char **environ;

doexec()
{
	char *argl[MAXARG];
	char args[LINSIZ];
	register char *p;
	register char **ap;
	register char *thisarg;

	ap = argl;
	p = args;
	*ap++ = symfil;
	for (rdc(); lastc != EOR;) {
		thisarg = p;
		if (lastc == '<' || lastc == '>') {
			*p++ = lastc;
			rdc();
		}
		while (lastc != EOR && lastc != SPC && lastc != TB) {
			*p++ = lastc;
			readchar();
		}
		if (lastc == SPC || lastc == TB)
			rdc();
		*p++ = 0;
		if (*thisarg == '<') {
			close(0);
			if (open(&thisarg[1], 0) < 0) {
				printf("%s: cannot open\n", &thisarg[1]);
				_exit(0);
			}
		}
		else if (*thisarg == '>') {
			close(1);
			if (creat(&thisarg[1], 0666) < 0) {
				printf("%s: cannot create\n", &thisarg[1]);
				_exit(0);
			}
		}
		else
			*ap++ = thisarg;
	}
	*ap = NULL;
	ptrace(P_INIT, 0, (int *)0, 0);
	execve(symfil, argl, environ);
	perror(symfil);
}

/*
 * wait for the process to stop;
 * pick up status and registers when it does
 */

#define	WSLEEP	10

extern int errno;

bpwait()
{
	register int w;
	int stat;
	int (*isig)();
	int nulsig();

	isig = signal(SIGINT, SIG_IGN);
	while ((w = wait(&stat)) != -1 && w != pid)
		;
	signal(SIGINT, isig);
	if (w == -1)
		errflg = "wait failed";
	else if ((stat & 0177) == 0177) {	/* trace status */
		signo = (stat >> 8) & 0177;
		mapimage();
		if (signo == SIGTRAP)
			signo = 0;
		else {
			sigprint();
			newline();
		}
		return;
	}
	else {
		errflg = "process terminated";
		sigcode = 0;
		if ((signo = stat & 0177) != 0)
			sigprint();
		if (stat & 0200) {
			prints(" - core dumped");
			corfil = "core";
		}
		pid = 0;
		setcor();
	}
}

/*
 * is the right-hand file a process image?
 */

trcimage()
{

	return (pid != 0);
}

/*
 * grab some data from the user block,
 * before maps are set up (ugh)
 */

int
trcunab(off)
int off;
{
	int data;

	errno = 0;
	data = ptrace(P_RDU, pid, off, 0);
	if (errno) {
		errflg = "can't read user block";
		return (0);
	}
	return (data);
}
