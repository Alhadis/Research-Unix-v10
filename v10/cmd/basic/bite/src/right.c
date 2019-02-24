/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// right.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Thu Mar  5 06:15:32 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) right.c: V1.1 3/5/81

 * function right$(string,n)
 *	where the last n chars of the string are returned
 *		n is a number or numerical expression
 */

extern	char scratch[];

char	*
right(args)
char	args[];
{
	double	evalx();
	char	*prncpy();
	char	*holdptr;
	char	num[20];
	int	number,length;
	char	result[80];

	holdptr = args;
	holdptr = prncpy(result,args)+1; /* point to string field */
	prncpy(num,holdptr);	/* get number of chars */
	evals(result,scratch);
	number=(int)evalx(num);	/* convert expr to integer */
	length=strlen(scratch);	/* get actual length of string */
	if(number > length)	/* if requested number > length */
		number = length; /* just return the string */
	strncpy(result,&scratch[length-number],number);
	result[number] = '\0';
	return(result);
}
