/*
 *
 *	UNIX debugger
 *
 */

#include "defs.h"
#include "regs.h"
#include "bkpt.h"

BKPT *bkpthead;

BOOL bpin;

int pid;
int sigcode, signo;

/* service routines for sub process control */

runpcs(runmode, keepsig)
{
	register int rc;
	register BKPT *bkpt;

	if (adrflg)
		rput(PC, (TREG)dot);
	printf("%s: running\n", symfil);
	while (--loopcnt >= 0) {
		rrest();
		if (runmode == SINGLE)
			runstep(keepsig);
		else {
			if ((bkpt = scanbkpt((ADDR)rtow(rget(PC)))) != NULL) {
				execbkpt(bkpt, keepsig);
				keepsig = 0;
			}
			setbp();
			runrun(keepsig);
		}
		keepsig = 0;
		bpwait();
		chkerr();
		delbp();
		/* real signal? */
		if (signo != 0) {
			keepsig = 1;
			rc = 0;
			continue;
		}
		/* single step? */
		if ((bkpt = scanbkpt((ADDR)rtow(rget(PC)))) == NULL) {
			keepsig = 0;
			rc = 0;
			continue;
		}
		/* breakpoint */
		dot = bkpt->loc;
		if (bkpt->flag == BKPTTMP)
			bkpt->flag = BKPTCLR;
		else if (bkpt->flag == BKPTSKIP) {
			execbkpt(bkpt, keepsig);
			keepsig = 0;
			loopcnt++;	/* we didn't really stop */
			continue;
		}
		else {
			bkpt->flag = BKPTSKIP;
			--bkpt->count;
			if ((bkpt->comm[0] != EOR && command(bkpt->comm, ':') == 0)
			||  bkpt->count != 0) {
				execbkpt(bkpt, keepsig);
				keepsig = 0;
				loopcnt++;
				continue;
			}
			bkpt->count = bkpt->initcnt;
		}
		rc = 1;
	}
	return(rc);
}

/*
 * finish the process off;
 * kill if still running
 */

endpcs()
{
	register BKPT *bk;

	if (pid) {
		killpcs();
		pid=0;
		for (bk=bkpthead; bk; bk = bk->nxtbkpt)
			if (bk->flag == BKPTTMP)
				bk->flag = BKPTCLR;
			else if (bk->flag != BKPTCLR)
				bk->flag = BKPTSET;
	}
	bpin = FALSE;
}

/*
 * start up the program to be debugged in a child
 */

setup()
{

	startpcs();
	if (errflg) {
		printf("%s\n", errflg);
		printf("%s: cannot execute\n", symfil);
		endpcs();
		error(0);
	}
	bpin = FALSE;
}

/*
 * skip over a breakpoint:
 * remove breakpoints, then single step
 * so we can put it back
 */
execbkpt(bk, keepsig)
BKPT *bk;
{
	runstep(keepsig);
	bk->flag = BKPTSET;
	bpwait();
	chkerr();
}

/*
 * find the breakpoint at adr, if any
 */

BKPT *
scanbkpt(adr)
ADDR adr;
{
	register BKPT *bk;

	for (bk = bkpthead; bk; bk = bk->nxtbkpt)
		if (bk->flag != BKPTCLR && bk->loc == adr)
			break;
	return(bk);
}

/*
 * remove all breakpoints from the process' address space
 */

delbp()
{
	register BKPT *bk;

	if (bpin == FALSE || pid == 0)
		return;
	for (bk = bkpthead; bk; bk = bk->nxtbkpt)
		if (bk->flag != BKPTCLR)
			bkput(bk, 0);
	bpin = FALSE;
}

/*
 * install all the breakpoints
 */

setbp()
{
	register BKPT *bk;

	if (bpin == TRUE || pid == 0)
		return;
	for (bk = bkpthead; bk; bk = bk->nxtbkpt)
		if (bk->flag != BKPTCLR)
			bkput(bk, 1);
	bpin = TRUE;
}
