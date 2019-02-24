/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	//////////// UNTILCOD.C /////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Mon Sep 17 23:06:13 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 * Either execute or skip command lines until specified code (endcod)
 * is reached.
 * If execflg = 1 then execute the command lines,
 * If execflg = 0 then skip over the command lines.
 * This routine terminates if:
 *	endcod is encountered
 *	end of text is encountered
 *	error is encountered
 *
 * This routine is used for the structured "if" for skipping
 * or executing until the "else" or "endif" is encountered.
 */
/*   "@(#) untilcod.c:  V 1.1  12/21/80" */

#include	"bas.h"
extern	int bflag,cflag;		/* break & continue flag */
extern	int esle,fidne;		/* else and endif codes */
extern	int _togo;		/* "goto" code */
	int	endfcnt;	/* nest count */
extern	int _fi;

untilcod(endcod,execflg)
int	endcod;		/* termination code */
int	execflg;	/* execution flag */
{
	extern int stpflg;	/* stop flag */
	extern char *ptr;

	if(bflag || cflag)	/* if break flag on */
	{
		endfcnt = 0;	/* clean up nest count */
		return(0);	/* UNRAVEL */
	}
	while(fetch(-1,&ptr) == 0 && stpflg == 0)
	{
		/*
		 * ALWAYS terminate if endif encountered and nest = 0
		 */
		if(inst.thing.opcode.lobyte == fidne && endfcnt == 0)
			break;
		if(execflg)	/* EXECUTION PART */
		{
			if(inst.thing.opcode.lobyte == _togo)
			{
				error(inst.thing.linno, 46); /* GOTO NOT ALLOW */
				return(-1);
			}
			/*
			 * IF TERMINATION CODE REACHED
			 */
			if(inst.thing.opcode.lobyte == endcod)
				break;		/*    break out */
			if(bascall(inst.thing.opcode.lobyte) != 0)
			{
				return(-1);
			}
			if(bflag || cflag)	/* if break got set */
			{
				endfcnt = 0; /* zero nest count */
				return(0);	/* UNRAVEL */
			}
		}
		else	/* NON EXECUTION PART */
		{
			if(inst.thing.opcode.lobyte == fidne && endfcnt > 0)
			{
				endfcnt--; /* dec endif nest count */
				continue; /* not last "if" when just 0 */
			}
			else if(inst.thing.opcode.lobyte == _fi)
			{
				endfcnt++;
			}
			if(inst.thing.opcode.lobyte == endcod && endfcnt == 0)
				break;
		}
	}
	return(0);
}
