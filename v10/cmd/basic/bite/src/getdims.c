/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// getdims.c /////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 06:16:54 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * GET ARRAY PARAMETERS (used by "dim", aputvar and agetvar)
 * Pass the array symbol name, the number of dimensions , and
 * the value of each dimension in dimlist where dimlist is an array
 * of floats. The values returned are integerized.
 *
 * This routine is used by "dim()", a_getvar(), a_putvar()
 *
 * This routine could potentially be used for device driver routines
 * as a means of getting number of args and the value of each.
 */
/*   "@(#) getdims.c:  V 1.3  3/4/81" */
#include	"bas.h"

getdims(afield,arname,numdim,dimlist)
char	afield[];	/* string to be scanned */
char	arname[];	/* array name */
double	*numdim;	/* number of dimensions */
double	dimlist[];	/* list of dimensions */
{
	char	expfld[40];	/* expression field */
	double	evalx();
	double	_int();
	double	maxdim;
	int	intdim;		/* integer form of number of dimensions */

	register char *ptr;
	register	i;
	ptr  = afield;	/* get pointer to command argument string */
	maxdim = MAXDIM;	/* convert maxdim to double */
	i = 0;
	while(*ptr != '(' && *ptr != '$' && *ptr != '\0' && i != 2)
	{
		arname[i++] = *ptr++;	/* copy array symbol name */
	}

	arname[i] = '\0';	/* null terminate array name */

	*ptr++;		/* bump past '(' */
	/*
	 * While not at end of string ('\0')
	 *
	 * Each field is terminated by a ',' except the last which
	 * is terminated by a ')' immediatly followed by a '\0'.
	 *
	 * CALCULATE NUMBER AND VALUE OF EACH DIMENSION
	 */
	*numdim = 0.0;
	i = 0;

	while(1)		/* scan fields */
	{
		expfld[i++] = *ptr++;	/* copy expression */
		if(*ptr == ',' || (*ptr == ')' && *(ptr+1) == '\0'))
		{
			*ptr++;		/* bump past field terminator */
			expfld[i] = '\0'; /* null terminate expr */
			i = 0;	/* init expr field pointer */
			/*
			 * Evaluate expression and put as dimension
			 */
			intdim = *numdim;
			dimlist[intdim] = _int(evalx(expfld));
			if(dimlist[intdim] <= 0.0) 
			{
				error(inst.thing.linno, 28); /* NEG DIM */
				return(-1);
			}
			*numdim += 1.0;	/* increment dimension count */
			if(*numdim > maxdim)
			{
				error(inst.thing.linno, 22); /* TOO MANY DIMS */
				return(-1);
			}
		}
		if(*ptr == '\0') break; /* end of dimension statment */
	}
	intdim++;	/* bump past last dimension */
	dimlist[intdim] = 0.0;	/* null terminate end of list */
	return(0);
}
