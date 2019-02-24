/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// savars.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Feb 27 06:32:38 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) savars.c: V1.2 3/4/81

 * String array variable Allocation and Fetch routines
 * For BITE
 *
 */
/*   "@(#) savars.c:  V 1.1  2/4/81" */

#include	"bas.h"
/*
 * this is the symbol table (table of pointers) for string array
 * variables. The first subscript 'j' is calculated by the
 * alpha name, the second subscript 'k' is calculated by the
 * numeric part + 1. If no numeric is specified, k = 0
 *
 * The pointer points to an area in memory allocated by dim.
 * The area is of similar form to that of numeric variables
 * except, the pointers to the strings rather than values are
 * actually stored. The header form is the same as that for numeric
 * variables except that the constants a1,a2,a3,..,etc.
 * are unnecessary due to allowing only one or two dimensions.
 * (see remarks at beginning of dim.c module and diagram)
 *
 * The specification for a string array variable is of the form:
 *	Alpha[Numeric]$(a,b)
 * The dimension "dim" statement is specified in the form:
 *	dim Alpha[numeric]$(XMAX[,YMAX])
 * The starting location is obtained by the equation:
 *	Loc = (a-1)*YMAX+b-1
 *
 *	where a <= XMAX
 *	      b <= YMAX
 *
 * If dimension YMAX is not given it is set to 1, however the
 * first byte of the header will specify only one dimension exists.
 *
 */
char *	sasymtab[26][11];
extern	char	*hicore;	/* pointer lowest used corespace from
				    top of user area */


struct FREELIST	 freelist[MAXFREE];
/*
 * //////// FETCH VALUE OF VARIABLE ////////
 *
 * calling format:
 *
 *	sagetvar(sname, &sptr);
 *
 *	where: sname = string containing VALID variable name
 *			i.e. [a-z] or a[0-9] - z[0-9]
 *	       sptr = pointer to string
 */
sagetvar(sname,sptr)
char	sname[];
char	**sptr; /* pointer to string pointer */
{
	char	*addr;

	if(sgetaddr(sname, &addr) < 0)
		return(-1);	/* error occurred in getting addr */

	if(addr == 0)		/*if no pointer for this variable */
				/*  (not allocated, yet) */
	{
		error2(inst.thing.linno,7,' '); /* UNASSIGNED VARIABLE */
		printf("- '%s'\n",sname); /* print variable name */
		*sptr = 0;		/* return zero anyway */
		return(-1);
	}
	else
	{
		*sptr = addr;		/* set ptr to data location */
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
 *	saputvar(sname, sptr);
 *
 *	where: sname = string pointer to VALID variable name
 *	       sptr = pointer to string to be copied
 */
saputvar(sname,sptr)
char	sname[];	/* valid variable string (name) */
char	*sptr;		/* pointer to string */
{
	register char *ptr,*cptr; /* pointer used for string copy to mem */
	char	*lookfree();
	int	ssize;		/* string size */
	int	useagain;	/* flag to use space again */
	char	*addr;		/* pointer array el. which is ptr to string */

	useagain = 0;
	cptr = sptr;	/* get source string pointer */
	ssize = strlen(sptr);	/* get size of string */
	if(sgetaddr(sname, &addr) < 0)
		return(-1);	/* error occurred in getting addr */
	if(addr != 0)		/* if this variable was used before */
	{
		ptr = addr;		/* point to old space */
		savefree(ptr);		/* save area old in free list */
		if((addr = ptr = lookfree(ssize)) != 0)
			useagain = 1;	/* if other hole is found */
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
		addr = ptr;		/* put new pointer in array table */

		if((unsigned)hicore & 1)	/* hicore to even boundry */
			hicore--;
	}
	ptr++;		/* bump past size */

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
/*
 *
 * Look in freelist to for smallest free area
 * which will fit new allocation. If none, return false.
 * The free list points to previously scrapped areas.
 */
char *
lookfree(size)
{
	register i;
	int	bestsize,besti;

	for(i=0, bestsize=0; i<MAXFREE; i++)
	{
		if(freelist[i].size == 0) /* skip null or deleted entries */
			continue;
		if(freelist[i].size >= size)
		{
			if(bestsize == 0) /* force first fit */
			{
				bestsize = freelist[i].size;
				besti = i;
			}
			else if(freelist[i].size < bestsize) /* better fit? */
			{
				bestsize = freelist[i].size;
				besti = i;
			}
		}
	}
	if(bestsize == 0)
		return(0);	/* nothing found */
	else
	{
		freelist[besti].size = 0;	/* take off free list */
		return(freelist[besti].fraddr);
	}
}

savefree(addr)
char	*addr;
{
	register i;
	int	size;
	int	there;

	size = *addr;
	for(i=0,there=0; i<MAXFREE; i++)
	{
		if((freelist[i].size != 0) && (freelist[i].fraddr == addr))
		{
			/* avoid dup entry */
			there = 1;
			break;
		}
	}
	if(!there)
	{
		for(i=0; i<MAXFREE; i++)
		{
			if(freelist[i].size == 0)
			{
				freelist[i].size = size; /* save size */
				freelist[i].fraddr = addr; /* save address */
				break;
			}
		}
	}
}
/*
 *
 * /// COMPUTE ARRAY VARIABLE ADDRESS ///
 */
sgetaddr(vstr, addr)
char	vstr[];		/* subscripted variable string */
char	**addr;		/* address of subscripted variable pointer */
{
	char	arname[3];	/* array name */
	int	offset;
	int	*ptr;		/* location pointer */
	int	asize;		/* actual array size */
	double	rsize;		/* requested size */
	register i;
	int	j,k;
	double	rdims[MAXDIM];	/* requested dims */
	int	adims[2];	/* actual dims */
	int	amax;		/* max number of dims */

	amax = 2;		/* no more than two dimensions */
	/*
	 * GET ARRAY SYMBOL NAME, NUMBER OF DIMENSIONS &
	 * PUT THE VALUE OF EACH DIMENSION IN DIMLIST
	 */
	if(getdims(vstr,arname,&rsize,rdims) < 0)
		return(-1);
	if(rsize == 1.0)
	{
		rdims[1] = 1.0;
	}
	j = arname[0] - 'a';		/* compute j subscript */

	/*
	 * compute k subscript
	 */
	if(arname[1] == '\0')		/* if no numeric part */
		k = 0;			/* then k = 0th column */
	else
		k = arname[1] - '0' + 1;

	if(sasymtab[j][k] == 0)		/* if not dimensioned complain */
	{
		error(inst.thing.linno, 25); /* NOT DIMENSIONED */
		return(-1);
	}
	ptr = (int *)sasymtab[j][k];		/* point to header */
	asize = *--ptr;				/* get actual size */
	if(asize != (int)rsize)		/* error if mismatch in actual to
					    requested size */
	{
		error(inst.thing.linno, 26); /* WRONG NUM OF DIMS */
		return(-1);
	}
	for(i=0; i<amax; i++)
	{
		adims[i] = *--ptr;
		if((int)rdims[i] > adims[i])
		{
			error(inst.thing.linno, 27); /* DIM > ACTUAL */
			return(-1);
		}
	}
	*--ptr;		/* BASE STARTS AFTER HEADER */
	/*
	 * Note that this structure starts from hi to low memory
 	 *	LOC = BASE - ((a-1)*YMAX+b-1)
	 */
	offset = ((int)rdims[0]-1)*adims[1]+((int)rdims[1]-1);
	*addr = (char *)(ptr-offset);
	return(0);
}
