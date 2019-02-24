/*
 *
 *	UNIX debugger
 *
 */

#include "defs.h"
#include "bkpt.h"

static	char	NOPCS[] = "no process";

extern	char	lastc;

/* sub process control */

subpcs(modif)
{
	register int	check;
	int	keepsig,runmode;
	register BKPT *bk;
	char *comptr;
	char *malloc();
	WORD defval();

	keepsig=0;
	loopcnt=cntval;
	switch (modif) {

		/* delete breakpoint */
	case 'd': 
	case 'D':
		if ((bk=scanbkpt(dot)) == NULL)
			error("no breakpoint set");
		bk->flag=BKPTCLR;
		return;

		/* set breakpoint */
	case 'b': 
	case 'B':
		if (bk=scanbkpt(dot))
			bk->flag=BKPTCLR;
		for (bk=bkpthead; bk; bk=bk->nxtbkpt)
			if (bk->flag == BKPTCLR)
				break;
		if (bk==0) {
			bk = (BKPT *)malloc(sizeof(*bk));
			if (bk == NULL)
				error("too many breakpoints");
			bk->nxtbkpt=bkpthead;
			bkpthead=bk;
		}
		bk->loc = dot;
		bk->initcnt = bk->count = cntval;
		bk->flag = modif == 'b' ? BKPTSET : BKPTTMP;
		check=MAXCOM-1;
		comptr=bk->comm;
		rdc();
		reread();
		do {
			*comptr++ = readchar();
		} while (check-- && lastc!=EOR);
		*comptr=0;
		reread();
		if (check)
			return;
		error("bkpt command too long");

		/* exit */
	case 'k' :
	case 'K':
		if (pid == 0)
			error(NOPCS);
		printf("%d: killed", pid);
		endpcs();
		return;

		/* run program */
	case 'r': 
	case 'R':
		endpcs();
		setup();
		runmode=CONTIN;
		break;

		/* single step */
	case 's': 
	case 'S':
		if (pid == 0) {
			setup();
			loopcnt--;
		}
		else {
			runmode=SINGLE;
			keepsig=defval((WORD)signo);
		}
		break;

		/* continue with optional signal */
	case 'c': 
	case 'C': 
	case 0:
		if (pid==0)
			error(NOPCS);
		runmode=CONTIN;
		keepsig=defval((WORD)signo);
		break;

	case 'g':	/* grab the current process */
		if (adrflg && adrval == 0) {
			if (pid == 0)
				error(NOPCS);
			ungrab();
		}
		else {
			grab();
			printf("stopped at%16t");
			delbp();
			printpc();
		}
		return;

	default:
		error("bad `:' command");
	}

	if (loopcnt>0 && runpcs(runmode,keepsig))
		printf("breakpoint%16t");
	else
		printf("stopped at%16t");
	delbp();
	printpc();
}
