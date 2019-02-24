
/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// val.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Thu Mar  5 05:37:44 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) val.c: V1.1 3/5/81

 * Return numerical value of string (like atof).
 * This function can be used to extract numerical values
 * from ASCII format file.
*/

extern	char	scratch[];

double
val(exp)
char exp[];
{
	double atof1();

	evals(exp,scratch);
	return(atof1(scratch));
}
