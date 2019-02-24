/*	"@(#) class.c: V 2.27 8/19/83"	*/
#include	<stdio.h>
#include	<values.h>
#include	"bas.h"
#include	"tokens.h"


/*
*	parses legal l-value
*/
class(sptr, field)
char	**sptr;			/* input string pointer */
char	*field;			/* pointer to output field */
{
	char	*fptr,		/* output field pointer */
		*to,
		nam[IDSIZE+1];
	int	type;

	fptr = field;		/* init output field pointer */
	skip00(sptr);		/* skip leading blanks and tabs */

	to = *sptr;
	switch (type = lex(sptr, nam))
	{
		case DBLAID:
		case SNGAID:
		case INTAID:
		case STRAID:
			++(*sptr);
			if (skiptopar(sptr) < 0)
				return(-1);
		case DBLID:
		case SNGID:
		case INTID:
		case STRID:
			while (to < *sptr)
				*fptr++ = *to++;
			*fptr = '\0';
			return(type);
		default:
			return(-1);
	}
}



#define	DBL	1
#define SNG	2
#define INT	3
#define STR	4

datatype(type)
int	type;
{
	switch(type)
	{
	case DBLID:
	case DBLAID: 	return(DBL);
	case SNGID:
	case SNGAID: 	return(SNG);
	case INTID:
	case INTAID: 	return(INT);
	case STRID:
	case STRAID: 	return(STR);
	}
	return(-1);
}


double
numval()
{
	if (exprtype == DOUBLE)
		return(exprval.dval);
	else if (exprtype == SINGLE)
		return(exprval.fval);
	else
		return((double) exprval.ival);
}


swap()
{
	extern	char	*scalaraddr(),
			*elemaddr();
	char	buf1[LINMAX+1],
		buf2[LINMAX+1];
	int 	type1,
		type2;
	double	numtmp;
	char	strtmp[STRMAX+1];

	if ((type1 = class(&expr, buf1)) < 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	skip00(&expr);
	if (*expr != ',')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	expr++;
	if ((type2 = class(&expr, buf2)) < 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	if (datatype(type1) != datatype(type2))
	{
		error(inst.thing.linno,inst.thing.stmtno, 80);
		return(-1);
	}

	/* arguments are string l-values */
	if (type1 == STRID || type1 == STRAID)
	{
		if (eval(buf1) < 0)
			return(-1);
		strcpy(strtmp, exprval.sval);
		if (eval(buf2) < 0)
			return(-1);
		if (type1 == STRID)
		{
			if (sputvar(buf1, exprval.sval) < 0)
				return(-1);
		}
		else if (type1 == STRAID)
		{
			if (saputvar(buf1, exprval.sval) < 0)
				return(-1);
		}
		if (type2 == STRID)
		{
			if (sputvar(buf2, strtmp) < 0)
				return(-1);
		}
		else if (type2 == STRAID)
		{
			if (saputvar(buf2, strtmp) < 0)
				return(-1);
		}
		return(0);
	}

	/* arguments are numeric l-values */
	if (eval(buf1) < 0)
		return(-1);
	numtmp = numval();
	switch(type1)
	{
	case DBLID:
			if (eval(buf2) < 0)
				return(-1);
			if (dputvar(buf1, numval()) < 0)
				return(-1);
			if (type2 == DBLID)
			{
				if (dputvar(buf2, numtmp) < 0)
					return(-1);
			} else {
				if (daputvar(buf2, numtmp) < 0)
					return(-1);
			}
			break;
	case DBLAID:
			if (eval(buf2) < 0)
				return(-1);
			if (daputvar(buf1, numval()) < 0)
				return(-1);
			if (type2 == DBLID)
			{
				if (dputvar(buf2, numtmp) < 0)
					return(-1);
			} else {
				if (daputvar(buf2, numtmp) < 0)
					return(-1);
			}
			break;
	case SNGID:
			if (eval(buf2) < 0)
				return(-1);
			if (fputvar(buf1, numval()) < 0)
				return(-1);
			if (type2 == SNGID)
			{
				if (fputvar(buf2, numtmp) < 0)
					return(-1);
			} else {
				if (faputvar(buf2, numtmp) < 0)
					return(-1);
			}
			break;
	case SNGAID:
			if (eval(buf2) < 0)
				return(-1);
			if (faputvar(buf1, numval()) < 0)
				return(-1);
			if (type2 == SNGID)
			{
				if (fputvar(buf2, numtmp) < 0)
					return(-1);
			} else {
				if (faputvar(buf2, numtmp) < 0)
					return(-1);
			}
			break;
	case INTID:
			if (eval(buf2) < 0)
				return(-1);
			if (iputvar(buf1, numval()) < 0)
				return(-1);
			if (type2 == INTID)
			{
				if (iputvar(buf2, numtmp) < 0)
					return(-1);
			} else {
				if (iaputvar(buf2, numtmp) < 0)
					return(-1);
			}
			break;
	case INTAID:
			if (eval(buf2) < 0)
				return(-1);
			if (iaputvar(buf1, numval()) < 0)
				return(-1);
			if (type2 == INTID)
			{
				if (iputvar(buf2, numtmp) < 0)
					return(-1);
			} else {
				if (iaputvar(buf2, numtmp) < 0)
					return(-1);
			}
			break;
	}
	return(0);
}


octtoi(string, numptr)
char	*string;
int	*numptr;
{
	int	n;
	char	*sptr;

	n = 0;
	sptr = string;
	do {
		if (n > (MAXINT/8))
			return(-1);
		n = n * 8 + *sptr - '0';
		if (n < 0)
			return(-1);
	} while (*(++sptr));
	*numptr = n;
	return(0);
}


dectoi(string, numptr)
char	*string;
int	*numptr;
{
	int	n;
	char	*sptr;

	n = 0;
	sptr = string;
	do {
		if (n > (MAXINT/10))
			return(-1);
		n = n * 10 + *sptr - '0';
		if (n < 0)
			return(-1);
	} while (*(++sptr));
	*numptr = n;
	return(0);
}


hextoi(string, numptr)
char	*string;
int	*numptr;
{
	int	n;
	char	*sptr;

	n = 0;
	sptr = string;
	do {
		if (n > (MAXINT/16))
			return(-1);
		if (NUM(*sptr))
			n = n * 16 + *sptr - '0';
		else
			n = n * 16 + *sptr - 'a' + 10;
		if (n < 0)
			return(-1);
	} while (*(++sptr));
	*numptr = n;
	return(0);
}


LINNO
atoln(string)
char	*string;
{
	register long	n;
	register char	*sptr;

	n = 0;
	sptr = string;
	while (*sptr == ' ' || *sptr =='\t')
			sptr++;
	while (*sptr >= '0' && *sptr <= '9')
	{
		if (n > (MAXLINNO/10))
			return(0);
		n = n * 10 + *sptr++ - '0';
		if (n > MAXLINNO)
			return(0);
	}
	while (*sptr == ' ' || *sptr =='\t')
			sptr++;
	if (*sptr != '\0')
		return(0);
	else
		return((LINNO) n);
}
