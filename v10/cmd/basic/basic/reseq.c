/*	"@(#) reseq.c: V 2.39 8/19/83"	*/

/* resequence the program buffer */

#include 	<stdio.h>
#include 	"bas.h"
#include 	"tokens.h"

#define retrn(X) {cleartops(); __end(); return(X);}

extern int	IFcmd,
		ELSEcmd,
		GOTOcmd,
		GOSUBcmd,
		RESTOREcmd,
		RESUMEcmd,
		ONcmd,
		RUNcmd,
		CHAINcmd,
		ERLcmd;

extern LINNO 	line_number,
		trapline;

static LINNO	curlinno,
		calline;

static UCHAR	callstmt;

extern char	_string[];

static struct REFP
	{
		LINNO			whoref;
		LINNO			refwhat;
		unsigned 		refcnt   : 14;
		unsigned 		notmoved : 1;
		unsigned 		updated  : 1;
		struct REFP		*next;
	} *refptr, *savit, *headptr;

extern struct {
	LINNO		ln;
	unsigned	updated:1;
	} breaklines[];

extern UCHAR	breakflg;

FILE	*tmpfile(),
	*fclose(),
	*fp;

extern	FILE	*tmpfp;

#define	STMTADV(ptr) (ptr) += CMDSIZE + 2 + (UCHAR) *((ptr) + CMDSIZE + 1)


breseq(begin, end, newbegin, inc)
LINNO	begin,
	end,
	newbegin;
int	inc;
{
	char	tmpline[ENCLINMAX+1];
	int	i,
		length,
		newnums;
	long	lcheck;
	FILE	*tmptmp;
	char	*linnoptrs[LINMAX/2];

	headptr = NULL;
	newnums = 0;
	fetch(MAXLINNO,LINE,&lbdptr);
	if (inst.thing.linno < begin) /* range is beyond last line of program */
		retrn(0);
	if (fetch(FIRST,LINE,&lbdptr) < 0)		/* get first line */
	{
		retrn(0);
	}
	else if (inst.thing.linno > end)/* range is before first program line */
		retrn(0);
	if (makerefs(begin,end,newbegin,inc,&newnums) < 0)
		retrn(-1);		/* if it references a line#, record it*/
	while (fetch(NEXT,LINE,&lbdptr) == 0)	/* search the remaining lines */
	{
		if (makerefs(begin,end,newbegin,inc,&newnums) < 0)
			retrn(-1);
	}
	lcheck = (long) newbegin + (long) inc * (long) (newnums - 1);
	if (lcheck > MAXLINNO)
	{
		error(calline,callstmt,89);  /* produce line number too big */
		retrn(-1);
	}
	if (begin != 0)
	{
		fp = tmpfile();
		fprintf(fp,"delete %d-%d\n",begin,end);
		tmptmp = tmpfp;
		tmpfp = NULL;
	}
	curlinno = newbegin;
	fetch(begin,LINE,&lbdptr);
	newlinnos(begin, inc);
	while (fetch(NEXT,LINE,&lbdptr) != -1)
	{
		if (inst.thing.linno > end)
			break;
		newlinnos(begin, inc);
	}
	if (begin != 0)
	{
		int savetrap;	/* don't let delete clobber error trapping */

		savetrap = trapline;
		trapline = 0;
		rewind(fp);
		while (bed(fp) == 0)  ;
		fclose(fp);
		trapline = savetrap;
		tmpfp = tmptmp;
	}
	else
		update(txtbuf);			/* update page pointers */
	refptr = headptr;
	while (refptr != NULL)
	{
		fetch(refptr->whoref,LINE,&lbdptr);
		if (refptr->updated)
		{
			getlinnos(curptr, linnoptrs);
			length = ln_update(curptr, lbdptr-curptr, 
					   linnoptrs[refptr->refcnt-1], 
					   refptr->refwhat, tmpline);
			line_insert(inst.thing.linno, tmpline, length);
		}
		else
			ederror(refptr->whoref,calline,callstmt,0);
		refptr = refptr->next;
	}
	while (headptr != NULL)
	{
		refptr = headptr;
		headptr = headptr->next;
		free(refptr);
	}
	if (breakflg)
	{
		for (i = 0; i < BREAKSIZE; ++i)
			if (breaklines[i].updated == 1)
				breaklines[i].updated = 0;
	}
	retrn(0);
}



/* find references to moved line numbers (between begin and end) on
   most recently fetched line and make nodes for them 			*/
makerefs(begin,end,newbegin,inc,newnptr)
LINNO	begin,
	end,
	newbegin;
