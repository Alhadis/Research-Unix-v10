/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// chksec.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Wed May  6 18:59:40 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) chksec.c: V1.1 6/17/81
 * Check for security breaching characters like '|' and '&' and ';'
 * for restricted command lines

*/

chksec(s)
char	s[];
{
	register int i;

	for(i=0; s[i] != '\0'; i++)
		if(s[i] == '&' || s[i] == '|' || s[i] == ';')
		{
			printf("DON'T BE ABSURD THIS IS A RESTRICTED PROGRAM\n");
			return(-1);
		}
	return(0);
}
