/*	"@(#) for.c: V 2.29 11/10/83"	*/

#include <stdio.h>
#include "bas.h"
#include "tokens.h"


extern int	FORcmd,
		NEXTcmd;

extern UCHAR	stmtno;

extern char	*ptr;

extern struct conrec	*contop;

struct sym	*stsrch();

extern double	lpvarval();

extern LINNO	nextno;				/* for run.c */

struct fortemplate {
	struct	sym *ctlvar;
	char	*lptr;
	double	increm;
	double	final;
}	forstack[LOOPSTACKSIZE];

short	fortop = -1;




_for()
{
	extern char	*eoexpr;
	char		*varptr,
			*tlptr,
			nam[IDSIZE+1];
	LINNO		ltmp;
	double		var,
			final,
			step;
	int		vartype,
			stmp,
			i;
	struct sym	*namptr;

	ltmp = inst.thing.linno;
	stmp = inst.thing.stmtno;
	tlptr = ptr;
	skip00(&expr);		/* skip preceding blanks and tabs */
	varptr = expr;
	switch(vartype = lex(&varptr, nam))
	{
	case DBLID:
	case SNGID:
	case INTID:
		break;
	default:					/* bad var name */
		error(inst.thing.linno,inst.thing.stmtno,3);
		return(-1);
	}
	namptr = stsrch(nam, vartype);
	i = varptr - expr;
	while ((expr[i] & 0377) != TOcode && (expr[i] & 0377) != '\0')
		++i;
	eoexpr = expr + i;
	if ((*(eoexpr++) & 0377) != TOcode)
	{       				/* make sure its a to */
		error(inst.thing.linno,inst.thing.stmtno,21);
		return(-1);
	}
	if (eval(eoexpr) < 0)			/* evaluate the TO expression */
		return(-1);
	switch(exprtype)
	{
	case STRING:  error(inst.thing.linno,inst.thing.stmtno, 49);
		      return(-1);
	case INTEGER: final = exprval.ival;
		      break;
	case DOUBLE:  final = exprval.dval;
		      break;
	case SINGLE:  final = exprval.fval;
		      break;
	}
	step = 1;			/* default value for step */
	if ((*eoexpr & 0377) == STEPcode)
	{					/* if step is present */
		eoexpr++;
		if (eval(eoexpr) < 0)
			return(-1);
		switch(exprtype)
		{
		case STRING:  error(inst.thing.linno,inst.thing.stmtno, 49);
			      return(-1);
		case INTEGER: step = exprval.ival;
			      break;
		case DOUBLE:  step = exprval.dval;
			      break;
		case SINGLE:  step = exprval.fval;
			      break;
		}
	}
	if (let() < 0)	/* handle the initial value expression */
		return(-1);
	namptr = stsrch(nam, vartype);
	var = lpvarval(namptr);
	if (step > 0)
	{
		if (var > final)
		{
			if (skiptonext(ltmp,stmp,namptr) < 0)
				return(-1);
			else
				return(0);
		}
	}
	else if (step < 0)
	{
		if (var < final)
		{
			if (skiptonext(ltmp,stmp,namptr) < 0)
				return(-1);
			else
				return(0);
		}
	}
	if (pushfor() < 0)
		return(-1);
	forstack[fortop].ctlvar = namptr;
	forstack[fortop].lptr = tlptr;
	forstack[fortop].increm = step;
	forstack[fortop].final = final;
	return(0);
}



next()
{
	double	var;
	int	vartype;
	char	buf[IDSIZE+1];

	skip00(&expr);
	if (*expr != '\0')
	{
		while (*expr != '\0')
		{
			vartype = lex(&expr, buf);
			if (fortop <= contop->fornest ||
				!samevar(forstack[fortop].ctlvar,buf,vartype))
			{
				error(inst.thing.linno,inst.thing.stmtno,18);
				return(-1);
			}
			var = lpvarval(forstack[fortop].ctlvar) +
							forstack[fortop].increm;
			lpvarset(forstack[fortop].ctlvar, var);
			if ((forstack[fortop].increm > 0 &&
						var > forstack[fortop].final)
				|| (forstack[fortop].increm < 0 &&
						var < forstack[fortop].final))
			{
				if (popfor() < 0)
					return(-1);
			}
			else
			{
				ptr = forstack[fortop].lptr;
				return(0);
			}
			skip00(&expr);
			if (*expr == ',')
			{
				++expr;
				skip00(&expr);
			}
		}
	}
	else
	{
		if (fortop <= contop->fornest || forstack[fortop].ctlvar ==NULL)
		{
			error(inst.thing.linno,inst.thing.stmtno,18);
			return(-1);
		}
		var = lpvarval(forstack[fortop].ctlvar) +
						forstack[fortop].increm;
		lpvarset(forstack[fortop].ctlvar, var);
		if (forstack[fortop].increm > 0 && var > forstack[fortop].final)
		{
			if (popfor() < 0)
				return(-1);
		}
		else if (forstack[fortop].increm < 0 &&
					var < forstack[fortop].final)
		{
			if (popfor() < 0)
				return(-1);
		}
		else
			ptr = forstack[fortop].lptr;
	}
	return(0);
}



skiptonext(line,stmt,nam)
LINNO		line;
int		stmt;
struct sym	*nam;
{
	int	vartype,
		forcnt = 0;
	char	buf[IDSIZE+1];

	while (fetch(NEXT, STMT, &ptr) == 0)
	{
		if (inst.thing.opcode == FORcmd)
		{
			++forcnt;
		}
		else if (inst.thing.opcode == NEXTcmd)
		{
			if (!forcnt)
			{
				vartype = lex(&expr,buf);
				if (vartype != 0 && !samevar(nam, buf, vartype))
				{
				     error(inst.thing.linno,inst.thing.stmtno,
									18);
				     return(-1);
				}
				return(0);
			}
			else
				--forcnt;
		}
	}
	error(line,stmt,74);
	fetch(line,STMT,&lbdptr);
	return(-1);
}



pushfor()
{
	if (++fortop >= LOOPSTACKSIZE)
	{						/* too many FORs */
		error(inst.thing.linno,inst.thing.stmtno,70);
		--fortop;
		return(-1);
	}
	return(0);
}


popfor()
{
	if (fortop <= contop->fornest)
	{
		error(inst.thing.linno,inst.thing.stmtno,18);
		return(-1);
	}
	forstack[fortop].ctlvar = NULL;
	forstack[fortop].lptr = NULL;
	forstack[fortop].increm = 0;
	forstack[fortop].final = 0;
	--fortop;
	return(0);
}
