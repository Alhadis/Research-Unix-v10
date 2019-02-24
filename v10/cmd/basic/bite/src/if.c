/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// if.c ////////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 07:52:59 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * BASIC IF routine
 *
 * MOD,J.P.Hawkins,2-FEB-81, Added string variable comparison capability.
 */
/*   "@(#) if.c:  V 1.5  9/9/81" */

#include	"bas.h"
extern	int	esle,fidne;	/* codes for "else" and "endif" */
extern char *eoexpr;	/* pointer to char after eval expr */
/*
#define	skip00() {while(*eoexpr == ' ' || *eoexpr == '\t') *eoexpr++;}
*/
#define	skip00()	{}	/* skip00 does nothing */
extern	struct FILTBL filtbl[];
__if()
{
#ifdef	STRINGS
	char	strbuf1[80];		/* pointer to string 1 */
	char	strbuf2[80];		/* pointer to string 2 */
	int	typflg;			/* 0 if numerical 1 if string */
	char	field[40];
#endif
	int	fnum;
	char	*savptr;
	double	evalx();
	int	true;		/* set if relation is true */
	int	elsflg;		/* set if structured "if" expected */
	char	relate;		/* relation code */
	char	ch;
	double	val1,val2;

	val1 = val2 = 0.0;	/* ini comparison values to zero */
	true = 0;		/* preset to false */
	elsflg = 0;		/* preset to non-structured "if" */
	savptr = expr;
	if(*savptr == '\17')  /* "more"? EOF CHECK */
	{
		savptr++;
		if(*savptr == '_') *savptr++;
		fnum = *savptr++ - '1';
		val1 = (double)more(fnum);
		relate = 6;	/* force '<>' relation check */
		eoexpr = savptr;
	}
	else
	{
#ifdef	STRINGS
		if(class(&savptr,field) < STCLASS)
		{
			typflg = 0;
#endif
			val1 = evalx(expr);	/* get first expression */
#ifdef	STRINGS
		}
		else
		{
			typflg = 1;
			evals(expr,strbuf1);
		}

#endif
		skip00();			/* skip spaces and tabs */
		relate = *eoexpr++;		/* get relational operator */
		if(!(relate >= 6 && relate <= 13))
		{
			error(inst.thing.linno, 10);	/* bad relational */
			return(-1);
		}
		skip00();			/* skip spaces and tabs */
#ifdef	STRINGS
		savptr = eoexpr;
		if(class(&savptr,field) < STCLASS)
		{
			if(typflg == 1)
			{
				error(inst.thing.linno, 53); /* MIXED TYPES */
				return(-1);
			}
#endif
			val2 = evalx(eoexpr);	/* get second expression */
#ifdef	STRINGS
		}
		else
		{
			if(typflg == 0)
			{
				error(inst.thing.linno, 53); /* MIXED TYPES */
				return(-1);
			}
			evals(eoexpr,strbuf2);
			val1 = (double)strcmp(strbuf1,strbuf2); /* form numerical
							       comarison base */
		}
#endif
	}
	skip00();			/* skip spaces and tabs */
	/*
	 * Check for keyword "goto", "go to" or "then"
	 */
	ch = *eoexpr;
	if(ch < '\0' || (ch > '\3') && ch != '\16')
	{
		error(inst.thing.linno, 9);	/* missing keyword in "if" */
		return(-1);
	}
	*eoexpr++;		/* bump past keyword */
	skip00();		/* skip to line num field */
	if(*eoexpr == '\0') /* if nothing follows "then" we expect
				structured "if" */
	{
		elsflg = 1;
	}

	/*
	 * Perform relational test in accordance
	 * with relate keyword.
	 * Set true to 1 if test passes or 0 if test fails
	 */
	switch(relate)
	{
		case 7:
		case 8:
			true = ((val1 <= val2)? 1 : 0);
			break;
		case 10:
		case 11:
			true = ((val1 >= val2)? 1 : 0);
			break;
		case 9:
			true = ((val1 < val2)? 1 : 0);
			break;
		case 12:
			true = ((val1 > val2)? 1 : 0);
			break;
		case 13:
			true = ((val1 == val2)? 1 : 0);
			break;
		case 6:
			true = ((val1 != val2)? 1 : 0);
			break;
		default:
			/* bad news */
			break;
	}
	if(elsflg == 0)		/* IF NORMAL BASIC "if" */
	{
		if(true)
		{
			expr = eoexpr;	/* goto wants to see the line num */
			if(ch < '\3' || (ch == '\3' && num(*expr)))
			{
				if(__goto()<0) /* goto the linnum */
					return(-1); /* if no error */
				return(0);
			}
			if(ch == '\3')
			{
				if(let() < 0) return(-1);
				return(0);
			}
			if(gosub()<0) /* gosub the linnum */
				return(-1); /* unless error */
			return(0);
		}
		return(0);		/* no action taken */
	}
	else		/* ELSE IF STRUCTURED "if" */
	{
		/* EXEC or SKIP until "else" */

		if(untilcod(esle,true) < 0)
			return(-1); /* FATAL ERROR RET. FROM BASCALL */

		/* if endif was encountered with no else,
		 * don't call untilcod again.
		 */

		if(inst.thing.opcode.lobyte == fidne)
			return(0);
		/* EXEC or SKIP until "endif" */

		return(untilcod(fidne,!(true)));
	}
}
