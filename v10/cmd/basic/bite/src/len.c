/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	//////////////// len ////////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Tue Feb 17 13:59:27 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) len.c: V1.2 3/4/81
 *
 * len(string)
 * returns length of string expression
 */

char	scratch[132];
double
len(exp)
char	*exp;
{
	evals(exp,scratch);
	return((double)strlen(scratch));
}
