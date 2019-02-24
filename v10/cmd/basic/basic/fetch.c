/*	"@(#) fetch.c: V 2.16 8/19/83"	*/

/*	fetch.c - routines for fetching program lines/statements from txtbuf
		  and performing encoding/decoding of these lines/statements. */

#include	<ctype.h>
#include	"bas.h"
#include	"tokens.h"

extern int	LETcmd,
		ELSEcmd,
		QUOcmd,
		REMcmd,
		GLOBcmd,
		SUBcmd,
		MIDcmd;

extern struct pages	*index;

/*
*	Statement Format:
*
*	----------------------------------------------------------------------
*	| header block | white space | length | expression part of statement |
*	----------------------------------------------------------------------
*	
*	the header block contains line number, statement number and opcode.
*
*	the white space byte counts the spaces after the line number or colon
*	which precedes the statement.
*
*	the length byte counts the number of characters (including the null)
*	in the rest of the encoded statement.
*
*	the expression part of the statement is the rest of the encoded line.
*
*/


/*
*	The constants FIRST, NEXT, LINE, and STMT are defined in bas.h
*	for use in calls to fetch as follows:
*
*	fetch(linno, stmtno, &ptr);
*		- fetch statement, stmtno, on line, linno, and set ptr to the
*		  beginning of the next statement.
*
*	fetch(FIRST, STMT, &ptr);
*		- fetch the first statement of the program and set ptr to the
*		  beginning of the next statement.
*
*	fetch(FIRST, LINE, &ptr);
*		- fetch the first statement of the program and set ptr to the
*		  beginning of the first statement on the next line.
*
*	fetch(linno, STMT, &ptr);
*		- fetch first statement on line, linno, and set ptr to the 
*		  beginning of the next statement.
*
*	fetch(linno, LINE, &ptr);
*		- fetch line, linno, and set ptr to the beginning of the first
*		  statement on the next line.
*
*	fetch(NEXT, STMT, &ptr);
*		- fetch the statement pointed to by ptr and set ptr to the
*		  beginning of the next statement.
*
*	fetch(NEXT, LINE, &ptr);
*		- fetch the statement pointed to by ptr (the first statement on
*		  the line) and set ptr to the beginning of the first statement
*		  on the next line.
*
*	return codes:
*		 0 - fetch was successful.
*		-1 - end of program encountered (or program space empty).
*		-2 - a line (or statement) was fetched with number next greater
*		     than the line number requested OR the line requested was
*		     greater than the last line number in the program so the 
*		     last line of the program was fetched. (requested line
*		     does not exist)
*
*	side effects:
*		inst 	- gets copy of CMDCODE structure for the fetched 
*			  statement.
*		curptr  - points into txtbuf to the beginning of the fetched 
*			  statement.
*		expr	- points into txtbuf to the beginning of the 
*			  expression part of fetched statement
*
*/

#define	STMTADV(ptr) (ptr) += CMDSIZE + 2 + (UCHAR) *((ptr) + CMDSIZE + 1)

