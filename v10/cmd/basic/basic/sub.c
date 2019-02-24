/*	"@(#) sub.c: V 2.30 6/8/83"	*/

/* routine to do character substitution for the editor */

#include <stdio.h>
#include "bas.h"

#define ertst if (ifnul(work,sline,sstmt)) return(-1)
#define retrn(X) {cleartops(); __end(); return(X);}


extern FILE 	*tmpfp;		/* file pointer for change file used by UNDO */

UCHAR	subflg = 0;

int	changed;			/* number of changed lines */

static char prevstring[] = NULL;

extern char	_string[];

extern char	*regex(),
		*regcmp(),
		*__loc1;				/* from regex */



LINNO	line;



sub()
{
	char	c,
		*lp,
		*work,
		*from,
		*to,
		what[LINMAX+1],
		with[LINMAX+1],
		temp[LINMAX+1],
		delim;

	int	lindig,
		matches,
		gflag = 0,
		delimseen,
		sstmt,
		i;

	LINNO 	sline,
		start,
		finish;

	sline = inst.thing.linno;
	sstmt = inst.thing.stmtno;
	skip00(&expr);			/* skip blanks and tabs before line # */

/* get line numbers for sub */

	if (lnpars(&start, &finish, &delimseen, &expr) < 0)
		return(-1);

	if (finish == 0 && delimseen)
		finish = MAXLINNO;
	else if (finish == 0)
		finish = start;
	else if (finish < start)
	{						/* start > finish */
		error(inst.thing.linno,inst.thing.stmtno, 41);
		retrn(-1);
	}
	line = start;
	skip00(&expr);
	work = expr;
	ertst;
	while ((c = *work) == ' ' || c == '\t')
	{
		ertst;
		work++;
	}
	delim = *work++;			/* get the delimeter */
	from = what;
	to = with;
	while (*work != delim)
	{
		ertst;
		if (*work == '\\')
			*from++ = *work++;
		ertst;
		*from++ = *work++;
	}
	*from = '\0';
	work++;					/* bump past delimeter */
	while (*work != delim && *work != '\0')
	{
		if (*work == '\\')
			*to++ = *work++;
		ertst;
		*to++ = *work++;
	}
	if (*work != '\0')
	{
		++work;
		skip00(&work);
		if (*work == 'g')
			gflag = 1;
		else if (*work != '\0')
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			retrn(-1);
		}
	}
	*to++ = '\0';
	if (*what == '\0')		/* use last search string */
	{
		if (*prevstring == '\0')	/* no previous string */
		{
			error(sline,sstmt,61);
			retrn(-1);
		}
		else
			strcpy(what,prevstring);
	}

	strcpy(prevstring,what);		/* save search string */
	lp = regcmp(what, 0);
	if (lp == 0)			/* bad pattern */
	{
		error(sline,sstmt,62);
		retrn(-1);
	}
	matches = 0;
	if ((i = fetch(line,LINE,&lbdptr)) != 0)     /* get line to be edited */
	{
		if (i == -1)
		{
			preprint();
			printf("Search Pattern Not Found\n");
			free(lp);
			retrn(-1);
		}
		else
		{
			if (inst.thing.linno < start)
			{				/* no such line */
				ederror(start,sline,sstmt,0);
				free(lp);
				retrn(-1);
			}
		}
	}
	changed = 0;
	subflg = 1;
        if (tmpfp != NULL)
		fseek(tmpfp,0L,0);
	filter(with);
	while (inst.thing.linno <= finish)
	{
		for (lindig = 1, i = inst.thing.linno; i >= 10; ++lindig)
			i /= 10;

		line_decode(curptr,temp);
		*(temp + strlen(temp) - 1) = '\0';
		if (gflag)
		{
			int	linchg = 0,
				pos;
			char	tmplin[LINMAX+1];

			strcpy(tmplin,temp);
			pos = lindig;
			while ((pos = do_sub(lp,temp,with,pos)) >= 0)
			{
				strcpy(temp,_string);
				++linchg;
			}
			if (linchg)
			{
				++changed;
				matches += linchg;
				strcat(_string,"\n");
				if (tmpfp != NULL)
				{		/* put line in change file */
					fputs(tmplin, tmpfp);
					fputs("\n", tmpfp);
				}
			}
			else
			{
				if (fetch(NEXT, LINE, &lbdptr) != 0)
					break;
				continue;
			}
		}
		else if (do_sub(lp,temp,with,lindig) >= 0)
		{
			++changed;
			++matches;
			strcat(_string,"\n");
			if (tmpfp != NULL)
			{
				fputs(temp, tmpfp);	/* put in change file */
				fputs("\n", tmpfp);
			}
		}
		else
		{
			if (fetch(NEXT, LINE, &lbdptr) != 0)
				break;
			continue;
		}
		line = inst.thing.linno;
		if (bed(0) < 0)
		{
			subflg = 0;
			retrn(-1);
		}
		if ((i = fetch(line,LINE,&lbdptr)) == 0)
		{
			prin();
			if (fetch(NEXT, LINE, &lbdptr) != 0)
				break;
		}
		else if (i != -2)
			break;		/* no more lines in txtbuf */
	}
	free(lp);
	subflg = 0;
	if (matches == 0)
	{
		preprint();
		printf("Search Pattern Not Found\n");
	}
	retrn(0);
}



