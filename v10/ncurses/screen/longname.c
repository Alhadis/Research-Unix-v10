/*	@(#) longname.c: 1.1 10/15/83	(1.3	4/21/82)	*/

/*
 *	This routine returns the long name of the terminal.
 */
char *
longname()
{
	register char	*cp;
	extern char ttytype[];

	for (cp=ttytype; *cp++; )		/* Go to end of string */
		;
	while (*--cp != '|' && cp>=ttytype)	/* Back up to | or beginning */
		;
	return ++cp;
}
