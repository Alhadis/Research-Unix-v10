/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// common.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 06:01:13 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * The "COMMON" statement sets a flag "_comflg" so that run does not
 * execute expunge()
 *
 * This has the effect of preserving variables for a subsequent run.
 * Each run clears the flag, hence, COMMON must be issued by every
 * subsequent "run" to preserve the variables for the next level.
 *
 */
/*   "@(#) common.c:  V 1.1  12/21/80" */
int	_comflg;
_common()
{
	_comflg = 1;
	return(0);
}
