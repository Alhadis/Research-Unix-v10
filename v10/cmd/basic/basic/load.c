 /*
 * CLEAR USER CORE AND LOAD BASIC PROGRAM
 */
/*	"@(#) load.c: V 2.23 8/19/83"	*/

#include <stdio.h>
#include "bas.h"
#include "tokens.h"

#define retrn(X) {cleartops(); __end(); return(X);}

extern FILE	*pathopen();

extern	UCHAR	singflg,
		stmtno,
		stpflg,
		stopstmt,
		trace,
		breakpoints,
		breakflg;

extern int	COMMONcmd;

extern LINNO	nextno,
		stopno;

extern struct {
	LINNO		ln;
	unsigned	updated:1;
	} breaklines[];

extern char	*prncpy();

char	filnam[STRMAX+1];


load()
{
	int	i;
	char	buf[LINMAX+1],
		*tp;
	FILE	*fp;
	short	runflg;

	runflg = 0;
	skip00(&expr);
	tp = expr;
	if (*tp != '\0')
	{
		tp = prncpy(buf, tp);
		if (*tp != '\0')
		{
			++tp;
			skip00(&tp);
			if (*tp != 'r' && *tp != 'R')
			{
				error(inst.thing.linno,inst.thing.stmtno, 21);
				retrn(-1);
			}
			runflg = 1;
		}
	}
	if (*expr == '\0' && *filnam == '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		retrn(-1);
	}
	else if (*expr != '\0')
	{
		if (eval(buf) < 0)
			retrn(-1);
		if (exprtype != STRING)
		{					/* numb. in string */
			error(inst.thing.linno,inst.thing.stmtno, 51);
			retrn(-1);
		}
		postb(exprval.sval);
		if ((fp = pathopen(exprval.sval)) == NULL)
			retrn(-1);
		fclose(fp);
	}
	lbdptr = curptr = endofprog = linptr = txtbuf;
	clear_pages();
	inst.thing.linno = 0;
	clearsubs();
	breakpoints = 0;
	inittrap();
	stopno = 0;
	stopstmt = STMT;
	for (i = 0; i < BREAKSIZE; ++i)
		breaklines[i].ln = 0;
	if (!singflg && !trace)
		breakflg = 0;
	initvar();
	for (i = 0; i < 4; i++)
		txtbuf[i] = '\0';
	if (runflg)
	{
		prncpy(buf, expr);
		expr = buf;
		if (merge() < 0)
			return(-1);
		nextno = FIRST;
		stmtno = STMT;
		stpflg = 0;
		return(0);
	}
	else
	{
		clall();
		return(merge());		/* load program */
	}
}



 /*
 * routine to clear the contents of the working storage
 */


new()
{
	int	i;

	cleartops();
	__end();
	filnam[0] = '\0';
	lbdptr = curptr = endofprog = linptr = txtbuf;
	clear_pages();
	inst.thing.linno = 0;
	clearsubs();
	breakpoints = 0;
	breakflg = 0;
	trace = 0;
	inittrap();
	stopno = 0;
	stopstmt = STMT;
	for (i = 0; i < BREAKSIZE; ++i)
		breaklines[i].ln = 0;
	clall();
	initvar();	/* clear variable stack and variable symbol tables */
	for (i = 0; i < 4; i++)
		txtbuf[i] = '\0';
	return(0);
}



 /* This routine is used to load BASIC programs form a UNIX file
 * the filenames must be suffixed ".b" which is appended automatically
 * by the "save" routine if the user leaves it off
*/

