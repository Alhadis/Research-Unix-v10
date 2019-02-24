/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////// EXPUNGE all variables ///////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 05:56:08 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * clear variable symbol tables and initialize core pointer
 * so that all variables are freed including subscripted vars.
 * and hicore points to top limit of txtbuf.
 *
 * This done on any invocation of the "run" command.
 * or "expunge" command.
 *
 * MOD,J.P.Hawkins, 4-FEB-81, to include string variable symbol table clr
 */
/*   "@(#) expunge.c:  V 1.3  3/1/81" */
#include	"bas.h"
extern	char	*hicore;	/* highest free user location pointer */
extern char	txtbuf[];	/* user core */
extern char *symtab[26][11], *asymtab[26][11]; /* symbol table pointers */
#ifdef	STRINGS
extern	struct	FREELIST freelist[];
extern char *	ssymtab[26][11];
extern char *	sasymtab[26][11];
#endif
expunge()
{
	register i;
	clrtab(symtab);		/* initialize regular variable
				    symbol table */
#ifdef	STRINGS
	clrtab(ssymtab);	/* initialize string variable
					symbol table */
	clrtab(sasymtab);	/* initialize string array variable
					symbol table */
	for(i=0; i<MAXFREE; i++) /* cleanup string freelist */
		freelist[i].size = 0;
#endif
	clrtab(asymtab);	/* initialize subscripted variable
				    symbol table */
	hicore = &txtbuf[PGSIZ*NMPAG]; /* point to top of user core */
}

/*
 * clear 26 rows, 11 columns
 * rows represent symbol letters a-z
 * columns represent no-number, then 0-9 = 11
 */
clrtab(table)
char *	table[26][11];
{
	register	j,k;	/* row and column indicies */

	for(j=0; j<26; j++)
		for(k=0; k<11; k++)
			table[j][k] = 0;
}
