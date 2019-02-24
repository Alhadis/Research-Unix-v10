 /* RUN BASIC Program
 */
/*	"@(#) run.c: V 2.37 8/19/83"	*/

#include	<stdio.h>
#include	<ctype.h>
#include	"bas.h"

#define getout(X)\
	signal(2,quit);\
	startno=0;\
	if (calline != IMMED)\
	{\
		if (!trace && breakpoints == 0)\
			breakflg = 0;\
		if (stopno == 0)\
		{\
			initstack();\
			clall();\
		}\
	}\
	return(X)


extern	UCHAR	trapflg,
		intrap;

LINNO	startno = 0,
	stopno = 0,		/* line number of stop statement */
	nextno = NEXT,
	savelin;

UCHAR	stmtno = STMT,
	savestmt,
	interrupt = 0,
	stopstmt,
 	conflg,
	trace = 0;

extern char	*daptr,
		_string[];

char	*ptr;

extern UCHAR	singflg,
		stpflg,
		breakpoints,
		breakflg;

extern int	stopl(),
		quit();

extern short	fortop,
		whiletop,
		endfcnt;

extern short 	startpos,
		margin;


extern struct gnode {
	LINNO		tolin;
	LINNO		fromlin;
	short		fromstmt;
	short		fornest;
	short		whilenest;
	short		ifnest;
	struct gnode	*next;
} gotolist[];


extern struct fortemplate {
	struct sym	*ctlvar;
	char	*lptr;
	double	increm;
	double	final;
} forstack[];

extern struct whiletemplate {
	char	*lptr;
	char	*exprptr;
	LINNO	linno;
	short	stmtno;
};

extern struct whiletemplate whilestack[];

struct conrec	constack[CONTEXTSIZE],
		*contop;

stopr()
{
	stpflg = 1;
	interrupt = 1;
	stopno = savelin;
	stopstmt = savestmt;
	preprint();
	if (inst.thing.linno != IMMED)
		printf("\nINTERRUPT LINE %d\n",stopno);
	else
		printf("\nINTERRUPT\n");
	signal(2,quit);
}



run()
{
	int	i;
	LINNO	runline;

	runline = inst.thing.linno;
	skip00(&expr);
	if (isdigit(*expr))
	{
		if ((startno = atoln(expr)) == 0)
		{					/* bad line number */
			error(inst.thing.linno,inst.thing.stmtno, 0);
			return(-1);
		}
		stmtno = STMT;
		savelin = inst.thing.linno;
		savestmt = inst.thing.stmtno;
		if (fetch(startno,STMT,&lbdptr) != 0)
		{
			error(savelin,savestmt, 0);	/* bad line number */
			inst.thing.linno = savelin;
			return(-1);
		}
		inst.thing.linno = savelin;
		inittrap();
	}
	if (startno == 0)
	{
		srand48(0L);				/* set standard seed */
		inittrap();
		initstack();
		skip00(&expr);		/* skip white space before filename */
		if (expr[0] != '\0')		/* if filename specified */
		{
			if (runload() < 0)	/* load file before running */
			{
				return(-1);	/* return if load error */
			}
		}
		else
		{
			initvar();
			clall();
			stopno = 0;
			stopstmt = STMT;
			clearsubs();	/* remove allocated user subroutines */
		}
	}
	nextno = FIRST;
	stmtno = STMT;
	return(0);
}


/*	TRON and TROFF are commands that turn program tracing on
 *	and off, respectively.
 */



tron()
{
	trace = 1;
	breakflg = 1;
	return(0);
}


troff()
{
	trace = 0;
	if (!singflg && breakpoints == 0)
		breakflg = 0;
	return(0);
}



 /* In BASIC, 'stop' and 'end' both stop the program execution however 
 * unlike many BASICS, the 'end' statement is optional. Simply hitting the
 * end of the program buffer will stop the program without error.
 */


stop()
{
	preprint();
	printf("STOP %d\n",inst.thing.linno);
	stpflg = 1;
	fetch(NEXT,STMT,&ptr);
	stopno = inst.thing.linno;
	stopstmt = inst.thing.stmtno;
	return(0);
}



__end()
{
	stpflg = 1;
	return(0);
}



initstack()
{
	int	i;

	cleartops();
	initgo();
	for (i = 0; i < LOOPSTACKSIZE; ++i)
	{
		forstack[i].ctlvar = NULL;
		forstack[i].lptr = NULL;
		forstack[i].increm = 0;
		forstack[i].final = 0;
		whilestack[i].lptr = NULL;
		whilestack[i].exprptr = NULL;
		whilestack[i].linno = 0;
		whilestack[i].stmtno = 0;
	}
	daptr = NULL;
	if (startpos != 1)
	{
		printf("\n");
		startpos = 1;
	}
	return(0);
}



cleartops()
{
	struct conrec	*tmp;

	tmp = contop;
	while (tmp >= constack)
	{
		tmp->fornest = -1;
		tmp->whilenest = -1;
		tmp->ifnest = 0;
		tmp->nextno = FIRST;
		tmp->stmtno = STMT;
		tmp->gosublin = 0;
		tmp->gosubstmt = 0;
		tmp->ptr = txtbuf;
		--tmp;
	}
	endfcnt = 0;
	fortop = -1;
	whiletop = -1;
	stopno = 0;
	stopstmt = STMT;
}



