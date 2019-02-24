/*	"@(#) go.c: V 1.13 6/8/83"	*/

#include <stdio.h>
#include "bas.h"

extern char	*ptr;

extern short	fortop,
		whiletop,
		endfcnt;

struct gnode {
	LINNO		tolin;
	LINNO		fromlin;
	short		fromstmt;
	short		fornest;
	short		whilenest;
	short		ifnest;
	struct gnode	*next;
}	gotolist[CACHESIZE];

struct gnode	*front;

extern int	FORcmd,
		IFcmd,
		ENDIFcmd,
		WHILEcmd,
		WENDcmd,
		NEXTcmd;

extern UCHAR	stmtno;

extern struct conrec	constack[],
			*contop;



__goto()
{
	LINNO	savno,
		start;
	short	savstmt;

	savno = inst.thing.linno;
	savstmt = inst.thing.stmtno;
	if ((start = atoln(expr)) == 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 89);
		return(-1);
	}
	stmtno = STMT;
	fetch(start,STMT,&ptr);
	if (inst.thing.linno != start)
	{
		error(savno,savstmt,0);
		inst.thing.linno = savno;
		return(-1);
	}
	if (savno == IMMED)			/* jumping into program */
	{
		fortop = contop->fornest;
		whiletop = contop->whilenest;
		endfcnt = contop->ifnest;
	}
	else if (!gofound(savno,start,savstmt))
	{
		if (countnest(savno,savstmt,start) < 0)
			return(-1);
	}
	else
	{
		if (changenest(front) < 0)
			return(-1);
		if (front->ifnest > 0)
			endfcnt -= front->ifnest;
	}
	ptr = curptr;
	return(0);
}



gofound(savno,start,stmtno)
LINNO	savno,
	start;
short	stmtno;
{
	struct gnode	*find,
			*trail;

	find = front;
	trail = NULL;
	while (find != NULL)
	{
		if (find->tolin == 0 && find->fromlin == 0)
			return(0);
		if (find->tolin == start && find->fromlin == savno
						&& find->fromstmt == stmtno)
		{
			if (trail != NULL)
			{
				trail->next = find->next;
				find->next = front;
				front = find;
			}
			return(1);
		}
		trail = find;
		find = find->next;
	}
	return(0);
}



addgo(newgo)
struct gnode	*newgo;
{
	struct gnode	*find,
			*trail;

	find = front;
	trail = NULL;
	while (find->next != NULL && (find->tolin != 0 && find->fromlin != 0))
	{
		trail = find;
		find = find->next;
	}
	if (trail != NULL)
		trail->next = find->next;
	if (find != front)
		find->next = front;
	front = find;
	front->tolin = newgo->tolin;
	front->fromlin = newgo->fromlin;
	front->fromstmt = newgo->fromstmt;
	front->fornest = newgo->fornest;
	front->whilenest = newgo->whilenest;
	front->ifnest = newgo->ifnest;
}



initgo()
{
	int	i;

	front = gotolist;
	for (i = 1; i < CACHESIZE; ++i)
	{
		gotolist[i-1].tolin = 0;
		gotolist[i-1].fromlin = 0;
		gotolist[i-1].fromstmt = 0;
		gotolist[i-1].fornest = 0;
		gotolist[i-1].whilenest = 0;
		gotolist[i-1].ifnest = 0;
		gotolist[i-1].next = gotolist + i;
	}
	gotolist[CACHESIZE-1].tolin = 0;
	gotolist[CACHESIZE-1].fromlin = 0;
	gotolist[CACHESIZE-1].fromstmt = 0;
	gotolist[CACHESIZE-1].fornest = 0;
	gotolist[CACHESIZE-1].whilenest = 0;
	gotolist[CACHESIZE-1].ifnest = 0;
	gotolist[CACHESIZE-1].next = NULL;
}




gosub()
{
	LINNO		savno,
			tarline;
	short		savstmt;

	savno = inst.thing.linno;	/* save the gosub line number */
	savstmt = inst.thing.stmtno;	/* for diagnostic reasons */
	skip00(&expr);
	if ((tarline = atoln(expr)) == 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 89);
		return(-1);
	}
	if (fetch(tarline,STMT,&lbdptr) != 0)
	{
		error(savno,savstmt,0);			/* Bad line number */
		inst.thing.linno = savno;
		return(-1);
	}
	if (push_context() < 0)
		return(-1);
	contop->gosublin = savno;
	contop->gosubstmt = savstmt;
	ptr = curptr;
	return(0);
}




