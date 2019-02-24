/*
 * BASIC Print Command
 */
/*	"@(#) print.c: V 2.34 6/29/83"	*/

#include	<stdio.h>
#include	<values.h>
#include	"bas.h"
#include	"tokens.h"

#define FIELDWIDTH	14	/* number of print fields */


unsigned char	contin = 1;	/* line continuation flag initted
				    to no continuation */

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

extern char	*prncpy(),
		*delimcpy();

extern int	PRINTPcmd;

char	*gcvt();

short	startpos = 1,
	margin = 80;

int	idnext;

__print()
{
	char		buf[LINMAX+1];
	short		dummystart = 1;
	int		filind;
	int		using = 0;

	skip00(&expr);			/* skip preceding blanks and tabs */
	if ((*expr & 0377) == USINGcode)
	{
		using = 1;
		++expr;
		skip00(&expr);
	}
	if (*expr == '#' || inst.thing.opcode == PRINTPcmd)
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
		if ((*expr & 0377) == USINGcode)
			using = 1;
		++expr;
		skip00(&expr);
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
		{
			if (using)
				return(prt_using(filtbl[filind].fildes,
						 &(filtbl[filind].curchr),
						 filind));
			else
				return(doprint(filtbl[filind].fildes,
					     &(filtbl[filind].curchr), 
					     &(filtbl[filind].contin), filind));
		}
		else
		{
			if (using)
				return(prt_using(NULL, &dummystart,filind));
			else
				return(doprint(NULL, &dummystart,
					     &(filtbl[filind].contin), filind));
		}
	}
	else
	{
		if (using)
			return(prt_using(stdout, &startpos, 0));
		else
			return(doprint(stdout, &startpos, &contin, 0));
	}
}


cput(c, filptr, startptr, ftblind)
char	c;
FILE	*filptr;
short	*startptr;
int	ftblind;
{
	if (filptr == stdout)
	{
		if ((*startptr) > margin || c == '\n')
		{
			putc('\n',filptr);
			(*startptr) = 1;
		}
		if (c != '\n')
		{
			putc(c,filptr);
			(*startptr)++;
		}
	}
	else if (filptr != NULL)
	{
		putc(c, filptr);
		if (c == '\n')
			(*startptr) = 1;
		else
			(*startptr)++;
	}
	else 
	{
		if (filtbl[ftblind].curchr == filtbl[ftblind].recsize)
		{
			error(inst.thing.linno,inst.thing.stmtno, 34);
			return(-1);
		}
		++(*startptr);
		filtbl[ftblind].buffer[filtbl[ftblind].curchr++] = c;
	}
	return(0);
}



