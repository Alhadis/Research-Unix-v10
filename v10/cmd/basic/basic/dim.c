/*	"@(#) dim.c: V 2.16 6/14/83"	*/

/*
*  dim.c:	routines for dimensioning arrays
*/

#include <stdio.h>
#include <values.h>
#include "bas.h"
#include "tokens.h"

extern struct sym	*stsrch(),
			*stinsert();

char		*strget(),
		*calloc();

short	base,
	dimseen;

short 	*makeheader();


/*
*	dimension an array
*/

dim()
{
	char	arname[IDSIZE + 1];
	short	numdim,
		dimlist[MAXDIM];
	char	*clptr;
	int	type;

	clptr = expr;
	while (1)
	{
		type = getinds(&clptr, arname, &numdim, dimlist);
		if (type < 0 || fnpref(arname))
		{
			error(inst.thing.linno,inst.thing.stmtno, 21);	/* bad dim syntax */
			return(-1);
		}
		switch (type)
		{
		case DBLAID:
			if (ddim(arname, numdim, dimlist))
				return(-1);
			break;
		case SNGAID:
			if (fdim(arname, numdim, dimlist))
				return(-1);
			break;
		case STRAID:
			if (sdim(arname, numdim, dimlist))
				return(-1);
			break;
		case INTAID:
			if (idim(arname, numdim, dimlist))
				return(-1);
			break;
		}
		skip00(&clptr);
		if (*clptr == ',')
			clptr++;
		else if (*clptr == '\0')
			return(0);
		else
		{
			error(inst.thing.linno,inst.thing.stmtno, 21); /*bad DIM syntax*/
			return(-1);
		}
	}
}


ddim(arname, numdim, dimlist)
char	arname[IDSIZE + 1];
short	numdim,
	dimlist[MAXDIM];
{
	long	i;
	long	size;
	struct sym 	*currec;

	dimseen = 1;
	if ((currec = stsrch(arname, DBLAID)) != NULL) /*already declared*/
	{
		error(inst.thing.linno,inst.thing.stmtno, 23);	/* Redundant DIM stmt. */
		return(-1);
	}

	if ((currec = stinsert(arname, DBLAID)) == NULL)
		return(-1);

	if ((currec->data.darr.header 
		= makeheader(numdim, dimlist, &size)) == NULL)
	{
		stremove(arname, DBLAID);
		return(-1);
	}
	if ((currec->data.darr.arblok = (double *) calloc((unsigned) size, (unsigned) sizeof(double)))
					== NULL)    /* not enough space for */
	{			       		    /* array elements       */
		stremove(arname, DBLAID);
		error(inst.thing.linno,inst.thing.stmtno, 84);	
		return(-1);
	}

	for (i=0; i < size; i++)
		*(currec->data.darr.arblok + i) = 0.0;
	return(0);
}


fdim(arname, numdim, dimlist)
char	arname[IDSIZE + 1];
short	numdim,
	dimlist[MAXDIM];
{
	long	i;
	long	size;
	struct sym 	*currec;

	dimseen = 1;
	if ((currec = stsrch(arname, SNGAID)) != NULL) /*already declared*/
	{
		error(inst.thing.linno,inst.thing.stmtno, 23);	/* Redundant DIM stmt. */
		return(-1);
	}

	if ((currec = stinsert(arname, SNGAID)) == NULL)
		return(-1);

	if ((currec->data.farr.header 
		= makeheader(numdim, dimlist, &size)) == NULL)
	{
		stremove(arname, SNGAID);
		return(-1);
	}
	if ((currec->data.farr.arblok = (float *) calloc((unsigned) size, (unsigned) sizeof(float)))
					== NULL)    /* not enough space for */
	{			       		    /* array elements       */
		stremove(arname, SNGAID);
		error(inst.thing.linno,inst.thing.stmtno, 84);	
		return(-1);
	}

	for (i=0; i < size; i++)
		*(currec->data.farr.arblok + i) = 0.0;
	return(0);
}


