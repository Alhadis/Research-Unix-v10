static	char sccsid[] = "@(#)runpcs.c 4.1 10/9/80";
#
/*
 *
 *	UNIX debugger
 *	process control routines for sdb
 *	this is a hack on top of a hack on top of a hack
 *
 */

#include "head.h"
#include <a.out.h>
#include <stab.h>
struct user u;
#include <stdio.h>
#include <sys/pioctl.h>
#include <sys/proc.h>
#include "base.h"

#ifndef SIGTRAP
#define	SIGTRAP SIGTRC
#endif

MSG		NOFORK;
MSG		ENDPCS;
MSG		BADWAIT;

ADDR		sigint;
ADDR		sigqit;
ADDR		userpc;

/* breakpoints */
BKPTR		bkpthead;

CHAR		lastc;

INT		fcor;
INT		fsym;
STRING		errflg;
int		errno;
INT		signo;

L_INT		dot;
STRING		symfil;
INT		wtflag;
INT		pid;
INT		adrflg;
L_INT		loopcnt;






getsig(sig)
{	return(sig);
}

runpcs(runmode,execsig)
{
	REG BKPTR	bkpt;
	IF adrflg THEN userpc=dot; FI
	WHILE --loopcnt>=0
	DO
		if (debug) printf("\ncontinue %x %d\n",userpc,execsig);
		IF runmode==SINGLE
		THEN delbp(); /* hardware handles single-stepping */
		ELSE /* continuing from a breakpoint is hard */
			IF bkpt=scanbkpt(userpc)
			THEN execbkpt(bkpt,execsig); execsig=0;
			FI
			setbp();
		FI
		setrun(runmode,userpc,execsig);
		bpwait(); chkerr(); execsig=0; delbp(); readregs();

	loop1:	IF (signo==0) ANDF (bkpt=scanbkpt(userpc))
		THEN /* stopped by BPT instruction */
			if (debug) printf("\n BPT code; '%s'%o'%o'%d",
				bkpt->comm,bkpt->comm[0],EOR,bkpt->flag);
			dot=bkpt->loc;
			IF bkpt->comm[0] != EOR
			THEN acommand(bkpt->comm);
			FI
			IF bkpt->flag==BKPTEXEC
			ORF ((bkpt->flag=BKPTEXEC)
				ANDF bkpt->comm[0]!=EOR)
			THEN execbkpt(bkpt,execsig); execsig=0; loopcnt++;
			     goto loop1;
 			ELSE bkpt->flag=BKPTSET; bkpt->count=bkpt->initcnt;
			FI
		ELSE execsig=signo;
		     if (execsig) break;
		FI
	OD
 		if (debug) printf("Returning from runpcs\n");
}

#define BPOUT 0
#define BPIN 1
INT bpstate;

endpcs()
{
	REG BKPTR	bkptr;
 		if (debug) printf("Entering endpcs with pid=%d\n");
	IF pid
	THEN killpcs(); pid=0; userpc=1;
	     FOR bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt
	     DO IF bkptr->flag
		THEN bkptr->flag=BKPTSET;
		FI
	     OD
	FI
	bpstate=BPOUT;
}

setup()
{
	int fd;
	long tsigs = ~0;

	fd = openproc(getpid());
	if (ioctl(fd, PIOCSEXEC, 0) < 0) {
		close(fd);
		error("no process ioctl");
	}
	close(fsym);
	fsym = -1;
	IF (pid = fork()) == 0
	THEN close(fd);
	     signal(SIGINT,sigint); signal(SIGQUIT,sigqit);
	     if (debug) printf("About to doexec  pid=%d\n",pid);
	     doexec(); _exit(0);
	ELIF pid == -1
	THEN error(NOFORK);
	ELSE ioctl(fd, PIOCREXEC, 0);
	     close(fd);
	     datmap.ufd = fcor = openproc(pid);
	     ioctl(fcor, PIOCSMASK, &tsigs);
	     bpwait();
	     readregs();
	if (debug) printf("About to open symfil = %s\n", symfil);
	     fsym=open(symfil,wtflag);
	     IF errflg
	     THEN printf("%s: cannot execute\n",symfil);
 		if (debug) printf("%d %s\n", errflg, errflg);
		  endpcs();
	     FI
	FI
	bpstate=BPOUT;
}

execbkpt(bkptr,execsig)
BKPTR	bkptr;
{
	if (debug) printf("exbkpt: %d\n",bkptr->count);
	delbp();
	setrun(SINGLE,bkptr->loc,execsig);
	bkptr->flag=BKPTSET;
	bpwait(); chkerr(); readregs();
}

doexec()
{
	char *argl[MAXARG], args[LINSIZ];
	register char c, redchar, *argsp, **arglp, *filnam;

	arglp = argl;
	argsp = args;
	*arglp++ = symfil;
	c = ' ';

	do {
		while (eqany(c, " \t")) {
			c = rdc();
		} 
		if (eqany(c, "<>")) {
			redchar = c;
			do {
				c = rdc();
			} while (eqany(c, " \t"));
			filnam = argsp;
			do {
				*argsp++ = c;
				c = rdc();
			} while (!eqany(c, " <>\t\n"));
			*argsp++ = '\0';
			if (redchar == '<') {
				close(0);
				if (open(filnam,0) < 0) {
					printf("%s: cannot open\n",filnam);
					fflush(stdout);
					_exit(0);
				}
			} else {
				close(1);
				if (creat(filnam,0666) < 0) {
					printf("%s: cannot create\n",filnam);
					fflush(stdout);
					 _exit(0);
				}
			}
		} else if (c != '\n') {
			*arglp++ = argsp;
			do {
				*argsp++ = c;
				c = rdc();
			} while(!eqany(c, " <>\t\n"));
			*argsp++ = '\0';
		}
	} while (c != '\n');
	*arglp = (char *) 0;
	if (debug) {
		char **dap;
		printf("About to execv(%s, %d)\n",symfil,argl);
		for (dap = argl; *dap; dap++) {
			printf("%s, ", *dap);
		}
	}
	execv(symfil, argl);
	perror("Returned from exect");
}