doprint(filptr, startpos, contin, ftblind)
FILE		*filptr;
short		*startpos;
unsigned char	*contin;
int		ftblind;
{
	char	*ptext;				/* input text pointer */
	int	tvalue;				/* tab value */
	double	value;
	char	buf[LINMAX+1];			/* number text storage */
	int	n;				/* number txtbuf index */

	ptext = expr;
	skip00(&ptext);
	if (*ptext == '\0')				/* if line empty */
	{
		(*contin) = 1;				/* force newline */
		(*startpos) = 1;
	}

	while (*ptext != '\0')
	{
		idnext = 1;
		skip00(&ptext);			/* skip blanks and tabs */
		if ((*ptext & 0377) == TABcode)
		{ 					/*if "tab" func*/
			(*contin) = 0;
			idnext = 0;
			ptext += 2;
			ptext = delimcpy(buf, ptext, ")");
			ptext++;
			if (eval(buf) < 0)
				return(-1);
			if (exprtype == STRING)
			{
				error(inst.thing.linno,inst.thing.stmtno, 49);
				return(-1);
			}
			else
			{
				if (exprtype == DOUBLE)
					value = exprval.dval;
				else if (exprtype == SINGLE)
					value = (double) exprval.fval;
				else
					value = (double) exprval.ival;
				tvalue = mkint(value);
			}
			if (tvalue < 1)
			{
				error(inst.thing.linno,inst.thing.stmtno, 55);
				tvalue = 1;
			}
			if ((*startpos) > tvalue)
			{
				if (cput('\n', filptr, startpos, ftblind) < 0)
					return(-1);
			}
			else if (tvalue > margin)
				tvalue -= margin * (int)((tvalue - 1) / margin);
			while ((*startpos) < tvalue)
			{
				if (cput(' ', filptr, startpos, ftblind) < 0)
					return(-1);
			}
		}
		else if ((*ptext & 0377) == SPCcode)
		{ 					/*if "tab" func*/
			(*contin) = 0;
			idnext = 0;
			ptext += 2;
			ptext = delimcpy(buf, ptext, ")");
			ptext++;
			if (eval(buf) < 0)
				return(-1);
			if (exprtype == STRING)
			{
				error(inst.thing.linno,inst.thing.stmtno, 49);
				return(-1);
			}
			else
			{
				if (exprtype == DOUBLE)
					value = exprval.dval;
				else if (exprtype == SINGLE)
					value = (double) exprval.fval;
				else
					value = (double) exprval.ival;
				tvalue = mkint(value);
			}
			if (tvalue < 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 55);
				tvalue = 0;
			}
			tvalue %= margin;
			while (tvalue--)
			{
				if (cput(' ', filptr, startpos, ftblind) < 0)
					return(-1);
				if ((*startpos) > margin)
				{
					if (cput('\n', filptr, startpos,
								ftblind) < 0)
						return(-1);
				}
			}
		}
		else
		{
			short	i;

			if (prnlex(&ptext,buf) == 0)
				goto delim;
			if ((i = asc_con(buf,&value)) < 0)
				return(-1);
			if (i == 1)
			{
				if ((*startpos) + strlen(buf) - 1 > margin)
				{
					if (cput('\n',filptr,startpos,
								ftblind) < 0)
						return(-1);
				}
			}
			else
			{
				/*
				 * PRECEDE POSITIVE NUMBERS WITH A BLANK
				 */

				if (value >= 0.0)
				{
					if ((*startpos) + strlen(buf)+1> margin)
					{
						if (cput('\n', filptr, startpos,
								ftblind) < 0)
							return(-1);
					}
					if (cput(' ', filptr, startpos,
								ftblind) < 0)
						return(-1);
				}
				else
				{
					if ((*startpos) + strlen(buf) > margin)
					{
						if (cput('\n', filptr, startpos,
								ftblind) < 0)
							return(-1);
					}
				}
			}
			n = 0;
			while (buf[n] != '\0')
			{
				if (cput(buf[n], filptr, startpos,
							ftblind) < 0)
					return(-1);
				++n;
			}
		}
delim:
		switch (*ptext)
		{
			case ',':
				(*contin) = 0;	/* turn on line continuation */

				/*
				 * this is a modulo count to
				 * set up field seperation
				 */

				if ((*startpos) + FIELDWIDTH > margin)
				{
					if (cput('\n', filptr, startpos, ftblind) < 0)
						return(-1);
				}
				else
				{
					if (cput(' ', filptr, startpos, ftblind) < 0)
						return(-1);
					while (((*startpos) % FIELDWIDTH) != 1)
						if (cput(' ', filptr, startpos, ftblind) < 0)
							return(-1);
				}
				ptext++;
				break;
			case '\0':
				(*contin) = 1;
				break;
			case ' ':
				if (!idnext)
				{
					(*contin) = 0;
					break;
				}
			case ';':
				ptext++;	/* bump past delim */
			default:
				(*contin) = 0;	/* turn on line continuation */
				break;
		}
		skip00(&ptext);			/* skip spaces and tabs */
	}
	if ((*contin))		/* if continuation off */
	{
		if (cput('\n', filptr, startpos, ftblind) < 0)
			return(-1); /*put in "newline" char*/
	}
	return(0);
}



prnlex(linptr,buf)
char	**linptr,
	buf[];
{
	char	*fptr,
		*tptr;

	fptr = buf;
	while (**linptr != '\0')
	{
		tptr = *linptr;
		switch (lex(linptr, fptr))
		{
			case STRCONST:
			case INTCONST:
			case FIXCONST:
			case SNGCONST:
			case DBLCONST:
			case STRID:
			case INTID:
			case DBLID:
			case SNGID:
				if (!idnext)
				{
					*linptr = tptr;
					goto itemdone;
				}
				idnext = 0;
				break;
			case STRAID:
			case INTAID:
			case DBLAID:
			case SNGAID:
				if (!idnext)
				{
					*linptr = tptr;
					goto itemdone;
				}
				++(*linptr);
				skiptopar(linptr);
				idnext = 0;
				break;
			case FUNCID:
				if (!idnext)
				{
					*linptr = tptr;
					goto itemdone;
				}
				if (**linptr == '(')
				{
					++(*linptr);
					skiptopar(linptr);
				}
				idnext = 0;
				break;
			case RPAR:
				idnext = 0;
				break;
			case LPAR:
				if (!idnext)
				{
					*linptr = tptr;
					goto itemdone;
				}
				skiptopar(linptr);
				idnext = 0;
				break;
			case COMMA:
			case LSEMI:
				*linptr = tptr;
				goto itemdone;
				break;
			default:
				if ((*fptr & 0377) == TABcode
				   || (*fptr &0377) == SPCcode)
				{
					*linptr = tptr;
					goto itemdone;
				}
				idnext = 1;
				break;
		}
		strncpy(fptr,tptr,*linptr - tptr);
		fptr += *linptr - tptr;
	}
itemdone:
	*fptr = '\0';
	if (fptr == buf)
		return(0);
	else
		return(1);
}




