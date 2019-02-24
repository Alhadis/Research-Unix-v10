/*	"@(#) def.c: V 1.21 8/19/83"	*/

/*
*	def.c: 	functions for handling the definition and calls
*		to user-defined functions.
*/

#include	<bas.h>
#include	"tokens.h"
#include	<stdio.h>
#include	<ctype.h>

struct sym	*arginsert(),
		*stinsert(),	
		*stsrch();

extern struct sym	*paramptr;

char		*strget();


/*
*	processes a user-defined function definition line
*/

def()
{
	struct sym	*fptr,
			*prevaptr,
			*aptr;
	char		funnam[IDSIZE+1],
			argnam[MAXARGS][IDSIZE+1],
			*cptr;
	short		funtype,
			argtype[MAXARGS],
			argno;
	int		i;


	cptr = expr;
	if (lex(&cptr,funnam) == FUNCID)
		return(defusr());

	cptr = expr;
	switch (funtype = getid(&cptr, funnam))	/*get function name*/
	{
	case DBLUID:
	case SNGUID:
	case INTUID:
	case STRUID:
			break;
	default:
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
	}

	if (*cptr == '(')
	{
		argno = 0;
		while (*cptr != ')' && *cptr != '\0')
		{
			cptr++;
			skip00(&cptr);
			if (argno >= MAXARGS)
			{
				error(inst.thing.linno,inst.thing.stmtno, 6);
				return(-1);
			}
			if ((argtype[argno] = getid(&cptr, argnam[argno])) < 0)
				return(-1);
			if (*cptr != ',' && *cptr != ')')
			{
				error(inst.thing.linno,inst.thing.stmtno, 21);
				return(-1);
			}
			argno++;
		}
		--argno;
		if (*cptr != ')')
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		cptr++;
		skip00(&cptr);
		if ((*cptr & 0377) != EQcode)	
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		cptr++;
		skip00(&cptr);
		if (*cptr == '\0')	/* nothing after '=' */
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);	/* bad def. syntax */
			return(-1);
		}

		/* get function symbol table record */
		if ((fptr = stsrch(funnam, funtype)) != NULL)	
		{
			error(inst.thing.linno,inst.thing.stmtno, 92);
			return(-1);
		}
		else if ((fptr = stinsert(funnam, funtype)) == NULL)
			return(-1);

		/*put function expression in txtbuf*/
		if ((fptr->data.udef.funcexp = strget(cptr)) == NULL)
			return(-1);
	
		if ((fptr->data.udef.arg = prevaptr =
				arginsert(argnam[0], argtype[0])) == NULL)
			return(-1);
		for(i=1; i <= argno; i++)
		{
			if ((aptr = arginsert(argnam[i], argtype[i])) == NULL)
				return(-1);
			fptr->data.udef.arg = aptr;
			aptr->next = prevaptr;
			prevaptr = aptr;
		}
		fptr->scratch = 0;
		return(0);
	}
	else if ((*cptr & 0377) == EQcode)
	{
		cptr++;
		skip00(&cptr);
		if (*cptr == '\0')	/* nothing after '=' */
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);	/* bad def. syntax */
			return(-1);
		}

		/* get function symbol table record */
		if ((fptr = stsrch(funnam, funtype)) != NULL)	
		{
			error(inst.thing.linno,inst.thing.stmtno, 92);
			return(-1);
		}
		else if ((fptr = stinsert(funnam, funtype)) == NULL)
			return(-1);

		/*put function expression in txtbuf*/
		if ((fptr->data.udef.funcexp = strget(cptr)) == NULL)
			return(-1);
		fptr->data.udef.arg = NULL;
		fptr->scratch = 0;
		return(0);
	}
	else
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);	/* bad def. syntax */
		return(-1);
	}

}



/*	DEFtype statement processors	*/

#define	DBL	1
#define SNG	2
#define INT	3
#define STR	4

unsigned char	deftype['z' - 'a' + 1];

definit()
{
	register	i;

	for (i = 0; i <= 'z' - 'a'; i++)
		deftype[i] = SNG;
}

defdbl()
{
	setdef(DBL);
}

defsng()
{
	setdef(SNG);
}

defint()
{
	setdef(INT);
}

defstr()
{
	setdef(STR);
}

setdef(type)
int	type;
{
	int	i,
		begin,
		end;

	skip00(&expr);
	while (1)
	{
		if (!ALPHA(*expr))
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
		else
			begin = *expr - 'a';
		expr++;
		skip00(&expr);
		if (*expr == '-')
		{
			expr++;
			skip00(&expr);
			if (!ALPHA(*expr))
			{
				error(inst.thing.linno,inst.thing.stmtno, 21);
				return(-1);
			}
			else
				end = *expr - 'a';
			expr++;
			skip00(&expr);
		}
		else
			end = begin;

		for (i = begin; i <= end; i++)
			deftype[i] = type;

		if (*expr == '\0')
			break;
		else if (*expr == ',')
		{
			expr++;
			skip00(&expr);
		}
		else
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
	}
	return(0);
}


/*	these functions called by lex to determine default type for a name */
defscalar(letter)
char	letter;
{
	if (letter >= 'a' && letter <= 'z')
		switch(deftype[letter - 'a'])
		{
		case SNG:	return(SNGID);
		case INT:	return(INTID);
		case DBL:	return(DBLID);
		case STR:	return(STRID);
		}
	return(TOKERR);
}


defarray(letter)
char	letter;
{
	if (letter >= 'a' && letter <= 'z')
		switch(deftype[letter - 'a'])
		{
		case SNG:	return(SNGAID);
		case INT:	return(INTAID);
		case DBL:	return(DBLAID);
		case STR:	return(STRAID);
		}
	return(TOKERR);
}


defudef(letter)
char	letter;
{
	if (letter >= 'a' && letter <= 'z')
		switch(deftype[letter - 'a'])
		{
		case SNG:	return(SNGUID);
		case INT:	return(INTUID);
		case DBL:	return(DBLUID);
		case STR:	return(STRUID);
		}
	return(TOKERR);
}
