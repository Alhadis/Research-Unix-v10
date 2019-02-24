 /*	File Access Preparation Routines     */
/* "@(#) bfiles.c: V 2.20 11/10/83" */
#include	<stdio.h>
#include	<values.h>
#include	<bas.h>
#include	<tokens.h>

extern char	*prncpy(),
		errbuf[];

/* number of user files and file table record */
#define		NMROFFILES	10

struct FILTBL 	{
		FILE		*fildes;
		char		mode;
		unsigned char	contin;
		short		currec;
		short		curchr;
		short		recsize;
		short		numrecs;
		char		*buffer;
		};

struct FILTBL	filtbl[NMROFFILES];

/*
 * 	open a file
 *
 * 	format:	open mode, [#]file number, file name
 */

openf()
{
	char	fname[STRMAX+1],
		c,
		buf[LINMAX+1];
	int	filno,
		reclen = 128;

	skip00(&expr);
	expr = prncpy(buf, expr);
	if (*expr != '\0')
		++expr;
	if (eval(buf) < 0)
		return(-1);
	if (exprtype != STRING)
	{				/* number in string */
		error(inst.thing.linno,inst.thing.stmtno, 51);
		return(-1);
	}
	if (strlen(exprval.sval) != 1)
	{
		sprintf(errbuf, ": %s\n", exprval.sval);
		error2(inst.thing.linno,inst.thing.stmtno,32,errbuf);	
		return(-1); /* unknown file mode */
	}
	c = *exprval.sval;
	skip00(&expr);
	if (*expr == '#')
	{
		++expr;
		skip00(&expr);
	}
	expr = prncpy(buf, expr);
	if (*expr != '\0')
		++expr;
	if (eval(buf) < 0)
		return(-1);
	if (exprtype == STRING)
	{		/* string in numeric */
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	if (exprtype == DOUBLE)
		filno = intify(exprval.dval);
	else if (exprtype == SINGLE)
		filno = intify(exprval.fval);
	else
		filno = exprval.ival;
	if (!legfildes(filno))
	{
		error(inst.thing.linno,inst.thing.stmtno, 47);
		return(-1);
	}
	filno--;
	expr = prncpy(buf, expr);
	if (eval(buf) < 0)
		return(-1);
	if (exprtype != STRING)
	{
		error(inst.thing.linno,inst.thing.stmtno, 51);
		return(-1);
	}
	strcpy(fname, exprval.sval);
	if (filtbl[filno].fildes != NULL)
	{
		sprintf(errbuf, ": #%d\n", filno + 1);
		error2(inst.thing.linno,inst.thing.stmtno,35,errbuf);	
		return(-1); /* file already open */
	}
	switch (c)
	{
		case 'i':
		case 'I':
			filtbl[filno].mode = 'r';
			buf[0] = 'r';
			break;
		case 'o':
		case 'O':
			filtbl[filno].mode = 'w';
			buf[0] = 'w';
			break;
		case 'a':
		case 'A':
			filtbl[filno].mode = 'w';
			buf[0] = 'a';
			break;
		case 'r':
		case 'R':
			if (*expr == ',')
			{
				expr++;
				if (eval(expr) < 0)
					return(-1);
				if (exprtype == STRING)
				{
					error(inst.thing.linno,
					      inst.thing.stmtno, 49);
					return(-1);
				}
				else if (exprtype == DOUBLE)
					reclen = intify(exprval.dval);
				else if (exprtype == SINGLE)
					reclen = intify(exprval.fval);
				else
					reclen = exprval.ival;
			}
			return(ropen(filno, fname, reclen));
		default:				/* unknown file mode */
			sprintf(errbuf, ": %c\n",c);
			error2(inst.thing.linno,inst.thing.stmtno,32, errbuf);
			return(-1);
	}
	if (*expr)	/* other stuff on line */
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	buf[1] = '\0';
	if ((filtbl[filno].fildes = fopen(fname, buf)) == NULL)
	{
		error(inst.thing.linno,inst.thing.stmtno, 2);
		return(-1);
	}
	filtbl[filno].contin = 1;
	filtbl[filno].curchr = 1;
	return(0);
}




/*
 *	close a list of files
 *
 *	format:	close [#]filenum [,[#]filenum]*
 */

_closef()
{
	char	buf[LINMAX+1];
	int	filno;

	skip00(&expr);
	if (*expr == '\0')
	{
		clall();
		return(0);
	}
	while (*expr != '\0')
	{
		if (*expr == '#')
		{
			++expr;
			skip00(&expr);
		}
		expr = prncpy(buf, expr);
		if (eval(buf) < 0)
			return(-1);
		if (exprtype == STRING)
		{				/* string in numeric expr. */
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		}
		if (exprtype == DOUBLE)
			filno = intify(exprval.dval);
		else if (exprtype == SINGLE)
			filno = intify(exprval.fval);
		else
			filno = exprval.ival;
		if (!legfildes(filno))
		{				/* file # not between 1 and 8 */
			error(inst.thing.linno,inst.thing.stmtno, 47);
			return(-1);
		}
		filno--;
		if (filtbl[filno].fildes == NULL)
		{
			sprintf(errbuf, ": #%d\n",filno + 1);
			error2(inst.thing.linno,inst.thing.stmtno,83, errbuf);
			if (*expr != '\0')
			{
				++expr;
				skip00(&expr);
			}
			continue;
		}
		if (filtbl[filno].mode == 'u')
			free(filtbl[filno].buffer);
		fclose(filtbl[filno].fildes);
		filtbl[filno].fildes = NULL;
		filtbl[filno].mode = '\0';
		filtbl[filno].recsize = 0;
		filtbl[filno].numrecs = 0;
		filtbl[filno].currec = 0;
		filtbl[filno].curchr = 0;
		filtbl[filno].buffer = NULL;
		if (*expr != '\0')
		{
			++expr;
			skip00(&expr);
		}
	}
	return(0);
}



/*
 * ////// CLOSEALL //////
 *
 * Close ALL open files.
 */

clall()
{
	register	i;
	int		marknonbuf();

	for (i = 0; i < NMROFFILES; i++)	/* scan file table */
	{
		if (filtbl[i].fildes == NULL)
			continue;
		if (filtbl[i].mode == 'u')
			free(filtbl[i].buffer);
		fclose(filtbl[i].fildes);
		filtbl[i].fildes = NULL;
		filtbl[i].mode = '\0';
		filtbl[i].numrecs = 0;
		filtbl[i].recsize = 0;
		filtbl[i].currec = 0;
		filtbl[i].curchr = 0;
		filtbl[i].buffer = NULL;
	}
	traverse(marknonbuf);	/* remove buffer variables */
	savmarked();
	return(0);
}



initfiles()
{
	register i;

	for (i=0; i<NMROFFILES; i++)
	{
		filtbl[i].fildes  = NULL;
		filtbl[i].mode    = '\0';
		filtbl[i].contin  = 0;
		filtbl[i].recsize = 0;
		filtbl[i].numrecs = 0;
		filtbl[i].currec  = 0;
		filtbl[i].curchr  = 0;
		filtbl[i].buffer  = NULL;
	}
}


legfildes(num)
int	num;
{
	if (num >= 1 && num <= NMROFFILES)
		return(1);
	return(0);
}



ropen(fnum, fname, reclen)
int	fnum;
char	fname[];
int	reclen;
{
	register	i;
	char		*malloc();
	long		flen();

	if (reclen <= 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 5);
		return(-1);
	}
	if ((filtbl[fnum].fildes = fopen(fname, "r+")) == NULL)
		if ((filtbl[fnum].fildes = fopen(fname, "w+")) == NULL)
		{
			error(inst.thing.linno,inst.thing.stmtno, 2);
			return(-1);
		}
	if ((filtbl[fnum].buffer = malloc((unsigned) reclen)) == NULL)
	{
		filtbl[fnum].fildes = NULL;
		error(inst.thing.linno,inst.thing.stmtno, 84);
		return(-1);
	}
	for (i=0; i<reclen; i++)
		filtbl[fnum].buffer[i] = '\0';
	filtbl[fnum].contin = 1;
	filtbl[fnum].recsize = reclen;
	filtbl[fnum].numrecs = flen(filtbl[fnum].fildes) / reclen;
	filtbl[fnum].currec = 0;
	filtbl[fnum].curchr = 0;
	filtbl[fnum].mode = 'u';
	return(0);
}