int	inc,
	*newnptr;
{
	int	oldlin;
	char	refcnt;
	char	*linnoptrs[LINMAX/2];

	refcnt = 0;
	getlinnos(curptr, linnoptrs);
	while (linnoptrs[refcnt] != NULL)
	{
		++refcnt;
		oldlin = atoi(linnoptrs[refcnt-1]);
		if (oldlin < begin || oldlin > end) /* not in move range */
			continue;
		refptr = (struct REFP *) malloc(sizeof(struct REFP));
		if (refptr == NULL)
		{
			while (headptr != NULL)
			{
				refptr = headptr;
				headptr = headptr->next;
				free(refptr);
			}
			error(calline,callstmt,53);  /* not enough space */
			return(-1);
		}
		refptr->next = headptr;
		headptr = refptr;
		if (inst.thing.linno >= begin && inst.thing.linno <= end)
		{
			refptr->whoref = newbegin + (*newnptr) * inc;
			refptr->notmoved = 0;
		}
		else
		{
			refptr->whoref = inst.thing.linno;
			refptr->notmoved = 1;
		}
		refptr->refwhat = oldlin;
		refptr->refcnt = refcnt;
		refptr->updated = 0;
	}
	if (inst.thing.linno >= begin && inst.thing.linno <= end)
		(*newnptr)++;
	return(0);
}



/* give most recently fetched line the new line number, curlinno, and write to 
   temporary file if a move is being processed (lines may be overwritten). */
newlinnos(begin, inc)
LINNO	begin;
int	inc;
{
	int	i;
	char	buf[LINMAX+1];
	char	*linptr;

	line_number = curlinno;
	if (trapline == inst.thing.linno)     /* move error trap routine line */
		trapline = line_number;
	if (breakflg)
	{
		if ((i = findbreak(inst.thing.linno)) != -1)
		{
			delbreak(i);
			insbreak(line_number);
			i = findbreak(line_number);
			breaklines[i].updated = 1;
		}
	}
	curlinno += inc;
	if (begin > 0)	/* potential reordering of lines - write out */
	{
		line_decode(curptr, buf);
		linptr = buf;
		while (NUM(*linptr))
			linptr++;
		fprintf(fp,"%d%s",line_number, linptr);
	}
	else
	{
		union bascmd	loc;

		while (1)
		{
			for (i = 0; i < CMDSIZE; ++i)
				loc.byte[i] = *(curptr+i);
			if (loc.thing.linno != inst.thing.linno)
				break;
			loc.thing.linno = line_number;
			for (i = 0; i < CMDSIZE; ++i)
				*(curptr+i) = loc.byte[i];
			STMTADV(curptr);
		}
	}
	refptr = headptr;
	while (refptr != NULL)
	{
		/* remove references by lines that will be overwritten */
		if (refptr->notmoved && refptr->whoref == line_number 
				     && begin > 0)
		{
			if (refptr == headptr)
			{
				headptr = refptr->next;
				free(refptr);
				refptr = headptr;
			}
			else
			{
				savit->next = refptr->next;
				free(refptr);
				refptr = savit->next;
			}
			continue;
		}
		/* update referenced line number */
		if (inst.thing.linno == refptr->refwhat && !refptr->updated)
		{
			refptr->refwhat = line_number;
			refptr->updated = 1;
		}
		savit = refptr;
		refptr = refptr->next;
	}
	return(0);
}


/* update the line number at linnoptr in line fptr with newlinno
   and put resulting line in line tptr, return length of new line  */
ln_update(fptr, length, linnoptr, newlinno, tptr)
char	*fptr;
int	length;
LINNO	newlinno;
char	*linnoptr;
char	*tptr;
{
	union bascmd	loc;
	char		*stmtptr;
	char		*lnptr; 		/*position of new line number*/
	register	i,
			j;
	int		oldnumlen,
			newnumlen,
			exprlen;

	stmtptr = fptr;
	while (1) 				/* find correct statement */
	{
		for (j=0; j<CMDSIZE; j++)
			loc.byte[j] = *(stmtptr + j);
		exprlen = (UCHAR) *(stmtptr + CMDSIZE + 1);
		if (stmtptr + CMDSIZE + 2 + exprlen > linnoptr)
			break;
		stmtptr += CMDSIZE + 2 + exprlen;
	}
	i = 0;
	while (fptr + i < stmtptr + CMDSIZE + 1) /* copy to expr length byte */
	{
		tptr[i] = fptr[i];
		i++;
	}
	lnptr = tptr + (linnoptr - fptr);
	newnumlen = sprintf(lnptr, "%d", newlinno);
	oldnumlen = 0;
	while (NUM(linnoptr[oldnumlen]))
		oldnumlen++;
	exprlen += (newnumlen - oldnumlen);
	tptr[i] = (UCHAR) exprlen;
	i++;
	while (tptr + i < lnptr)	/* copy to line number */
	{
		tptr[i] = fptr[i];
		i++;
	}
	while (i + oldnumlen < length) /* copy rest of line */
	{
		tptr[i + newnumlen] = fptr[i + oldnumlen];
		i++;
	}
	return(length + (newnumlen - oldnumlen));
}


