/*	"@(#) binops.c: V 1.2 6/14/83"	*/

/* binops.c - semantics for binary operators */

#include	<math.h>
#include	<setjmp.h>
#include	<values.h>
#include	"bas.h"

extern STAKREC 	semstack[],
		*semtop;

extern char	strstack[],
		*strtop;

extern		jmp_buf	overenv;	  	/* for overflow trapping */


#define	STRSTR	1
#define	STRINT	2
#define STRSNG  3
#define	STRDBL  4
#define	INTSTR  5
#define	INTINT  6
#define	INTSNG  7
#define	INTDBL  8
#define	SNGSTR  9
#define	SNGINT  10
#define	SNGSNG  11
#define	SNGDBL  12
#define	DBLSTR  13
#define	DBLINT  14
#define	DBLSNG  15
#define	DBLDBL  16

int	types[4][4] = 	{
			{ STRSTR, STRINT, STRSNG, STRDBL},
			{ INTSTR, INTINT, INTSNG, INTDBL},
			{ SNGSTR, SNGINT, SNGSNG, SNGDBL},
			{ DBLSTR, DBLINT, DBLSNG, DBLDBL},
			};


and()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		(semtop-1)->val.ival &= semtop->val.ival;
		semtop--;
		break;
	case INTSNG:
		(semtop-1)->val.ival &= mkint((double) semtop->val.fval);
		semtop--;
		break;
	case INTDBL:
		(semtop-1)->val.ival &= mkint(semtop->val.dval);
		semtop--;
		break;
	case SNGINT:
		(semtop-1)->val.ival = mkint((double) (semtop-1)->val.fval) 
					& semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case SNGSNG:
		(semtop-1)->val.ival = mkint((double) (semtop-1)->val.fval) 
					& mkint((double) semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case SNGDBL:
		(semtop-1)->val.ival = mkint((double) (semtop-1)->val.fval) 
					& mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	case DBLINT:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) & semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case DBLSNG:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) 
					& mkint((double) semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case DBLDBL:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) 
					& mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	return(0);
}



div()
{
	register	neg;
	register	singtry;

	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.ival;
		break;
	case INTSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.fval;
		break;
	case INTDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		break;
	case SNGINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.ival;
		break;
	case SNGSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.fval;
		break;
	case SNGDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		break;
	case DBLINT:
		singtry = 0;
		semtop->val.dval = semtop->val.ival;
		break;
	case DBLSNG:
		singtry = 0;
		semtop->val.dval = semtop->val.fval;
		break;
	case DBLDBL:
		singtry = 0;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	neg = (semtop-1)->val.dval < 0 ^ semtop->val.dval < 0;
	if (setjmp(overenv) != 0)
	{
		if (neg)
			(semtop-1)->val.dval = -MAXDOUBLE;
		else
			(semtop-1)->val.dval = MAXDOUBLE;
		(--semtop)->type = DOUBLE;
		return(0);
	}
	else if (semtop->val.dval == 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 29);
		(semtop-1)->val.dval = (neg) ? -MAXDOUBLE : MAXDOUBLE;
		(--semtop)->type = DOUBLE;
		return(0);
	}
	else
		(semtop-1)->val.dval = (semtop-1)->val.dval / semtop->val.dval;
	semtop--;
	if (singtry && semtop->val.dval >= -maxfloat 
		    && semtop->val.dval <= maxfloat)
	{
		semtop->val.fval = semtop->val.dval;
		semtop->type = SINGLE;
	}
	else
		semtop->type = DOUBLE;
	return(0);
}



