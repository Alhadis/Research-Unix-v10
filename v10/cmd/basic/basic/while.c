/*	"@(#) while.c: V 1.9 8/19/83"	*/

#include <stdio.h>
#include "bas.h"


extern int	WHILEcmd,
		WENDcmd;

extern UCHAR	singflg,
		stmtno;

extern struct conrec	*contop;

extern char	*ptr;

extern LINNO	nextno;				/* for run.c */

struct whiletemplate {
	char	*lptr;
	char	*exprptr;
	LINNO	linno;
	short	stmtno;
}	whilestack[LOOPSTACKSIZE];

short		whiletop = -1;


__while()
{
	double	var;

	if (eval(expr) < 0)
		return(-1);
	switch(exprtype)
	{
		case STRING:
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		case INTEGER:
			var = (double) exprval.ival;
			break;
		case DOUBLE:
			var = exprval.dval;
			break;
		case SINGLE:
			var = exprval.fval;
			break;
	}
	if (!var)
	{
		if (skiptowend(inst.thing.linno,inst.thing.stmtno) < 0)
			return(-1);
		else
			return(0);
	}
	if (pushwhile() < 0)
		return(-1);
	whilestack[whiletop].lptr = ptr;
	whilestack[whiletop].exprptr = expr;
	whilestack[whiletop].linno = inst.thing.linno;
	whilestack[whiletop].stmtno = inst.thing.stmtno;
	return(0);
}



wend()
{
	double	var;

	if (whiletop <= contop->whilenest || whilestack[whiletop].lptr == NULL)
	{
		error(inst.thing.linno,inst.thing.stmtno, 59);
		return(-1);
	}
	inst.thing.linno = whilestack[whiletop].linno;
	inst.thing.stmtno = whilestack[whiletop].stmtno;
	if (eval(whilestack[whiletop].exprptr) < 0)
		return(-1);
	switch(exprtype)
	{
		case STRING:
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		case INTEGER:
			var = (double) exprval.ival;
			break;
		case DOUBLE:
			var = exprval.dval;
			break;
		case SINGLE:
			var = exprval.fval;
			break;
	}
	if (var)
		ptr = whilestack[whiletop].lptr;
	else if (popwhile() < 0)
		return(-1);
	return(0);
}




skiptowend(line,stmt)
LINNO	line;
int	stmt;
{
	int	whilecnt = 0;

	while (fetch(NEXT,STMT,&ptr) == 0)
	{
		if (inst.thing.opcode == WHILEcmd)
		{
			++whilecnt;
		}
		else if (inst.thing.opcode == WENDcmd)
		{
			if (!whilecnt)
				return(0);
			else
				--whilecnt;
		}
	}
	error(line,stmt,58);
	fetch(line,STMT,&lbdptr);
	return(-1);
}



pushwhile()
{
	if (++whiletop >= LOOPSTACKSIZE)
	{						/* too many WHILES */
		error(inst.thing.linno,inst.thing.stmtno,57);
		--whiletop;
		return(-1);
	}
	return(0);
}


popwhile()
{
	if (whiletop <= contop->whilenest)
	{
		error(inst.thing.linno,inst.thing.stmtno,59);
		return(-1);
	}
	whilestack[whiletop].lptr = NULL;
	whilestack[whiletop].exprptr = NULL;
	whilestack[whiletop].linno = 0;
	whilestack[whiletop].stmtno = 0;
	--whiletop;
	return(0);
}
