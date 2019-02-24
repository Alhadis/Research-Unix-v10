/*	"@(#) error.c: V 2.74 6/20/83"	*/

#include	"bas.h"
#include	"tokens.h"

char		errbuf[LINMAX+1];

#define	NUMERRS	97

static char *mesg[NUMERRS] =
{
	"Refers To A Nonexistent Line Number",		/* 0 */
	"File Open",					/* 1 */
	"Cannot Open File",				/* 2 */
	"Illegal Variable Name",			/* 3 */
	"Bad Filename",					/* 4 */
	"Bad Record Length Value",			/* 5 */
	"Too Many Parameters In Function Definition",	/* 6 */
	"End Of File Encountered",			/* 7 */
	"Expression Syntax",				/* 8 */
	"Too Many BLOAD Statements",			/* 9 */
	"Function Not Defined",				/* 10 */
	"Array Not Defined",				/* 11 */
	"Can't Continue",				/* 12 */
	"Missing Or Illegal Delimiter",			/* 13 */
	"External System Error",			/* 14 */
	"File Not Found Or Wrong Permissions",		/* 15 */
	"Unbalanced Parenthesis",			/* 16 */
	"Increment Value",				/* 17 */
	"NEXT With No Or Wrong FOR In Progress",	/* 18 */
	"Cannot Process Imaginary Number",		/* 19 */
	"What ?",					/* 20 */
	"Statement Syntax",				/* 21 */
	"Too Many Dimensions",				/* 22 */
	"Redundant DIM Statement",			/* 23 */
	"Out Of Working Storage Space",			/* 24 */
	"BLOAD Link Editing Failed",			/* 25 */
	"Wrong Number Of Dimensions",			/* 26 */
	"File Not Open For Random Access",		/* 27 */
	"Negative Array Index",				/* 28 */
	"Divide By Zero",				/* 29 */
	"Bad Field Length",				/* 30 */
	"System Call Failed",				/* 31 */
	"Unknown File Mode",				/* 32 */
	"Out Of Data",					/* 33 */
	"Field Overflow",				/* 34 */
	"File Number Already Used",			/* 35 */
	"File Not Open For Output",			/* 36 */
	"File Not Open For Input",			/* 37 */
	"Expression Must Yield A Number Greater Than -1",/* 38 */
	"Missing Line Number",				/* 39 */
	"Bad Record Number",				/* 40 */
	"Starting Line Number Is Larger Than Ending Line Number",	/* 41 */
	"Bad Floating Point Number In File Buffer",	/* 42 */
	"Can't Send Output To Line Printer",		/* 43 */
	"ELSE With No IF",				/* 44 */
	"ENDIF With No IF",				/* 45 */
	"Bad Error Number",				/* 46 */
	"Bad File Number",				/* 47 */
	"Expression Too Complicated",			/* 48 */
	"String In Numeric Expression",			/* 49 */
	"Function Argument of Wrong Type",		/* 50 */
	"Numeric In String Expression",			/* 51 */
	"Wrong Number of Arguments in Function Call",	/* 52 */
	"Too Many Line Number Reference Updates For MOVE or RENUM", /* 53 */
	"RESUME Encountered With No Error Trapped",	/* 54 */
	"Argument Out Of Range",			/* 55 */
	"Cannot SAVE Program in File",			/* 56 */
	"WHILE Statements Nested Too Deeply",		/* 57 */
	"WHILE With No WEND",				/* 58 */
	"WEND With No WHILE",				/* 59 */
	"Overflow",					/* 60 */
	"No Previous Search String",			/* 61 */
	"Bad Search Pattern",				/* 62 */
	"Cannot Change To Directory",			/* 63 */
	"Cannot CD In A Restricted Shell",		/* 64 */
	"Function Directly Or Indirectly Calls Itself",	/* 65 */
	"Attempted To Take The LOG Of A Negative Number",	/* 66 */
	"Attempted To Take The LOG Of Zero",		/* 67 */
	"Line For INPUT Statement Is Too Long",		/* 68 */
	"INPUT Data Does Not Match Variable List -- Try Again",	/* 69 */
	"FOR Statements Nested Too Deeply",		/* 70 */
	"SETBREAK Needs At Least One Line Number",	/* 71 */
	"GOSUBs Nested Too Deeply",			/* 72 */
	"RETURN With No GOSUB",				/* 73 */
	"FOR With No NEXT",				/* 74 */
	"Too Many Breakpoints",				/* 75 */
	"No Breakpoint Set",				/* 76 */
	"Multiple OPTION BASE Statements",		/* 77 */
	"Breakpoint Already Set",			/* 78 */
	"IF With No ENDIF",				/* 79 */
	"Type Mismatch",				/* 80 */
	"Invalid DATA Item",				/* 81 */
	"Zero Raised To Negative Power",		/* 82 */
	"File Not Open",				/* 83 */
	"Out Of Data Space",				/* 84 */
	"Negative Number Raised To A Non-integer Power",	/* 85 */
	"Array Index Out Of Range",			/* 86 */
	"String Longer Than Maximum String Length",	/* 87 */
	"Array Index Less Than Array Base",		/* 88 */
	"Illegal Line Number",				/* 89 */
	"Unknown OPTION Parameter",			/* 90 */
	"OPTION BASE Must Be Constant 0 or 1",		/* 91 */
	"Function Already Defined",			/* 92 */
	"OPTION BASE Must Precede First DIM Statement",	/* 93 */
	"Integer Overflow",				/* 94 */
	"Single Precision Overflow",			/* 95 */
	"Illegal Format in PRINT USING Statment",	/* 96 */
};


