/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	********** GOSUB-RETURN ***********
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 17:32:25 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) gosub.c:  V 1.1  12/21/80" */

#include "bas.h"
int rtnflg;
extern int singflg;
extern int singst;
gosub()
{
	extern char *ptr;
	extern int stpflg;
	char *save;
	int savno;
	rtnflg = 0;

	save = ptr; /* save the incoming program pointer */
	savno=inst.thing.linno;/* save the gosub line number for diagnostic reasons */
	if(singflg == 1)
	{
		if(singwt() < 0)
		{
			rtnflg=stpflg=singflg=0;
			return(0);
		}
	}
	if(fetch(atoi(expr),&ptr) != 0)
	{
		error(savno,0); /* Bad line number */
		ptr=save;
		inst.thing.linno=savno;
		return(-1);
	}
	/* the following is a local duplicate of the run command */
	if(inst.thing.linno == singst)
		singflg = 1;
	if(singflg == 1)
	{
		if(singex() < 0)
		{
			singflg=stpflg=rtnflg=0;
			return(0);
		}
	}
	else
	if(bascall(inst.thing.opcode.lobyte) != 0)
	{
		ptr=save;
		return(-1);
	}
	while(fetch(-1,&ptr) == 0 && stpflg == 0 && rtnflg == 0)
	{
	if(inst.thing.linno == singst)
		singflg=1;
	if(singflg == 1)
	{
		if(singex() < 0)
		{
			singflg=stpflg=rtnflg=0;
			return(0);
		}
	}
	else
		if(bascall(inst.thing.opcode.lobyte) != 0)
		{
			ptr=save;
			return(-1);
		}
	}
	ptr=save;
	if(rtnflg==0)
	{
		error(savno,14);
		return(-1);
	}
	stpflg = 0;
	rtnflg=0;
	return(0);
}
/* This is the extremely complex retrun function */
__return()
{
	rtnflg = 1;
	return(0);
}
