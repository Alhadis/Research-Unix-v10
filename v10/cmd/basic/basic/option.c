/*	"@(#) option.c: V 1.10 8/19/83"	*/

/*
*	option.c: 	routines for option statement
*/

#include	"bas.h"

#include	"tokens.h"

extern short	base,
		dimseen;

extern char	errbuf[];

short		opbaset;

/*
*	processes option statement
*/

option()
{
	char	opstr[LINMAX+1],
		*optr;
	int	opnum;

	optr = opstr;
	skip00(&expr);
	while (ALPHA(*expr))
		*optr++ = *expr++;		/* get option name */
	*optr = '\0';
	skip00(&expr);
	if (!strcmp(opstr, "base"))
	{
		if (opbaset)
		{
			error(inst.thing.linno,inst.thing.stmtno, 77);
			return(-1);
		}
		if (dimseen)
		{
			error(inst.thing.linno,inst.thing.stmtno, 93);
			return(-1);
		}
		if (lex(&expr, opstr) != INTCONST)
		{
			error(inst.thing.linno,inst.thing.stmtno, 91);
			return(-1);
		}
		opnum = atoi(opstr);
		if ((opnum == 1) || (opnum == 0))
			base = opnum;
		else
		{
		        error(inst.thing.linno,inst.thing.stmtno, 91);
			return(-1);
		}
		opbaset = 1;
	}
	else
	{
		sprintf(errbuf,": %s\n", opstr);
		error2(inst.thing.linno,inst.thing.stmtno,90, errbuf);
	}
	return(0);
}
