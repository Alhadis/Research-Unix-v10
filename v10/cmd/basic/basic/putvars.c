/*	"@(#) putvars.c: V 1.10 6/14/83"	*/

/*
*	putvars.c: functions for assigning to scalar variables
*/

#include	<stdio.h>
#include	<values.h>
#include	"bas.h"
#include	"tokens.h"

struct sym 	*stsrch(), *stinsert();	/*symbol table routines*/

char		*strget();


/*
*	give double precision variable a value
*/

dputvar(vstr, value)
char 	*vstr;
double	value;
{
	struct sym	*currec;
	char		vnam[IDSIZE+1];

	lex(&vstr, vnam);
	if ((currec = stsrch(vnam, DBLID)) == NULL)
		if ((currec = stinsert(vnam, DBLID)) == NULL)
			return(-1);
	if (currec->isbufvar)
	{
		char	*to, *from;
		int	i;

		to = currec->data.bufvar.start;
		from = (char *) & (value);
		for (i=0; i<currec->data.bufvar.length; i++)
			*to++ = *from++;
		return(0);
	}
	currec->data.dval = value;
	return(0);
}


/*
*	give single precision variable a value
*/

fputvar(vstr, value)
char 	*vstr;
double	value;
{
	struct sym	*currec;
	char		vnam[IDSIZE+1];

	lex(&vstr, vnam);
	if ((currec = stsrch(vnam, SNGID)) == NULL)
		if ((currec = stinsert(vnam, SNGID)) == NULL)
			return(-1);
	if (currec->isbufvar)
	{
		char	*to, *from;
		int	i;
		float	val;

		val = mkfloat(value);
		to = currec->data.bufvar.start;
		from = (char *) & (val);
		for (i=0; i<currec->data.bufvar.length; i++)
			*to++ = *from++;
		return(0);
	}
	currec->data.fval = mkfloat(value);
	return(0);
}



/*
*	give integer variable a value
*/

iputvar(vstr, value)
char 	*vstr;
double	value;
{
	struct sym	*currec;
	char		vnam[IDSIZE+1];

	lex(&vstr, vnam);
	if ((currec = stsrch(vnam, INTID)) == NULL)
		if ((currec = stinsert(vnam, INTID)) == NULL)
			return(-1);
	if (currec->isbufvar)
	{
		char	*to, *from;
		int	i;
		int	val;

		val = mkint(value);
		to = currec->data.bufvar.start;
		from = (char *) & (val);
		for (i=0; i<currec->data.bufvar.length; i++)
			*to++ = *from++;
		return(0);
	}
	currec->data.ival = mkint(value);
	return(0);
}



/*
*	 give string variable a value
*/

sputvar(vstr, sptr)
char	*vstr;
char	*sptr;
{
	struct sym	*currec;
	char	sname[IDSIZE+1];
	extern	int	RSETcmd;

	lex(&vstr, sname);
	if ((currec = stsrch(sname, STRID)) == NULL)
	{
		if ((currec = stinsert(sname, STRID)) == NULL)
			return(-1);
	}
	else if (currec->isbufvar)
	{
		char	*to, *from;
		int	spaces, count;

		count = currec->data.bufvar.length;
		to = currec->data.bufvar.start;
		from = sptr;
		spaces = count - strlen(sptr);
		if (inst.thing.opcode == RSETcmd)
			while (spaces-- > 0)
			{
				*to++ = ' ';
				count--;
			}
		while (count > 0 && *from)
		{
			*to++ = *from++;
			count--;
		}
		while (count-- > 0)
			*to++ = ' ';
		return(0);
	} else 
		strfree(currec->data.string);

	if ((currec->data.string = strget(sptr)) == NULL)
	{
		currec->data.string = strget("");
		return(-1);
	}
	return(0);
}


char	*
scalaraddr(vstr)
char	*vstr;
{
	struct sym	*currec;
	char		sname[IDSIZE+1];
	int		type;

	type = lex(&vstr, sname);
	if ((currec = stsrch(sname, type)) == NULL)
	{
		if ((currec = stinsert(sname, type)) == NULL)
			return(NULL);
		switch(type)
		{
		case SNGID:
			currec->data.fval = 0.0;
			break;
		case DBLID:
			currec->data.dval = 0.0;
			break;
		case INTID:
			currec->data.ival = 0;
			break;
		case STRID:
			currec->data.string = strget("");
			break;
		}
	}
	switch(type)
	{
	case SNGID:
		return((char *) &(currec->data.fval));
	case DBLID:
		return((char *) &(currec->data.dval));
	case INTID:
		return((char *) &(currec->data.ival));
	case STRID:
		return((char *) &(currec->data.string));
	}
	return(NULL);
}