__return()
{
	if (contop->gosublin == 0)
	{
		if (contop - 1 >= constack && (contop - 1)->gosublin == IMMED)
			__end();
		else
		{   				/* return with no gosub */
			error(inst.thing.linno,inst.thing.stmtno,73);
			return(-1);
		}
	}
	else
		fetch(contop->gosublin,contop->gosubstmt,&ptr);
	pop_context();
	return(0);
}



countnest(curline,curstmt,tarline)
LINNO	curline;
short	curstmt;
LINNO	tarline;
{
	int	ifseen;
	struct gnode	newgo;

	if (curline == IMMED)
	{
		initstack();
		return(0);
	}
	newgo.tolin = tarline;
	newgo.fromlin = curline;
	newgo.fromstmt = curstmt;
	newgo.fornest = 0;
	newgo.whilenest = 0;
	newgo.ifnest = 0;
	ifseen = 0;
	if (curline >= tarline)				/* backward jump */
	{
		fetch(tarline,STMT,&ptr);
		while (inst.thing.linno != curline
					|| inst.thing.stmtno != curstmt)
		{
			if (inst.thing.opcode == FORcmd)
				--(newgo.fornest);
			else if (inst.thing.opcode == NEXTcmd)
				++(newgo.fornest);
			else if (inst.thing.opcode == IFcmd)
			{
				while ((*expr & 0377) != THENcode 
							&& *expr != '\0')
					++expr;
				if (*expr != '\0')
				{
					++expr;
					skip00(&expr);
					if (*expr == '\0')
						++ifseen;
				}
			}
			else if (inst.thing.opcode == ENDIFcmd)
			{
				if (ifseen > 0)
					--ifseen;
			}
			else if (inst.thing.opcode == WHILEcmd)
				--(newgo.whilenest);
			else if (inst.thing.opcode == WENDcmd)
				++(newgo.whilenest);
			fetch(NEXT,STMT,&ptr);
		}
		newgo.ifnest = ifseen;
		if (changenest(&newgo) < 0)
			return(-1);
		addgo(&newgo);
		endfcnt -= ifseen;
		if (endfcnt < 0)
			endfcnt = 0;
		fetch(tarline,STMT,&ptr);
	}
	else if (tarline > curline)			/* forward jump */
	{
		fetch(curline,curstmt,&ptr);
		fetch(NEXT,STMT,&ptr);
		while (inst.thing.linno != tarline)
		{
			if (inst.thing.opcode == NEXTcmd)
			{
				if (popfor() < 0)
					return(-1);
				--(newgo.fornest);
			}
			else if (inst.thing.opcode == FORcmd)
			{
				if (pushfor() < 0)
					return(-1);
				++(newgo.fornest);
			}
			if (inst.thing.opcode == WENDcmd)
			{
				if (popwhile() < 0)
					return(-1);
				--(newgo.whilenest);
			}
			else if (inst.thing.opcode == WHILEcmd)
			{
				if (pushwhile() < 0)
					return(-1);
				++(newgo.whilenest);
			}
			else if (inst.thing.opcode == IFcmd)
			{
				while ((*expr & 0377) != THENcode 
							&& *expr != '\0')
					++expr;
				if (*expr != '\0')
				{
					++expr;
					skip00(&expr);
					if (*expr == '\0')
						++ifseen;
				}
			}
			else if (inst.thing.opcode == ENDIFcmd)
			{
				if (ifseen == 0)
				{
					if (endfcnt > -1)
					{
						--endfcnt;
						++(newgo.ifnest);
					}
				}
				else
					--ifseen;
			}
			fetch(NEXT,STMT,&ptr);
		}
		addgo(&newgo);
	}
	return(0);
}



changenest(ptr)
struct gnode	*ptr;
{
	int	i;

	i = ptr->fornest;
	if (i > 0)
	{
		while (i-- > 0)
			if (pushfor() < 0)
				return(-1);
	}
	else
	{
		while (i++ < 0)
			if (popfor() < 0)
				return(-1);
	}
	i = ptr->whilenest;
	if (i > 0)
	{
		while (i-- > 0)
			if (pushwhile() < 0)
				return(-1);
	}
	else
	{
		while (i++ < 0)
			if (popwhile() < 0)
				return(-1);
	}
	return(0);
}