eqv()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		(semtop-1)->val.ival = ~((semtop-1)->val.ival ^ semtop->val.ival);
		semtop--;
		break;
	case INTSNG:
		(semtop-1)->val.ival = ~((semtop-1)->val.ival 
					^ mkint((double) semtop->val.fval));
		semtop--;
		break;
	case INTDBL:
		(semtop-1)->val.ival = ~((semtop-1)->val.ival^mkint(semtop->val.dval));
		semtop--;
		break;
	case SNGINT:
		(semtop-1)->val.ival = ~(mkint((double) (semtop-1)->val.fval) 
					^ semtop->val.ival);
		(--semtop)->type = INTEGER;
		break;
	case SNGSNG:
		(semtop-1)->val.ival = ~(mkint((double) (semtop-1)->val.fval) 
					^ mkint((double) semtop->val.fval));
		(--semtop)->type = INTEGER;
		break;
	case SNGDBL:
		(semtop-1)->val.ival = ~(mkint((double) (semtop-1)->val.fval) 
					^ mkint(semtop->val.dval));
		(--semtop)->type = INTEGER;
		break;
	case DBLINT:
		(semtop-1)->val.ival = ~(mkint((semtop-1)->val.dval)^semtop->val.ival);
		(--semtop)->type = INTEGER;
		break;
	case DBLSNG:
		(semtop-1)->val.ival = ~(mkint((semtop-1)->val.dval) 
					^ mkint((double) semtop->val.fval));
		(--semtop)->type = INTEGER;
		break;
	case DBLDBL:
		(semtop-1)->val.ival = ~(mkint((semtop-1)->val.dval) 
					^ mkint(semtop->val.dval));
		(--semtop)->type = INTEGER;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	return(0);
}



eq()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case STRSTR:
		strtop = (semtop-1)->val.sval;
		if (strcmp((semtop-1)->val.sval, semtop->val.sval) == 0)
			(semtop-1)->val.ival = -1;
		else
			(semtop-1)->val.ival = 0;
		break;
	case INTINT:
		(semtop-1)->val.ival = -((semtop-1)->val.ival == semtop->val.ival);
		break;
	case INTSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.ival == semtop->val.fval);
		break;
	case INTDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.ival == semtop->val.dval);
		break;
	case SNGINT:
		(semtop-1)->val.ival = -((semtop-1)->val.fval == semtop->val.ival);
		break;
	case SNGSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.fval == semtop->val.fval);
		break;
	case SNGDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.fval == semtop->val.dval);
		break;
	case DBLINT:
		(semtop-1)->val.ival = -((semtop-1)->val.dval == semtop->val.ival);
		break;
	case DBLSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.dval == semtop->val.fval);
		break;
	case DBLDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.dval == semtop->val.dval);
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 80);
		return(-1);
	}
	(--semtop)->type = INTEGER;
	return(0);
}



raise()
{
	register	singtry;

	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.ival;
		break;
	case INTSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.fval;
		break;
	case INTDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		break;
	case SNGINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.ival;
		break;
	case SNGSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.fval;
		break;
	case SNGDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		break;
	case DBLINT:
		singtry = 0;
		semtop->val.dval = semtop->val.ival;
		break;
	case DBLSNG:
		singtry = 0;
		semtop->val.dval = semtop->val.fval;
		break;
	case DBLDBL:
		singtry = 0;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	if (setjmp(overenv) != 0)
		(semtop-1)->val.dval = MAXDOUBLE;
	else
	{
		if ((semtop-1)->val.dval == 0 && semtop->val.dval == 0)
			(semtop-1)->val.dval = 1;
		else if ((semtop-1)->val.dval == 0 && semtop->val.dval < 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 82);
			(semtop-1)->val.dval = MAXDOUBLE;
		}
		else
		{
			errno = 0;
			(semtop-1)->val.dval = pow((semtop-1)->val.dval, 
							semtop->val.dval);
			if (errno == DOMAIN)
				return(-1);
		}
	}
	semtop--;
	if (singtry && semtop->val.dval >= -maxfloat 
		    && semtop->val.dval <= maxfloat)
	{
		semtop->val.fval = semtop->val.dval;
		semtop->type = SINGLE;
	}
	else
		semtop->type = DOUBLE;
	return(0);
}



ge()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case STRSTR:
		strtop = (semtop-1)->val.sval;
		if (strcmp((semtop-1)->val.sval, semtop->val.sval) >= 0)
			(semtop-1)->val.ival = -1;
		else
			(semtop-1)->val.ival = 0;
		break;
	case INTINT:
		(semtop-1)->val.ival = -((semtop-1)->val.ival >= semtop->val.ival);
		break;
	case INTSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.ival >= semtop->val.fval);
		break;
	case INTDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.ival >= semtop->val.dval);
		break;
	case SNGINT:
		(semtop-1)->val.ival = -((semtop-1)->val.fval >= semtop->val.ival);
		break;
	case SNGSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.fval >= semtop->val.fval);
		break;
	case SNGDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.fval >= semtop->val.dval);
		break;
	case DBLINT:
		(semtop-1)->val.ival = -((semtop-1)->val.dval >= semtop->val.ival);
		break;
	case DBLSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.dval >= semtop->val.fval);
		break;
	case DBLDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.dval >= semtop->val.dval);
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 80);
		return(-1);
	}
	(--semtop)->type = INTEGER;
	return(0);
}