/*
 *	This is a copy of libc's gcvt.c.  The reason that it's here
 *	is to make the %g conversion wait until the exponent is <= -7
 *	to convert to e format.  Also, no leading zeros are printed
 *	in numbers of the form .xxxxxx .  A trailing space is appended
 *	to the number to conform to the ANSI standard.
 */



/*	@(#)gcvt.c	1.4	*/
/*LINTLIBRARY*/
/*
 * gcvt  - Floating output conversion to
 *
 */

extern char *ecvt();

char *
gcvt(number, ndigit, buf)
double	number;
int	ndigit;
char	*buf;
{
	int sign, decpt;
	register char *p1, *p2 = buf;
	register int i;
	int	j;
	char	*t1;
	int	sig,
		trail;

	p1 = ecvt(number, ndigit, &decpt, &sign);
	sig = trail = 0;
	t1 = p1;
	while (*t1)
		switch(*t1++)
		{
		case '0' :
			sig++;
			trail++;
			break;
		default  :
			sig++;
			trail = 0;
			break;
		}
	sig -= trail;
	if (sign)
		*p2++ = '-';
	if (decpt > ndigit || decpt - sig < -ndigit) {	/* E-style */
		decpt--;
		*p2++ = *p1++;
		*p2++ = '.';
		for (i = 1; i < ndigit; i++)
			*p2++ = *p1++;
		while (p2[-1] == '0')
			p2--;
		if (ndigit != 15)
			*p2++ = 'E';			/* single precision */
		else
			*p2++ = 'D';			/* double precision */
		if (decpt < 0) {
			decpt = -decpt;
			*p2++ = '-';
		} else
			*p2++ = '+';
		for (i = 1000; i != 0; i /= 10) /* 3B or CRAY, for example */
			if (i <= decpt || i < 10) /* create exponent */
				*p2++ = (decpt / i) % 10 + '0';
	} else {
		j = 1;
		if (decpt <= 0) {
			if (number == 0)
				*p2++ = '0';
			*p2++ = '.';
			while (decpt < 0) {
				decpt++;
				*p2++ = '0';
				++j;
			}
		}
		for (i = j; i <= ndigit; i++) {
			*p2++ = *p1++;
			if (i == decpt)
				*p2++ = '.';
		}
		if (ndigit < decpt) {
			while (ndigit++ < decpt)
				*p2++ = '0';
			*p2++ = '.';
		}
		while (*--p2 == '0' && p2 > buf)
			;
		if (*p2 != '.')
			p2++;
	}
	*p2++ = ' ';
	*p2 = '\0';
	return (buf);
}



skiptopar(ptext)
char	**ptext;
{
	int	pcnt = 1,
		type;
	char	buf[LINMAX+1];

	while (type = lex(ptext, buf))
	{
		if (pcnt <= 0)
			break;
		switch (type)
		{
			case LPAR:
				++pcnt;
				break;
			case RPAR:
				if (--pcnt <= 0)
					return(0);
				break;
			default:
				break;
		}
	}
	return(-1);
}



width()
{
	char	buf[LINMAX+1];

	expr = prncpy(buf,expr);
	if (eval(buf) < 0)
		return(-1);
	switch (exprtype)
	{
		case STRING:
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		case DOUBLE:
			if (exprval.dval < 1 || exprval.dval > MAXINT)
			{
				error(inst.thing.linno,inst.thing.stmtno, 55);
				return(-1);
			}
			margin = intify(exprval.dval);
			break;
		case SINGLE:
			if (exprval.fval < 1 || exprval.fval > MAXINT)
			{
				error(inst.thing.linno,inst.thing.stmtno, 55);
				return(-1);
			}
			margin = intify(exprval.fval);
			break;
		case INTEGER:
			if (exprval.ival < 1)
			{
				error(inst.thing.linno,inst.thing.stmtno, 55);
				return(-1);
			}
			margin = exprval.ival;
			break;
	}
	return(0);
}



preprint()
{
	if (startpos != 1)
	{
		putc('\n',stdout);
		startpos = 1;
	}
}



/*
 *	asc_con takes the ascii representation of an expression (in buf),
 *	evaluates it, and calls gcvt to convert the value back to ascii
 *	(only for numerics).  asc_con returns -1 for error, 0 for numeric
 *	result (which is returned in the second argument), and 1 for string
 *	result.
 */

asc_con(buf,value)
char	*buf;
double	*value;
{
	*value = 0.0;
	if (eval(buf) < 0)
		return(-1);
	switch (exprtype)
	{
		case STRING:
			strcpy(buf,exprval.sval);
			return(1);
		case DOUBLE:
			*value = exprval.dval;
			gcvt(*value, 15, buf);
			break;
		case SINGLE:
			*value = exprval.fval;
			gcvt(*value, 6, buf);
			break;
		case INTEGER:
			*value = (double) exprval.ival;
			gcvt(*value, 6, buf);
			break;
	}
	return(0);
}
