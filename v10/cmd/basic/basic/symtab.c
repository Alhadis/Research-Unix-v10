/*	"@(#) symtab.c: V 1.23 8/19/83"	*/

/*
*	symtab.c:  symbol table managing routines
*/

#include	<stdio.h>
#include  	"bas.h"
#include	"tokens.h"

extern short	base,
		dimseen,
		opbaset;

char 	*hicore,
	*strget();

struct  sym	*htab[HTSIZE];		/*hash table*/

struct sym	*paramptr = NULL;	/*points to parameter of currently
					  executing user-defined function */
void		free();



/*
*	search parameter list and return pointer if object found
*/

struct sym *
paramsrch(vstr, type)
char		*vstr;
short		type;
{
	struct sym	*cptr;

	cptr = paramptr;
	while (cptr != NULL)
		if ((strcmp(cptr->name, vstr) == 0) && (cptr->symtype == type))
			return(cptr);
		else
			cptr = cptr->next;
	return(NULL);
}



/*
*	search symbol table and return pointer if object found
*/

struct sym *
stsrch(vstr, type)
char		*vstr;
short		type;
{
	struct sym	*cptr;

	if ((cptr = paramsrch(vstr, type)) != NULL)
		return(cptr);
	cptr = htab[hash(vstr)];
	while (cptr != NULL)
		if ((strcmp(cptr->name, vstr) == 0) && (cptr->symtype == type))
			return(cptr);
		else
			cptr = cptr->next;
	return(NULL);
}



/*
*	put object in symbol table and return pointer
*/

struct sym *
stinsert(vstr, type)
char	*vstr;
short   type;
{
	struct sym 	*currec;
	int		index;

	if ((currec = (struct sym *) malloc(sizeof(struct sym))) == NULL)
	{
		error(inst.thing.linno,inst.thing.stmtno, 84);
		return(NULL);
	}
	index = hash(vstr);
	if ((currec->name = strget(vstr)) == NULL)
	{
		free(currec);
		return(NULL);
	}
	switch (currec->symtype = type)
	{
	case DBLAID:	currec->data.darr.arblok = NULL;
			currec->data.darr.header = NULL;
			break;
	case SNGAID:	currec->data.farr.arblok = NULL;
			currec->data.farr.header = NULL;
			break;
	case INTAID:	currec->data.iarr.arblok = NULL;
			currec->data.iarr.header = NULL;
			break;
	case STRAID:	currec->data.sarr.arblok = NULL;
			currec->data.sarr.header = NULL;
			break;
	}
	currec->scratch = 0;
	currec->isbufvar = 0;
	currec->next = htab[index];
	htab[index] = currec;
	return(currec);
}



/*	functions used by for() for loop variable access	*/
double
lpvarval(ptr)
struct sym	*ptr;
{
	double	dtmp;
	float	ftmp;
	int	itmp,
		i;
	char	*to,
		*from;

	switch(ptr->symtype)
	{
	case SNGID:	if (ptr->isbufvar)
			{
				if (getfloat(ptr->data.bufvar.start, &ftmp) < 0)
					return(0);
				return(ftmp);
			} else
				return(ptr->data.fval);
	case INTID:	if (ptr->isbufvar)
			{
				to = (char *) & itmp;
				from = ptr->data.bufvar.start;
				for (i=0; i<ptr->data.bufvar.length; i++)
					*to++ = *from++;
				return(itmp);
			} else
				return(ptr->data.ival);
	case DBLID:	if (ptr->isbufvar)
			{
				if (getdouble(ptr->data.bufvar.start,&dtmp) < 0)
					return(0);
				return(dtmp);
			} else
				return(ptr->data.dval);
	}
	return(0);
}



lpvarset(ptr, value)
struct sym	*ptr;
double		value;
{
	double  dtmp;
	float	ftmp;
	int	itmp,
		i;
	char	*to,
		*from;

	switch(ptr->symtype)
	{
	case SNGID:	if (ptr->isbufvar)
			{
				ftmp = mkfloat(value);
				from = (char *) & ftmp;
				to = ptr->data.bufvar.start;
				for (i=0; i<ptr->data.bufvar.length; i++)
					*to++ = *from++;
			} else
				ptr->data.fval = mkfloat(value);
			break;
	case INTID:	if (ptr->isbufvar)
			{
				itmp = mkint(value);
				from = (char *) & itmp;
				to = ptr->data.bufvar.start;
				for (i=0; i<ptr->data.bufvar.length; i++)
					*to++ = *from++;
			} else
				ptr->data.ival = mkint(value);
			break;
	case DBLID:	if (ptr->isbufvar)
			{
				dtmp = value;
				from = (char *) & dtmp;
				to = ptr->data.bufvar.start;
				for (i=0; i<ptr->data.bufvar.length; i++)
					*to++ = *from++;
			} else
				ptr->data.dval = value;
			break;
	}
}


