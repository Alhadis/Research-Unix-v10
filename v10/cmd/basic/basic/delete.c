 /* This routine is
 * called by "bed" to delete lines either singly or in large batches

*/
/*	"@(#) delete.c: V 2.19 6/21/83"	*/

#include <stdio.h>
#include "bas.h"

extern LINNO 	line_number,
		trapline;

extern FILE 	*tmpfp;

extern int	changed;

extern UCHAR	subflg,
		trace,
		breakflg,
		breakpoints;


delete()
{
	int	i;

	i = dodelete();
	cleartops();
	__end();
	return(i);
}



dodelete()
{
	char	temp[LINMAX+1],
		*from,
		*to,
		*first;
	int	i,
		delim,
		shrinkage;
	LINNO	start,
		finish,
		delline;

	UCHAR   delstmt;

	delline = inst.thing.linno;
	delstmt = inst.thing.stmtno;
	skip00(&expr);
	if (expr[0] == '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno,21);
		return(-1);
	}
	else 
	{
		if (lnpars(&start, &finish, &delim, &expr) < 0)
			return(-1);
		skip00(&expr);
		if (*expr != '\0')
		{
			error(delline,delstmt,21);
			return(-1);
		}
	}

	if (finish == 0 && delim)
		finish = MAXLINNO;
	else if (finish == 0)
		finish = start;
	else if (finish < start)
	{
		ederror(inst.thing.linno,delline,delstmt, 41);		
		return(-1); 	/* start > finish */
	}
	if (fetch(start, LINE, &lbdptr) == -1 || inst.thing.linno < start)
	{
		ederror(start,delline,delstmt,0);
		return(-1);
	}
	first = to = curptr;		/* beginning of area to be deleted */
	if (tmpfp != NULL && !subflg) 
	{
		fseek(tmpfp, 0L, 0);
		changed = 0;
	}
	while (inst.thing.linno <= finish)
	{
		if (inst.thing.linno == trapline)
			trapline = 0;
		if (breakflg && !trace)
		{
			if ((i = findbreak(inst.thing.linno)) != -1)
			{
				delbreak(i);
				--breakpoints;
			}
		}
		line_decode(curptr,temp);
		if (tmpfp != NULL && !subflg) 
		{
			fputs(temp, tmpfp);
			changed++;
		}
		from = lbdptr;			/* end of area to be deleted */
		if (fetch(NEXT, LINE, &lbdptr) == -1) 
			break;
	}
	if (tmpfp != NULL && !subflg && changed == 0)
	{
		ederror(start,delline,delstmt,0);
		return(-1);
	}
	shrinkage = from - to;
	while (from < linptr)		/* remove deleted area from txtbuf */
		*to++ = *from++;
	linptr -= shrinkage;
	endofprog -= shrinkage;
	update(first);			/* update page pointers */
	return(0);
}
