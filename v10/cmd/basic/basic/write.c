/*
 *	"@(#) write.c: V 1.6 6/29/83"
 */
/*	WRITE command	*/

#include	<stdio.h>
#include	<values.h>
#include	"bas.h"
#include	"tokens.h"

extern char	*prncpy(),
		*delimcpy();

extern int	WRITEPcmd;

struct FILTBL 	{
		FILE		*fildes;
		char		mode;
		unsigned char	contin;
		short		currec;
		short		curchr;
		short		recsize;
		short		numrecs;
		char		*buffer;
		};

extern	struct FILTBL filtbl[];

extern short	startpos,
		margin;

extern unsigned char	contin;

bwrite()
{
	char		buf[LINMAX+1];
	short		dummystart = 1;
	int		filind;

	skip00(&expr);			/* skip preceding blanks and tabs */
	if (*expr == '#' || inst.thing.opcode == WRITEPcmd)
	{
		if (*expr == '#')
			++expr;
		skip00(&expr);
		expr = prncpy(buf, expr);
		if (eval(buf) < 0)
			return(-1);
		if (exprtype == STRING)
		{					/* string in numeric */
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		}
		if (exprtype == DOUBLE)
			exprval.ival = intify(exprval.dval);
		else if (exprtype == SINGLE)
			exprval.ival = intify(exprval.fval);
		if (!legfildes(exprval.ival))
		{
			error(inst.thing.linno,inst.thing.stmtno, 47);
			return(-1);
		}
		++expr;
		filind = exprval.ival - 1;
		if (filtbl[filind].fildes == NULL)
		{
			error(inst.thing.linno,inst.thing.stmtno,83);
			return(-1);
		}
		if (filtbl[filind].mode == 'r')
		{
			error(inst.thing.linno,inst.thing.stmtno,36);
			return(-1);
		}
		else if (filtbl[filind].mode == 'w')
			return(dowrite(filtbl[filind].fildes,
					&(filtbl[filind].curchr), 
					&(filtbl[filind].contin), filind));
		else
			return(dowrite(NULL, &dummystart,
					&(filtbl[filind].contin), filind));
	}
	else
		return(dowrite(stdout, &startpos, &contin, 0));
}




dowrite(filptr, startpos, contin, ftblind)
FILE		*filptr;
short		*startpos;
unsigned char	*contin;
int		ftblind;
{
	char	*wtext;				/* input text pointer */
	double	value;
	char	buf[LINMAX+1];			/* number text storage */
	int	n;				/* number txtbuf index */

	wtext = expr;
	skip00(&wtext);
	while (*wtext != '\0')
	{
		skip00(&wtext);			/* skip blanks and tabs */
		wtext = delimcpy(buf,wtext,";,");
		if (*buf == '\0')
			goto delim;
		if (eval(buf) < 0)
			return(-1);
		if (exprtype == STRING)
		{
			if ((*startpos) + strlen(exprval.sval)-1 > margin)
			{
				if (cput('\n', filptr, startpos, ftblind) < 0)
					return(-1);
			}
			if (cput('"', filptr, startpos, ftblind) < 0)
				return(-1);
			n = 0;
			while (exprval.sval[n] != '\0')
			{
				if (cput(exprval.sval[n], filptr,
							startpos, ftblind) < 0)
					return(-1);
				++n;
			}
			if (cput('"', filptr, startpos, ftblind) < 0)
				return(-1);
		}
		else
		{
			if (exprtype == DOUBLE)
				value = exprval.dval;
			else if (exprtype == SINGLE)
				value = exprval.fval;
			else
				value = (double) exprval.ival;
			gcvt(value, 6, buf);	/* convert to ascii */
			buf[strlen(buf) - 1] = '\0';
			if ((*startpos) + strlen(buf) > margin)
			{
				if (cput('\n', filptr, startpos, ftblind) < 0)
					return(-1);
			}
			n = 0;
			while (buf[n] != '\0')
			{
				if (cput(buf[n], filptr, startpos, ftblind) < 0)
					return(-1);
				++n;
			}
		}
delim:
		skip00(&wtext);			/* skip spaces and tabs */
		switch (*wtext)
		{
			case '\0':
				break;
			case ',':
			case ';':
				if (cput(',', filptr, startpos, ftblind) < 0)
					return(-1);
				++wtext;	/* bump past delim */
				break;
			default:
				error(inst.thing.linno,inst.thing.stmtno, 13);
				return(-1);
		}
		skip00(&wtext);			/* skip spaces and tabs */
	}
	(*startpos) = 1;
	(*contin) = 1;
	if (cput('\n', filptr, startpos, ftblind) < 0)
		return(-1); 			/*put in "newline" char*/
	return(0);
}
