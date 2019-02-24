/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	***** SINGLE STEP CONTROLLER ******
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Sun Aug 26 11:35:49 1979 *****
 *	***********************************
 *	***********************************

*/
/*   "@(#) sing.c:  V 1.1  12/21/80" */

/* single step monitor */
#include "bas.h"
extern char _string[];
extern int stpflg;
int singst = 32767;
int singflg;
extern char *ptr;
int stopl(),quit();
sing()
{
	char *ch;
	ch = expr;
	singflg = -1; /* control from sing but not single stepping */
	expunge();
	restore();
	signal(2,stopl);
	if(fetch(0,&ptr) < 0)
	{
		error(0,5);
		singflg = 0;
		signal(2,quit);
		return(-1);
	}
	if(ch[0] != '\0')
		singst = atoi(ch);
	else
		singst = inst.thing.linno;
	while(singex() == 0 && stpflg == 0)
		if(fetch(-1,&ptr) < 0)
		{
			singflg = 0; /* done single stepping */
			signal(SIGINT, quit);
			 return(0);
		}
	singflg=stpflg=0;
	signal(SIGINT, quit);
	return(0);
}
extern int subog;
singex()
{
	char *save;
	int hold;
	if(inst.thing.linno == singst)
		singflg = 1;
	if(singflg == 1)
	{
	save = expr;
	prin();
	expr = save;
	hold = inst.thing.opcode.lobyte;
	}
	if(bascall(inst.thing.opcode.lobyte) != 0)
		error(inst.thing.opcode.lobyte,15); /* execution error */
	if(singflg == -1)
		return(0);
	if(hold == subog)
		return(0);
	return(singwt());
}
singwt()
{
	char *save;
	if(singflg == 0) return(-1);
	while(1)
	{
 		printf("%c",'^');
		if(fgets(_string,LINMAX,stdin) == 0) return(-1); /* ctl d */
		if(_string[0] == '\n') return(0);
		save = ptr;
		bed((FILE *)0);
		ptr = save;
		if(singflg == -1) break;
	}
	return(0);
}
con()
{
	if(expr[0] != 0)
		singst = atoi(expr);
	else
		singst = 32767;
	singflg = -1;
}