samevar(ptr, name, type)
struct sym	*ptr;
char		name[];
int		type;
{
	if (ptr == NULL)
		return(0);
	if (!strcmp(name, ptr->name) && type == ptr->symtype)
		return(1);
	return(0);
}



/*
*	put argument in symbol table and return index + 1,
*	do not make entry in hash table. return 0 if insert failed
*/

struct sym	*
arginsert(vstr, type)
char	*vstr;
short   type;
{
	struct sym 	*currec;

	if ((currec = (struct sym *) malloc(sizeof(struct sym))) == NULL)
	{
		error(inst.thing.linno,inst.thing.stmtno, 84);
		return(NULL);
	}
	if ((currec->name = strget(vstr)) == NULL)
	{
		free(currec);
		return(NULL);
	}
	currec->symtype = type;
	if (type == STRID)
		currec->data.string = NULL;
	currec->scratch = 0;
	currec->isbufvar = 0;
	currec->next = NULL;
	return(currec);
}



/*
*	hash function for symbol table searches and inserts
*/

hash(vstr)
char *vstr;
{
	unsigned long num;
	int  i;
	char *ptr;

	ptr = vstr;
	num = 0;
	i = 0;
	while ((*ptr) && (i < IDSIZE))
	{
		num = 256*num + (*ptr);
		ptr++;
		i++;
	}
	if (*ptr)
		num += 4096*(*ptr);
	else
		num += 4096*(*(ptr-1));
	return (num % HTSIZE);
}



/*
*	free symbol table record and return 1 if scratch not set
*	otherwise do nothing and return 0.
*/
recremove(recptrptr)
struct sym	**recptrptr;
{
	struct sym	*recptr;
	register	j;
	int		arrsize;

	recptr = *recptrptr;
	if (recptr->scratch) 			/* symbol marked for saving */
	{
		recptr->scratch = 0;
		return(0);
	}
	*recptrptr = recptr->next;
	if (recptr->name != NULL)
		strfree(recptr->name);
	switch (recptr->symtype)
	{
	case STRID :	if (recptr->data.string != NULL && !recptr->isbufvar)
				strfree(recptr->data.string);
			break;
	case DBLAID : 	if (recptr->data.darr.arblok != NULL)
				free(recptr->data.darr.arblok);
			if (recptr->data.darr.header != NULL)
				free(recptr->data.darr.header);
		       	break;
	case SNGAID : 	if (recptr->data.farr.arblok != NULL)
				free(recptr->data.farr.arblok);
			if (recptr->data.farr.header != NULL)
				free(recptr->data.farr.header);
		       	break;
	case INTAID : 	if (recptr->data.iarr.arblok != NULL)
				free(recptr->data.iarr.arblok);
			if (recptr->data.iarr.header != NULL)
				free(recptr->data.iarr.header);
		       	break;
	case STRAID : 	if (recptr->data.sarr.arblok != NULL)
			{
				arrsize = numelems(recptr->data.sarr.header);
				for (j = 0; j < arrsize; j++)
					strfree(*(recptr->data.sarr.arblok+j));
				free(recptr->data.sarr.arblok);
			}
			if (recptr->data.sarr.header != NULL)
				free(recptr->data.sarr.header);
		       	break;
	case DBLUID :
	case SNGUID :
	case INTUID :
	case STRUID :
			if (recptr->data.udef.funcexp != NULL)
				strfree(recptr->data.udef.funcexp);
			while (recptr->data.udef.arg != NULL)
				recremove(&(recptr->data.udef.arg));
			break;
	}
	free(recptr);
	return(1);
}



/*
*	remove record with given key from symbol and free up space
*/
stremove(name, type)
char	name[];
short	type;
{
	struct sym	**cptrptr;

	cptrptr = &htab[hash(name)];
	while (*cptrptr != NULL)
		if (!strcmp(name, (*cptrptr)->name) 
		    && type == (*cptrptr)->symtype)
		{
			recremove(cptrptr);
			return(1);
		} else
			cptrptr = &((*cptrptr)->next);
	return(0);
}



