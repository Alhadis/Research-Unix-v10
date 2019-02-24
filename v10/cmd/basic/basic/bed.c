/*	"@(#) bed.c: V 2.49 11/10/83"	*/

#include <stdio.h>
#include <ctype.h>
#include "bas.h"
#include "tokens.h"

char	*linptr;	/* points just past the IMMED line 		*/
char	*endofprog;	/* points just past the end of the program lines
			   (at the IMMED line).				*/

char	_string[LINMAX+1];

int	inc;

UCHAR	autflg,
	newaut = 0;

union bascmd	inst;

LINNO	line_number;

char	*curptr, 	/* fetch sets this to point to the beginning of the
			   current line.  other routines may change it. */
	*lbdptr, 	/* used for the <cr> printing of "next" line    */
	*expr;		/* points to expression part of fetched statement */


extern UCHAR	stpflg,
		singflg,
		norestore,
		nonl,
		intrap,
		stmtno,
		interrupt;

extern int	stopr(),
		quit();

extern short	fortop,
		whiletop,
		endfcnt;

extern char	nullstrng,
		*hicore,
		*ptr,
		errbuf[];

extern LINNO	startno,
		nextno,
		stopno;

extern struct conrec	*contop;

extern short	startpos;

bed(fp)
FILE *fp;
{
	LINNO		old_ln;
	int 		linlen,
			overwrite;
	char		*strptr,
			*tmpptr,
			linbuf[ENCLINMAX+1],
			ascno[LINMAX+1];

startover:
	strptr = _string;
	ascno[0] = '\0';
	stpflg = nonl = overwrite = 0;
	if (autflg)	/* if auto line numbering in affect */
	{
		if (line_number + inc > MAXLINNO && !newaut)
		{
			error(IMMED, 1, 89);		/* illegal line # */
			autflg = 0;
			if (fp != 0)
				goto startover;		/* try again */
			else
				return(-1);
		}
		newaut = 0;
		line_number += inc;
		sprintf(ascno,"%d ",line_number);
	}

	/* print the prompt character */
	if (fp != 0) {
		preprint();
		if (isatty(fileno(fp)))
		{
			printf("*%s",ascno);
			startpos += strlen(ascno) + 1;
		}
		if (autflg && fetch(line_number, LINE, &lbdptr) == 0)
		{
			printf("*");
			++startpos;
			overwrite = 1;
		}
		if (lclfgets(strptr,LINMAX,fp) == 0)
			return(-1);			/* EOF */
	}

/* get a line of input from the user and store it in compressed
 * source form unless the line number is zero in which case
 * execute the instruction immediately
*/
	tmpptr = strptr;
	if (autflg)
	{
		if (overwrite)
		{
			skip00(&tmpptr);
			if (*tmpptr == '\n')
				goto startover;	 /* save line from over-write */
			tmpptr = strptr;
		}
	}
	else
		skip00(&strptr);

	if (strptr[strlen(strptr)-1] == '\n')
		strptr[strlen(strptr)-1] = '\0';

	/* get the line number if any */
	if (isdigit(*strptr))
	{
		lex(&strptr, ascno);
		if ((line_number = atoln(ascno)) == 0)
		{
			stoperror(IMMED, 89);
			if (fp != 0)
				goto startover;		/* try again */
			else
				return(-1);
		}
	}
	else if (!autflg)
	{
		old_ln = line_number;
		line_number = IMMED;
	}

/*
 * encode the line and save the resulting length for
 * later insertion into txtbuf
 */

	if ((linlen = line_encode(line_number,strptr,linbuf)) == 0)
	{
		if (line_number == IMMED) /* print the next line */
		{
			fetch(old_ln, LINE, &lbdptr);
			if (fetch(NEXT, LINE, &lbdptr) != 0)
			{
				if (fetch(FIRST, LINE, &lbdptr) != 0)
				{
					goto startover;
				}
			}
			prin();
			line_number = inst.thing.linno;
			return(0);
		}
		else if (autflg)	/* prompt with next line number */
		{
			if (isatty(fileno(fp)))
				goto startover;
			else
				return(-1);
		}

		/* only a line number was entered - delete that line */
		expr = ascno;
		delete();
		return(0);
	}

	/* insert line into txtbuf */
	if (line_number != IMMED)
	{
		cleartops();
		__end();
	}
	if (line_insert(line_number, linbuf, linlen) < 0)
	{
		if (isatty(fileno(fp)))
			goto startover;
		else 
			return(-1);
	}
/* DEBUG   
	dmpprog("After Successful Insertion");
	dmppages("After Successful Insertion");
*/

	/* If the line number is IMMED then we have
	 * a command for immediate execution */

	if (line_number == IMMED)
	{
		UCHAR	tsingflg;

		tsingflg = singflg;
		norestore = 0;
		line_number = old_ln;
		startno = IMMED;
		stmtno = STMT;
		dorun();		/* execute the immediate statement */
		if (!norestore)
			singflg = tsingflg;
	}
	return(0);
}