fetch(linno, stmtno, ptrptr)
LINNO	linno;
int	stmtno;
char	**ptrptr;
{
	register	i;
	int		j;
	union bascmd	loc;
	char		*searchptr;

	switch(linno)
	{
	case FIRST:
		curptr = *ptrptr = txtbuf;
		if (endofprog == txtbuf)	/* no program */
			return(-1);
		for (i=0; i<CMDSIZE; i++)
			inst.byte[i] = curptr[i];
		expr = curptr + CMDSIZE + 2;
		if (stmtno == LINE)
			do {
				STMTADV(*ptrptr);
				if (*ptrptr == endofprog || *ptrptr >= linptr)
					break;
				for (i=0; i<CMDSIZE; i++)
					loc.byte[i] = *((*ptrptr)+i);
			} while (loc.thing.stmtno != 1);
		else
			STMTADV(*ptrptr);
		return(0);
	case NEXT:
		if (*ptrptr == endofprog || *ptrptr >= linptr)/*end of program*/
			return(-1);
		curptr = *ptrptr;
		for (i=0; i<CMDSIZE; i++)
			inst.byte[i] = curptr[i];
		expr = curptr + CMDSIZE + 2;
		if (stmtno == LINE)
			do {
				STMTADV(*ptrptr);
				if (*ptrptr == endofprog || *ptrptr >= linptr)
					break;
				for (i=0; i<CMDSIZE; i++)
					loc.byte[i] = *((*ptrptr)+i);
			} while (loc.thing.stmtno != 1);
		else
			STMTADV(*ptrptr);
		return(0);
	case IMMED:
		curptr = endofprog;
		for (i=0; i<CMDSIZE; i++)
			inst.byte[i] = curptr[i];
		if (stmtno == LINE)
		{
			expr = curptr + CMDSIZE + 2;
			*ptrptr = linptr;
			return(0);
		}
		else if (stmtno != 1)
		{
			STMTADV(curptr);
			if (advance(&curptr, stmtno) < 0)
				return(-1);
			else
				for(i=0; i<CMDSIZE; i++)
					inst.byte[i] = curptr[i];
		}
		expr = curptr + CMDSIZE + 2;
		*ptrptr = expr + (UCHAR) *(expr-1);
		return(0);
	default:
		if (endofprog == txtbuf)	/* no program */
		{
			curptr = *ptrptr = txtbuf;
			return(-1);
		}
		for (j=0; j<NMPAG-1; j++)	/*set j to correct page index*/
			if (linno <= index[j].maxlin || index[j+1].maxlin == 0)
				break;
		searchptr = curptr = index[j].begin;
		for (i=0; i<CMDSIZE; i++)
			inst.byte[i] = curptr[i];
		while (inst.thing.linno < linno)
		{
			STMTADV(searchptr);
			if (searchptr == endofprog || searchptr >= linptr)
				break;
			for (i=0; i<CMDSIZE; i++)
				loc.byte[i] = *(searchptr+i);
			if (loc.thing.linno != inst.thing.linno)
			{
				curptr = searchptr;
				inst = loc;
			}
		}
		if (inst.thing.linno != linno)
		{
			expr = curptr + CMDSIZE + 2;
			*ptrptr = curptr;
			if (stmtno == LINE)
				do {
					STMTADV(*ptrptr);
					if (*ptrptr == endofprog 
							|| *ptrptr >= linptr)
						break;
					for (i=0; i<CMDSIZE; i++)
						loc.byte[i] = *((*ptrptr)+i);
				} while (loc.thing.stmtno != 1);
			else
				STMTADV(*ptrptr);
			return(-2);
		}
		else if (stmtno == LINE)
		{
			expr = curptr + CMDSIZE + 2;
			*ptrptr = curptr;
			do {
				STMTADV(*ptrptr);
				if (*ptrptr == endofprog || *ptrptr >= linptr)
					break;
				for (i=0; i<CMDSIZE; i++)
					loc.byte[i] = *((*ptrptr)+i);
			} while (loc.thing.stmtno != 1);
			return(0);
		}
		else if (stmtno != 1)
		{
			STMTADV(curptr);
			if (advance(&curptr, stmtno) < 0)
				return(-1);
			else
				for(i=0; i<CMDSIZE; i++)
					inst.byte[i] = curptr[i];
		}
		expr = curptr + CMDSIZE + 2;
		*ptrptr = expr + (UCHAR) *(expr-1);
		return(0);
	}
}


/* advance *ptrptr to next line and return line number OR 0 if at program end */
LINNO
line_advance(ptrptr)
char	**ptrptr;
{
	union bascmd	loc;
	register	i;

	do {
		STMTADV(*ptrptr);
		if (*ptrptr == endofprog || *ptrptr >= linptr)
			return(0);
		for (i=0; i<CMDSIZE; i++)
			loc.byte[i] = *((*ptrptr)+i);
	} while (loc.thing.stmtno != 1);
	return(loc.thing.linno);
}



/* advance *ptrptr to statement number stmtno OR first statement on next line */
advance(ptrptr, stmtno)
char	**ptrptr;
int	stmtno;
{
	union bascmd  loc;
	register	i;

	while (*ptrptr != endofprog && *ptrptr < linptr)
	{
		for (i=0; i<CMDSIZE; i++)
			loc.byte[i] = *((*ptrptr)+i);
		if (loc.thing.stmtno == stmtno || loc.thing.stmtno == 1)
			return(0);
		*ptrptr += CMDSIZE + 2 + (UCHAR) *(*ptrptr + CMDSIZE + 1);
	}
	return(-1);
}



/* This table is used to encode and decode embedded keywords for
   storage compression and execution speed			*/

