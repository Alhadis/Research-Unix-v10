/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// class.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 16:52:56 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *
 *      ALGEBRAIC EXPRESSION FIELD and OPERATOR CLASSIFIER
 *                 and SYNTAX CHECKER
 *		(LEXICAL analyzer of sorts)
 *
 * MOD,J.P.Hawkins,31-JAN-81 to recognize string variables and strings
 * MOD,J.P.Hawkins,17-FEB-81 to recognize string functions
 *
 * calling format:
 *
 *	field class (or error) = class(&sptr, field);
 *
 *	where the ADDRESS of the Pointer sptr MUST be passed
 *		the input string pointer will be MODIFIED and
 *		LEFT at a point after the field just parsed
 *
 *		field = pointer to txtbuf where a null terminated copy
 *			of the field just parsed will be left
 *
 *		error code = negative of class
 *			(i.e. a certain type field was attempted
 *			 but something illegal happened
 */
/*   "@(#) class.c:  V 1.4  3/1/81" */
#include	"bas.h"
#define cpychr()	{*fptr++ = **sptr ; *sptr += 1; }
/*
 * the legdlm(X) test implies that those fields as checked by class
 * may have the defined limited char values following the field
 * the chars 1 through 37(8) are keywords (defined in bed.c) which
 * replace "goto","go to","then","to","step","<=","=<","<",">=","=>",">"
 * "=" or "<>"  which is done at input time to compact the code and to
 * simplify run time tests for these combinations
 */
#define legdlm(X) ((any(X," ^*+/-)")  || (X>='\0' && X <= '\37')))
/*
#define	skip00()	{while(**sptr == ' ' || **sptr == '\t') *sptr +=1;}
*/
#define	skip00()	{}	/* skip00 does nothing */
class(sptr, field)
char	**sptr;		/* input string pointer */
char	*field;		/* pointer to output field */
{
	char	*fptr;	/* output field pointer */
	int	pcnt;	/* parenthesis best counter */
#ifdef	STRINGS
	int	strflg;	/* string function flag */
#endif
	pcnt = 0;	/* init paren nest counter */
	fptr = field;	/* init output field pointer */
	skip00();		/* skip leading blanks and tabs */
	/*
	 * QUOTED LITERAL STRING?
	 */
#ifdef	STRINGS
	if(**sptr == '"')	/* if beginning of string */
	{
		*sptr += 1;	/* bump past 1st quote */
		while(!(**sptr == '"' || **sptr == '\0'))
			cpychr();
		if(**sptr == '"')
		{
			*sptr += 1; /* bump past end delimiter */
			*fptr = '\0'; /* null terminate */
			return(STCLASS); /* LITERAL STRING CLASS */
		}
		return(-STCLASS); /* unbalanced quotes */
	}
#endif
	if(alpha(**sptr))	/* if 1st char is alpha */
	{
		cpychr();	/* copy first alpha */
		if(alpha(**sptr))	/* if second alpha it must be func */
		{
			/*
			 * copy until first non-alphanumeric
			 * this must be a FUNCTION FIELD.
			 * The first non-alpha numeric must be a '('
			 */
			while(alpha(**sptr) || num(**sptr))
				cpychr();
#ifdef	STRINGS
			strflg = 0;
			if(**sptr == '$')	/* string function? */
			{
				strflg = 1;	/* yes */
				cpychr();
			}
#endif

			if(**sptr == '(') /* func name followed by '(' */
			{
				pcnt = 1; /* count first nest deep */
				cpychr(); /* copy initial '(' */
				/*
				 * scan stuff inside of parens.
				 * parens must be balanced to terminate
				 * function field.
				 */
				while(pcnt > 0 && **sptr != 0)
				{
					switch(**sptr){
						case '(':
							pcnt += 1;
							break;
						case ')':
							pcnt -= 1;
							break;
						default:
							break;
					}
					cpychr();
				}
				if(**sptr == 0 && pcnt != 0)
				{
					*fptr='\0';
					/*
					 * line terminates before
					 * function defined.
					 */
#ifdef	STRINGS
					if(strflg)
						return(-SFCLASS);
					else
#endif
						return(-FNCLASS);
				}
				if(legdlm(**sptr))
				{
					*fptr = '\0'; /* null term */
					/*
					 * Return function class code
					 */
#ifdef	STRINGS
					if(strflg)
						return(SFCLASS);
					else
#endif
						return(FNCLASS);
				}
				else /* illegal delim */
					return(-FNCLASS);
			}
			else
			{
				return(-FNCLASS); /* function def without
						      '('  */
			}
		}
		else	/* VARIABLE OR VARIABLE ARRAY FIELD */
		{
			if(num(**sptr))
				cpychr(); /* cpy numeric part of var name */
#ifdef	STRINGS
			strflg = 0;
			if(**sptr == '$')	/* string function? */
			{
				strflg = 1;	/* yes */
				cpychr();
			}
#endif
			if(**sptr == '(') /* array paren? */
			{
				pcnt = 1;	/* init paren count */
				cpychr();	/* copy 1st paren */
				/*
				 * scan stuff inside of parens.
				 * parens must be balanced to terminate
				 * function field.
				 */
				while(pcnt > 0 && **sptr != 0)
				{
					switch(**sptr){
						case '(':
							pcnt += 1;
							break;
						case ')':
							pcnt -= 1;
							break;
						default:
							break;
					}
					cpychr();
				}
				if(**sptr == 0 && pcnt != 0)
				{
					*fptr='\0';
					/*
					 * line terminates before array
					 * defined
					 */
#ifdef	STRINGS
					if(strflg)
						return(-SACLASS);
					else
#endif
						return(-VACLASS);
				}
				if(legdlm(**sptr))
				{
					*fptr = '\0'; /* null term */
#ifdef	STRINGS
					if(strflg)
						return(SACLASS);
					else
#endif
						return(VACLASS);
				}
				else
				{
					*fptr = '\0';
					/* ill delim */
#ifdef	STRINGS
					if(strflg)
						return(-SACLASS);
					else
#endif
						return(-VACLASS);
				}
			}
			else /* VARIABLE NAME FIELD */
			{
				if(legdlm(**sptr)) /* legal delimiter */
				{
					*fptr = '\0'; /* null term */
#ifdef	STRINGS
					if(strflg)
						return(SVCLASS);
					else
#endif
						return(VRCLASS);
				}
				else
				{
#ifdef	STRINGS
					if(strflg)
						return(-SVCLASS);
					else
#endif
						return(-VRCLASS); /* ill delim */
				}
			}
		}
	}
	else /* EITHER NUMERIC OR OPERATOR FIELD */
	{
		if(num(**sptr) || **sptr == '.')
		{
			cpychr(); /* copy 1st dig of num */
			while(num(**sptr)) /* copy all consec dig */
				cpychr();
			if(**sptr == '.') /* embedded '.' is allowed */
			{
				cpychr();	/* copy '.' */
				while(num(**sptr)) /* get digits following
						      decimal point */
					cpychr();
			}
			if(**sptr == 'e' || **sptr == 'E')
			{
				cpychr();
				/*
				 * HANDLE EXPONENTIAL NOTATION TYPE
				 */
				if(**sptr == '+' || **sptr == '-')
				{
					cpychr();
					if(num(**sptr))
					{
						/* copy exponent */
						while(num(**sptr))
							cpychr();
					}
					else
					{
						return(-NMCLASS);
					}
				}
				else
				{
					return(-NMCLASS);
				}
			}
			if(legdlm(**sptr)) /* check for legal delim */
			{
				*fptr = '\0'; /* null term */
				return(NMCLASS); /* numeric */
			}
			else
				return(-NMCLASS); /* ill delim */
		}
		else /* OPERATOR FIELD */
		{
			if(any(**sptr, "^*/+-()"))
			{
				cpychr(); /* copy the operator */
				*fptr = '\0'; /* term with null */
				return(OPCLASS); /* operator */
			}
			else
				return(-OPCLASS); /* illegal delim */
		}
	}
}
/*
 *
 * //////// TRUE IF ANY CHARS IN STRING as MATCH c ////////
 */
any(c, as)
int c;
char *as;
{
	register char *s;

	s = as;
	while(*s)
		if(*s++ == c)
			return(1);
	return(0);
}

/*
 * //// TRUE IF ALPHA lower case ////
 */
alpha(c)
char	c;
{
	if(c >= 'a' && c <= 'z')
		return(1);
	else
		return(0);
}

/*
 * //// TRUE IF NUMERIC ////
 */
num(c)
char	c;
{
	if(c >= '0' && c <= '9')
		return(1);
	else
		return(0);
}