sdim(arname, numdim, dimlist)
char	arname[IDSIZE + 1];
short	numdim,
	dimlist[MAXDIM];
{
	long	i;
	long	size;
	struct sym 	*currec;



	dimseen = 1;
	if ((currec = stsrch(arname, STRAID)) != NULL) /*already declared*/
	{
		error(inst.thing.linno,inst.thing.stmtno, 23);	/* Redundant DIM stmt. */
		return(-1);
	}
	if ((currec = stinsert(arname, STRAID)) == NULL)
		return(-1);

	if ((currec->data.sarr.header 
		= makeheader(numdim, dimlist, &size)) == NULL)
	{
		stremove(arname, STRAID);
		return(-1);
	}

	if ((currec->data.sarr.arblok =  (char **) calloc((unsigned) size, (unsigned) sizeof(char *)))
					== NULL)    /* not enough space for */
	{			       		    /* array elements       */
		stremove(arname, STRAID);
		error(inst.thing.linno,inst.thing.stmtno, 84);	
		return(-1);
	}

	for (i=0; i < size; i++)
		*(currec->data.sarr.arblok + i) = strget("");
	return(0);
}


idim(arname, numdim, dimlist)
char	arname[IDSIZE + 1];
short	numdim,
	dimlist[MAXDIM];
{
	long	i;
	long	size;
	struct sym 	*currec;


	dimseen = 1;
	if ((currec = stsrch(arname, INTAID)) != NULL) /*already declared*/
	{
		error(inst.thing.linno,inst.thing.stmtno, 23);	/* Redundant DIM stmt. */
		return(-1);
	}

	if ((currec = stinsert(arname, INTAID)) == NULL)
		return(-1);

	if ((currec->data.iarr.header 
		= makeheader(numdim, dimlist, &size)) == NULL)
	{
		stremove(arname, INTAID);
		return(-1);
	}

	if ((currec->data.iarr.arblok =  (int *) calloc((unsigned) size, (unsigned) sizeof(int)))
					== NULL)    /* not enough space for */
	{			       		    /* array elements       */
		stremove(arname, INTAID);
		error(inst.thing.linno,inst.thing.stmtno, 84);	
		return(-1);
	}

	for (i=0; i < size; i++)
		*(currec->data.iarr.arblok + i) = 0;
	return(0);
}


/*
*	construct array header block
*
*	block format:
*
*	+----------------+
*	| # of dimensions|
*	+----------------+
*	|  dimension 1   |
*	+----------------+
*	|      a2	 |	the ai's are calculated for 
*	+----------------+	efficient array referencing
*	|  dimension 2	 |
*	+----------------+	ak+1 = (dimk - base + 1)(dimk-1 - base + 1)
*	       .			...(dim1 - base + 1)
*	       .
*	       .		see aputvars.c for how these factors are
*	+----------------+      used for array referencing.
*	|     ak	 |
*	+----------------+
*	|  dimension k	 |
*	+----------------+
*/

short *
makeheader(numdim, dimlist, sizeptr)
short		numdim,
		dimlist[];
long		*sizeptr;
{
	long 	prod;
	int	i;
	short   *start,
		*placeptr;


	if ((placeptr = start = (short *) 
		calloc((unsigned) 2*numdim, (unsigned) sizeof(short))) == NULL)
	{
		error(inst.thing.linno,inst.thing.stmtno, 84);
		return(NULL);
	}
	*placeptr++ = numdim;
	*placeptr++ = dimlist[0];
	if (dimlist[0] < base)
	{
		free(start);
		error(inst.thing.linno,inst.thing.stmtno, 88);
		return(NULL);
	}
	prod = 1;
	for (i = 1; i <= (numdim - 1); i++)
	{
		prod *= dimlist[i-1] - base + 1;
		if (prod >= MAXSHORT)
		{
			free(start);
			error(inst.thing.linno,inst.thing.stmtno, 84);
			return(NULL);
		}
		*placeptr++ = prod;
		*placeptr++ = dimlist[i];
		if (dimlist[i] < base)
		{
			free(start);
			error(inst.thing.linno,inst.thing.stmtno, 88);
			return(NULL);
		}
	}

	*sizeptr = prod * (dimlist[numdim - 1] - base + 1);
	if (*sizeptr >= MAXSHORT)
	{
		free(start);
		error(inst.thing.linno,inst.thing.stmtno, 84);
		return(NULL);
	}
	return(start);
}


numelems(headptr)
short	*headptr;
{
	int 	penultprod,
		lastdim;

	if (*headptr == 1)
		return(*(headptr+1) - base + 1);
	penultprod = *(headptr + 2 * (*headptr-1));
	lastdim = *(headptr + 2 * (*headptr-1) + 1);
	return(penultprod * (lastdim - base + 1));
}