/*
*	initialize symbol table and data areas,
*	remove existing variables & insert predefined functions
*/
initvar()
{
	register	 i;
	extern double	lastrand;

	lastrand = 0;
	base = 0;
	dimseen = 0;
	opbaset = 0;
	paramptr = NULL;
	definit();		/* initialize default type array */
	for (i = 0; i < HTSIZE; i++)
		while (htab[i] != NULL)
			recremove(&htab[i]);
	freeinit();		/* initialize string space */
	hicore = &txtbuf[PGSIZ * NMPAG + ENCLINMAX];
}



/*
*	traverse symbol table performing func() on each node.  func has the form
*
*		int
*		func(recptr)
*		struct sym	*recptr;
*/
traverse(func)
int	(*func)();
{
	register	i;
	struct sym	*recptr;

	for (i=0; i<HTSIZE; i++)
	{
		recptr = htab[i];
		while (recptr != NULL)
		{
			(*func)(recptr);
			recptr = recptr->next;
		}
	}
}



/*
*	CLEAR statement handler - zeroes out all data
*/
clearproc()
{
	initvar();
}


/*
*	dump a symbol table record
*/
dmpsym(recptr)
struct sym	*recptr;
{
	struct sym	*argptr;

	printf("%s", recptr->name);
	switch(recptr->symtype)
	{
	case EXTFUNC:
			printf(" - external subroutine at 0x%x",recptr->data.faddr);
			printf("   scratch = %d\n", recptr->scratch);
			break;
	case DBLID:
			if (recptr->isbufvar)
			{
				int	i;
				double	val;
				char	*to, *from;

				to = (char *) & val;
				from = recptr->data.bufvar.start;
				for (i=0; i < recptr->data.bufvar.length; i++)
					*to++ = *from++;
				printf("# = %g in field", val);
				printf(" size %d",recptr->data.bufvar.length);
			} else
				printf("# = %g", recptr->data.dval);
			printf("   scratch = %d\n", recptr->scratch);
			break;
	case DBLAID:
			if (recptr->data.darr.header != NULL)
				printf("#(%d)", numelems(recptr->data.darr.header));
			else
				printf("#(0)");
			printf("   scratch = %d\n", recptr->scratch);
			break;
	case DBLUID:
			printf("#() = %s", recptr->data.udef.funcexp);
			printf("   scratch = %d\n", recptr->scratch);
			argptr = recptr->data.udef.arg;
			while (argptr != NULL)
			{
				printf("\t");
				dmpsym(argptr);
				argptr = argptr->next;
			}
			break;
	case SNGID:
			if (recptr->isbufvar)
			{
				int	i;
				float	val;
				char	*to, *from;

				to = (char *) & val;
				from = recptr->data.bufvar.start;
				for (i=0; i < recptr->data.bufvar.length; i++)
					*to++ = *from++;
				printf("! = %g in field", val);
				printf(" size %d",recptr->data.bufvar.length);
			} else
				printf("! = %g", recptr->data.fval);
			printf("   scratch = %d\n", recptr->scratch);
			break;
	case SNGAID:
			if (recptr->data.farr.header != NULL)
				printf("!(%d)", numelems(recptr->data.farr.header));
			else
				printf("!(0)");
			printf("   scratch = %d\n", recptr->scratch);
			break;
	case SNGUID:
			printf("!() = %s", recptr->data.udef.funcexp);
			printf("   scratch = %d\n", recptr->scratch);
			argptr = recptr->data.udef.arg;
			while (argptr != NULL)
			{
				printf("\t");
				dmpsym(argptr);
				argptr = argptr->next;
			}
			break;
	case INTID:
			if (recptr->isbufvar)
			{
				int	i;
				int	val;
				char	*to, *from;

				to = (char *) & val;
				from = recptr->data.bufvar.start;
				for (i=0; i < recptr->data.bufvar.length; i++)
					*to++ = *from++;
				printf("%% = %d in size", val);
				printf(" size %d",recptr->data.bufvar.length);
			} else
				printf("%% = %d", recptr->data.ival);
			printf("   scratch = %d\n", recptr->scratch);
			break;
	case INTAID:
			if (recptr->data.iarr.header != NULL)
				printf("%%(%d)", numelems(recptr->data.iarr.header));
			else
				printf("%%(0)");
			printf("   scratch = %d\n", recptr->scratch);
			break;
	case INTUID:
			printf("%%() = %s", recptr->data.udef.funcexp);
			printf("   scratch = %d\n", recptr->scratch);
			argptr = recptr->data.udef.arg;
			while (argptr != NULL)
			{
				printf("\t");
				dmpsym(argptr);
				argptr = argptr->next;
			}
			break;
	case STRID:
			if (recptr->isbufvar)
			{
				char	*buf[STRMAX+1];

				strncpy(buf, recptr->data.bufvar.start,
					     recptr->data.bufvar.length);
				buf[recptr->data.bufvar.length] = '\0';
				printf("$ = %s in field", buf);
				printf(" size %d",recptr->data.bufvar.length);
			} else
				printf("$ = %s", recptr->data.string);
			printf("   scratch = %d\n", recptr->scratch);
			break;
	case STRAID:
			if (recptr->data.sarr.header != NULL)
				printf("$(%d)", numelems(recptr->data.sarr.header));
			else
				printf("$(0)");
			printf("   scratch = %d\n", recptr->scratch);
			break;
	case STRUID:
			printf("$() = %s", recptr->data.udef.funcexp);
			printf("   scratch = %d\n", recptr->scratch);
			argptr = recptr->data.udef.arg;
			while (argptr != NULL)
			{
				printf("\t");
				dmpsym(argptr);
				argptr = argptr->next;
			}
			break;
	}
}



