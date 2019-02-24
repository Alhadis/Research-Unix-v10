/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// up-lo.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Thu Mar  5 11:38:41 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) up-lo.c: V1.1 3/5/81

*/

/*
 * Change lower case to upper case
 */
char	*
upc(str)
char	str[];
{
	register i;
	char	work[80];

	evals(str,work);
	for(i=0; work[i] != '\0'; i++)
	{
		if(work[i] >= 'a' && work[i] <= 'z')
			work[i] -= ('a'-'A');
	}
	return(work);
}

/*
 * Change upper case to lower case
 */
char	*
loc(str)
char	str[];
{
	register i;
	char	work[80];

	evals(str,work);
	for(i=0; work[i] != '\0'; i++)
	{
		if(work[i] >= 'A' && work[i] <= 'Z')
			work[i] += ('a'-'A');
	}
	return(work);
}
