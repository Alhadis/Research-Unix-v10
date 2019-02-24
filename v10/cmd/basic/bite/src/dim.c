/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// dim.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:24:13 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 * Array allocator for
 *	BITE
 * Reference: Donald.E.Knuth
 *		"Fundamental Algorithms"
 *		Vol. 1 PP 296,298
 *
 * Storage is allocated so that:
 *
 * LOC(ARRAY[I,J,K,L]) = BASEADDR + a1*I + a2*J + a3*K + L
 *
 * The constants a1,a2,a3,a4 ... ak-1 are calculated at dimension time
 * and stored as part of the header in the array data.
 * The storage of these constants could be saved by doing a calculation
 * every time an array element is referenced. The tradeoff is access
 * speed versus some core real estate.
 *
 *		+---------------+	BEGINNING OF ALLOCATED SPACE
 *		|		|	NUMBER OF DIMENSIONS
 *		+---------------+
 *		|	I1	|	DIMENSION 1
 *		+---------------+
 *		|	I2	|	DIMENSION 2
 *		+---------------+
 *		|	I3	|	DIMENSION 3
 *		+---------------+
 *		| ............  |
 *		+---------------+
 *		|	Ik	|	DIMENSION k
 *		+---------------+
 *		|	a1	|	Constants a1 .... ak-1
 *		+---------------+
 *		| ............	|
 *		+---------------+
 *		|	ak-1	|
 *		+---------------+
 *		|		|	BEGINNING OF DATA AREA
 *		+---------------+
 *		|		|
 *		~~		~~
 *		|		|
 *		+---------------+
 *		|		|
 *		+---------------+
 */
/*   "@(#) dim.c:  V 1.4  3/4/81" */
/*
 *
 * ////// DIMENSION STATEMENT //////
 */
/*   "@(#) dim.c:  V 1.4  3/4/81" */
#include	"bas.h"
extern	char	*hicore;
extern	int	_comflg;	/* common flag */
char *	asymtab[26][11];	/* array name symbol table */
#ifdef	STRINGS
char *	sasymtab[26][11];	/* string array name symbol table */
#endif

dim()
{
	char	*clptr;	/* temp pointer used for class */
	char	expfld[40];	/* expression field */
	int	type;

	clptr = expr;
	type = class(&clptr,expfld);
	switch(type)
	{
	case VACLASS:
		return(ndim());
		break;
#ifdef	STRINGS
	case SACLASS:
		return(sdim());
		break;
#endif
	default:
		error(inst.thing.linno, 21);	/* BAD DIMENSION SYNTAX */
		return(-1);
	}
	return 0;
}
#ifdef	STRINGS
/*
 * String dimension
 */
sdim()
{
	char	arname[3];	/* array symbol name */
	double	dimlist[MAXDIM];	 /* dimension list */
	double	numdim;		/* number of dimensions */
	int	totsiz;		/* space to be allocated  */
	unsigned int bytsiz;	/* same as totsiz in int form */
	register int	*iptr;
	int	i;

	int	j,k;	/* symbol table indicies */


	/*
	 * GET ARRAY SYMBOL NAME, NUMBER IF DIMENSIONS &
	 * PUT THE VALUE OF EACH DIMENSION IN DIMLIST
	 */
	if(getdims(expr,arname,&numdim,dimlist) < 0)
		return(-1);
	if(numdim == 1.0)		/* if one dim specified */
	{
		dimlist[1] = 1.0;	/* it's an N X 1 array */
	}
	j = arname[0] - 'a';	/* compute j subscript */

	/*
	 * compute k subscript
	 */
	if(arname[1] == '\0')	/* if no numeric part */
		k = 0;		/* then k = 0th column */
	else
		k = arname[1] - '0' + 1;

	/*
	 * SET SYMBOL TABLE POINTER
	 */
	if(sasymtab[j][k] == 0)
	{
		alloc(&sasymtab[j][k],sizeof(int));
	}
	else
	{
		if(_comflg)	/* if vars commoned */
			return(0);	/* just no_op */
		error(inst.thing.linno, 23);	/* REDUNDANT DIM STATEMENT */
		return(-1);
	}
	/*
	 * CALCULATE MEMORY NEEDED FOR DATA
	 */
		totsiz = (int)(dimlist[0] * dimlist[1]);

	totsiz += 2;	/* add space for header */

	totsiz = sizeof(int) * totsiz; /* convert to size in bytes */
	/*
	 * IF TOTSIZ IS GREATER THAN HIGHEST POS 16 BIT INTEGER,
	 * FORCE IT TO EQUAL THAT MAX SO THAT SIZE CHECK FAILS.
	 * OTHERWISE BYTSIZ WILL CONVERT TO 0 AND APPEAR TO BE
	 * LEGALLY SMALL.
	 */
	if(totsiz > 32767) totsiz = 32767;
	bytsiz = totsiz;	/* convert size to integer */
	/*
	 * Return error if not enough space left
	 */
	if((hicore - bytsiz) <= linptr)
	{
		error(inst.thing.linno, 24);	/* NOT ENOUGH ADDITIONAL CORE */
		return(-1);
	}
	i = (int)hicore - (int)bytsiz;
	while((unsigned)hicore > i)
	{
		*hicore = '\0';	/* zero each location in array */
		hicore -= 1;
	}

	iptr = (int *)sasymtab[j][k];	/* get pointer to header */
	*--iptr = (int)numdim;		/* put in number of dims */
	*--iptr = (int)dimlist[0];	/* put in XMAX */
	*--iptr = (int)dimlist[1];	/* put in YMAX */
	return(0);
}
#endif
/*
 * Numerical dimension
 */
