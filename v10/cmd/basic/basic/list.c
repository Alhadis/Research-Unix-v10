/*	"@(#) list.c: V 2.27 6/29/83"	*/

#include <stdio.h>
#include "bas.h"
#include "tokens.h"

UCHAR 	stpflg;

static FILE	*fd = stdout;

stopl()
{
	stpflg = 1;
	preprint();
	printf("\n%s %d\n","INTERRUPT LINE",inst.thing.linno);
	fd = stdout;
	signal(2,stopl);
}



extern int	LLISTcmd,
		inc;

extern LINNO	line_number;

#define retrn(x) {signal(2,quit);\
		if (llist && fd != stdout) pclose(fd);\
		else if (fd != stdout) fclose(fd);\
		fd=stdout;\
		return(x);}

extern FILE	*popen();


list()
{
	LINNO		start,
			finish;
	int		delim,
			llist = 0,
			quit();

	/* if there is no expression then list the
	 * entire program */

	signal(2,stopl);
	if (inst.thing.opcode == LLISTcmd)
		llist = 1;
	skip00(&expr);
	if (*expr == ',')
	{
		if (getlistname() < 0)
			retrn(-1);
		expr = &nullstrng;
	}
	if (*expr == '\0')
	{
		if (fetch(FIRST,LINE,&lbdptr) < 0)
		{
			retrn(0);
		}
		if (llist)
		{
			if ((fd = popen("/usr/bin/lp","w")) == NULL)
			{
				error(inst.thing.linno,inst.thing.stmtno, 43);
				retrn(-1);
			}
		}
		prin();
		while (fetch(NEXT,LINE,&lbdptr) == 0 && stpflg == 0)
			prin();
		retrn(0);
	}

	/* there is an expression so parse it into
	 * start and finish */

	if (lnpars(&start, &finish, &delim, &expr) < 0)
		return(-1);

	if (finish == 0 && delim)
		finish = MAXLINNO;
	else if (finish == 0)
		finish = start;
	else if (finish < start)
	{						/* start > finish */
		error(inst.thing.linno,inst.thing.stmtno, 41);
		retrn(-1);
	}
	skip00(&expr);
	if (!llist && *expr != '\0' && getlistname() < 0)
		retrn(-1);
	if (fetch(start,LINE,&lbdptr) == -1)
	{
		retrn(0);
	}
	if (llist)
	{
		if ((fd = popen("/usr/bin/lp","w")) == NULL)
		{
			error(inst.thing.linno,inst.thing.stmtno, 43);
			retrn(-1);
		}
	}
	prin();					/* print the first line */
	if (inst.thing.linno < finish)
	while (fetch(NEXT,LINE,&lbdptr) == 0 && stpflg == 0)
	{
		if (inst.thing.linno > finish)
		{
			lbdptr = curptr;
			retrn(0);
		}
		prin();
	}
	retrn(0);
}



getlistname()
{
	if (*expr != ',')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	++expr;
	if (eval(expr) < 0)
		return(-1);
	if (exprtype != STRING)
	{
		error(inst.thing.linno,inst.thing.stmtno, 51);
		return(-1);
	}
	if ((fd = fopen(exprval.sval,"w")) == NULL)
	{
		error(inst.thing.linno,inst.thing.stmtno, 2);
		return(-1);
	}
	return(0);
}



/* print most recently FETCHed line */

prin()
{
	char	buf[LINMAX+1];

	line_decode(curptr,buf);
	prin1(buf);
	return(0);
}



extern short	startpos,
		margin;


prin1(buf)
char	*buf;
{
	int	i,
		j;

	j = strlen(buf);
	for (i = 0; i < j; ++i)
	{
		if (startpos > margin)
		{
			putc('\n',fd);
			startpos = 1;
		}
		putc(*(buf+i),fd);
		if (*(buf+i) == '\n')
			startpos = 1;
		else
			++startpos;
	}
	line_number = inst.thing.linno;
	return(0);
}



extern struct FUNCTAB	functab[];

extern char 	*keyword[];

expand(in,out)
char	**in,
	**out;
{
	register char	c;
	int		quoflg;

	quoflg = 0;
	c = *(*in)++;
	while (c != '\0')
	{
		if (c == '"')
			quoflg = quoflg ? 0 : 1;
		if (((c & 0377) == 0377) && !quoflg) /* expand function name */
		{
			strcpy(*out, functab[(**in & 0377) - 1].name);
			*out += strlen(*out);
			(*in)++;
		}
		else if (((c & 0377) >= 0200) && !quoflg) /* expand keyword */
		{
			strcpy(*out, keyword[(c & 0377) - 0200]);
			*out += strlen(*out);
		}
		else
			*(*out)++ = c;
		c = *(*in)++;
	}
	**out = '\0';
	return(0);
}




/*
 *	parse line numbers of the form
*
 *		[start] - [finish]
 *
 */

lnpars(x,y,delim,ptr)
LINNO	*x,
	*y;
int 	*delim;
char	**ptr;
{
	char	buf[LINMAX+1];

	*x = 0;
	*y = 0;
	*delim = 0;
	switch (lex(ptr, buf))
	{
		case INTCONST:
			if ((*x = atoln(buf)) == 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 89);
				return(-1);
			}
			break;
		case LDOT:
			*x = line_number;
			break;
		case LMINUS:
			*delim = 1;
			goto secondnum;
		default:
			error(inst.thing.linno,inst.thing.stmtno, 39);	/* no starting line # */
			return(-1);
	}
	switch (lex(ptr, buf))
	{
		case 0:
			break;
		case LMINUS:
			*delim = 1;
			break;
		default:
			*ptr -= 1;
			return(0);
	}
secondnum:
	switch (lex(ptr, buf))
	{
		case 0:
			*y = 0;
			break;
		case LDOT:
			*y = line_number;
			break;
		case INTCONST:
			if ((*y = atoln(buf)) == 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 89);
				return(-1);
			}
			break;
		default:
			*ptr -= 1;
			break;
	}
	return(0);
}
