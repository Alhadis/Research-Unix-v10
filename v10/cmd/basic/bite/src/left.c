/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// left.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Thu Mar  5 06:15:32 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) left.c: V1.1 3/5/81

 * function left$(string,n)
 *	where the first n chars of the string are returned
 *		n is a number or numerical expression
 */

extern char	scratch[];

char	*
left(args)
char	args[];
{
	double	evalx();
	char	*prncpy();
	char	*holdptr;
	char	num[20];
	int	number;
	char	result[80];

	holdptr = args;
	holdptr = prncpy(result,args)+1; /* point to string field */
	prncpy(num,holdptr);	/* get number of chars */
	evals(result,scratch);
	number=(int)evalx(num);	/* convert expr to integer */
	strncpy(result,scratch,number);	/* get the beginning seg of string*/
	result[number] = '\0';
	return(result);
}
