 /*
 *	VARIABLE ASSIGNMENT ROUTINE for
 *	    LET command
 */
/*	"@(#) let.c: V 2.20 6/29/83"	*/

#include	<stdio.h>
#include	<bas.h>
#include	"tokens.h"

double		numval();	/* returns numeric expression value */

let()
{
	int	type;					/* field type */
	char	varnam[LINMAX+1];
	char	*lexptr;			/* string pointer for let */

	lexptr = expr;				/* set text pointer */
	if ((type = class(&lexptr, varnam)) < 0)	/* get field type */
	{
		error(inst.thing.linno,inst.thing.stmtno, 3);	/* ill var name */
		return(-1);
	}
	skip00(&lexptr);
	if ((*lexptr++ & 0377) == EQcode)		/* if code for '=' */
	{
		if (eval(lexptr) < 0)		/* eval right side */
			return(-1);
		switch (type)
		{
			case DBLID:
				if (exprtype == STRING)
				{		/* STRING IN NUMERIC EXPR. */
					error(inst.thing.linno,inst.thing.stmtno, 49);
					return(-1);
				}
				if (dputvar(varnam, numval()) < 0)
					return(-1);
				return(0);
			case DBLAID:		 /* subscripted var field */
				if (exprtype == STRING)
				{		/* STRING IN NUMERIC EXPR. */
					error(inst.thing.linno,inst.thing.stmtno, 49);
					return(-1);
				}
				if (daputvar(varnam, numval()) < 0)
					return(-1);
				return(0);
			case SNGID:
				if (exprtype == STRING)
				{		/* STRING IN NUMERIC EXPR. */
					error(inst.thing.linno,inst.thing.stmtno, 49);
					return(-1);
				}
				if (fputvar(varnam, numval()) < 0)
					return(-1);
				return(0);
			case SNGAID:		 /* subscripted var field */
				if (exprtype == STRING)
				{		/* STRING IN NUMERIC EXPR. */
					error(inst.thing.linno,inst.thing.stmtno, 49);
					return(-1);
				}
				if (faputvar(varnam, numval()) < 0)
					return(-1);
				return(0);
			case INTID: 		/* integer variable name */
				if (exprtype == STRING)
				{		/* STRING IN NUMERIC EXPR. */
					error(inst.thing.linno,inst.thing.stmtno, 49);
					return(-1);
				}
				if (iputvar(varnam, numval()) < 0)
					return(-1);
				return(0);
			case INTAID:		 /* subscripted var field */
				if (exprtype == STRING)
				{		/* STRING IN NUMERIC EXPR. */
					error(inst.thing.linno,inst.thing.stmtno, 49);
					return(-1);
				}
				if (iaputvar(varnam, numval()) < 0)
					return(-1);
				return(0);
			case STRID:			 /* String variable */
				if (exprtype != STRING)
				{		/* NUMERIC IN STRING EXPR. */
					error(inst.thing.linno,inst.thing.stmtno, 51);
					return(-1);
				}
				if (sputvar(varnam, exprval.sval) < 0)
					return(-1);
				return(0);
			case STRAID: 		/* String array variable */
				if (exprtype != STRING)
				{		/* NUMERIC IN STRING EXPR. */
					error(inst.thing.linno,inst.thing.stmtno, 51);
					return(-1);
				}
				if (saputvar(varnam, exprval.sval) < 0)
					return(-1);
				return(0);
			default:
				break;
		}
	}
	else
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);		 /* missing = */
		return(-1);
	}
	return(0);
}


midstmt()
{
	double	numval();
	char	nambuf[LINMAX+1];
	char	expbuf[LINMAX+1];
	char	*subject,
		**subptr,
		*elemaddr(),
		*scalaraddr(),
		*replace,
		*delimcpy();
	int	sublen,
		n,
		m = STRMAX;
	struct sym	*stsrch(),
			*currec;

	if (*expr++ != '(')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	switch (class(&expr, nambuf))
	{
	case STRID:
		if ((currec = stsrch(nambuf, STRID)) != NULL)
			if (currec->isbufvar)
			{
				subject = currec->data.bufvar.start;
				sublen  = currec->data.bufvar.length;
				break;
			}
		if ((subptr = (char **) scalaraddr(nambuf)) == NULL)
			return(-1);
		subject = *subptr;
		sublen  = strlen(subject);
		break;
	case STRAID:
		if ((subptr = (char **) elemaddr(nambuf)) == NULL)
			return(-1);
		subject = *subptr;
		sublen  = strlen(subject);
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	skip00(&expr);
	if (*expr++ != ',')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	expr = delimcpy(expbuf, expr, ",)");
	if (eval(expbuf) < 0)
		return(-1);
	if (exprtype == STRING)
	{
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	n = numval();
	if (*expr == ',')
	{
		expr++;
		expr = delimcpy(expbuf, expr, ")");
		if (eval(expbuf) < 0)
			return(-1);
		if (exprtype == STRING)
		{
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		}
		m = numval();
		if (n + m > sublen)
		{
			error(inst.thing.linno,inst.thing.stmtno, 55);
			return(-1);
		}
	}
	if (*expr++ != ')')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	skip00(&expr);
	if ((*expr++ & 0377) != EQcode)
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	if (eval(expr) < 0)
		return(-1);
	if (exprtype != STRING)
	{
		error(inst.thing.linno,inst.thing.stmtno, 51);
		return(-1);
	}
	replace = exprval.sval;
	if (n < 1 || n > sublen || m < 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 55);
		return(-1);
	}
	subject += n - 1;
	sublen  -= n - 1;
	while (sublen-- > 0 && *replace && m-- > 0)
		*subject++ = *replace++;
	return(0);
}