gt()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case STRSTR:
		strtop = (semtop-1)->val.sval;
		if (strcmp((semtop-1)->val.sval, semtop->val.sval) > 0)
			(semtop-1)->val.ival = -1;
		else
			(semtop-1)->val.ival = 0;
		break;
	case INTINT:
		(semtop-1)->val.ival = -((semtop-1)->val.ival > semtop->val.ival);
		break;
	case INTSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.ival > semtop->val.fval);
		break;
	case INTDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.ival > semtop->val.dval);
		break;
	case SNGINT:
		(semtop-1)->val.ival = -((semtop-1)->val.fval > semtop->val.ival);
		break;
	case SNGSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.fval > semtop->val.fval);
		break;
	case SNGDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.fval > semtop->val.dval);
		break;
	case DBLINT:
		(semtop-1)->val.ival = -((semtop-1)->val.dval > semtop->val.ival);
		break;
	case DBLSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.dval > semtop->val.fval);
		break;
	case DBLDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.dval > semtop->val.dval);
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 80);
		return(-1);
	}
	(--semtop)->type = INTEGER;
	return(0);
}



idiv()
{
	int	divisor;

	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		if (semtop->val.ival == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 29);
			(semtop-1)->val.ival = ((semtop-1)->val.ival < 0) ? -MAXINT:
									     MAXINT;
		} else
			(semtop-1)->val.ival = (semtop-1)->val.ival / semtop->val.ival;
		--semtop;
		break;
	case INTSNG:
		if ((divisor = mkint(semtop->val.fval)) == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 29);
			(semtop-1)->val.ival = ((semtop-1)->val.ival < 0) ? -MAXINT:
									     MAXINT;
		} else
			(semtop-1)->val.ival = (semtop-1)->val.ival / divisor;
		--semtop;
		break;
	case INTDBL:
		if ((divisor = mkint(semtop->val.dval)) == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 29);
			(semtop-1)->val.ival = ((semtop-1)->val.ival < 0) ? -MAXINT:
									     MAXINT;
		} else
			(semtop-1)->val.ival = (semtop-1)->val.ival / divisor;
		--semtop;
		break;
	case SNGINT:
		if (semtop->val.ival == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 29);
			(semtop-1)->val.ival = ((semtop-1)->val.fval < 0) ? -MAXINT:
									     MAXINT;
		} else
			(semtop-1)->val.ival = mkint((semtop-1)->val.fval) / 
								semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case SNGSNG:
		if ((divisor = mkint(semtop->val.fval)) == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 29);
			(semtop-1)->val.ival = ((semtop-1)->val.fval < 0) ? -MAXINT:
									     MAXINT;
		} else
			(semtop-1)->val.ival = mkint((semtop-1)->val.fval) / divisor;
		(--semtop)->type = INTEGER;
		break;
	case SNGDBL:
		if ((divisor = mkint(semtop->val.dval)) == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 29);
			(semtop-1)->val.ival = ((semtop-1)->val.fval < 0) ? -MAXINT:
									     MAXINT;
		} else
			(semtop-1)->val.ival = mkint((semtop-1)->val.fval) / divisor;
		(--semtop)->type = INTEGER;
		break;
	case DBLINT:
		if (semtop->val.ival == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 29);
			(semtop-1)->val.ival = ((semtop-1)->val.dval < 0) ? -MAXINT:
									     MAXINT;
		} else
			(semtop-1)->val.ival = mkint((semtop-1)->val.dval) / 
								semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case DBLSNG:
		if ((divisor = mkint(semtop->val.fval)) == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 29);
			(semtop-1)->val.ival = ((semtop-1)->val.dval < 0) ? -MAXINT:
									     MAXINT;
		} else
			(semtop-1)->val.ival = mkint((semtop-1)->val.dval) / divisor;
		(--semtop)->type = INTEGER;
		break;
	case DBLDBL:
		if ((divisor = mkint(semtop->val.dval)) == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 29);
			(semtop-1)->val.ival = ((semtop-1)->val.dval < 0) ? -MAXINT:
									     MAXINT;
		} else
			(semtop-1)->val.ival = mkint((semtop-1)->val.dval) / divisor;
		(--semtop)->type = INTEGER;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	return(0);
}