pad(fnum, torec)
int	fnum,
	torec;
{
	register 	need,
			i;

	need = torec - filtbl[fnum].numrecs;
	fseek(filtbl[fnum].fildes, 0, 2);	/* seek to end */
	while (need-- > 0)
	{
		for (i=1; i<=filtbl[fnum].recsize; i++)
			putc('\0', filtbl[fnum].fildes);
		(filtbl[fnum].numrecs)++;
	}
}


position(fnum, recno)
int	fnum,
	recno;
{
	filtbl[fnum].currec = recno;
	filtbl[fnum].curchr = 0;
	fseek(filtbl[fnum].fildes, (recno - 1) * filtbl[fnum].recsize, 0);
}


rget(fnum, recno)
int	fnum,
	recno;
{
	pad(fnum, recno);
	position(fnum, recno);
	fread(filtbl[fnum].buffer,1,filtbl[fnum].recsize,filtbl[fnum].fildes);
}


rput(fnum, recno)
int	fnum,
	recno;
{
	pad(fnum, recno - 1);
	position(fnum, recno);
	fwrite(filtbl[fnum].buffer,1,filtbl[fnum].recsize,filtbl[fnum].fildes);
	if (recno > filtbl[fnum].numrecs)
		filtbl[fnum].numrecs = recno;
}


