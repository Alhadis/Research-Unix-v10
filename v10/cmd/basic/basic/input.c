/*	"@(#) input.c: V 2.34 6/17/83"	*/


#include	<stdio.h>
#include	<values.h>
#include	<setjmp.h>
#include	"bas.h"
#include	"tokens.h"

struct Item {
	int	type;
	char	field[LINMAX+1];
};

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

extern struct FILTBL filtbl[];
extern char	*readcpy();
extern UCHAR	stpflg,
		trapflg;
extern LINNO	trapline;
extern short	startpos;
extern double	atof1();
extern int	INPUTPcmd;

extern jmp_buf	overenv;

UCHAR	nonl = 0;


__input()
{
	char		*tptr,
			*ptr;			/* pointer to program string */
	register int	j;
	char		*prncpy();
	int		k,
			first,
			count,
			noprompt = 0,
			fildes,		/* file designator for BASIC */
			stilldata;
	FILE		*fd;			/* file descriptor */
	char		linbuf[LINMAX+1];		/*use for input string*/
	struct Item	item[20];
	char		tempbuf[LINMAX+1];		/* gets buf */

startover:
	if (stpflg)
		return(0);
	ptr = expr;			/* point to command operand string */
	fd = stdin;
	count = 0;
	skip00(&ptr);			/* skip preceding blanks and tabs */

	if (*ptr == '\0')
	{						/* bad input syntax */
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	if (*ptr == '#' || inst.thing.opcode == INPUTPcmd)
	{
		if (*ptr == '#')
			++ptr;
		skip00(&ptr);
		ptr = prncpy(tempbuf, ptr);
		if (eval(tempbuf) < 0)
			return(-1);
		if (exprtype == STRING)
		{			/* string in numeric */
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
		fildes = exprval.ival - 1;
		++ptr;
		if (filtbl[fildes].mode == 'w')
		{				/* FILE NOT OPEN FOR READ */
			error(inst.thing.linno,inst.thing.stmtno,37);
			return(-1);
		}
		skip00(&ptr);
		return(doinputp(fildes, ptr));
	}
	else
	{
		if (*ptr == ';')
		{
			nonl = 1;
			++ptr;
			skip00(&ptr);
		}
		if (*ptr == '"')
		{
			int	i = 0;

			++ptr;
			while (*ptr != '"')
				tempbuf[i++] = *ptr++;
			tempbuf[i] = '\0';
			++ptr;
			skip00(&ptr);
			if (*ptr != ';' && *ptr != ',')
			{					/* bed delim */
				error(inst.thing.linno,inst.thing.stmtno, 13);	
				return(-1);
			}
			else
			{
				if (*ptr == ',')
					noprompt = 1;
				printf("%s", tempbuf);
				startpos += strlen(tempbuf);
				++ptr;
			}
			skip00(&ptr);
		}
	}
	first = 0;
	tempbuf[0]  = '\0';
	if (!noprompt)
	{
		printf("? ");			/* input PROMPT */
		startpos += 2;
	}
	lclfgets(tempbuf,LINMAX,fd);
	tempbuf[strlen(tempbuf)-1] = '\0';
	tptr = tempbuf;
	stilldata = 0;
	while (1)
	{
		item[count].type = class(&ptr, item[count].field);
		switch (item[count].type)
		{
			case DBLID:
			case DBLAID:
			case SNGID:
			case SNGAID:
			case INTID:
			case INTAID:
			case STRID:
			case STRAID:
				break;
			default:
				error(inst.thing.linno,inst.thing.stmtno, 3);
				return(-1);		/* BAD INPUT ITEM */
		}
		if (first < LINMAX)
		{
			tptr = readcpy(linbuf + first, tptr);
			skip00(&ptr);
			if (*tptr == '\0' && *ptr != '\0')
			{	/* more l-values than input data */
				error(inst.thing.linno,inst.thing.stmtno, 69);
				if (trapline && trapflg)
					return(-1);
				goto startover;
			}
			if (linbuf[first] == '\0' && item[count].type != STRID 
						  && item[count].type != STRAID)
			{				/* NULL numeric input */
				linbuf[first] = '0';
				linbuf[first+1] = '\0';
			}
		}
		else
		{					/* LINE TOO LONG */
			error(inst.thing.linno,inst.thing.stmtno, 68);
			if (trapline && trapflg)
				return(-1);
			goto startover;
		}
		if (!(j = isnum(linbuf + first)) && item[count].type != STRID
						 && item[count].type != STRAID)
		{					/* TYPE MISMATCH */
			error(inst.thing.linno,inst.thing.stmtno, 69);
			if (trapline && trapflg)
				return(-1);
			goto startover;
		}
		if (setjmp(overenv) != 0)
			goto startover;
		if (j && atof1(linbuf + first) == MAXDOUBLE)
		{						/* OVERFLOW */
			error(inst.thing.linno,inst.thing.stmtno, 60);
			if (trapline && trapflg)
				return(-1);
			goto startover;
		}
		++count;
		skip00(&ptr);
		skip00(&tptr);
		if (*tptr == ',')
		{
			tptr++;
			skip00(&tptr);
			stilldata = 1;
		}
		else if (*tptr == '\0')
		{
			stilldata = 0;
			skip00(&ptr);
		}
		else
		{					/* delimiter error */
			error(inst.thing.linno,inst.thing.stmtno, 13);
			if (trapline && trapflg)
				return(-1);
			goto startover;
		}
		if (*ptr == ',')
		{
			ptr++;
		}
		else if (*ptr == '\0')
			break;
		else
		{					/* delimiter error */
			error(inst.thing.linno,inst.thing.stmtno, 13);
			return(-1);
		}
		while (linbuf[first] != '\0')
			++first;
		++first;
	}
	if (stilldata)		/* more input data than l-values */
	{
		error(inst.thing.linno,inst.thing.stmtno, 69);
		if (trapline && trapflg)
			return(-1);
		goto startover;
	}
	k = 0;
	for (j = 0; j < count; ++j)
	{
		if (item[j].type != STRID && item[j].type != STRAID)
		{
			eval(linbuf+k);
			if (exprtype == INTEGER)
				exprval.dval = (double) exprval.ival;
			else if (exprtype == SINGLE)
				exprval.dval = (double) exprval.fval;
		}
		switch (item[j].type)
		{
			case DBLID:
				if (dputvar(item[j].field, exprval.dval) < 0)
					return(-1);
				break;
			case DBLAID:		/* subscripted variable class */
				if (daputvar(item[j].field,exprval.dval) < 0)
					return(-1);
				break;
			case SNGID:
				if (fputvar(item[j].field, exprval.dval) < 0)
					return(-1);
				break;
			case SNGAID:		/* subscripted variable class */
				if (faputvar(item[j].field,exprval.dval) < 0)
					return(-1);
				break;
			case INTID:		/* integer variable name */
				if (iputvar(item[j].field,exprval.dval) < 0)
					return(-1);
				break;
			case INTAID:		/* subscripted variable class */
				if (iaputvar(item[j].field,exprval.dval) < 0)
					return(-1);
				break;
			case STRID:
				if (sputvar(item[j].field,linbuf+k) < 0)
					return(-1);
				break;
			case STRAID:
				if (saputvar(item[j].field,linbuf+k) < 0)
					return(-1);
				break;
			default:
				break;
		}
		while (linbuf[k] != '\0')
			++k;
		++k;
	}
	return(0);
}



/*	get character from file opened with file descriptor ftblind + 1	*/
cget(ftblind)
int	ftblind;
{
	if (filtbl[ftblind].mode == 'r')
		return(getc(filtbl[ftblind].fildes));
	else
	{
		if (filtbl[ftblind].curchr >= filtbl[ftblind].recsize)
			return(EOF);
		return(filtbl[ftblind].buffer[filtbl[ftblind].curchr++]);
	}
}


/*	unget character to file opened with file descriptor ftblind + 1	*/
cunget(c,ftblind)
int	c,
	ftblind;
{
	if (filtbl[ftblind].mode == 'r')
		ungetc(c,filtbl[ftblind].fildes);
	else
		filtbl[ftblind].curchr--;
}


/*	get number from file opened with file descriptor ftblind + 1	*/
readnum(ftblind, buf)
int	ftblind;
char	buf[];
{
	int 	c,
		count;

	count = 0;
	while ((c = cget(ftblind)) == ' ' || c == '\t' || c == '\n') ;
	if (c == EOF)
	{
		error(inst.thing.linno,inst.thing.stmtno, 33);	/* Out Of Data */
		return(-1);
	}
	do {
		if (c == ',' || c == EOF)
		{
			buf[count] = '\0';
			return(0);
		}
		else
		{
			if (++count <= STRMAX)
				buf[count - 1] = c;
		}
	} while ((c = cget(ftblind)) != ' ' && c != '\t' && c != '\n');
	buf[count] = '\0';
	while ((c = cget(ftblind)) == ' ' || c == '\t' || c == '\n') ;
	if (c != ',' && c != EOF)
		cunget(c, ftblind);
	return(0);
}


/*	get string from file opened with file descriptor ftblind + 1	*/
readstring(ftblind, buf)
int	ftblind;
char	buf[];
{
	int 	c,
		count;

	count = 0;
	while ((c = cget(ftblind)) == ' ' || c == '\t' || c == '\n') ;
	if (c == EOF)
	{
		error(inst.thing.linno,inst.thing.stmtno, 33);	/* Out Of Data */
		return(-1);
	}
	else if (c == '"')
	{
		while ((c = cget(ftblind)) != '"' && c != EOF)
		{
			if (++count > STRMAX)
			{
				error(inst.thing.linno,inst.thing.stmtno, 103);
				return(-1);
			}
			buf[count - 1] = c;
		}
		if (c != EOF)
			c = cget(ftblind);
	}
	else
		do {
			if (c == ',' )
			{
				c = cget(ftblind);
				break;
			}
			else if ( c == EOF)
				break;
			else
			{
				buf[count] = c;
				if (++count == STRMAX)
				{
					buf[count] = '\0';
					return(0);
				}
			}
		} while ((c = cget(ftblind)) != '\n');
	buf[count] = '\0';
	while (c == ' ' || c == '\t' || c == '\n')
		c = cget(ftblind);
	if (c != ',' && c != EOF)
		cunget(c, ftblind);
	return(0);
}


/*	process input# statement	*/
doinputp(ftblind, ptr)
int	ftblind;
char	*ptr;
{
	char	buf[LINMAX+1],
		valbuf[STRMAX+1],
		*lptr;
	int	type;
	double	val;

	lptr = ptr;
	do {
		skip00(&lptr);
		switch(type = class(&lptr, buf))
		{
		case SNGID:
		case SNGAID:
		case DBLID:
		case DBLAID:
		case INTID:
		case INTAID:
			if (readnum(ftblind, valbuf) < 0)
				return(-1);
			if (!isnum(valbuf))
			{
				error(inst.thing.linno,inst.thing.stmtno, 69);
				return(-1);
			}
			if (setjmp(overenv) != 0)
				return(-1);
			if ((val = atof1(valbuf)) == MAXDOUBLE)
			{
				error(inst.thing.linno,inst.thing.stmtno, 60);
				return(-1);
			}
			break;
		case STRID:
		case STRAID:
			if (readstring(ftblind, valbuf) < 0)
				return(-1);
			break;
		default:
			error(inst.thing.linno,inst.thing.stmtno, 3);
			return(-1);
		}
		switch (type)
		{
		case SNGID:
			if (fputvar(buf, val) < 0)
				return(-1);
			break;
		case SNGAID:
			if (faputvar(buf, val) < 0)
				return(-1);
			break;
		case DBLID:
			if (dputvar(buf, val) < 0)
				return(-1);
			break;
		case DBLAID:
			if (daputvar(buf, val) < 0)
				return(-1);
			break;
		case INTID:
			if (iputvar(buf, val) < 0)
				return(-1);
			break;
		case INTAID:
			if (iaputvar(buf, val) < 0)
				return(-1);
			break;
		case STRID:
			if (sputvar(buf, valbuf) < 0)
				return(-1);
			break;
		case STRAID:
			if (saputvar(buf, valbuf) < 0)
				return(-1);
			break;
		}
	} while (lex(&lptr, buf) == COMMA);
	if (*lptr)
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	return(0);
}



/* linput is the function that handles the LINE INPUT command
 * from either the terminal or a file.
 */

linput()
{
	int	i = 0,
		code,
		fildes,
		filflg = 0;
	char	prompt[STRMAX+1],
		buf[STRMAX+1],
		field[STRMAX+1];
	FILE	*fd;
	extern struct FILTBL filtbl[];

	prompt[0] = '\0';
	fd = stdin;
	buf[0]= '\0';
	skip00(&expr);
	code = *expr & 0377;
	if (code == INPUTcode)
		expr++;
	else if (code != INPUTPcode)
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	if (*expr == ';')
	{
		nonl = 1;
		++expr;
	}
	else if (code == INPUTPcode)
	{
		++expr;
		skip00(&expr);
		expr = prncpy(field, expr);
		if (eval(field) < 0)
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
		fildes = exprval.ival - 1;
		filflg = 1;
		++expr;
		if (filtbl[fildes].mode != 'r')
		{				/* file not open for reading */
			error(inst.thing.linno,inst.thing.stmtno, 37);
			return(-1);
		}
		fd = filtbl[fildes].fildes;
		skip00(&expr);
	}
	else if (*expr != ' ' && *expr != '\t')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	skip00(&expr);
	if (*expr == ';')
	{
		nonl = 1;
		++expr;
	}
	else if (*expr == '#')
	{
		++expr;
		skip00(&expr);
		expr = prncpy(field, expr);
		if (eval(field) < 0)
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
		fildes = exprval.ival - 1;
		filflg = 1;
		++expr;
		if (filtbl[fildes].mode != 'r')
		{				/* file not open for reading */
			error(inst.thing.linno,inst.thing.stmtno, 37);
			return(-1);
		}
		skip00(&expr);
		fd = filtbl[fildes].fildes;
	}
	skip00(&expr);
	if (*expr == '"' && !filflg)
	{
		++expr;
		while (*expr != '"')
			prompt[i++] = *expr++;
		prompt[i] = '\0';
		++expr;
		skip00(&expr);
		if (*expr != ';')
		{
			error(inst.thing.linno,inst.thing.stmtno, 13);		/* bad delim */
			return(-1);
		}
		else
			++expr;
		skip00(&expr);
	}
	if ((i = class(&expr, field)) != STRID && i != STRAID)
	{
		error(inst.thing.linno,inst.thing.stmtno, 3);	/* bad variable in INPUT */
		return(-1);
	}
	skip00(&expr);
	if (*expr != '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	if (!filflg)
	{
		printf("%s",prompt);
		startpos += strlen(prompt);
	}
	lclfgets(buf,STRMAX,fd);
	buf[strlen(buf)-1] = '\0';
	switch (i)
	{
		case STRID:
			if (sputvar(field, buf) < 0)
				return(-1);
			break;
		case STRAID:
			if (saputvar(field, buf) < 0)
				return(-1);
			break;
		default:
			preprint();
			printf("Type mismatch\n");
			return(-1);
	}
	return(0);
}
