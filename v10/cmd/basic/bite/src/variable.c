/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	//////////// variable.c /////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 17:00:44 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * Variable Allocation and Fetch routines
 * For BITE
 *
 */
/*   "@(#) variable.c:  V 1.2  3/4/81" */
#include	"bas.h"
union VARIABLE varbyts;
/*
 * this is the symbol table (table of pointers) for numerical
 * variables. The first subscript 'j' is calculated by the
 * alpha name, the second subscript 'k' is calculated by the
 * numeric part + 1. If no numeric is specified, k = 0
 */
char	*hicore;	/* pointer lowest used corespace from
				    top of user area */


/*
 * //////// FETCH VALUE OF VARIABLE ////////
 *
 * calling format:
 *
 *	getvar(vstr, &value);
 *
 *	where: vstr = string containing VALID variable name
 *			i.e. [a-z] or a[0-9] - z[0-9]
 *	       float value = where actual value is returned to
 */
getvar(vstr,valptr)
char	vstr[];
double	*valptr;
{
	int	j,k;	/* symbol table indicies */
	register char i;	/* index to float bytes */
	register char *ptr;	/* pointer register used to copy bytes */

	j = vstr[0] - 'a';	/* compute j subscript */

	/*
	 * compute k subscript
	 */
	if(vstr[1] == '\0')	/* if no numeric part */
		k = 0;		/* then k = 0th column */
	else
		k = vstr[1] - '0' + 1; /* use numeric + 1th column */

	if(symtab[j][k] == 0)	/*if no pointer for this variable */
				/*  (not allocated, yet) */
	{
		error2(inst.thing.linno,7,' '); /* UNNASSIGNED VARIABLE */
		printf("- '%s'\n",vstr); /* print variable name */
		*valptr = 0.0;		/* return zero anyway */
		return(-1);
	}
	else
	{
		ptr = symtab[j][k]; /* set byte copy ptr to data location */

		for(i=PREC-1; i >= 0; --i) /* copy the float byte-by byte */
			varbyts.var4th[i] = *--ptr;

		*valptr = varbyts.var;	/* now pass the value back to
					    the calling routine */
	}
	return(0);
}
/*
 *
 * //// ASSIGN A VALUE TO A VARIABLE ////
 * /////// ALLOCATE SPACE IF NEC. ///////
 *
 * put the value into the allocated location.
 * allocate a location if not already allocated
 * calling format:
 *
 *	putvar(vstr, value);
 *
 *	where: vstr = string pointer to VALID variable name
 *	       value = value to be assigned
 */
putvar(vstr,value)
char	vstr[];		/* valid variable string (name) */
double	value;		/* value to be assigned */
{
	register char *ptr;	/* pointer register used to copy bytes */
	register char i;	/* index to float bytes */
	int	j,k;	/* symbol table indicies */

	j = vstr[0] - 'a';	/* compute j subscript */

	/*
	 * compute k subscript
	 */
	if(vstr[1] == '\0')	/* if no numeric part */
		k = 0;		/* then k = 0th column */
	else
		k = vstr[1] - '0' + 1;

	varbyts.var = value;	/* stuff the float number (2 words)
				    in the float template to be copied
				     into data area byte-by-byte */
	if(symtab[j][k] == 0)	/* if no space yet allocated */
		alloc(&symtab[j][k], PREC); /* then allocate space and
					     return pointer to symbol tab */
	ptr = symtab[j][k];	/* set copy register to data address */

	for(i=PREC-1; i>=0; --i)	/* copy into storage byte-by-byte */
		*--ptr = varbyts.var4th[i];
}
/*
 *
 * //// INITIALIZE VARIABLES ////
 */
initvar()
{
	extern char	txtbuf[];
	hicore = &txtbuf[PGSIZ*NMPAG];	/* init pointer to top of user
					    data space */
}
/*
 *
 * //// ALLOCATE STORAGE FOR SPECIFIED BYTES ////
 * return starting address of allocated space
 *
 * The data area is a TOP-DOWN allocation technique which follows
 * rules that are inverse to BOTTOM-UP allocation or storage.
 * The address of a datum is actually the address of the next higher
 * core byte. When data is retrieved or stored, a pointer to that address
 * is obtained, then each byte is retrieved or stored by first
 * decrementing the pointer, THEN retrieving or inserting the byte in
 * quiestion. Unlike bottom-up storage, the beginning of the area
 * is always one address above-it and the end of the area is the last
 * byte stored.
 * In a bottom-up storage the beginning of data IS the first byte
 * and the end of the area or piece of data or entire data area is
 * AFTER the end of the data area or datum.
 * These rules are consistant with the hardware of the PDP-11
 * albeit auto-increment/decrement data retrieval and takes maximum
 * advantage of the harware, thus producing the least amount of code.
 *
 * If these rules are adhered to at all times, there will be no error
 * in data storage or retrieval.
 *
 * In light of the above, the value 'hicore' reflects the last byte
 * occupied by the top-down storage area/txtbuf/stack or whatever.
 */
alloc(datadr, numbyts)
char	**datadr;	/* pointer to symbol table entry */
int	numbyts;	/* number of bytes to be allocated */
{
		*datadr = hicore; /* set symtab entry to loc of data */
		hicore -= numbyts; /* bump down pointer to point to
				       lowest occupied data space */
}