/*
*	dump symbol table
*/
dmpsymtab(msg)
char	*msg;
{
	printf("Symbol Table Dump %s\n", msg);
	traverse(dmpsym);
}


unmark(recptr)
struct sym	*recptr;
{
	recptr->scratch = 0;
}


/*	mark buffer variables	*/
marknonbuf(recptr)
struct sym	*recptr;
{
	if (recptr->isbufvar)
		recptr->scratch = 0;
	else
		recptr->scratch = 1;
}


/*	mark user-defined functions	*/
markfunc(recptr)
struct sym	*recptr;
{
	switch(recptr->symtype)
	{
	case DBLUID:
	case SNGUID:
	case INTUID:
	case STRUID:
	case EXTFUNC:
		recptr->scratch = 1;
		break;
	default:
		recptr->scratch = 0;
		break;
	}
}


/*	remove all but the marked symbol table records	*/
savmarked()
{
	struct sym	**cptrptr;
	int i;

	for (i=0; i<HTSIZE; i++)
	{
		cptrptr = &htab[i];
		while (*cptrptr != NULL)
			if (!recremove(cptrptr))
				cptrptr = &((*cptrptr)->next);
	}
}


/*
*	processes COMMON statement:  marks variables to be saved
*/
_common()
{
	struct sym	*cptr;
	int		type;
	char		tokbuf[LINMAX+1];

	do {
		switch(type = lex(&expr, tokbuf))
		{
		case DBLID:
		case SNGID:
		case INTID:
		case STRID:
			if ((cptr = stsrch(tokbuf, type)) != NULL)
				cptr->scratch = 1;
			break;
		case DBLAID:
		case SNGAID:
		case INTAID:
		case STRAID:
			if ((cptr = stsrch(tokbuf, type)) != NULL)
				cptr->scratch = 1;
			else
			{
				traverse(unmark);
				error(inst.thing.linno,inst.thing.stmtno, 11);
				return(-1);
			}
			if (*(++expr) != ')')
			{
				traverse(unmark);
				error(inst.thing.linno,inst.thing.stmtno, 21);
				return(-1);
			}
			expr++;
			break;
		default:
			traverse(unmark);
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
	} while ((type = lex(&expr, tokbuf)) == COMMA);
	if (type != 0)
	{
		traverse(unmark);
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	return(0);
}


/*
*	processes ERASE statement
*/
erase()
{
	char		tokbuf[LINMAX+1];
	int		type;

	do {
		switch(lex(&expr, tokbuf))
		{
		case DBLID:
			if (!stremove(tokbuf, DBLAID))
			{
				error(inst.thing.linno,inst.thing.stmtno, 11);
				return(-1);
			}
			break;
		case SNGID:
			if (!stremove(tokbuf, SNGAID))
			{
				error(inst.thing.linno,inst.thing.stmtno, 11);
				return(-1);
			}
			break;
		case INTID:
			if (!stremove(tokbuf, INTAID))
			{
				error(inst.thing.linno,inst.thing.stmtno, 11);
				return(-1);
			}
			break;
		case STRID:
			if (!stremove(tokbuf, STRAID))
			{
				error(inst.thing.linno,inst.thing.stmtno, 11);
				return(-1);
			}
			break;
		default:
			error(inst.thing.linno,inst.thing.stmtno, 21);
			return(-1);
		}
	} while ((type = lex(&expr, tokbuf)) == COMMA);
	if (type != 0)
	{
		error(inst.thing.linno,inst.thing.stmtno, 21);
		return(-1);
	}
	return(0);
}
