/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// asc.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Thu Mar  5 05:49:25 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) asc.c: V1.1 3/6/81

 * Return character value of single first char in ascii expr.
*/

extern	scratch[];

double
asc(s)
char	s[];
{
	char	work[80];

	evals(s,work);
	return((double)work[0]);
}
