/*
 *	static char *ID_symbl2 = "@(#) symbols2.h: 1.3 12/10/83";
*/

/*
 *	NINSTRS	= number of legal assembler instructions (from ops.out)
 *	NHASH	= the size of the hash symbol table
 *	NSYMS	= the number of symbols allowed in a user program
 */

#if	FLOAT
#define	NINSTRS	394
#else
#define	NINSTRS	358
#endif

#if	pdp11
#define NHASH	1693
#else
#define	NHASH	4999
#endif

#define	NSYMS	(NHASH - NINSTRS)
