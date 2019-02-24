/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// equal.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 08:11:11 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * This function is similar, but NOT the same as strcmp
 * If the string being tested begins with the character string
 * being compared to the test passes and is non-zero (length of string)
 * If the test fails a zero is returned.
 * this philosophy lends itself to code that appears like
 *	if(equal(buf,"jim"))
 */
/*   "@(#) equal.c:  V 1.1  12/21/80" */
equal(ptr,string)
char	*ptr;
char	*string;
{
	register char	*i, *j;
	i = ptr;
	j = string;

	while((*i == *j) && (*j != '\0'))
	{
		j++;
		i++;
	}
	if(*j == '\0')
		return(j - string);
	return(0);
}