BKPTR	scanbkpt(adr)
ADDR adr;
{
	REG BKPTR	bkptr;
	FOR bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt
	DO IF bkptr->flag ANDF bkptr->loc==adr
	   THEN break;
	   FI
	OD
	return(bkptr);
}

delbp()
{
	REG ADDR	a;
	REG BKPTR	bkptr;
	IF bpstate!=BPOUT
	THEN
		FOR bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt
		DO	IF bkptr->flag
			THEN a=bkptr->loc;
				prwrite(a, &bkptr->ins, 1);
			FI
		OD
		bpstate=BPOUT;
	FI
}

setbp()
{
	REG ADDR		a;
	REG BKPTR	bkptr;
	char bpt = BPT;

	IF bpstate!=BPIN
	THEN
		FOR bkptr=bkpthead; bkptr; bkptr=bkptr->nxtbkpt
		DO IF bkptr->flag
		   THEN a = bkptr->loc;
			prread(a, &bkptr->ins, 1);
			prwrite(a, &bpt, 1);
			IF errflg
			THEN error("cannot set breakpoint: ");
			     printf("%s:%d @ %d\n", adrtoprocp(dot)->pname,
				adrtolineno(dot), dot);
			     errflg = 0;
			FI
		   FI
		OD
		bpstate=BPIN;
	FI
}

#define	WSLEEP	10

bpwait()
{
	register int w;
	struct proc p;
	int pc;
	int stat;
	int (*isig)();
	int nulsig();
	extern int errno;

	isig = signal(SIGINT, SIG_IGN);
	/*
	 * alarm stuff is just in case
	 */
	for (;;) {
		signal(SIGALRM, nulsig);
		alarm(WSLEEP);
		if (ioctl(fcor, PIOCWSTOP, 0) >= 0)
			errno = 0;
		alarm(0);
		if (errno == 0) {
			signal(SIGINT, isig);
			ioctl(fcor, PIOCGETPR, &p);
			signo = p.p_cursig;
			prread(UBASE + PC, &pc, sizeof(pc));
			if (signo == SIGTRAP || signo == SIGSTOP)
				signo = 0;
			else if (pc == extaddr("_dbsubn"))
				signo = 0;
			else
				sigprint();
			return;
		}
		if (errno == ENOENT)
			break;
		/* still there, still running.  try again. */
	}
	/*
	 * process has died; wait and report status
	 * should check if it's really our child
	 */
	signal(SIGALRM, nulsig);
	alarm(WSLEEP);
	while ((w = wait(&stat)) != -1 && w != pid)
		;
	alarm(0);
	pid = 0;
	signal(SIGINT, isig);
	close(fcor);
	pid = 0;
	corfil = NULL;
	errflg = ENDPCS;
	if (w == -1)
		errflg = BADWAIT;
	else {
		if ((stat & 0177) == 0177)
			printf("trace status?  0%o\n", stat);
		else {
			if ((signo = stat & 0177) != 0)
				sigprint();
			if (stat & 0200) {
				printf(" - core dumped");
				corfil = "core";
				setcor();
			}
			printf("\n");
		}
	}
}

nulsig() {}

REGLIST reglist[];
readregs()
{
	/*get REG values from pcs*/
	REG i;
	FOR i=24; --i>=0; 
	DO prread(UBASE + reglist[i].roffs, (char *)&u + reglist[i].roffs, sizeof(int));
	OD
 	userpc = *(ADDR *)((char *)&u + PC);
}

char 
readchar() {
	lastc = *argsp++;
	if (lastc == '\0') lastc = '\n';
	return(lastc);
}

char
rdc()
{
	register char c;

	c = *argsp++;
	return(c == '\0' ? '\n' : c);
}

/*
 * miscellaneous /proc hooks
 */

setrun(mode, pc, sig)
{
	int ps;

	if (pc != 1)
		prwrite(UBASE + PC, &pc, sizeof(pc));
	if (mode == SINGLE) {
		prread(UBASE + PSL, &ps, sizeof(ps));
		ps |= TBIT;
		prwrite(UBASE + PSL, &ps, sizeof(ps));
	}
	if (sig == 0)
		ioctl(fcor, PIOCCSIG, 0);
	ioctl(fcor, PIOCRUN, 0);
}

killpcs()
{
	long ksig = SIGKILL;

	ioctl(fcor, PIOCCSIG, 0);
	ioctl(fcor, PIOCKILL, &ksig);
	ioctl(fcor, PIOCRUN, 0);
	/*
	 * assert that it's our child
	 */
	while (wait((int *)0) >= 0)
		;
}

int
openproc(id)
int id;
{
	char buf[20];
	int fd;

	sprintf(buf, "/proc/%d", id);
	if ((fd = open(buf, 2)) < 0) {
		perror("can't open process");
		longjmp(env, 0);
	}
	return (fd);
}

prread(addr, buf, size)
char *buf;
{

	lseek(fcor, (long)addr, 0);
	if (read(fcor, buf, size) != size)
		errflg = "can't read process";
}

prwrite(addr, buf, size)
char *buf;
{

	lseek(fcor, (long)addr, 0);
	if (write(fcor, buf, size) != size)
		errflg = "can't read process";
}
