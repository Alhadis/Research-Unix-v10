 /*
 * BASIC IF routine
 */
/*	"@(#) if.c: V 2.35 8/19/83"	*/

#include	<stdio.h>
#include	<ctype.h>
#include	"bas.h"

extern LINNO	nextno;

extern int	GOTOcmd,
		GOSUBcmd,
		BANGcmd,
		IFcmd,
		ELSEcmd,
		ENDIFcmd;

extern struct conrec	*contop;

int	true;				/* set if relation is true */

short	endfcnt = 0;

extern char	*eoexpr,		/* pointer to char after eval expr */
		*keyword[],
		*ptr;

extern UCHAR	stmtno;


__if()
{
	char	ch;
	LINNO	l;
	int	s;

	if (eval(expr) < 0)
		return(-1);
	switch (exprtype)
	{
	case STRING:	error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
	case INTEGER:	true = (exprval.ival == 0 ? 0 : 1);
			break;
	case DOUBLE:	true = (exprval.dval == 0 ? 0 : 1);
			break;
	case SINGLE:	true = (exprval.fval == 0 ? 0 : 1);
			break;
	}
	ch = *eoexpr;
	if ((ch & 0377) != THENcode && (ch & 0377) != GOTOcode 
				    && (ch & 0377) != GOSUBcode)
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	eoexpr++;
	skip00(&eoexpr);
	l = inst.thing.linno;
	s = inst.thing.stmtno;
	if (*eoexpr == '\0' && fetch(NEXT, STMT, &ptr) == 0 &&
							l != inst.thing.linno)
	{				/* if nothing follows "then" we */
		fetch(l, s, &ptr);		/* expect structured "if" */
		++endfcnt;
		if (!true)
			return(untilcod(ELSEcmd));	/* skip to ELSE */
		else
			return(0);
	}
	else					/* normal BASIC "IF" */
	{
		fetch(l, s, &ptr);
		expr = eoexpr;
		if (true)
		{
			if (((ch & 0377) == GOTOcode || (ch & 0377) == THENcode)
 						    && NUM(*expr))
			{
				if (__goto() < 0) 	/* goto the linnum */
					return(-1);	/* if no error */
				return(0);
			}
			if ((ch & 0377) == THENcode && !NUM(*expr))
				return(if_exec());	/* executable stat. */
			if ((ch & 0377) == GOSUBcode)
			{
				if (gosub() < 0)	/* gosub the linnum */
					return(-1);	/* unless error */
				return(0);
			}
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		else				/* search for possible ELSE */
		{
			if (to_else() < 0)		/* skip to ELSE part */
				return(0);		/* no action taken */
			skip00(&expr);
			if (NUM(*expr))			/* GOTO line number */
				return((__goto() < 0) ? -1 : 0);
			else
				return(if_exec());	/* executable stmt */
		}
	}
}



/*	routine to handle executable statement in one-line IF-THEN-ELSE    */
if_exec()
{
	char		keyword[OPMAX];
	int		j;

	/* shell escape */
	if (*expr == '!')
	{
		++expr;
		j = BANGcmd;
	}
	/* GOTO & GOSUB  (already encoded) */
	else if ((*expr & 0377) == GOTOcode || (*expr & 0377) == GOSUBcode)
	{
		j = ((*expr & 0377) == GOTOcode) ? GOTOcmd : GOSUBcmd;
		++expr;
	}
	/* NEXT (already encoded) */
	else if ((*expr & 0377) == NEXTcode)
	{
		j = encode("next");
		++expr;
	}
		/* other keywords */
	else
		j = getcommand(&expr,keyword);

	return(bascall(j));
}



/*
 *	to_else fetches statements until a matching ELSE is found.  If the
 *	ELSE is found, 0 is returned.
 *	IF no matching ELSE is present, -1 is returned and the last
 *	statement on the line is fetched.
 */


to_else()
{
	int	i,
		s,
		nest = 0;
	LINNO	l;

	if (strncmp(expr,"if",2) == 0)
	{			/* found a nested IF? */
		expr += 2;
		if (*expr == ' ' || *expr == '\t')
			++nest;		/* YES */
	}
	l = inst.thing.linno;
	s = inst.thing.stmtno;
	while ((i = fetch(NEXT, STMT, &ptr)) == 0 && l == inst.thing.linno)
	{
		if (inst.thing.opcode == ELSEcmd)
		{					/* found an ELSE? */
			if (nest == 0)
				break;			/* found it! */
			else
				--nest;			/* wrong one */
		}
		else if (inst.thing.opcode == IFcmd)
			++nest;
		l = inst.thing.linno;
		s = inst.thing.stmtno;
	}
	if (i == -1 || l != inst.thing.linno)
	{
		fetch(l, s, &ptr);
		return(-1);				/* no ELSE */
	}
	else
		return(0);
}



_else()
{
	LINNO	l;
	int	s;

	if (inst.thing.stmtno == 1)
	{
		skip00(&expr);
		if (*expr != '\0')
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		l = inst.thing.linno;
		s = inst.thing.stmtno;
		if (fetch(NEXT, STMT, &ptr) == 0 && l == inst.thing.linno)
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		fetch(l, s, &ptr);
		if (endfcnt <= contop->ifnest)
		{
			error(inst.thing.linno,inst.thing.stmtno, 44);	/* ELSE WITH NO IF */
			return(-1);
		}
		return(untilcod(ENDIFcmd));
	}
	else
	{
		l = inst.thing.linno;
		s = inst.thing.stmtno;
		while (fetch(NEXT, STMT, &ptr) == 0 && l == inst.thing.linno)
		{
			l = inst.thing.linno;
			s = inst.thing.stmtno;
		}
		fetch(l, s, &ptr);
		return(0);
	}
}




_endif()
{
	LINNO	l;
	int	s;

	if (endfcnt <= contop->ifnest)
	{
		error(inst.thing.linno,inst.thing.stmtno, 45);		/* ENDIF WITH NO IF */
		return(-1);
	}
	skip00(&expr);
	if (*expr != '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	l = inst.thing.linno;
	s = inst.thing.stmtno;
	if (fetch(NEXT, STMT, &ptr) == 0 && l == inst.thing.linno)
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	fetch(l, s, &ptr);
	--endfcnt;
	return(0);
}



 /* Skip command lines until specified code (endcod)
 * is reached.
 * This routine terminates if:
 *	endcod is encountered
 *	end of text is encountered
 *	error is encountered
 *
 * This routine is used for the structured "if" for skipping
 * or executing until the "else" or "endif" is encountered.
 */



untilcod(endcod)
int	endcod;					/* termination code */
{
	while (fetch(NEXT,STMT,&ptr) == 0)
	{
		/*
		 * ALWAYS terminate if endif encountered
		 */

		if (inst.thing.opcode == ENDIFcmd ||
				inst.thing.opcode == endcod)
			break;

			/*
			 * IF TERMINATION CODE REACHED
			 */

		if (inst.thing.opcode == IFcmd)
		{
			while ((*expr & 0377) != THENcode && *expr != '\0')
				++expr;
			if (*expr != '\0')
			{
				++expr;
				skip00(&expr);
				if (*expr == '\0')
					untilcod(ENDIFcmd);
			}
		}
	}
	if (endcod == ELSEcmd && inst.thing.opcode == ELSEcmd)
		fetch(NEXT, STMT, &ptr);
	nextno = inst.thing.linno;
	stmtno = inst.thing.stmtno;
	return(0);
}