imp()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		(semtop-1)->val.ival = ~(semtop-1)->val.ival | semtop->val.ival;
		semtop--;
		break;
	case INTSNG:
		(semtop-1)->val.ival = ~(semtop-1)->val.ival 
					| mkint((double) semtop->val.fval);
		semtop--;
		break;
	case INTDBL:
		(semtop-1)->val.ival = ~(semtop-1)->val.ival | mkint(semtop->val.dval);
		semtop--;
		break;
	case SNGINT:
		(semtop-1)->val.ival = ~mkint((double) (semtop-1)->val.fval) 
					| semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case SNGSNG:
		(semtop-1)->val.ival = ~mkint((double) (semtop-1)->val.fval) 
					| mkint((double) semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case SNGDBL:
		(semtop-1)->val.ival = ~mkint((double) (semtop-1)->val.fval) 
					| mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	case DBLINT:
		(semtop-1)->val.ival = ~mkint((semtop-1)->val.dval) | semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case DBLSNG:
		(semtop-1)->val.ival = ~mkint((semtop-1)->val.dval) 
					| mkint((double) semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case DBLDBL:
		(semtop-1)->val.ival = ~mkint((semtop-1)->val.dval) 
					| mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	return(0);
}



le()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case STRSTR:
		strtop = (semtop-1)->val.sval;
		if (strcmp((semtop-1)->val.sval, semtop->val.sval) <= 0)
			(semtop-1)->val.ival = -1;
		else
			(semtop-1)->val.ival = 0;
		break;
	case INTINT:
		(semtop-1)->val.ival = -((semtop-1)->val.ival <= semtop->val.ival);
		break;
	case INTSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.ival <= semtop->val.fval);
		break;
	case INTDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.ival <= semtop->val.dval);
		break;
	case SNGINT:
		(semtop-1)->val.ival = -((semtop-1)->val.fval <= semtop->val.ival);
		break;
	case SNGSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.fval <= semtop->val.fval);
		break;
	case SNGDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.fval <= semtop->val.dval);
		break;
	case DBLINT:
		(semtop-1)->val.ival = -((semtop-1)->val.dval <= semtop->val.ival);
		break;
	case DBLSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.dval <= semtop->val.fval);
		break;
	case DBLDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.dval <= semtop->val.dval);
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 80);
		return(-1);
	}
	(--semtop)->type = INTEGER;
	return(0);
}



lt()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case STRSTR:
		strtop = (semtop-1)->val.sval;
		if (strcmp((semtop-1)->val.sval, semtop->val.sval) < 0)
			(semtop-1)->val.ival = -1;
		else
			(semtop-1)->val.ival = 0;
		break;
	case INTINT:
		(semtop-1)->val.ival = -((semtop-1)->val.ival < semtop->val.ival);
		break;
	case INTSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.ival < semtop->val.fval);
		break;
	case INTDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.ival < semtop->val.dval);
		break;
	case SNGINT:
		(semtop-1)->val.ival = -((semtop-1)->val.fval < semtop->val.ival);
		break;
	case SNGSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.fval < semtop->val.fval);
		break;
	case SNGDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.fval < semtop->val.dval);
		break;
	case DBLINT:
		(semtop-1)->val.ival = -((semtop-1)->val.dval < semtop->val.ival);
		break;
	case DBLSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.dval < semtop->val.fval);
		break;
	case DBLDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.dval < semtop->val.dval);
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 80);
		return(-1);
	}
	(--semtop)->type = INTEGER;
	return(0);
}



