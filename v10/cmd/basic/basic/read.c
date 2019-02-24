 /* "read" locates the next item of information in a 'data' statement
 * and places it in variables defined by a list following the read
 * command.

*/
/*	"@(#) read.c: V 2.27 6/27/83"	*/

#include "bas.h"
#include "tokens.h"

extern char	errbuf[];

char	*daptr;

static char	*itmptr;

extern int	DATAcmd;

char	*prncpy(),
	*readcpy();

UCHAR	strflg;

LINNO	rsavno;

int	rsavstmt;

getnext()
{
	while (fetch(NEXT,STMT,&daptr) == 0)
	{
		if (inst.thing.opcode == DATAcmd)
		{
			itmptr = expr;
			inst.thing.linno = rsavno;
			inst.thing.stmtno = rsavstmt;
			return(0);
		}
	}
	error(rsavno,rsavstmt,33);			/* OUT OF DATA */
	inst.thing.linno = rsavno;
	inst.thing.stmtno = rsavstmt;
	return(-1);
}



__read()
{
	char	*ptr;
	char	*savptr;
	int	type,
		comlast;
	char	field[LINMAX+1],
		ascop[LINMAX+1],
		ascno[LINMAX+1];

	skip00(&expr);		/* skip over preceding spaces and tabs */
	ptr = expr;
	rsavno = inst.thing.linno;
	rsavstmt = inst.thing.stmtno;
	if (daptr == 0)
	{
		if (fetch(FIRST,STMT,&daptr) != 0)
		{
			error(rsavno,rsavstmt,33);	/* OUT OF DATA */
			inst.thing.linno = rsavno;
			inst.thing.stmtno = rsavstmt;
			return(-1);
		}
		if (inst.thing.opcode == DATAcmd)
			itmptr = expr;
		else if (getnext() < 0)
			return(-1);
	}
	comlast = 0;
	while (1)
	{
		skip00(&ptr);		/* skip embedded blanks and tabs */
		skip00(&itmptr);
		ptr = prncpy(ascop,ptr);
		if (*itmptr == '\0' && !comlast)
			if (getnext() < 0)
				return(-1);
		itmptr = readcpy(ascno, itmptr);
		if (*itmptr == ',')
		{
			comlast = 1;
			itmptr++;
		}
		else if (*itmptr != '\0')
		{
			error(rsavno, rsavstmt, 13);	/* Bad Delimeter */
			inst.thing.linno = rsavno;
			inst.thing.stmtno = rsavstmt;
			return(-1);
		}
		else
			comlast = 0;
		savptr = ascop;
		if ((type = class(&savptr,field)) != STRID && type != STRAID)
		{
			if (strflg && type >= 0)
			{				/* TYPE MISMATCH */
				error(rsavno,rsavstmt, 80);
				inst.thing.linno = rsavno;
				inst.thing.stmtno = rsavstmt;
				return(-1);
			}
			if (*ascno == '\0')
				strcpy(ascno, "0");
			else if (!isnum(ascno))
			{				/* TYPE MISMATCH */
				error(rsavno,rsavstmt, 80);
				inst.thing.linno = rsavno;
				inst.thing.stmtno = rsavstmt;
				return(-1);
			}
			eval(ascno);
			if (exprtype == INTEGER)
				exprval.dval = (double) exprval.ival;
			else if (exprtype == SINGLE)
				exprval.dval = (double) exprval.fval;
			switch (type)
			{
			case DBLAID:
				if (daputvar(field,exprval.dval) <0)
					return(-1);
				break;
			case DBLID:
				if (dputvar(field,exprval.dval) < 0)
					return(-1);
				break;
			case SNGAID:
				if (faputvar(field,exprval.dval) <0)
					return(-1);
				break;
			case SNGID:
				if (fputvar(field,exprval.dval) < 0)
					return(-1);
				break;
			case INTAID:
				if (iaputvar(field, exprval.dval) < 0)
					return(-1);
				break;
			case INTID:
				if (iputvar(field, exprval.dval) < 0)
					return(-1);
				break;
			default:			/* BAD READ ITEM */
				error(rsavno,rsavstmt, 3);
				inst.thing.linno = rsavno;
				inst.thing.stmtno = rsavstmt;
				return(-1);
			}
		}
		else
		{
			switch (type)
			{
				case STRAID:
					if (saputvar(field,ascno) < 0)
						return(-1);
					break;
				case STRID:
					if (sputvar(field,ascno) < 0)
						return(-1);
					break;
				default:			/* BAD READ */
					error(rsavno,rsavstmt, 3);
					inst.thing.linno = rsavno;
					inst.thing.stmtno = rsavstmt;
					return(-1);
			}
		}
		if (*ptr == '\0')
			return(0);
		if (*ptr++ != ',')
		{
			error(rsavno,rsavstmt, 13);	/* illegal delim */
			inst.thing.linno = rsavno;
			inst.thing.stmtno = rsavstmt;
			return(-1);
		}
	}
}



