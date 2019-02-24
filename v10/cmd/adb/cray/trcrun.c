/*
 * machine-specific functions for running the debugged process
 * v7-style (ptrace)
 */

#include "defs.h"
#include "regs.h"
#include "ptrace.h"
#include "bkpt.h"
#include "space.h"
#include <sys/param.h>
#include <signal.h>

extern char lastc, peekc;
extern ADDR txtsize;

static BKPT stepbk, step2bk;

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
	WORD ins;
	ADDR pc;
	BKPT bk;

	/*
	 * this is the hard part:
	 * the cray can't single step
	 */
	pc = (ADDR)rtow(rget(PC));
	ins = stow(sget(pc, CORF|INSTSP));
	switch (ins & 0177000) {
	case 05000:	/* j bXX */
		pc = (ADDR)rtow(rget(B00 + (ins&077))) * 2;
		break;

	case 010000:	/* jaz exp */
	case 011000:	/* jan exp */
	case 012000:	/* jap exp */
	case 013000:	/* jam exp */
	case 014000:	/* jsz exp */
	case 015000:	/* jsn exp */
	case 016000:	/* jsp exp */
	case 017000:	/* jsm exp */
		/* conditional branch: need 2 breaks */
		step2bk.loc = pc + 4;
		bkput(&step2bk, 1);
		step2bk.flag = BKPTTMP;
		/* fall through ... */
	case 06000:	/* j exp */
	case 07000:	/* r exp */

		pc = (ADDR)(((ins & 0777)<<16) | stow(sget(pc+2, CORF|INSTSP))) * 2;
		break;

	case 020000:
	case 021000:
	case 040000:
	case 041000:
	case 0100000:
	case 0101000:
	case 0102000:
	case 0103000:
	case 0104000:
	case 0105000:
	case 0106000:
	case 0107000:
	case 0110000:
	case 0111000:
	case 0112000:
	case 0113000:
	case 0114000:
	case 0115000:
	case 0116000:
	case 0117000:
	case 0120000:
	case 0121000:
	case 0122000:
	case 0123000:
	case 0124000:
	case 0125000:
	case 0126000:
	case 0127000:
	case 0130000:
	case 0131000:
	case 0132000:
	case 0133000:
	case 0134000:
	case 0135000:
	case 0136000:
	case 0137000:
		pc += 4;	/* two-parcel instruction */
		break;

	default:
		pc += 2;
		break;
	}
	stepbk.loc = pc;
	bkput(&stepbk, 1);
	stepbk.flag = BKPTTMP;
	runrun(keepsig);
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
		/*
		 * awful hack to get breakpoints right
		 */
		if (signo == SIGEMT)
			rput(PC, wtor(rtow(rget(PC))-2));
		if (signo == SIGTRAP || signo == SIGEMT)
			signo = 0;
		else {
			sigprint();
			newline();
		}
		if (stepbk.flag) {
			stepbk.flag = 0;
			bkput(&stepbk, 0);
		}
		if (step2bk.flag) {
			step2bk.flag = 0;
			bkput(&step2bk, 0);
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
