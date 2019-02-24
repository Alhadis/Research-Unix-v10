/*	"@(#) copy.c: V 1.19 11/10/83"	*/

/*
 *	copy: copy a set of lines to another part of the
 *		program buffer.  Any existing lines that
 *		have the same line numbers as the ones being
 *		created by the copy command will be overwritten.
 */

#include <stdio.h>
#include <ctype.h>
#include "bas.h"

#define retrn(X)	{cleartops(); __end(); return(X);}

extern char	_string[];


copy()
{
	int	increm,
		i,
		delim,
		copies;

	LINNO	start,
		finish,
		copyline;

	long	newnum;

	UCHAR	copystmt;

	char	linbuf[ENCLINMAX+1],
		*linptr,
		buf[10],
		*to,
		*next;

	copyline = inst.thing.linno;
	copystmt = inst.thing.stmtno;
	skip00(&expr);			/* skip preceding blanks and tabs */

/*	get values for starting number, ending number,
 *	new starting number, and increment.
 */

	if (lnpars(&start, &finish, &delim, &expr) < 0)
		return(-1);

	if (finish == 0 && delim)
		finish = MAXLINNO;
	else if (finish == 0)
		finish = start;
	else if (finish < start)
	{
		error(copyline,copystmt,41);	/*start > finish*/
		retrn(-1);
	}
	skip00(&expr);
	if (*expr++ != ',')
	{
		error(copyline,copystmt,13);		/* missing delimiter */
		retrn(-1);
	}
	to = buf;
	skip00(&expr);
	while (isdigit(*expr))
		*to++ = *expr++;
	*to = '\0';
	if ((newnum = atoln(buf)) == 0)
	{
		error(copyline,copystmt, 89);
		retrn(-1);
	}
	skip00(&expr);
	if (*expr == ',')
	{
		++expr;
		to = buf;
		skip00(&expr);
		while (isdigit(*expr))
			*to++ = *expr++;
		*to = '\0';
		increm = atoi(buf);
		if (increm == 0)
		{
			error(copyline,copystmt,17);/*missing increment value*/
			retrn(-1);
		}
	}
	else if (*expr != '\0')
	{
			error(copyline,copystmt,13);	/* missing delimiter */
			retrn(-1);
	}
	else
		increm = 10;			/* default value */

	if ((i = fetch(start, LINE, &lbdptr)) != 0)
	{
		if (i == -1)
		{
			error(copyline,copystmt,0); /* working storage empty */
			retrn(-1);
		}
		else				/* i == -2 */
		{
			if (inst.thing.linno < start)
			{
				ederror(start,copyline,copystmt,0);
				retrn(-1); /* no such line */
			}
		}
	}
	next = lbdptr;
	copies = 0;
	while (inst.thing.linno <= finish)
	{
		++copies;
		line_decode(curptr, linbuf);
		linptr = linbuf;
		while (NUM(*linptr))
			linptr++;
		sprintf(_string, "%d%s", newnum, linptr);
		if (bed(0) < 0)
			retrn(-1);
		fetch(newnum, LINE, &lbdptr);
		prin();
		newnum += increm;
		if (newnum > MAXLINNO)
		{
			error(copyline,copystmt,89); /* line number too big */
			retrn(-1);
		}
		fetch(start, LINE, &next);
		if (fetch(NEXT, LINE, &next) != 0)	/* no more lines */
			break;
		start = inst.thing.linno;
	}					/* while */

	if (copies == 0)
	{
		ederror(start,copyline,copystmt,0);
		retrn(-1); /* no lines copied: no such line */
	}
	retrn(0);
}