merge()
{
	char	buf[LINMAX+1];
	FILE	*fp;

	strcpy(buf,filnam);
	if (expr[0] == '\0' && filnam[0] == '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	if (expr[0] != 0)
	{
		if (strlen(expr) > LINMAX)
		{
			error(inst.thing.linno,inst.thing.stmtno,4);
			return(-1);
		}
		if (eval(expr) < 0)
			return(-1);
		if (exprtype != STRING)
		{					/* numb. in str exp */
			error(inst.thing.linno,inst.thing.stmtno, 51);
			return(-1);
		}
		strcpy(filnam,exprval.sval);
	}
	postb(filnam);			/* attach .b postfix if not there */
	if ((fp = pathopen(filnam)) == NULL)
	{
		strcpy(filnam,buf);
		return(-1);
	}
	while (bed(fp) == 0);
	fclose(fp);
	cleartops();
	__end();
	fetch(FIRST,LINE,&lbdptr); 		/* set txtbuf to first line */
	return(0);
}



runload()
{
	int	i;
	char	buf[LINMAX+1],
		*tp;
	FILE	*fp;
	short	rflg;

	rflg = 0;
	skip00(&expr);
	tp = expr;
	if (*tp != '\0')
	{
		tp = prncpy(buf, tp);
		if (*tp != '\0')
		{
			if (*tp != ',')
			{
				error(inst.thing.linno,inst.thing.stmtno, 21);
				return(-1);
			}
			++tp;
			skip00(&tp);
			if (*tp != 'r')
			{
				error(inst.thing.linno,inst.thing.stmtno, 21);
				return(-1);
			}
			rflg = 1;
		}
	}
	if (*expr == '\0' && *filnam == '\0')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	else if (*expr != '\0')
	{
		if (eval(buf) < 0)
			return(-1);
		if (exprtype != STRING)
		{					/* numb. in string */
			error(inst.thing.linno,inst.thing.stmtno, 51);
			return(-1);
		}
		postb(exprval.sval);
		if ((fp = pathopen(exprval.sval)) == NULL)
			return(-1);
	}
	lbdptr = curptr = endofprog = linptr = txtbuf;
	clear_pages();
	inst.thing.linno = 0;
	clearsubs();
	breakpoints = 0;
	stopno = 0;
	stopstmt = STMT;
	for (i = 0; i < BREAKSIZE; ++i)
		breaklines[i].ln = 0;
	if (!singflg && !trace)
		breakflg = 0;
	initvar();
	for (i = 0; i < 4; i++)
		txtbuf[i] = '\0';
	while (bed(fp) == 0);
	fclose(fp);
	strcpy(filnam,exprval.sval);
	if (!rflg)
		clall();
	return(0);
}


/*
*	processes CHAIN statement
*/
chain()
{
	char	*lptr,
		*nexpr,
		fname[STRMAX+1],
		buf[LINMAX+1];
	int	i,
		mrgflg = 0,
		allflg = 0,
		delflg = 0,
		markfunc();
	FILE	*fp;

	nextno = FIRST;
	stmtno = STMT;
	nexpr = expr;
	gettoken(&nexpr, buf);
	if (!strcmp(buf, "merge"))
		mrgflg = 1;
	else
		nexpr = expr;
	nexpr = prncpy(fname, nexpr);	/* get file name */
	if (*nexpr == '\0')
		goto process;
	else if (*nexpr != ',')
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	nexpr++;
	switch (gettoken(&nexpr, buf))
	{
	case INTCONST : if ((nextno = atoln(buf)) == 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 89);
				return(-1);
			}
			skip00(&nexpr);
			break;
	case COMMA    : nexpr--;
			break;
	default       : error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
	}
	if (*nexpr == '\0')
		goto process;
	nexpr++;
	switch (gettoken(&nexpr, buf))
	{
	case COMMA    : nexpr--;
			break;
	case '\0'     : 
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
	default	      : if (!strcmp(buf, "all"))
			{
				allflg = 1;
				skip00(&nexpr);
				break;
			}
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
	}
	if (*nexpr == '\0')
		goto process;
	nexpr++;
	gettoken(&nexpr, buf);
	if (!strcmp(buf, "delete"))
	{
		delflg = 1;
		strcpy(buf, nexpr);
	}
	else
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
process:
	if (eval(fname) < 0)
		return(-1);
	if (exprtype != STRING)
	{
		error(inst.thing.linno,inst.thing.stmtno, 51);
		return(-1);
	}
	strcpy(fname, exprval.sval);
	postb(fname);
	if ((fp = pathopen(fname)) == NULL)
		return(-1);

	if (!allflg)		/* save only COMMONed variables */
	{
		fetch(FIRST, STMT, &lptr);
		do {
			if (inst.thing.opcode == COMMONcmd)
				if (_common() < 0)
					return(-1);
		} while (fetch(NEXT, STMT, &lptr) >= 0);
		if (mrgflg)
			traverse(markfunc);	/*don't delete user-defined's*/
		savmarked();
	}
	if (delflg)
	{
		expr = buf;
		if (dodelete() < 0)
			return(-1);
		else
			stpflg = 0;
	}
	if (!mrgflg)
	{
		lbdptr = curptr = endofprog = linptr = txtbuf;
		clear_pages();
		inst.thing.linno = 0;
		cleartops();
		clearsubs();
		inittrap();
		stopno = 0;
		stopstmt = STMT;
		for (i=0; i<BREAKSIZE; i++)
			breaklines[i].ln = 0;
		if (!singflg && !trace)
			breakflg = 0;
		for (i=0; i<4; i++)
			txtbuf[i] = '\0';
	}
	while (bed(fp) == 0) ;
	fclose(fp);
	if (nextno != FIRST && fetch(nextno, STMT, &lptr) != 0)
	{
		error(nextno, stmtno, 0);
		return(-1);
	}
	return(0);
}
