/*	"@(#) sing.c: V 2.20 8/19/83"	*/

/* single step monitor */

#include <stdio.h>
#include "bas.h"

extern char	_string[];

extern UCHAR	stmtno,
		stopstmt,
		breakflg,
		breakpoints,
		trace;

UCHAR	singflg,
	norestore;

extern char	*ptr;

extern LINNO	stopno,
		nextno;




sing()
{
	char	*save;
	LINNO	sline;
	int	sstmt;

	sline = inst.thing.linno;
	sstmt = inst.thing.stmtno;
	if (singflg)
		return(0);
	save = expr;
	if (fetch(FIRST,LINE,&lbdptr) != 0)
		return(0);
	expr = save;
	singflg = 1;
	breakflg = 1;
	skip00(&expr);
	if (*expr != '\0')
	{
		save = expr;
		for ( ; *save != '\0' && NUM(*save); ++save) ;
		skip00(&save);
		if (*save != '\0')
		{
			error(sline,sstmt,89);		/* sing takes line # */
			singflg = 0;
			return(-1);
		}
		if ((nextno = atoln(expr)) == 0)
		{
			error(sline,sstmt, 89);
			return(-1);
		}
		stmtno = 1;
		if (fetch(nextno,STMT,&lbdptr) != 0)
		{
			error(sline,sstmt, 0);
			singflg = 0;
			return(-1);
		}
	}
	else if (stopno != 0)
	{
		nextno = stopno;
		stmtno = stopstmt;
		stopno = 0;
		stopstmt = STMT;
	}
	else
	{
		nextno = FIRST;
		stmtno = STMT;
		srand48(0L);				/* set standard seed */
		inittrap();
		initstack();
		initvar();
		clall();
		stopno = 0;
		stopstmt = STMT;
		clearsubs();		/* remove allocated user subroutines */
	}
	return(0);
}



con()
{
	if (singflg != 0)				/* single stepping */
	{
		singflg = 0;
		norestore = 1;
		if (breakpoints == 0 && !trace)
			breakflg = 0;
		return(0);
	}
	else if (stopno == 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 12);
		return(-1);
	}
	if (pop_context() < 0)		/* breakpoint or stop statement */
		return(-1);
	nextno = stopno;
	stmtno = stopstmt;
	stopno = 0;
	stopstmt = STMT;
	return(0);
}
