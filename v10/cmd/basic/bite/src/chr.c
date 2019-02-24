
/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	//////////////// chr ////////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Mon Feb 16 22:19:00 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) chr.c: V1.2 3/4/81

 * String function chr$(expr) where expr is a numerical expression
 * and chr$ returns the character associated with that value.
 */

char *chr(num)
char	*num;
{
	double	evalx();
static	char	tmpnum[2];

	/*
	 * Evaluate expression num and mask all but low 8 bits
	 * then form null terminated single character string.
	 */
	tmpnum[0] = (char)0377 & (char)evalx(num);
	tmpnum[1] = '\0';
	return(tmpnum);
}