minus()
{
	int	neg;
	register	singtry;

	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.ival;
		break;
	case INTSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.fval;
		break;
	case INTDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		break;
	case SNGINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.ival;
		break;
	case SNGSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.fval;
		break;
	case SNGDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		break;
	case DBLINT:
		singtry = 0;
		semtop->val.dval = semtop->val.ival;
		break;
	case DBLSNG:
		singtry = 0;
		semtop->val.dval = semtop->val.fval;
		break;
	case DBLDBL:
		singtry = 0;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}

	neg = (semtop-1)->val.dval < 0;
	if (setjmp(overenv) != 0)
	{
	      if (neg)
		      (semtop-1)->val.dval = -MAXDOUBLE;
	      else
		      (semtop-1)->val.dval = MAXDOUBLE;
	}
	else
	      (semtop-1)->val.dval = (semtop-1)->val.dval - semtop->val.dval;
	semtop--;
	if (singtry && semtop->val.dval >= -maxfloat 
		    && semtop->val.dval <= maxfloat)
	{
		semtop->val.fval = semtop->val.dval;
		semtop->type = SINGLE;
	}
	else
		semtop->type = DOUBLE;
	return(0);
}



mod()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		(semtop-1)->val.ival = (semtop-1)->val.ival % semtop->val.ival;
		--semtop;
		break;
	case INTSNG:
		(semtop-1)->val.ival = (semtop-1)->val.ival % mkint(semtop->val.fval);
		--semtop;
		break;
	case INTDBL:
		(semtop-1)->val.ival = (semtop-1)->val.ival % mkint(semtop->val.dval);
		--semtop;
		break;
	case SNGINT:
		(semtop-1)->val.ival = mkint((semtop-1)->val.fval) % semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case SNGSNG:
		(semtop-1)->val.ival = mkint((semtop-1)->val.fval) % 
							mkint(semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case SNGDBL:
		(semtop-1)->val.ival = mkint((semtop-1)->val.fval) % 
							mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	case DBLINT:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) % semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case DBLSNG:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) % 
							mkint(semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case DBLDBL:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) % 
							mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	return(0);
}



mult()
{
	int	neg;
	register	singtry;

	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.ival;
		break;
	case INTSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.fval;
		break;
	case INTDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		break;
	case SNGINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.ival;
		break;
	case SNGSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.fval;
		break;
	case SNGDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		break;
	case DBLINT:
		singtry = 0;
		semtop->val.dval = semtop->val.ival;
		break;
	case DBLSNG:
		singtry = 0;
		semtop->val.dval = semtop->val.fval;
		break;
	case DBLDBL:
		singtry = 0;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}

	neg = (semtop-1)->val.dval < 0 ^ semtop->val.dval < 0;
	if (setjmp(overenv) != 0)
	{
	      if (neg)
		      (semtop-1)->val.dval = -MAXDOUBLE;
	      else
		      (semtop-1)->val.dval = MAXDOUBLE;
	}
	else
	      (semtop-1)->val.dval = (semtop-1)->val.dval * semtop->val.dval;
	semtop--;
	if (singtry && semtop->val.dval >= -maxfloat 
		    && semtop->val.dval <= maxfloat)
	{
		semtop->val.fval = semtop->val.dval;
		semtop->type = SINGLE;
	}
	else
		semtop->type = DOUBLE;
	return(0);
}



ne()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case STRSTR:
		strtop = (semtop-1)->val.sval;
		if (strcmp((semtop-1)->val.sval, semtop->val.sval) != 0)
			(semtop-1)->val.ival = -1;
		else
			(semtop-1)->val.ival = 0;
		break;
	case INTINT:
		(semtop-1)->val.ival = -((semtop-1)->val.ival != semtop->val.ival);
		break;
	case INTSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.ival != semtop->val.fval);
		break;
	case INTDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.ival != semtop->val.dval);
		break;
	case SNGINT:
		(semtop-1)->val.ival = -((semtop-1)->val.fval != semtop->val.ival);
		break;
	case SNGSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.fval != semtop->val.fval);
		break;
	case SNGDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.fval != semtop->val.dval);
		break;
	case DBLINT:
		(semtop-1)->val.ival = -((semtop-1)->val.dval != semtop->val.ival);
		break;
	case DBLSNG:
		(semtop-1)->val.ival = -((semtop-1)->val.dval != semtop->val.fval);
		break;
	case DBLDBL:
		(semtop-1)->val.ival = -((semtop-1)->val.dval != semtop->val.dval);
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 80);
		return(-1);
	}
	(--semtop)->type = INTEGER;
	return(0);
}