#define KEYTABSIZE	29

char	*keyword[KEYTABSIZE] =
{
"<",
"<=",
"<>",
"=",
"=<",
"=>",
">",
"><",
">=",
"and",
"as",
"eqv",
"error",
"gosub",
"goto",
"imp",
"input",
"input#",
"mod",
"next",
"not",
"or",
"spc",
"step",
"tab",
"then",
"to",
"using",
"xor"
};



keysearch(key)
char	key[];
{
	register	m,
			l,
			u,
			n;

	l = 0;
	u = KEYTABSIZE - 1;
	while (l <= u)
	{
		m = (l + u) / 2;
		if ((n = strcmp(key, keyword[m])) > 0)
			l = m + 1;
		else if (n == 0)
			return(m);
		else 
			u = m - 1;
	}
	return(-1);
}


/* convert a string to lower case characters */
lower(string)
char	*string;
{
	register char	*sptr;
	int		qflg = 0;

	sptr = string;
	while (*sptr)
	{
		if (*sptr == '"')
			qflg = qflg ? 0 : 1;
		else if (!qflg)
			*sptr = (char) tolower((int) *sptr);
		sptr++;
	}
}



/* encode expression part of a statement.  stop at \0, :, or ' */
embedcodes(inlinptr, encline)
char	**inlinptr,
	encline[];
{
	char	*leadptr,
		*trailptr,
		*encptr,
		token[LINMAX+1];
	int	i;

	leadptr = *inlinptr;
	lower(leadptr);
	encptr = encline;
	while (isspace(*leadptr))
		*encptr++ = *leadptr++;
	trailptr = leadptr;
	while (gettoken(&leadptr, token))	/* not end of line */
	{
		if (!strcmp(token, ":") || !strcmp(token, "'") 
					|| !strcmp(token, "else"))
			break;	/* statement separator OR comment command 
						       OR else statement */
		if (!strcmp(token, "go"))/* allow go<ws>to and go<ws>sub */
		{
			char	bozo[LINMAX+1],
				*silly;

			silly = leadptr;
			gettoken(&silly, bozo);
			if (!strcmp(bozo, "to") || !strcmp(bozo, "sub"))
			{
				strcat(token, bozo);
				leadptr = silly;
			}
		}
		if ((i = keysearch(token)) >= 0)
		{
			if (((i+200) == TABcode || (i+200) == SPCcode)
			    && *leadptr != '(') 
					/* only tab( and spc( are encoded */
				while (trailptr < leadptr)
					*encptr++ = *trailptr++;
			else
				*encptr++ = (char) 0200 + i;
		}
		else if ((i = funcsearch(token)) >= 0)
		{
			*encptr++ = (char) 0377;
			*encptr++ = (unsigned char) (i + 1);
		}
		else
			while (trailptr < leadptr)
				*encptr++ = *trailptr++;
		while (isspace(*leadptr))
			*encptr++ = *leadptr++;
		trailptr = leadptr;
	}
	*inlinptr = trailptr;
	*encptr = '\0';
}



/* return command code and put text in buf, return -1 if illegal command */
getcommand(lptrptr, buf)
char	**lptrptr,
	buf[];
{
	char	*end;
	int	code;

	end = *lptrptr;
	if (gettoken(&end, buf) == FUNCID && (buf[1] & 0377) - 1 == MIDcmd)
	{
		*lptrptr = end;
		return(encode("mid$"));
	}
	if (!strcmp(buf, ":"))
		return(encode(""));	/* null statement */
	lower(buf);
	if (!strcmp(buf, "go"))		/* allow go<ws>to and go<ws>sub */
	{
		char	bozo[LINMAX+1],
			*silly;

		silly = end;
		gettoken(&silly, bozo);
		lower(bozo);
		if (!strcmp(bozo, "to") || !strcmp(bozo, "sub"))
		{
			strcat(buf, bozo);
			end = silly;
		}
	}
	if ((code = encode(buf)) < 0) 	/* not found in command table */
	{
		strcpy(buf, "\010");	/* implied LET */
		return(LETcmd);
	}
	*lptrptr = end;
	return(code);
}