UCHAR	trapflg = 0,
	intrap = 0,
	ers;

int	err = 0;

LINNO 	trapline = 0,
	erl = 0;

extern LINNO	nextno,
		startno;

extern char	*ptr;

extern UCHAR 	stmtno;


error(line,stmt,num)
LINNO	line;
UCHAR	stmt;
int	num;
{
	error2(line,stmt,num,"\n");
}



extern char nullstrng;

error2(line,stmt,num,c)
LINNO	line;
UCHAR	stmt;
int	num;
char	*c;
{
	erl = line;
	ers = stmt;
	err = num;
	if (trapline && !intrap)
	{
		trapflg = 1;
		nextno = trapline;
		stmtno = STMT;
	}
	else if (num < NUMERRS)
	{
		preprint();
		if (line != IMMED)
			printf("ERROR LINE %d: %s%s",line,mesg[num],c);
		else
			printf("ERROR %s%s",mesg[num],c);
	}
}


stoperror(line, num)
LINNO	line;
int	num;
{
	if (num < NUMERRS)
	{
		preprint();
		if (line != IMMED)
			printf("ERROR LINE %d: %s\n",line,mesg[num]);
		else
			printf("ERROR %s\n",mesg[num]);
	}
}



ederror(line,erline,erstmt,num)
LINNO	line;
LINNO	erline;
UCHAR	erstmt;
int	num;
{
	erl = erline;
	ers = erstmt;
	err = num;
	if (trapline && !intrap)
	{
		nextno = trapline;
		stmtno = STMT;
		trapflg = 1;
	}
	else if (num < NUMERRS)
	{
		preprint();
		if (line != IMMED)
			printf("ERROR LINE %d: %s\n",line,mesg[num]);
		else
			printf("ERROR %s\n",mesg[num]);
	}
	return(0);
}


errorproc()
{
	int ernum;

	eval(expr);
	switch(exprtype)
	{
	case DOUBLE:
		if (exprval.dval < 0 || exprval.dval >= NUMERRS)
		{
			error(inst.thing.linno,inst.thing.stmtno, 46);
			return(-1);
		}
		else
			error(inst.thing.linno,inst.thing.stmtno, ernum = intify(exprval.dval));
		break;
	case SINGLE:
		if (exprval.fval < 0 || exprval.fval >= NUMERRS)
		{
			error(inst.thing.linno,inst.thing.stmtno, 46);
			return(-1);
		}
		else
			error(inst.thing.linno,inst.thing.stmtno, ernum = intify(exprval.fval));
		break;
	case INTEGER:
		if (exprval.ival < 0 || exprval.ival >= NUMERRS)
		{
			error(inst.thing.linno,inst.thing.stmtno, 46);
			return(-1);
		}
		else
			error(inst.thing.linno,inst.thing.stmtno, ernum = exprval.ival);
		break;
	case STRING:
		error(inst.thing.linno,inst.thing.stmtno, 46);
		return(-1);
	}
	/* return 0 if error does not terminate execution */
	if (ernum == 29 || ernum == 105 || ernum == 0)
		return(0);
	else
		return(-1);
}


onerrproc()
{
	char	buf[LINMAX+1];
	LINNO	num;
	LINNO	reallinno;
	UCHAR	realstmtno;

	while ((*expr & 0377) != GOTOcode)
		if (*expr)
			expr++;
		else
		{
			error(inst.thing.linno,inst.thing.stmtno, 8);
			return(-1);
		}
	expr += 2;
	if (lex(&expr, buf) != INTCONST)
	{
		error(inst.thing.linno,inst.thing.stmtno, 89);
		return(-1);
	}
	if (!strcmp(buf, "0"))
		num = 0;
	else if ((num = atoln(buf)) == 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 89);
		return(-1);
	}
	if (num == 0)
	{
		if (intrap)
		{
			trapline = intrap = 0;
			error(erl,ers,err);
			return(-1);
		}
	}
	else
	{
		reallinno = inst.thing.linno;
		realstmtno = inst.thing.stmtno;
		if (fetch(num, LINE, &lbdptr) < 0)
		{
			error(reallinno,realstmtno,0);
			return(-1);
		}
	}
	trapline = num;
	return(0);
}


resume()
{
	char	buf[LINMAX+1];
	LINNO	num;
	LINNO	reallinno;
	UCHAR   realstmt;

	if (!intrap)
	{
		error(inst.thing.linno,inst.thing.stmtno, 54);
		return(-1);
	}
	switch(gettoken(&expr, buf))
	{
	case 0:
		nextno = erl;
		stmtno = ers;
		break;
	case INTCONST:
		if (atoi(buf) == 0)
		{
			nextno = erl;
			stmtno = ers;
			break;
		}
		if ((num = atoln(buf)) == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 0);
			return(-1);
		}
		reallinno = inst.thing.linno;
		realstmt  = inst.thing.stmtno;
		if (fetch(num, LINE, &lbdptr) < 0)
		{
			error(reallinno,realstmt,0);
			return(-1);
		}
		nextno = num;
		stmtno = STMT;
		if (countnest(erl, ers, nextno) < 0)
			return(-1);
		break;
	default:
		if ((*buf & 0377) != NEXTcode)
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		fetch(erl,ers, &ptr);
		nextno = NEXT;
		stmtno = STMT;
		break;
	}
	intrap = 0;
	return(0);
}


inittrap()
{
	trapline = 0;
	trapflg = 0;
	intrap = 0;
	err = 0;
	erl = IMMED;
	ers = 0;
}