/* puts pointers to line numbers on "line" in "ptrarray" */
getlinnos(line, ptrarray)
char	*line;
char	*ptrarray[];
{
	LINNO	line_num;
	union bascmd	loc;
	char		*stmtptr;
	register	i;
	int		nrefs;

	nrefs = 0;
	stmtptr = line;
	for (i=0; i<CMDSIZE; i++)
		loc.byte[i] = *stmtptr++;
	stmtptr += 2;
	line_num = loc.thing.linno;
	do {
		nrefs += stmtlinnos(loc.thing.opcode, stmtptr, ptrarray+nrefs);
		stmtptr += (UCHAR) * (stmtptr-1);
		for (i=0; i<CMDSIZE; i++)
			loc.byte[i] = *stmtptr++;
		stmtptr += 2;
	} while (loc.thing.linno == line_num);
}



/* find line numbers in statement, put pointers to them into ptrarray,
   and return the number of line numbers found				*/
stmtlinnos(command, line, ptrarray)
short	command;
char	*line,
	*ptrarray[];
{
	char	*before,
		*after,
		tokbuf[LINMAX+1];
	int	nextref,
		ttype;

	nextref = 0;
	after = line;
	skip00(&after);
	before = after;
	if (command == CHAINcmd)
		while (lex(&after, tokbuf))
			if (!strcmp("delete", tokbuf))
			{
				int	i;

				for (i=1; i <=3; i++)
				{
					skip00(&after);
					before = after;
					if (lex(&after, tokbuf) == INTCONST)
						ptrarray[nextref++] = before;
				}
				ptrarray[nextref] = NULL;
				return(nextref);
			}
	if (command == RESUMEcmd)
	{
		if (lex(&after, tokbuf) != INTCONST)
		{
			ptrarray[nextref] = NULL;
			return(nextref);
		}
		else if (atoi(tokbuf) == 0)
		{
			ptrarray[nextref] = NULL;
			return(nextref);
		}
		ptrarray[nextref++] = before;
		ptrarray[nextref] = NULL;
		return(nextref);
	}
	if (command == GOTOcmd || command == GOSUBcmd 
	    || command == RESTOREcmd || command == RUNcmd)
	{
		if (lex(&after, tokbuf) == INTCONST)
			ptrarray[nextref++] = before;
		ptrarray[nextref] = NULL;
		return(nextref);
	}
	if (command == ELSEcmd)
	{
		if (lex(&after, tokbuf) == INTCONST)
		{
			ptrarray[nextref++] = before;
			ptrarray[nextref] = NULL;
			return(nextref);
		} else
			after = before;
	}
	if (command == IFcmd || command == ELSEcmd || command == ONcmd)
		while (ttype = gettoken(&after, tokbuf))
		{
			if (!strcmp(tokbuf, "resume"))
			{
				skip00(&after);
				before = after;
				if (lex(&after,tokbuf) != INTCONST 
				|| atoi(tokbuf) == 0)
				{
					after = before;
					continue;
				}
				ptrarray[nextref++] = before;
			}
			else if ((*tokbuf & 0377) == THENcode 
				|| !strcmp(tokbuf, "run") 
				|| !strcmp(tokbuf, "restore"))
			{
				skip00(&after);
				before = after;
				if (lex(&after,tokbuf) != INTCONST 
				|| atoi(tokbuf) == 0)
				{
					after = before;
					continue;
				}
				ptrarray[nextref++] = before;
			}
			else if ((*tokbuf & 0377) == GOTOcode 
				|| (*tokbuf & 0377) == GOSUBcode)
			{
				do {
					skip00(&after);
					before = after;
					if (lex(&after, tokbuf) != INTCONST)
						return(nextref);
					else if (atoi(tokbuf) == 0)
						continue;
					ptrarray[nextref++] = before;
				} while (lex(&after, tokbuf) == COMMA);
				after = before;
			}
			else if (ttype == FUNCID 
				&& ((tokbuf[1] & 0377) - 1) == ERLcmd)
			{
				skip00(&after);
				before = after;
				ttype = lex(&after, tokbuf);
				if (!relop(ttype))
				{
					after = before;
					continue;
				}
				skip00(&after);
				before = after;
				if (lex(&after, tokbuf) != INTCONST)
				{
					after = before;
					continue;
				}
				ptrarray[nextref++] = before;
			}
			else if (!strcmp("delete", tokbuf))
			{
				int	i;

				for (i=1; i <=3; i++)
				{
					skip00(&after);
					before = after;
					if (lex(&after, tokbuf) == INTCONST)
						ptrarray[nextref++] = before;
				}
			}
			skip00(&after);
		}
	ptrarray[nextref] = NULL;
	return(nextref);
}