restore()
{
	LINNO	line;
	char	buf[LINMAX+1];

	rsavno = inst.thing.linno;
	rsavstmt = inst.thing.stmtno;
	skip00(&expr);
	if (*expr != '\0')			/* restore to line # */
	{
		if (lex(&expr, buf) != INTCONST)
		{					/* must be line # */
			error(inst.thing.linno,inst.thing.stmtno, 0);
			return(-1);
		}
		skip00(&expr);
		if (*expr != '\0')
		{					/* must be line # */
			error(inst.thing.linno,inst.thing.stmtno, 0);
			return(-1);
		}
		if ((line = atoln(buf)) == 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 89);
			return(-1);
		}
		if (fetch(line,STMT,&daptr) < 0)
		{
			error(rsavno,rsavstmt, 0);	/* not a valid line # */
			inst.thing.linno = rsavno;
			inst.thing.stmtno = rsavstmt;
			return(-1);
		}
		itmptr = expr;
		inst.thing.linno = rsavno;
		inst.thing.stmtno = rsavstmt;
	}
	else
		daptr = 0;
	return(0);
}



char *
readcpy(to, from)
char	*to,
	*from;
{
	register int	i;

	strflg = 0;
	skip00(&from);
	i = 0;
	if (*from == '"')
	{
		strflg = 1;
		from++;
		while (*from != '"' && *from != '\0')
			to[i++] = *from++;
		from++;
		skip00(&from);
	}
	else
	{
		while (*from != '\0')
		{
			if (*from == ',')
				break;
			to[i++] = *from++;
		}
		while ((to[i-1] == ' ' || to[i-1] == '\t') && i > 0)
			--i;
	}
	to[i] = '\0';
	return(from);
}



char	toktbl[8][5] = {
	{02, 01, 010, 0203, 010},
	{010, 010, 010, 0204, 010},
	{010, 01, 010, 0203, 010},
	{010, 0204, 05, 0203, 010},
	{010, 010, 05, 0204, 010},
	{06, 010, 010, 0207, 010},
	{010, 010, 010, 0207, 010},
	{010, 010, 010, 0207, 010}
};



isnum(str)
char	*str;
{
	short	state,
		nextstate,
		accept,
		tok;

	state = 0;
	while (*str != '\0')
	{
		tok = token(*str++);
		nextstate = (short) ((toktbl[state][tok]) & 0177);
		accept = (short) ((toktbl[state][tok]) & 0200);
		if (nextstate == 010)
			return(0);
		else if (accept && *str == '\0')
			return(1);
		state = nextstate;
	}
	return(0);
}



token(c)
char	c;
{
	switch (c)
	{
		case '+':
		case '-':
			return(0);
		case '.':
			return(1);
		case 'e':
		case 'E':
			return(2);
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return(3);
		default:
			return(4);
	}
}



char *
delimcpy(to,from, delims)
char	*to,
	*from,
	*delims;
{
	int	pcnt,
		type;
	char	buf[LINMAX+1];
	char	*dptr,
		*fptr,
		*nptr;

	pcnt = 0;
	nptr = from;
	do {
		fptr = nptr;
		skip00(&fptr);
		if (pcnt <= 0)
		{
			dptr = delims;
			while (*dptr)
				if (*fptr == *dptr++)
				{
					strncpy(to, from, fptr - from);
					to[fptr-from] = '\0';
					return(fptr);
				}
		}
		nptr = fptr;
		switch (type = lex(&nptr, buf))
		{
			case LPAR:
				pcnt += 1;
				break;
			case RPAR:
				pcnt -= 1;
				break;
			default:
				break;
		}
	} while (type != 0 && type < TOKERR);
	strncpy(to, from, fptr - from);
	to[fptr-from] = '\0';
	return(fptr);
}



char *
prncpy(to,from)
char	*to,
	*from;
{
	return(delimcpy(to, from, ","));
}
