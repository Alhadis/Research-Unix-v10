/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// svars.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sat Jan 31 14:59:54 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * String variable Allocation and Fetch routines
 * For BITE
 *
 */
/*   "@(#) svars.c:  V 1.1  2/4/81" */

#include	"bas.h"
/*
 * this is the symbol table (table of pointers) for string
 * variables. The first subscript 'j' is calculated by the
 * alpha name, the second subscript 'k' is calculated by the
 * numeric part + 1. If no numeric is specified, k = 0
 */
char *	ssymtab[26][11];
extern	char	*hicore;	/* pointer lowest used corespace from
				    top of user area */


/*
 * //////// FETCH VALUE OF VARIABLE ////////
 *
 * calling format:
 *
 *	sgetvar(sname, &sptr);
 *
 *	where: sname = string containing VALID variable name
 *			i.e. [a-z] or a[0-9] - z[0-9]
 *	       sptr = pointer to string
 */
sgetvar(sname,sptr)
char	sname[];
char	**sptr; /* pointer to string pointer */
{
	int	j,k;	/* symbol table indicies */

	j = sname[0] - 'a';	/* compute j subscript */

	/*
	 * compute k subscript
	 */
	if(sname[1] == '\0')	/* if no numeric part */
		k = 0;		/* then k = 0th column */
	else
		k = sname[1] - '0' + 1; /* use numeric + 1th column */

	if(ssymtab[j][k] == 0)	/*if no pointer for this variable */
				/*  (not allocated, yet) */
	{
		error2(inst.thing.linno,7,' '); /* UNASSIGNED VARIABLE */
		printf("- '%s$'\n",sname); /* print variable name */
		*sptr = 0;		/* return zero anyway */
		return(-1);
	}
	else
	{
		*sptr = ssymtab[j][k]; /* set ptr to data location */
		*sptr += 1;		/* bump past size byte */
	}
	return(0);
}
/*
 *
 * //// ASSIGN A VALUE TO A VARIABLE ////
 * /////////// ALLOCATE SPACE  //////////
 *
 * Copy the string into the allocated location.
 *
 * calling format:
 *
 *	sputvar(sname, sptr);
 *
 *	where: sname = string pointer to VALID variable name
 *	       sptr = pointer to string to be copied
 */
sputvar(sname,sptr)
char	sname[];	/* valid variable string (name) */
char	*sptr;		/* pointer to string */
{
	register char *ptr,*cptr; /* pointer used for string copy to mem */
	int	ssize;		/* string size */
	int	useagain;	/* flag to use space again */
	int	j,k;	/* symbol table indicies */

	useagain = 0;
	cptr = sptr;	/* get source string pointer */
	j = sname[0] - 'a';	/* compute j subscript */

	/*
	 * compute k subscript
	 */
	if(sname[1] == '\0')	/* if no numeric part */
		k = 0;		/* then k = 0th column */
	else
		k = sname[1] - '0' + 1;

	ssize = strlen(sptr);	/* get size of string */
	if(ssymtab[j][k] != 0)	/* if this variable was used before */
	{
		ptr = ssymtab[j][k]; /* point to old space */
		if(ssize <= *ptr)	/* if there's enough room */
		{
			useagain = 1;	/* use same space */
			ptr++;		/* bump past size */
		}
	}
	if(!useagain)	/* if this is the first time this var was used */
	{
		/*
		 * Return error if not enough space left
		 */
		if((hicore - (ssize+2)) <= linptr)
		{
			error(inst.thing.linno, 24); /* NOT ENOUGH ADD. CORE */
			return(-1);
		}
		hicore -= (ssize+2);	/* set new hicore mark - */
					/* include space for null and size */
		ptr = hicore;		/* set pointer for copy */
		*ptr = ssize;		/* put size of 1st string in 1st loc */
		ssymtab[j][k] = ptr;	/* put pointer in sym. table */

		if((unsigned)hicore & 1)	/* hicore to even boundry */
			hicore--;
		ptr++;		/* bump past size */
	}

	while(*ptr++ = *cptr++); /* copy string into storage */

	return(0);
}
#ifdef notdef
/*
 * Return size of string up to first null character
 */
static	strlen(s)
char	*s;
{
	register count;
	for(count = 0; s[count]; count++);
	return(count);
}
#endif