/* insert line into program space */
line_insert(line_number, linbuf, linlen)
LINNO	line_number;
char	linbuf[];
int	linlen;
{
	register	i;
	register char	*from,
			*to;
	int		ret,
			difference;


	if (line_number == IMMED)	/* immediate execution line goes at */
	{				/* the end of the program space     */
		for (i=0; i<linlen; i++)
			endofprog[i] = linbuf[i];
		linptr = endofprog + linlen;
		return(0);
	}

	ret = fetch(line_number, LINE, &lbdptr);

	if (ret == -1 || line_number > inst.thing.linno)
	/* program space empty OR line falls at end of program */
	{ 
		if (endofprog + linlen + ENCLINMAX >= hicore)
		{
			stoperror(line_number,24);
			return(-1);
		}
		lbdptr = endofprog;			/* for <cr> print */
		from = linptr - 1;
		linptr += linlen;
		to = linptr - 1;
		while (from >= endofprog) 	/* move IMMED line down */
			*to-- = *from--;
		for (i = 0; i < linlen; i++)
			*endofprog++ = linbuf[i];
		update(lbdptr);
	}
	else if (line_number < inst.thing.linno) /* insert line into program */
	{
		from = linptr - 1;
		linptr += linlen;
		endofprog += linlen;
		if (endofprog + ENCLINMAX >= hicore) /*program space overflow*/
		{
			linptr -= linlen;
			endofprog -= linlen;
			stoperror(line_number,24);
			return(-1);
		}
		to = linptr - 1;
		while (from >= curptr)
			*to-- = *from--;
		for (i=0; i<linlen; i++)
			curptr[i] = linbuf[i];
		update(curptr);		/* rebuild page index */
		lbdptr = curptr;	/* for <cr> print */
	}
	else
	/* if we arrive at this point in the program, line_number is
	 * equal to inst.thing.linno and we must replace the old line
	 * with the new one. There are three possibilities:
	 * 1) the new line and the old one are the same length,
	 * 2) the new line is shorter than the old one,
	 * 3) the new line is longer than the old one.
	 */
	{
		difference = linlen - (lbdptr - curptr);
		if (difference == 0)				/*case 1 */
		{
			for (i=0; i<linlen; i++)
				curptr[i] = linbuf[i];
			lbdptr = curptr;
		}
		else if (difference < 0)			/* case 2 */
		{
			difference = -difference;
			for (i=0; i<linlen; i++)
				*(curptr + i) = linbuf[i];
			to = curptr + linlen;
			from = curptr + linlen + difference;
			while (from < linptr)
				*to++ = *from++;
			lbdptr = curptr;
			endofprog -= difference;
			linptr -= difference;
			update(curptr);
		}
		else						/* case 3 */
		{
			if (endofprog + difference + ENCLINMAX >= hicore)
			{ 				/*program overflow*/
				stoperror(line_number,24);
				return(-1);
			}
			from = linptr - 1;
			endofprog += difference;
			linptr += difference;
			to = linptr - 1;
			while (from >= curptr)
				*to-- = *from--;
			for (i=0; i<linlen; i++)
				curptr[i] = linbuf[i];
			update(curptr);
			lbdptr = curptr;
		}
	}
return(0);
}


/*  Initiate Auto Line Numbering  */

autnum()
{
	char *prncpy();
	char buf[LINMAX+1];	/* temporary buffer */

	cleartops();
	__end();
	autflg = 1;
	newaut = 1;
	switch(lex(&expr,buf))
	{
		case 0:
			line_number = 0;
			inc = 10;
			return(0);	/* no expression keep the defaults */
		case LDOT:
			break;
		case INTCONST:
			if ((line_number = atoln(buf)) == 0)
			{
				autflg = 0;
				error(inst.thing.linno,inst.thing.stmtno, 89);
				return(-1);
			}
			break;
		case COMMA:
			line_number = 10;
			goto secondnum;
		default:
			autflg = 0;
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
	}
	switch(lex(&expr,buf))
	{
		case 0:
			inc = 10;
			line_number -= inc;
			return(0);
		case COMMA:
			break;
		default:
			autflg = 0;
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
	}
secondnum:
	switch(lex(&expr,buf))
	{
		case 0:
			break;
		case INTCONST:
			if ((inc = atoln(buf)) == 0)
			{
				autflg = 0;
				error(inst.thing.linno,inst.thing.stmtno, 21);
				return(-1);
			}
			break;
		default:
			autflg = 0;
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
	}
	line_number -= inc;
	return(0);
}



dmpprog(msg)
char	msg[];
{
	dmprange(txtbuf, linptr, msg);
}


dmprange(fptr, tptr, msg)
char	*fptr;
char	*tptr;
char	msg[];
{
	char		*ptr;
	union bascmd	loc;
	int		i;

	printf("Program Dump %s\n",msg);
	ptr = fptr;
	while (ptr < tptr)
	{
		for (i=0; i<CMDSIZE; i++)
			loc.byte[i] = *ptr++;
		dmpcmd(loc);
		printf(" ws = %d", (UCHAR) *ptr++);
		printf(" exprlen = %d", (UCHAR) *ptr++);
		printf(" %s\n", ptr);
		ptr += strlen(ptr) + 1;
	}
}


dmpcmd(inst)
union bascmd inst;
{
	printf("line# = %d statement# = %d opcode = %d",
		inst.thing.linno,inst.thing.stmtno,inst.thing.opcode);
}