putgetline(fnumptr, rnumptr)
int	*fnumptr,
	*rnumptr;
{
	char	buf[LINMAX+1];

	skip00(&expr);
	if (*expr == '#')
	{
		expr++;
		skip00(&expr);
	}
	expr = prncpy(buf, expr);
	if (eval(buf) < 0)
		return(-1);
	if (exprtype == STRING)
	{
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	if (exprtype == DOUBLE)
		*fnumptr = intify(exprval.dval);
	else if (exprtype == SINGLE)
		*fnumptr = intify(exprval.fval);
	else
		*fnumptr = exprval.ival;
	if (!legfildes(*fnumptr))
	{
		error(inst.thing.linno,inst.thing.stmtno, 47);
		return(-1);
	}
	if (*expr == ',')
	{
		expr++;
		if (eval(expr) < 0)
			return(-1);
		if (exprtype == DOUBLE)
			*rnumptr = intify(exprval.dval);
		else if (exprtype == SINGLE)
			*rnumptr = intify(exprval.fval);
		else
			*rnumptr = exprval.ival;
		if (*rnumptr <= 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 40);
			return(-1);
		}
	}
	else
		*rnumptr = 0;
	return(0);
}


__put()
{
	int 	fnum,
		rnum;

	if (putgetline(&fnum, &rnum) < 0)
		return(-1);
	if (filtbl[fnum-1].fildes == NULL || filtbl[fnum-1].mode != 'u')
	{
		error(inst.thing.linno,inst.thing.stmtno, 27);
		return(-1);
	}
	if (rnum == 0)
		rnum = filtbl[fnum-1].currec + 1;
	rput(fnum - 1, rnum);
	return(0);
}


__get()
{
	int 	fnum,
		rnum;

	if (putgetline(&fnum, &rnum) < 0)
		return(-1);
	if (filtbl[fnum-1].fildes == NULL || filtbl[fnum-1].mode != 'u')
	{
		error(inst.thing.linno,inst.thing.stmtno, 27);
		return(-1);
	}
	if (rnum == 0)
		rnum = filtbl[fnum-1].currec + 1;
	rget(fnum - 1, rnum);
	return(0);
}


field()
{
	char	buf[LINMAX+1];
	int	type,
		bytes,
		byteno,
		filno;
	struct sym	*stinsert(),
			*currec;

	skip00(&expr);
	if (*expr == '#')
		expr++;
	expr = prncpy(buf, expr);
	if (eval(buf) < 0)
		return(-1);
	if (exprtype == STRING)
	{
		error(inst.thing.linno,inst.thing.stmtno, 49);
		return(-1);
	}
	else if (exprtype == DOUBLE)
		filno = intify(exprval.dval);
	else if (exprtype == SINGLE)
		filno = intify(exprval.fval);
	else 
		filno = exprval.ival;
	if (!legfildes(filno))
	{
		error(inst.thing.linno,inst.thing.stmtno, 47);
		return(-1);
	}
	filno--;
	if (filtbl[filno].mode != 'u')
	{
		error(inst.thing.linno,inst.thing.stmtno, 27);
		return(-1);
	}
	byteno = 0;
	while (*expr)
	{
		if (*expr != ',')
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		expr++;
		if (eval(expr) < 0)
			return(-1);
		if (exprtype == STRING)
		{
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		}
		else if (exprtype == DOUBLE)
			bytes = intify(exprval.dval);
		else if (exprtype == SINGLE)
			bytes = intify(exprval.fval);
		else 
			bytes = exprval.ival;
		if (bytes <= 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 30);
			return(-1);
		}
		expr = eoexpr;
		if ((*expr & 0377) != AScode)
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		expr++;
		switch (type = lex(&expr, buf))
		{
		case SNGID:
			if (bytes != sizeof(float))
			{
				error(inst.thing.linno,inst.thing.stmtno, 30);
				return(-1);
			}
			break;
		case DBLID:
			if (bytes != sizeof(double))
			{
				error(inst.thing.linno,inst.thing.stmtno, 30);
				return(-1);
			}
			break;
		case INTID:
			if (bytes != sizeof(int))
			{
				error(inst.thing.linno,inst.thing.stmtno, 30);
				return(-1);
			}
			break;
		case STRID:
			break;
		default:
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		if (byteno + bytes > filtbl[filno].recsize)
		{
			error(inst.thing.linno,inst.thing.stmtno, 34);
			return(-1);
		}
		if ((currec = stinsert(buf, type)) == NULL)
			return(-1);
		currec->isbufvar = 1;
		currec->data.bufvar.length = bytes;
		currec->data.bufvar.start  = filtbl[filno].buffer + byteno;
		byteno += bytes;
		skip00(&expr);
	}
	return(0);
}


dmpfiles(msg)
char	*msg;
{
	int	i;
	char	buf[128];

	printf("File Table Dump: %s\n", msg);
	for (i=0; i<NMROFFILES; i++)
	{
		printf("%d: fildes=%d mode=%c currec=%d recsize=%d numrecs=%d\n"
			,i,filtbl[i].fildes,filtbl[i].mode,filtbl[i].currec,
			filtbl[i].recsize,filtbl[i].numrecs);
		strncpy(buf, filtbl[i].buffer, filtbl[i].recsize);
		buf[filtbl[i].recsize] = '\0';
		printf("    buffer=%s\n", buf);
	}
}