do_sub(lp,from,to,pos)
char	*lp,
	*from,
	*to;
int	pos;
{
	char	*next,
		*strptr;
	int	diff;

	next = regex(lp, from + pos);		/* don't match on line# */
	if (next == 0)
		return(-1);
	diff = (__loc1 + strlen(to)) - from;
	strptr = _string;
	while (*from != '\n' && *from != '\0')
	{
		if (from == __loc1)
		{
			from = next;
			__loc1 -= 1;
			while (*to)
				*strptr++ = *to++;
		}
		else
			*strptr++ = *from++;
	}
	if (from == __loc1)
		while (*to)
			*strptr++ = *to++;
	*strptr = '\0';
	return(diff);
}



ifnul(work,sline,sstmt)
char	*work;
LINNO	sline;
int	sstmt;
{
	if (*work == '\0')
	{
		error(sline,sstmt,20);
		return(1);
	}
	return(0);
}


/* undo the last substitute */



undo()
{
	LINNO	curline;

	if (tmpfp != NULL)
	{
		fseek(tmpfp,0L,0);
		while ((changed-- > 0) &&
				(lclfgets(_string, LINMAX, tmpfp) != NULL))
		{
			if ((curline = atoi(_string)) == 0)
				return(-1);
			if (bed(0) < 0)
				retrn(-1);
			fetch(curline, LINE, &lbdptr);
			prin();
		}
	}
	retrn(0);
}



/*
 *	global search command
 */



glob()
{
	char	line[LINMAX+1],
		buf[LINMAX+1],
		delim,
		*lp,
		*next;
	int	sstmt,
		matches;
	LINNO	sline;

	sline = inst.thing.linno;
	sstmt = inst.thing.stmtno;
	cleartops();
	__end();
	matches = 0;
	lp = line;
	skip00(&expr);
	if (*expr == '\0')
	{
		error(sline,sstmt,62);			/* bad search pattern */
		retrn(-1);
	}
	delim = *expr++;
	if (*expr == '\0')
	{
		error(sline,sstmt,62);			/* bad search pattern */
		retrn(-1);
	}
	while (*expr != delim && *expr != '\n' && *expr != '\0')
		*lp++ = *expr++;		/* copy search pattern */
	*lp = '\0';
	if (fetch(FIRST, LINE, &lbdptr) != 0)
	{
		preprint();
		printf("Search Pattern Not Found\n");
		retrn(-1);
	}
	lp = regcmp(line,0);
	if (lp == 0)					/* bad pattern */
	{
		error(sline,sstmt,62);
		retrn(-1);
	}
	line_decode(curptr,buf);	/* form the expanded line for regex */
	*(buf + strlen(buf) - 1) = '\0';
	next = regex(lp, buf);
	if (next != 0)
	{
		++matches;		/* found one */
		*(buf + strlen(buf)) = '\n';
		prin1(buf);
	}
	while (fetch(NEXT, LINE,&lbdptr) == 0)
	{
		line_decode(curptr,buf);
		*(buf + strlen(buf) - 1) = '\0';
		next = regex(lp, buf);
		if (next != 0)
		{
			++matches;		/* found one */
			*(buf + strlen(buf)) = '\n';
			prin1(buf);
		}
	}
	free(lp);
	if (matches == 0)			/* no lines found */
	{
		preprint();
		printf("Search Pattern Not Found\n");
	}
	retrn(0);
}


/*
 *	filter out '\' of a string
 */


filter(buf)
char	*buf;
{
	char	*tmp;

	if (strlen(buf) == 0)
		return(0);
	if (strlen(buf) == 1 && *buf == '\\')
		*buf = '\0';
	while (*buf != '\0')
	{
		if (*buf == '\\' && *(buf + 1) != '\\')
			for (tmp = buf + 1; *(tmp-1) != '\0'; ++tmp)
				*(tmp-1) = *tmp;
		else if (*buf == '\\')
		{
			for (tmp = buf + 1; *(tmp-1) != '\0'; ++tmp)
				*(tmp-1) = *tmp;
			buf += 1;
		}
		else
			++buf;
	}
	return(0);
}
