/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// run.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:29:21 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 * RUN BASIC Program
 */
/*   "@(#) run.c:  V 1.1  12/21/80" */

#include	"bas.h"
#define getout(X)\
	signal(SIGINT, quit);\
	stpflg=0;\
	_comflg=0;\
	if(X < 0)\
		error(inst.thing.linno,15);\
	return(X)
	int runcnt = 0;	/* nest count for run */
	char *ptr;
extern int stopl();
extern int singst;
extern int stpflg;
extern int endfcnt;	/* structured "if" nest count */
extern int forflg;	/* nested for count */
extern int bflag,cflag;	/* break count */
extern int quit();
extern int _comflg;	/* common flag */
run()
{
	singst = 32767;
	endfcnt = 0;	/* initialize "if" nest count */
	bflag = cflag = 0;
	forflg = 0;
	if(!(_comflg))	/* If "common" not issued between last &
				this invocation of "run" */
		expunge();	/* de-allocate cariable stack and
				    symbol tables to keep memory optimum */
		restore();
	runcnt += 1;	/* bump nest count */
	if(expr[0] != '\0')	/* if filename specified */
	{
		if(load() < 0)	/* load file before running */
		{
			runcnt -= 1;	/* dec nest count */
			getout(-1); /* return if load error */
		}
	} else {
		if(runcnt > 1)
		{
			error(inst.thing.linno, 6);
			runcnt -= 1; /* dec nest count */
			getout(-1);
		}
	}
	if(fetch(0,&ptr) < 0)	/* any lines? */
	{
		error(inst.thing.linno, 5);
		runcnt -= 1;	/* dec nest count */
		getout(0);	/* nop, can't do anything */
	}
	signal(SIGINT, stopl);
	if(bascall(inst.thing.opcode.lobyte) == -1) /* execute first instruction */
	{
		runcnt -= 1;		/* dec nest count */
		getout(-1);		/*  and return if error */
	}

	/*
	 * fetch each instruction sequentially and execute
	 * until last line.
	 * Fetch sets up the xxxxx
	 */
	while((fetch(-1,&ptr) == 0) && (stpflg == 0))
		if(bascall(inst.thing.opcode.lobyte) == -1)
		{
			runcnt -= 1; /* dec nest count */
			getout(-1);
		}
	runcnt -= 1;	/* dec nest count */
	getout(0);
}
