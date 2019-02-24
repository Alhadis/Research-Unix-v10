/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// avars.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:11:02 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * Process ARRAY (subscripted) VARABLES
 * STORAGE & FETCH ROUTINES
 */
/*   "@(#) avars.c:  V 1.2  3/4/81" */
#include	"bas.h"
char * asymtab[26][11];	/* symbol table */
/*
 * STORE ARRAY VARIABLE
 */
aputvar(vstr, value)
char	vstr[];
double	value;
{
	char 	*addr;	/* address of data */
	char	*putflot();

	if(getaddr(vstr, &addr) < 0)
		return(-1);

	putflot(value, addr);	/* store the number in the array location */
	return(0);
}

/*
 * FETCH ARRAY VARIABLE
 */
agetvar(vstr, value)
char	vstr[];
double	*value;
{
	char *	addr;
	char	*getflot();

	if(getaddr(vstr, &addr) < 0)
		return(-1);

	getflot(value, addr);
	return(0);
}
/*
 *
 * ////// CALCULATE ARRAY VARIABLE ADDRESS //////
 *
 *
 */
getaddr(vstr, addr)
char	vstr[];		/* subscripted variable string */
char	**addr;		/* addres of subscripted variable */
{
	char	arname[3];	/* array name */
	char	*ptr;		/* location pointer */
	char	*getflot(),*putflot();
	int	offset;		/* offset to data point */
	int	asize;		/* actual number of dims stored */
	double	dasize;		/* actual number of dims stored */
	double	rsize;		/* number of dims in request */
	double	adims[MAXDIM];	/* actual dims */
	double	rdims[MAXDIM];	/* requested dims */
	double	acons[MAXDIM-1];	/* actual constants */
	register i;		/* counter */
	int	j,k;
	int	amax;		/* integer form of size */

	/*
	 * GET REQUESTED PARAMS
	 */
	if(getdims(vstr, arname, &rsize, rdims) < 0)
		return(-1);	/* error in spec */

	j = arname[0] - 'a';
	/*
	 * compute k subscript
	 */
	if(arname[1] == '\0')
		k = 0;
	else
		k = arname [1] - '0' + 1;

	if(asymtab[j][k] == 0)	/* If not dimensioned, complain */
	{
		error(inst.thing.linno, 25); /* NOT DIMENSIONED */
		return(-1);
	}

	ptr = asymtab[j][k];	/* get pointer to header */
	ptr = getflot(&dasize, ptr);	/* get actual size */

	if(dasize != rsize)	/* error if mismatch in actual to
					requested size */
	{
		error(inst.thing.linno, 26); /* WRONG NUMBER OF DIMS */
		return(-1);
	}
	amax = asize = dasize;		/* convert size to integer */
	for(i=0; i<amax; i++)
	{
		ptr = getflot(&adims[i], ptr);
		if((rdims[i] ) > adims[i]) /* make spec start with 1 */
		{
			error(inst.thing.linno, 27); /* DIM LARGER THAN ACTUAL */
			return(-1);
		}
		rdims[i] -= 1;	/* actual number used starts at 0 */
	}
	/*
	 * GET CONSTANTS
	 */
	for(i=0; i<(amax-1); i++)
		ptr = getflot(&acons[i], ptr);
	acons[i] = 0.0;	/* null terminate */

	/*
	 * CALCULATE BYTE OFFSET IN A POSITIVE SENSE
	 */
	for(i=0, offset = 0; acons[i] != 0.0; i++)
	{
		offset += acons[i] * rdims[i];
	}
	offset += rdims[i] + 2 * asize;
	offset *= PREC;
	*addr = asymtab[j][k] - offset;
	return(0);
}
