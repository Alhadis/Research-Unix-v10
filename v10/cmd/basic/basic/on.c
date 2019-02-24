/*	"@(#) on.c: V 2.17 11/10/83"	*/

/* routine to handle on goto */


#include <values.h>
#include <bas.h>
#include "tokens.h"


char		*prncpy();

extern char	*expr,
		*eoexpr;



on()
{
	int	i,
		j;
	double	res;
	char	buf[LINMAX+1],
		*checkptr,
		c;

	checkptr = expr;
	skip00(&checkptr);
	if ((*checkptr & 0377) == ERRORcode)
		return(onerrproc());
	if (eval(expr) < 0)
		return(-1);
	switch(exprtype)
	{
	case STRING:  error(inst.thing.linno,inst.thing.stmtno, 49);
		      return(-1);
	case INTEGER: res = exprval.ival;
		      break;
	case DOUBLE:  res = exprval.dval;
		      break;
	case SINGLE:  res = exprval.fval;
		      break;
	}
	if (res >= MAXINT)
	{
		error(inst.thing.linno,inst.thing.stmtno,38);
		return(-1);
	}
	if ((j = (res + 0.5)) < 0)
	{
		error(inst.thing.linno,inst.thing.stmtno,38);
		return(-1);
	}
	else if (j == 0)
		return(0);
	skip00(&eoexpr);	/* skip blanks and tabs before 1st keyword */
	if (((c = *eoexpr++) & 0377) != GOTOcode && (c & 0377) != GOSUBcode)
	{
		error(inst.thing.linno,inst.thing.stmtno,8);		/* expression syntax */
		return(-1);
	}
	for (i = 0; i < j; i++)
	{
		skip00(&eoexpr);     /* skip blanks and tabs between line #'s */
		eoexpr = prncpy(buf,eoexpr);
		if (*eoexpr++ == '\0' && i != (j - 1))
			return(0);
	}
	expr = buf;
	if ((c & 0377) != GOSUBcode)
	{
		if (__goto() < 0)
			return(-1);
		return(0);
	}
	if (gosub() < 0)
		return(-1);
	return(0);
}
