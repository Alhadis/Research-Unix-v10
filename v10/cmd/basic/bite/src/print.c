/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// print.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:02:55 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *

 *
 * BASIC Print Command
 *
 * MOD,J.P.Hawkins,3-FEB-81 to handle string variables
 */
/*   "@(#) print.c:  V 1.5  5/6/81" */
#include	"bas.h"
#define NMFIELD 5	/* number of print fields */
#define	PSIZE	80	/* max print columns */
/*
#define skip00() {while(*ptext == ' ' || *ptext == '\t') *ptext++;}
*/
#define	skip00()	{}	/* skip00 does nothing */
static	int	contin = 1;	/* line continuation flag initted
				    to no continuation */
extern	struct FILTBL filtbl[];
#ifdef	TEST
char	busaddr[2];	/* IBV11 bus address (one char term by null) */
buspr()
{
	char	*ptr;

	ptr = expr;	/* point to beginning of string */

	if(*ptr++ != '\'')
	{
		error(inst.thing.linno, 100); /* MISSING ' DELIM */
		return(-1);
	}

	busaddr[0] = *ptr++;	/* next char is buss address */
	busaddr[1] = '\0';	/* null terminate */

	expr = ptr;		/* make prguts start at current pointer */
	prguts(1);		/* call prguts with buslfg on */
}
#endif

__print()
{
	prguts(0);
}

/*
 *
 * GUTS TO NORMAL PRINT AND IEEE BUS OUTPUT
 */
prguts(busflg)
int	busflg;		/* true if talking to IBV11 bus */
{
#ifdef	STRINGS
	char	*savptr;
	char	field[80];
#endif
	int	tvalue;		/* tab value */
	int	fildes;		/* file designator for BASIC */
	double	value;		/* evaluated floating point value */
	double	evalx();	/* expression avaluator */
	char	pbuild[PSIZE];	/* print string build area */
	char	numbuf[80];	/* number text storage */
	register i;		/* build txtbuf index */
	int n;			/* number txtbuf index */
	int	fd;		/* file descriptor */
	char *ptext;		/* input text pointer */
	fd = 1;			/* init fd to 1 for normal print */
	ptext = expr;		/* point to beginning of text area */

	i = 0;

if(!(busflg))
{
	if(*ptext == '_' && (*(ptext+1) >= '0' && *(ptext+1) <= '8'))
	{
		*ptext++;
		fildes = *ptext - '1';	/* get BASIC file designator */
		fd = filtbl[fildes].fildes;
		*ptext++;
		if(filtbl[fildes].mode != 'w')	/* file must be open
						     for writing */
		{
			error(inst.thing.linno, 36); /* FILE NOT OPEN FOR WRITE */
			return(-1);
		}
	}
}
	if(*ptext == '\0')	/* if line empty */
		contin = 1;	/* force newline */

	while(*ptext != '\0')
	{
		skip00();	/* skip blanks and tabs */
		if(equal(ptext,"tab"))	/* if "tab" function */
		{
			contin = 1;
			ptext += 3;	/* skip "tab" string */
			if(*ptext != '(')
			{
				error(inst.thing.linno, 30);
				return(-1);
			}
			else
			{
				n = 0;
				cpyprn(numbuf,&ptext,&n); /* get expr */
				numbuf[n] = '\0';
				value = evalx(numbuf);
				if(value < 0.0) value = 0.0;
				tvalue = value;
				while((i < tvalue) && (i < PSIZE-2))
					pbuild[i++] = ' ';
			}
		}
		else if(*ptext == '"')	/* if string delimiter */
		{
			contin = 1;	/* flip to no line continuation */
			*ptext++;	/* bump past delimiter */
			while(*ptext != '"' && *ptext != '\0')
			{
				if(i > PSIZE-2) i = PSIZE-2;
				pbuild[i++] = *ptext++; /* copy text
							into build area */
			}
			if(*ptext == '\0') /* if null before end quote */
			{
				error(inst.thing.linno, 11); /* error */
				return(-1);
			}
			else if(i >= PSIZE-2)
			{
				i = PSIZE-2; /* truncate line at line limit */
			}
			else;

			*ptext++;	/* bump past end delim */
			skip00();	/* skip blanks and tabs */
		}
		else if(*ptext == ',' || *ptext == ';')
		{
				pbuild[i++] = ' ';
		}
		else
		{
			contin = 1;	/* flip to no line continuation */
			n = 0;
			while(*ptext != ';' && *ptext != ',' && *ptext != '\0')
			{
				/*
				 * SKIP EVERYTHING ENCLOSED IN BALANCED ()
				 * This is so commas in array specs
				 * ARENT interpreted as print delims
				 */
				if(*ptext == '(')
				{
					cpyprn(numbuf,&ptext,&n);
				}
				else
				{
					numbuf[n++] = *ptext++;
				}
			}
			numbuf[n] = '\0';	/* null terminate expr */
#ifdef	STRINGS
			savptr=numbuf;
			if(class(&savptr,field) < STCLASS)
			{
#endif
				value = evalx(numbuf); /* eval the expr. */
				/*
			 	* PRECEDE POS. NUMS WITH A BLANK
			 	* IF NOT A BUS CALL
			 	*/
				if(busflg == 0)
					if(value>=0.0) pbuild[i++] = ' ';

				sprintf(numbuf,"%g", value); /* conv to ascii */
#ifdef	STRINGS
			}
			else
			{
				evals(numbuf,numbuf);
			}
#endif
			n=0;
			while(numbuf[n] != '\0' && i < PSIZE-2)
			{
				pbuild[i++] = numbuf[n++];
			}
			if(i >= PSIZE-2)
			{
				i = PSIZE-2; /* truncate line */
			}
		}
		switch(*ptext)
		{
			case ';':
				contin = 0; /* turn on line continuation */
				*ptext++;	/* bump past delim */
				break;
			case ',':
				contin = 0; /* turn on line continuation */
				/*
				 * this is a modulo count to
				 * set up field seperation
				 */
				while((i%(PSIZE/NMFIELD)) && i < PSIZE-2)
					pbuild[i++] = ' '; /* blank fill */
				*ptext++;
				break;
			case '\0':
				break;
			default:
				error(inst.thing.linno, 13); /* missing delim */
				return(0);
				/* bad news */
				break;
		}
		skip00();	/* skip spaces and tabs */
	}
	if(contin && !(busflg)) /* if continuation off
				    and not a bus command */
	{
		if(i >= PSIZE-2)
			i = PSIZE-2; /* truncate line at maximum */
		pbuild[i++] = '\n'; /* put in "newline" char */
	}

	pbuild[i] = '\0';	/* null term string */
	/*
	 * write to terminal if fd = 1 otherwise write to file
	 */
#ifdef	TEST
	if(busflg)
	{
		ibs(busaddr,pbuild);
		return(0);
	}
#endif

	if(fd == 1)
		printf("%s",pbuild);	/* PRINT OUT THE LINE */
	else
		write(fd, pbuild, i);
	return(0);
}
/*
 *
 * /// COPY CHARS WITHIN BALANCED PARENS TO NUMBUF ///
 */
cpyprn(numbuf,txtptr,n)
char	numbuf[];
char	**txtptr;
int	*n;
{
	int	pcnt;

	pcnt = 1;
	numbuf[(*n)++] = *(*txtptr)++;
	while(pcnt>0 && **txtptr != '\0')
	{
		switch(**txtptr)
		{
			case '(':
				pcnt += 1;
				break;
			case ')':
				pcnt -= 1;
				break;
			default:
				break;
		}
		numbuf[(*n)++] = *(*txtptr)++;
	}
}
