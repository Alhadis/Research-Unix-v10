/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	************ FOR-NEXT *************
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 17:27:09 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) for.c:  V 1.2  3/4/81" */

#include "bas.h"
extern int singflg;
extern int rof;
extern int singst;
int bflag,forflg,forcnt,cflag;
_for()
{
	extern int nxt,stpflg;
	extern char *eoexpr,*ptr,*curptr;
	char *save,c,*sav1;
	char nam[5];
	double	evalx(),final,step,var,round;
	int i,savno;
	forflg++; /* mark for in progress for break */
	for(i=0;(c=expr[i]) != '\15' && c != '\0' && i < 4;i++)
		nam[i]=c;
	nam[i]=0;
	if(!(nam[0] == 'e' && nam[1] == 'v')) /* if not an infinite loop */
	{
		step = 1; /* default value for step */
		let(); /* handle the initial value expression */
		if(*eoexpr++ != '\004') /* make sure its a to */
		{
			error(inst.thing.linno,9);
			forflg=bflag=0;
			return(-1);
		}
		final=evalx(eoexpr); /*evaluate the "to" expression */
		if(*eoexpr == '\005') /* see if step is present */
		{
			eoexpr++;
			step=evalx(eoexpr);
		}
		round=step/2;
	}
	if(singflg == 1) /* if single stepping */
		if(singwt() < 0)
		{
			singflg=stpflg=0;
			forflg--;
			return(0);
		}
	save = ptr;
	savno = inst.thing.linno;
	while(fetch(-1,&ptr) == 0 && stpflg == 0)
	{
		if(inst.thing.linno == singst)
			singflg = 1;
		if(inst.thing.opcode.lobyte == nxt)
		{
			if(bflag)
			{
				if(forcnt != 0)
				{
					forcnt--;
					continue;
				}
			}
			if(singflg == 1)
			{
				sav1 = expr;
				prin();
				expr = sav1;
			}
			if(strcmp(nam,expr) != 0)
			{
				ptr=curptr;
				forflg--;
				return(0);
			}
			if(cflag) /* if the continue flag is set */
			{
				if(cflag == 1)
					cflag = 0;
				else
				{
					cflag--;
					forflg--;
					return(0);
				}
			}
			if(bflag) /* if the break flag is set */
			{
				bflag--;
				forflg--;
				return(0);
			}
			if(!(nam[0] == 'e' && nam[1] == 'v'))
			{
				getvar(expr,&var);
				var += step;
				if(step > 0)
				{
				if(var > final + round) /* if all done */
				{
					forflg--;
					return(0);
				}
				}
				else
				if(var < final+round)
				{
					forflg--;
					return(0);
				}
				putvar(expr,var);
			}
			ptr = save;
			if(singflg == 1)
				if(singwt() < 0)
				{
					singflg=stpflg=0;
					forflg--;
					return(0);
				}
			continue;
		}
		if(singflg == 1) /* if single stepping */
		{
			if(singex() < 0)
			{
			singflg = 0;
			forflg--;
			return(0);
			}
		}
		else
		{
			if(bflag || cflag)
			{
				if(inst.thing.opcode.lobyte == rof) /* if a for */
					forcnt++;
				continue;
			}
			if(bascall(inst.thing.opcode.lobyte) != 0)
			{
			forflg=0;
			return(-1);
			}
		}
	}
	forflg--;
	if(!stpflg)
	error(savno,48);
	return(0);
}
next()
{
	error(inst.thing.linno,18);
	return(-1);
}
brake() /*set the break flag */
{
	setflg(&bflag);
	return(0);
}
cont() /* set the continue flag */
{
	setflg(&cflag);
	return(0);
}

setflg(flg)
int *flg;
{
	if(forflg == 0) /* if no for in progress */
	{
		error(inst.thing.linno,47);
		return(-1); /* fatal */
	}
	if(expr[0] != '\0') /* if there is an expression */
		*flg = evalx(expr);
	else
		*flg = 1;
	return(0);
}