dmpreflist(msg)
char	msg[];
{
	struct REFP	*ptr;

	printf("Dunp Of Line Number Reference List %s\n",msg);
	ptr = headptr;
	while (ptr != NULL)
	{
		printf("whoref=%d refwhat=%d refcnt=%d notmoved=%d updated=%d\n"
			,ptr->whoref,ptr->refwhat,ptr->refcnt,ptr->notmoved,
			ptr->updated);
		ptr = ptr->next;
	}
}



renum()
{
	LINNO	begin,
		end,
		newbegin;
	int	inc;
	char	tokbuf[LINMAX+1],
		*cmdline;

	calline = inst.thing.linno;
	callstmt = inst.thing.stmtno;
	cmdline = expr;
	begin = 0;
	end = MAXLINNO;
	newbegin = 10;
	inc = 10;
	skip00(&cmdline);
	if (*cmdline == '\0')
		return(breseq(begin, end, newbegin, inc));
	switch (lex(&cmdline, tokbuf))
	{
	case INTCONST:
			if ((newbegin = atoln(tokbuf)) == 0)
			{
				error(calline,callstmt,89);
				return(-1);
			}
			break;
	case COMMA:    
			goto secondnum;
	default:       	error(calline,callstmt,13);
   		       	return(-1);
	}
	switch(lex(&cmdline, tokbuf))
	{
	case COMMA:	break;
	case 0:		return(breseq(begin, end, newbegin, inc));
	default:	error(calline,callstmt,13);
			return(-1);
	}
secondnum:
	switch(lex(&cmdline, tokbuf))
	{
	case INTCONST:  if ((begin = atoln(tokbuf)) == 0)
			{
				error(calline,callstmt,89);
				return(-1);
			}
			break;
	case COMMA:	goto thirdnum;
	default:	error(calline,callstmt,39);
			return(-1);
	}
	switch(lex(&cmdline, tokbuf))
	{
	case COMMA:	break;
	case 0:		return(breseq(begin, end, newbegin, inc));
	default:	error(calline,callstmt,13);
			return(-1);
	}
thirdnum:
	switch(lex(&cmdline, tokbuf))
	{
	case INTCONST:  if ((inc = atoi(tokbuf)) == 0)
			{
				error(calline,callstmt,17);
				return(-1);
			}
			break;
	default:	error(calline,callstmt,17);
			return(-1);
	}
	return(breseq(begin, end, newbegin, inc));
}



mov()
{
	char		tokbuf[LINMAX+1];
	LINNO		begin,
			end,
			newbegin;
	int 		inc;
	int		delim;

	calline = inst.thing.linno;
	callstmt = inst.thing.stmtno;
	if (lnpars(&begin, &end, &delim, &expr) < 0)
		return(-1);

	if (end == 0 && delim)
		end = MAXLINNO;
	else if (end == 0)
		end = begin;
	else if (end < begin)
	{
		error(calline,callstmt,41);/* start > finish */
		retrn(-1);
	}
	if (lex(&expr, tokbuf) != COMMA)
	{
		error(calline,callstmt,39);
		return(-1);
	}
	if (lex(&expr, tokbuf) != INTCONST)
	{
		error(calline,callstmt, 39);	/* missing start value */
		return(-1);
	}
	if ((newbegin = atoln(tokbuf)) == 0)
	{
		error(calline,callstmt,89);
		return(-1);
	}
	switch(lex(&expr, tokbuf))
	{
	case COMMA:	if (lex(&expr, tokbuf) != INTCONST)
			{
				error(calline,callstmt,17);   
				return(-1); /* missing increm. value */
			}
			if ((inc = atoln(tokbuf)) == 0)
			{
				error(calline,callstmt,89);
				return(-1);
			}
			break;
	case 0:		inc = 10;
			break;
	default:	error(calline,callstmt,13);
			return(-1);
	}
	if (fetch(begin,LINE,&lbdptr) == -2)
	{
		if (inst.thing.linno > end || fetch(NEXT,LINE,&lbdptr) == -1)
		{
			ederror(begin,calline,callstmt,0);
			return(-1);
		}
	}
	return(breseq(begin, end, newbegin, inc));
}
