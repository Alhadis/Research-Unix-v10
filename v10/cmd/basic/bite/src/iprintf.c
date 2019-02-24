/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// iprintf.c /////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:38:43 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 * BITE PRINTF
 * INTERPRETIVE PRINTF FOR "BITE" BASIC f and g FORMATS ALLOWED ONLY
 *
 */
/*   "@(#) iprintf.c:  V 1.3  9/22/81" */
int fd;
#include	"bas.h"
extern	struct FILTBL filtbl[];

iprintf()
{
	int	prerr;
	char	s[100];
	register x;
	prerr = isprintf(s);
	if(fd == 1)			/* normal printf */
		printf(s);
	else				/* file printf */
	{
		for(x=0; s[x] != '\0'; x++); /* count chars */
		write(fd, s, x);	/* printf to file */
	}
	return(prerr);
}
/*
 *
 *
 */
isprintf(s)
char	s[];
{
	char	format[80];	/* format string */
	char	expbfr[40];	/* expression buffer */
	double	evalx();
	int	fildes;
	int	i;
	char	*ptr;
	int	fcount;		/* number of args format string asks for */
	double	arg[10];


	ptr = expr;
	fd = 1;		/* set fd for normal printf */

	if(*ptr == '_' && (*(ptr+1) >= '0' && *(ptr+1) <= '8'))
	{
		*ptr++;
		fildes = *ptr - '1';	/* get BASIC file designator */
		fd = filtbl[fildes].fildes;
		*ptr++;
		if(filtbl[fildes].mode != 'w')	/* file must be open
						     for writing */
		{
			error(inst.thing.linno, 36); /* FILE NOT OPEN FOR WRITE */
			return(-1);
		}
	}
	/*
	 * COPY FORMAT STRING
	 */
	if(*ptr++ != '"')
	{
		error(inst.thing.linno, 11); /* missing quotes */
		return(-1);
	}
	fcount = 0;

	for(i=0; (*ptr != '"') && (*ptr != '\0'); )
	{
		if(*ptr == '%')	/* format delimiter */
		{
			fcount += 1; /* bump expected arg count */
			format[i++] = *ptr++;
		}
		else if(*ptr == '\\') /* special character */
		{
			ptr++;	/* look at next char */
			switch(*ptr)
			{
				case 'n': /* NEWLINE */
					format[i] = '\n';
					break;
				case 'r': /* CARRIAGE RET */
					format[i] = '\r';
					break;
				case 't': /* TAB */
					format[i] = '\t';
					break;
				case 'b': /* BACKSPACE */
					format[i] = '\b';
					break;
				default:
					format[i] = *ptr;
					break;
			}
			ptr++;
			i++;
		}
		else
			format[i++] = *ptr++;
	}
	format[i] = '\0';
	if(*ptr == '\0')
	{
		error(inst.thing.linno, 11); /* unbalanced quotes */
		return(-1);
	}
	ptr++;
	if(*ptr != ',' && fcount == 0)
	{
		sprintf(s,format);
		return(0);
	}
	i = 0;
	while(*ptr != '\0')
	{
		ptr++;
		ptr = (char *)prncpy(expbfr,ptr); /* get nxt expr field */
		arg[i++] = evalx(expbfr);
	}
	if(i != fcount)		/* no. args == no. expected args */
	{
		error(inst.thing.linno, 39); /* arg count mismatch */
		return(-1);
	}
	if(fcount > 10)
	{
		error(inst.thing.linno, 40); /* MORE THAN 10 ARGS */
		return(-1);
	}
	sprintf(s,format,arg[0],arg[1],arg[2],arg[3],arg[4],arg[5],arg[6],
		arg[7],arg[8],arg[9]);
	return(0);
}
