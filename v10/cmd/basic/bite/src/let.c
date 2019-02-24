/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// let.c ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Sun Aug 26 06:06:16 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 *	VARIABLE ASSIGNMENT ROUTINE for
 *	    LET command
 */
/*   "@(#) let.c:  V 1.4  3/4/81" */

#include	"bas.h"
/*
#define	skip00() {while(*lexptr == ' ' || *lexptr == '\t') *lexptr++;}
*/
#define	skip00()	{}	/* does nothing */

let()
{
	int	type;	/* field type */
	char	varnam[40];
	double	evalx();
	double	value;
#ifdef	STRINGS
	char	field[512];
#else
	char	field[40];
#endif
	char	*lexptr;	/* string pointer for let */

	lexptr = expr;		/* set text pointer */

	type = class(&lexptr,field);	/* get field type */

	switch(type)
	{
		case VRCLASS:
		case VACLASS:
#ifdef	STRINGS
		case SVCLASS:
		case SACLASS:
#endif
			break;
		default:
			error(inst.thing.linno, 3); /* ill var name */
			return(-1);
			break;
	}
	strcpy(varnam, field);	/* copy variable name */

	skip00();
	if(*lexptr++ == '\15')	/* if code for '=' */
	{
		switch(type)
		{
			case VRCLASS: /* regular variable name */
				value = evalx(lexptr);	/* eval right side */
				putvar(varnam, value);
				return(0);
				break;
			case VACLASS: /* subscripted var field */
				value = evalx(lexptr);	/* eval right side */
				aputvar(varnam, value);
				return(0);
				break;
#ifdef	STRINGS
			case SVCLASS: /* String variable */
				evals(lexptr,field); /* expand expression */
				sputvar(varnam,field); /* Store the string */
				break;
			case SACLASS: /* String array variable */
				evals(lexptr,field);
				saputvar(varnam,field);
				break;
#endif
			default:
				break;
		}
	}
	else
	{
		error(inst.thing.linno, 8); /* missing = */
		return(-1);
	}
	return 0;
}
