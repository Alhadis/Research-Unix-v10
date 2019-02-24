/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// ext$ ////////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Tue Feb 17 07:33:14 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) ext.c: V1.3 3/4/81

 * BASIC ext$(string,pos,len)
 * return substring of string starting at position pos length len
 * where string is a string expression and pos and len are
 * numeric expressions.
 */

#include	"bas.h"
char *ext(args)
char	args[];
{
	double	evalx();
	char	*prncpy();
	char	string[80],result[80];
	char	pos[20],num[20];
	int	posit,number;
	char	*holdptr,*sptr;

	holdptr = args;
	holdptr=prncpy(result,args)+1; /* get str field and point past ',' */
	sptr=result;
	holdptr=prncpy(pos,holdptr)+1; /* get position expr & point past ',' */
	prncpy(num,holdptr);	   /* get size expression */
	posit = (int)evalx(pos)-1; /* evaluate the position field */
	number = (int)evalx(num);  /* evaluate the size field */
	evals(sptr,string);
	strncpy(result,&string[posit],number); /* put the segment in result */
	result[number] = '\0';		/* null terminate result */

	if(posit > strlen(string))
	{
		error(inst.thing.linno, 55); /* OUT OF STRING RANGE */
	}

	return(result);
}
