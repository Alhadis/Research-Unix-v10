/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	///////////// string.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Thu Mar  5 07:48:24 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) string.c: V1.1 3/5/81
 *
 * function string$(n,string)
 *	Where n repetitions of the first char in string are returned
 */

extern char	scratch[];

char	*
string(args)
char	args[];
{
	double	evalx();
	char	*prncpy();
	char	*holdptr;
	char	num[20];
	int	number;
	char	result[80];
	register	i;

	holdptr = args;
	holdptr = prncpy(num,args)+1;	/* get number */
	prncpy(result,holdptr);		/* get string */
	evals(result,scratch);
	number=(int)evalx(num);	/* convert expr to integer */

	for(i=0; i<number; i++)
		result[i] = scratch[0];

	result[number] = '\0';
	return(result);
}