or()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		(semtop-1)->val.ival |= semtop->val.ival;
		semtop--;
		break;
	case INTSNG:
		(semtop-1)->val.ival |= mkint((double) semtop->val.fval);
		semtop--;
		break;
	case INTDBL:
		(semtop-1)->val.ival |= mkint(semtop->val.dval);
		semtop--;
		break;
	case SNGINT:
		(semtop-1)->val.ival = mkint((double) (semtop-1)->val.fval) 
					| semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case SNGSNG:
		(semtop-1)->val.ival = mkint((double) (semtop-1)->val.fval) 
					| mkint((double) semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case SNGDBL:
		(semtop-1)->val.ival = mkint((double) (semtop-1)->val.fval) 
					| mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	case DBLINT:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) | semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case DBLSNG:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) 
					| mkint((double) semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case DBLDBL:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) 
					| mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	return(0);
}



plus()
{
	register char	*tptr;
	int		neg;
	register	singtry;

	switch(types[(semtop-1)->type][semtop->type])
	{
	case STRSTR:
		tptr = semtop->val.sval - 1;
		while (*tptr = *(tptr+1))
			tptr++;
		if ((tptr - (semtop-1)->val.sval) > STRMAX)
		{
			error(inst.thing.linno,inst.thing.stmtno, 87);
			return(-1);
		}
		strtop--;
		semtop--;
		return(0);
	case INTINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.ival;
		break;
	case INTSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		semtop->val.dval = semtop->val.fval;
		break;
	case INTDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.ival;
		break;
	case SNGINT:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.ival;
		break;
	case SNGSNG:
		singtry = 1;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		semtop->val.dval = semtop->val.fval;
		break;
	case SNGDBL:
		singtry = 0;
		(semtop-1)->val.dval = (semtop-1)->val.fval;
		break;
	case DBLINT:
		singtry = 0;
		semtop->val.dval = semtop->val.ival;
		break;
	case DBLSNG:
		singtry = 0;
		semtop->val.dval = semtop->val.fval;
		break;
	case DBLDBL:
		singtry = 0;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 80);
		return(-1);
	}
	neg = (semtop-1)->val.dval < 0;
	if (setjmp(overenv) != 0)
	{
	      if (neg)
		      (semtop-1)->val.dval = -MAXDOUBLE;
	      else
		      (semtop-1)->val.dval = MAXDOUBLE;
	}
	else
	      (semtop-1)->val.dval = (semtop-1)->val.dval + semtop->val.dval;
	semtop--;
	if (singtry && semtop->val.dval >= -maxfloat 
		    && semtop->val.dval <= maxfloat)
	{
		semtop->val.fval = semtop->val.dval;
		semtop->type = SINGLE;
	}
	else
		semtop->type = DOUBLE;
	return(0);
}



xor()
{
	switch(types[(semtop-1)->type][semtop->type])
	{
	case INTINT:
		(semtop-1)->val.ival ^= semtop->val.ival;
		semtop--;
		break;
	case INTSNG:
		(semtop-1)->val.ival ^= mkint((double) semtop->val.fval);
		semtop--;
		break;
	case INTDBL:
		(semtop-1)->val.ival ^= mkint(semtop->val.dval);
		semtop--;
		break;
	case SNGINT:
		(semtop-1)->val.ival = mkint((double) (semtop-1)->val.fval) 
					^ semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case SNGSNG:
		(semtop-1)->val.ival = mkint((double) (semtop-1)->val.fval) 
					^ mkint((double) semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case SNGDBL:
		(semtop-1)->val.ival = mkint((double) (semtop-1)->val.fval) 
					^ mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	case DBLINT:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) ^ semtop->val.ival;
		(--semtop)->type = INTEGER;
		break;
	case DBLSNG:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) 
					^ mkint((double) semtop->val.fval);
		(--semtop)->type = INTEGER;
		break;
	case DBLDBL:
		(semtop-1)->val.ival = mkint((semtop-1)->val.dval) 
					^ mkint(semtop->val.dval);
		(--semtop)->type = INTEGER;
		break;
	default:
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	return(0);
}