ndim()
{
	char	*putflot(),*getflot();
	char	arname[3];	/* array symbol name */
	double	conlist[MAXDIM]; /* list of constants a1 -- ak-1 */
	double	dimlist[MAXDIM]; /* dimension list */
	double	numdim;		/* number of dimensions */
	double	numcon;		/* number of constants */
	double	totsiz;		/* space to be allocated in float */
	unsigned int bytsiz;	/* same as totsiz in int form */

	register char *ptr;
	register	i;
	register	x;
	int	j,k;	/* symbol table indicies */

	ptr = expr;	/* get pointer to command argument string */

	/*
	 * GET ARRAY SYMBOL NAME, NUMBER IF DIMENSIONS &
	 * PUT THE VALUE OF EACH DIMENSION IN DIMLIST
	 */
	if(getdims(expr,arname,&numdim,dimlist) < 0)
		return(-1);

	/*
	 * CALCULATE CONSTANTS a1....ak-1
	 * and PUT THEM IN CONLIST
	 */
	for(x=1; dimlist[x] != 0.0; x++)
	{
		conlist[x-1] = 1;	/* init this entry */
		for(i=x; dimlist[i] != 0.0; i++)
		{
			conlist[x-1] *= (dimlist[i] );
		}
	}
	conlist[x-1] = 0.0;
	numcon = x-1;	/* number of constants */
	j = arname[0] - 'a';	/* compute j subscript */

	/*
	 * compute k subscript
	 */
	if(arname[1] == '\0')	/* if no numeric part */
		k = 0;		/* then k = 0th column */
	else
		k = arname[1] - '0' + 1;

	/*
	 * SET SYMBOL TABLE POINTER
	 */
	if(asymtab[j][k] == 0)
		alloc(&asymtab[j][k],PREC);
	else
	{
		if(_comflg)	/* if vars commoned */
			return(0);	/* just no_op */
		error(inst.thing.linno, 23);	/* REDUNDANT DIM STATEMENT */
		return(-1);
	}

	totsiz = 1.0;
	/*
	 * CALCULATE MEMORY NEEDED FOR DATA
	 */
	for(i=0; dimlist[i] != 0.0; i++)
	{
		totsiz *= dimlist[i];
	}

	totsiz += numdim + numcon;	/* add space needed for dimensions
					     and constants */

	totsiz = PREC * totsiz; /* convert to size in bytes */
	/*
	 * IF TOTSIZ IS GREATER THAN HIGHEST POS 16 BIT INTEGER,
	 * FORCE IT TO EQUAL THAT MAX SO THAT SIZE CHECK FAILS.
	 * OTHERWISE BYTSIZ WILL CONVERT TO 0 AND APPEAR TO BE
	 * LEGALLY SMALL.
	 */
	if(totsiz > 32767.0) totsiz = 32767.0;
	bytsiz = totsiz;	/* convert size to integer */
	/*
	 * Return error if not enough space left
	 */
	if((hicore - bytsiz) <= linptr)
	{
		error(inst.thing.linno, 24);	/* NOT ENOUGH ADDITIONAL CORE */
		return(-1);
	}
	i = (int)hicore - (int)bytsiz;
	while((unsigned)hicore > i)
	{
		*hicore = '\0';	/* zero each location in array */
		hicore -= 1;
	}

	ptr = asymtab[j][k];	/* get pointer to header */
	/*
	 * PUT NUMBER OF DIMENSIONS IN HEADER
	 */
	ptr = putflot(numdim, ptr);

	/*
	 * PUT DIMENSIONS IN HEADER
	 */
	for(i=0; dimlist[i] != 0.0; i++)
		ptr = putflot(dimlist[i], ptr);

	/*
	 * PUT CONSTANT LIST IN HEADER
	 */
	for(i=0; conlist[i] != 0.0; i++)
		ptr = putflot(conlist[i], ptr);
	return(0);
}
/*
 *
 * ////// PUTFLOT //////
 *
 * Copy floating point number into allocated memory
 * return new pointer value
 */
char	*
putflot(value,ptr)
double	value;
char	*ptr;
{
	register char *p;	/* register pointer */
	register	i;	/* regster index */

	p = ptr;	/* set reister to address */

	varbyts.var = value;	/* stuff float into byte template */

	/*
	 * copy float number into allocated space
	 * byte by byte from top down
	 */
	for(i=PREC-1; i>=0; --i)
		*--p = varbyts.var4th[i];

	return(p);	/* return new pointer value */
}

/*
 * ////// GETFLOT //////
 *
 * Copy floating point number OUT of allocated memory
 * return the floating point value in *valptr
 * return the next pointer down
 */
char	*
getflot(valptr,ptr)
double	*valptr;
char	*ptr;
{
	register char *p;	/* register pointer */
	register	i;	/* register index */

	p = ptr;	/* set register to address */

	for(i=PREC-1; i>=0; --i)
		varbyts.var4th[i] = *--p;

	*valptr = varbyts.var;	/* return the value to calling routine */

	return(p);	/* return new pointer */
}
