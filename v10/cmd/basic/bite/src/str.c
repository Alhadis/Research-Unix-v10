/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// str.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Thu Mar  5 10:38:56 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) str.c: V1.1 3/5/81
 * BASIC	str$(expr)
 *		returns ASCII string representing evaluated expression
 */
#include	"bas.h"

char	*
str(exp)
char	exp[];
{
	double	evalx(),val;
	char	num[40];
	char	s[40];

	val = evalx(exp);		/* convert to numerical value */
	sprintf(s, "%g", val);		/* convert the number to ASCII */
	num[0] = '\0';
	if(val >= 0.0)			/* put blank in place of - sign */
		strcat(num," ");
	strcat(num,s);			/* copy number to result */
	return(num);
}
