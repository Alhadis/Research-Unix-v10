/* "@(#) breakpoint.c: V 1.10 8/19/83" */

/*
 *	setbreak: Set up breakpoints at the line numbers
 *		  given in expr.
 */

#include <stdio.h>
#include "bas.h"

extern UCHAR	trace;

extern char	errbuf[];

UCHAR 	breakpoints = 0,
	breakflg = 0;

struct {
	LINNO		ln;
	unsigned	updated:1;
	} breaklines[BREAKSIZE];

setbreak()
{
	char	buf[10],
		*to,
		*save;

	LINNO	setline,
		saveline;

	UCHAR	savestmt;

	save = expr;
	skip00(&save);
	if (*save == '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno,71);
		return(-1); /* setbreak needs 1 line # */
	}
	saveline = inst.thing.linno;
	savestmt = inst.thing.stmtno;
	while (*save != '\0')
	{
		to = buf;
		while (NUM(*save) && to < buf + 10)
			*to++ = *save++;
		*to = '\0';
		skip00(&save);
		if (*save == ',')
			++save;
		else if (*save != '\0')
		{
			inst.thing.linno = saveline;
			inst.thing.stmtno = savestmt;
			error(saveline,savestmt,13);	/* bad delimiter */
			return(-1);
		}
		if ((setline = atoln(buf)) == 0)
		{
			error(saveline,savestmt,89);
			skip00(&save);
			continue;
		}
		if (fetch(setline, LINE, &lbdptr) != 0)
		{
			sprintf(errbuf, ": %d\n", setline);
			inst.thing.linno = saveline;
			error2(saveline,savestmt,0,errbuf);/* no such line # */
			skip00(&save);
			continue;
		}
		if (findbreak(setline) != -1)
		{
			sprintf(errbuf, ": %d\n", setline);
			inst.thing.linno = saveline;
			error2(saveline,savestmt,78,errbuf);/* no such line # */
			skip00(&save);
			continue;
		}
		if (++breakpoints > BREAKSIZE)
		{
			inst.thing.linno = saveline;
			sprintf(errbuf, ".  %d already set. Cannot set %d,%s\n",
						BREAKSIZE,setline,save);
			error2(inst.thing.linno,inst.thing.stmtno,75,errbuf);
			--breakpoints;		/* too many breakpoints */
			return(-1);
		}
		breakflg = 1;
		insbreak(setline);
		skip00(&save);
	}
	return(0);
}



/*
 *	clearbreak: Clear the breakpoints at the line numbers
 *			specified by expr.  If expr points to
 *			NULL, interactively clear all breakpoints.
 */

clearbreak()
{
	char	buf[10],
		*to,
		answer[LINMAX+1],
		*save;

	int	i;

	LINNO	line;

	save = expr;
	skip00(&save);
	if (*save == '\0')		/* interactively clear breakpoints */
	{
		if (breakpoints == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno,76);
			return(-1);
		}
		i = 0;
		while (breakpoints > 0 && i < BREAKSIZE)
		{
			if (breaklines[i].ln == 0)
				break;
			preprint();
			printf("Delete Breakpoint At Line %d?",
							breaklines[i].ln);
			lclfgets(answer,LINMAX,stdin);
			if (*answer == 'y' || *answer == 'Y')
			{
				delbreak(i);
				--breakpoints;
			}
			else
				++i;
		}
	}
	while (*save != '\0')
	{
		to = buf;
		while (NUM(*save) && to < buf + 10)
			*to++ = *save++;
		*to = '\0';
		skip00(&save);
		if (*save == ',')
			++save;
		else if (*save != '\0')
		{
			error(inst.thing.linno,inst.thing.stmtno,13);
			return(-1); 	/* bad delimiter */
		}
		if ((line = atoln(buf)) == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 89);
			skip00(&save);
			continue;
		}
		if ((i = findbreak(line)) < 0)
		{
			sprintf(errbuf," : %d\n",line);
			error2(inst.thing.linno,inst.thing.stmtno,76,errbuf);
		}
		else
		{
			delbreak(i);
			--breakpoints;
		}
		skip00(&save);
	}
	if (breakpoints <= 0)
	{
		breakpoints = 0;
		if (!trace)
			breakflg = 0;
	}
	return(0);
}



findbreak(line)
LINNO line;
{
	int	i;

	for (i = 0; i < breakpoints && i < BREAKSIZE; ++i)
	{
		if (breaklines[i].ln == line)
			return(i);
		else if (breaklines[i].ln > line)
			return(-1);
	}
	return(-1);
}


delbreak(index)
int index;
{
	if (breaklines[index].ln == 0)
		return(-1);
	for ( ; index + 1 < BREAKSIZE; ++index)
	{
		if (breaklines[index+1].ln == 0)
			break;
		breaklines[index].ln = breaklines[index+1].ln;
	}
	breaklines[index].ln = 0;
	return(0);
}



insbreak(line)
LINNO line;
{
	register	i,
			tmp,
			tline;

	tline = line;
	for (i = 0; i < breakpoints && i < BREAKSIZE; ++i)
	{
		if (breaklines[i].ln > line)
			break;
		else if (breaklines[i].ln == 0)
		{
			breaklines[i].ln = line;
			return(0);
		}
	}
	for ( ; i < breakpoints && i < BREAKSIZE; ++i)
	{
		tmp = breaklines[i].ln;
		breaklines[i].ln = tline;
		tline = tmp;
	}
	if (i < BREAKSIZE)
		breaklines[i].ln = tline;
	return(0);
}