/* encode an entire program line and return length of encoded line */
line_encode(linno, inline, outline)
LINNO	linno;
char	inline[],
	outline[];
{
	int		i,
			length = 0,	/* length of encoded line */
			stmtno = 1;
	UCHAR		exprlen,
			colflg = 0,	/* statement ended in a ':' */
			ws;
	char		buf[LINMAX+1],
			*inlinptr,
			*outlinptr,
			*tmpptr;
	union bascmd	loc;

	inlinptr = inline;
	outlinptr = outline;
	do {
		loc.thing.linno = linno;
		loc.thing.stmtno = stmtno;
		ws = 0;
		while (1)			/* scan white space */
		{
			if (*inlinptr == ' ')
				ws += 1;
			else if (*inlinptr == '\t')
				ws += 8;
			else
				break;
			inlinptr++;
		}
		if (*inlinptr == '\0')			/* empty line */
			return(0);
		if (ws == 0 && stmtno == 1)/*force one blank after line number*/
			ws = 1;
		tmpptr = inlinptr;
		loc.thing.opcode = getcommand(&inlinptr, buf);
		if (loc.thing.opcode == ELSEcmd && colflg)
		{
			loc.thing.opcode = encode("");
			inlinptr = tmpptr;
		}
		if (!nocode(loc.thing.opcode))
			embedcodes(&inlinptr, outlinptr + CMDSIZE + 2);
		else
		{
			i = 0;
			while (*inlinptr == ' ' || *inlinptr == '\t')
				*(outlinptr + CMDSIZE + 2 + i++) = *inlinptr++;
			*(outlinptr + CMDSIZE + 2 + i) = '\0';
			while (gettoken(&inlinptr, buf))
			{
				if ((!strcmp(buf, ":") || !strcmp(buf, "'")
						       || !strcmp(buf, "else"))
						&& loc.thing.opcode != GLOBcmd
						&& loc.thing.opcode != SUBcmd
						&& loc.thing.opcode != REMcmd
						&& loc.thing.opcode != QUOcmd)
				{
					inlinptr -= strlen(buf);
					break;
				}
				else
					strcpy(outlinptr+CMDSIZE+2+i, buf);
				i += strlen(buf);
				while (*inlinptr == ' ' || *inlinptr == '\t')
					*(outlinptr+CMDSIZE+2+i++) = *inlinptr++;
				*(outlinptr + CMDSIZE + 2 + i) = '\0';
			}
		}
		exprlen = strlen(outlinptr + CMDSIZE + 2) + 1;
		for (i=0; i<CMDSIZE; i++)
			*outlinptr++ = loc.byte[i];
		*outlinptr++ = ws;
		*outlinptr++ = exprlen;
		outlinptr += exprlen;
		length += CMDSIZE + 2 + exprlen;
		stmtno++;
		if (*inlinptr == ':')
		{
			inlinptr++;
			colflg = 1;
		}
		else
			colflg = 0;
	} while (*inlinptr);
	return(length);
}


/* decode a program line in txtbuf into its printable equivalent */
line_decode(in,out)
char	*in,
	*out;
{
	union bascmd	line;
	LINNO		firstline;
	int		i;
	char		*op,
			*inptr,
			*outptr;
	int		ws;

	inptr = in;
	outptr = out;
	for (i = 0; i < CMDSIZE; ++i)
		line.byte[i] = *inptr++;
	ws = (UCHAR) *inptr++;
	*inptr++;
	firstline = line.thing.linno;
	outptr += sprintf(outptr,"%d",line.thing.linno);
	while (1)
	{
		i = ws / 8;
		while (i--)
			*outptr++ = '\t';
		i = ws % 8;
		while (i--)
			*outptr++ = ' ';
		if (line.thing.opcode != LETcmd)
		{
			op = decode(line.thing.opcode);
			outptr += sprintf(outptr,"%s",op);
		}
		if (!nocode(line.thing.opcode))
			expand(&inptr, &outptr);
		else
		{
			while (*inptr)
				*outptr++ = *inptr++;
			++inptr;
		}
		for (i = 0; i < CMDSIZE; ++i)
			line.byte[i] = *inptr++;
		ws = (UCHAR) *inptr++;
		*inptr++;
		if (line.thing.linno == firstline)
		{
			if (line.thing.opcode != QUOcmd &&
						line.thing.opcode != ELSEcmd)
				*outptr++ = ':';
		}
		else
			break;
	}
	*outptr++ = '\n';
	*outptr = '\0';
	return(0);
}