dorun()
{
	LINNO	calline;

	calline = startno;
	if (push_context() < 0)
		return(-1);
	stpflg = 0;
	interrupt = 0;
	if (fetch(startno,stmtno,&ptr) < 0)			/* any lines? */
	{
		getout(0);			/* nop, can't do anything */
	}
	signal(2,stopr);
	if (inst.thing.linno != IMMED)
	{
		savelin = inst.thing.linno;
		savestmt = inst.thing.stmtno;
		if (breakflg)
		{
			if (singflg)
			{
				prin();
				while (singflg)
				{
					preprint();
					printf("^");
					if (lclfgets(_string,LINMAX,stdin) == 0)
					{
						getout(-1);
					}
					if (_string[0] == '\n')
						break;
					bed(0);
					if (stpflg)
					{
						getout(-1);
					}
					fetch(savelin,savestmt,&ptr);
				}
			}
			else if (findbreak(inst.thing.linno) != -1 &&
					inst.thing.stmtno == 1 && !conflg)
			{
				preprint();
				printf("Breakpoint At Line %d\n",
							inst.thing.linno);
				stopno = inst.thing.linno;
				stopstmt = inst.thing.stmtno;
				stpflg = 1;
				conflg = 1;
				getout(0);
			}
			else if (trace)
			{
				char	tracestr[9];
				int	i;

				sprintf(tracestr,"[%d] ",inst.thing.linno);
				for (i = 0; tracestr[i] != '\0'; ++i)
					cput(tracestr[i],stdout,&startpos,0);
			}
		}
		conflg = 0;
	}

	startno = 0;
	nextno = NEXT;
	stmtno = STMT;
	if (bascall(inst.thing.opcode) == -1)
	{ 	/* exec 1st instruction */
		if (trapflg)
		{
			trapflg = 0;
			intrap = 1;
		}
		else
		{
			getout(-1);		/*  and return if error */
		}
	}
	else if (trapflg)
	{
		trapflg = 0;
		intrap = 1;
	}

	/*
	 * fetch each instruction sequentially and execute
	 * until last line.
	 */

	while ((fetch(nextno,stmtno,&ptr) == 0) && stpflg == 0)
	{
		nextno = NEXT;
		stmtno = STMT;
		if (inst.thing.linno != IMMED)
		{
			savelin = inst.thing.linno;
			savestmt = inst.thing.stmtno;
			if (breakflg)
			{
				if (singflg)
				{
					prin();
					while (singflg)
					{
						preprint();
						printf("^");
						if (lclfgets(_string,LINMAX,
								stdin) == 0)
						{
							getout(-1);
						}
						if (_string[0] == '\n')
							break;
						bed(0);
						if (stpflg)
						{
							getout(-1);
						}
						fetch(savelin,savestmt,&ptr);
					}
				}
				else if (findbreak(inst.thing.linno) != -1 &&
					      inst.thing.stmtno == 1 && !conflg)
				{
					preprint();
					printf("Breakpoint At Line %d\n",
							inst.thing.linno);
					stopno = inst.thing.linno;
					stopstmt = inst.thing.stmtno;
					stpflg = 1;
					conflg = 1;
					getout(0);
				}
				else if (trace)
				{
					char	tracestr[9];
					int	i;

					sprintf(tracestr,"[%d] ",
							inst.thing.linno);
					for (i = 0; tracestr[i] != '\0'; ++i)
						cput(tracestr[i],stdout,
								&startpos,0);
				}
			}
			conflg = 0;
		}

		if (bascall(inst.thing.opcode) == -1)
		{
			if (trapflg)
			{
				trapflg = 0;
				intrap = 1;
			}
			else
			{
				getout(-1);
			}
		}
		else if (trapflg)
		{
			trapflg = 0;
			intrap = 1;
		}
	}
	singflg = 0;
	if (interrupt)
	{
		getout(0);
	}
	if (fortop > contop->fornest && stopno == 0)
	{
		stoperror(IMMED,74);			/* FOR with no NEXT */
		pop_context();
		getout(-1);
	}
	if (whiletop > contop->whilenest && stopno == 0)
	{
		stoperror(IMMED,58);			/* WHILE with no WEND */
		pop_context();
		getout(-1);
	}
	if (endfcnt > contop->ifnest && stopno == 0)
	{
		stoperror(IMMED,79);			/* IF with no ENDIF */
		pop_context();
		getout(-1);
	}
	pop_context();
	getout(0);
}



push_context()
{
	if (++contop >= constack + CONTEXTSIZE)
	{
		error(inst.thing.linno,inst.thing.stmtno, 72);
		return(-1);
	}
	contop->fornest = fortop;
	contop->whilenest = whiletop;
	contop->ifnest = endfcnt;
	contop->stmtno = stmtno;
	contop->nextno = nextno;
	contop->gosublin = 0;
	contop->gosubstmt = 0;
	contop->ptr = ptr;
	return(0);
}



pop_context()
{
	if (contop < constack)
		return(-1);
	fortop = contop->fornest;
	whiletop = contop->whilenest;
	endfcnt = contop->ifnest;
	stmtno = contop->stmtno;
	nextno = contop->nextno;
	ptr = contop->ptr;
	--contop;
	return(0);
}
