/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	/////////////// evals ///////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Mon Feb  2 07:33:54 1981 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	@(#) evals.c: V1.7 3/5/81

 * This is the string expression "evaluator". It's purpose is to
 * concatinate combinations of literals and expanded string variables
 * as they are fetched from memory. The only legal operation is '+'
 *
 * MOD,J.P.Hawkins,17-FEB-81 added string function handling
 * MOD,J.P.Hawkins,28-FEB-81 added string array handling
*/

#include	"bas.h"
extern	int	stpflg;
extern	char	*eoexpr;
evals(exps,result)
char	exps[];
char	*result;
{
	int	type;		/* field classification */
	char	*objadr;	/* string object pointer */
	char	cat[512];	/* concatination buffer */
	char	*expptr;	/* expression string pointer */
	char	field[80];

	expptr = exps;
	cat[0] = '\0';		/* initialize buffer */
/*
 *
 * The only ways out of this loop are string terminator or
 * expression terminator (null char)
 * the codes 1 through 37(8) represent encoded keywords (defined in bed.c)
 * those being:
 * goto,go to,then,to,step,<=,=<,<,>=,=>,>, =, or <>
 *
 * This "while" loop is
 * not indented so "all this stuff" fits across standard page
 */
while(!(*expptr >= '\0' && *expptr <= '\37'))
{
	if((type=class(&expptr,field)) < 0) /* get field and type */
	{
		error(inst.thing.linno, 8);	/* EXPR SYNTAX */
		error(inst.thing.linno, 15);		/* FATAL ERROR */
		stpflg = 1;
		result[0] = '\0';	/* leave null string */
		return(0);/* return zero on error */
	}

	switch(type)
	{
		case SVCLASS:
			sgetvar(field,&objadr); /* expand variable into
						   object pointer */
			strcat(cat,objadr);
			break;
		case STCLASS:
			strcat(cat,field);	/* copy literal into
						    object pointer */
			break;
		case SFCLASS:	/* STRING FUNCTION */
			strcall(field,&objadr);
			strcat(cat,objadr);
			break;
		case SACLASS:	/* STRING ARRAY CLASS */
			sagetvar(field,&objadr); /* expand variable into
						   object pointer */
			strcat(cat,objadr);
			break;
		case VRCLASS:	/* VARIABLE CLASS */
		case VACLASS:	/* SUBSCRIPTED VARIABLE */
		case NMCLASS:	/* NUMERIC FIELD */
		case FNCLASS:	/* FUNCTION REFERENCE */
			error(inst.thing.linno, 51);	/* NUM in ST. */
			stpflg = 1;
			return(0);
			break;
		case OPCLASS:	/* OPERATOR */
			switch(*field) {
				case '+':
				case ')':
					break;
				default:
					error2(inst.thing.linno, 52, ' ');
					printf("- '%c'\n",*field);
					break;
			}
			break;
		default:
			printf("WHAT HAPPENED? TYPE = %d\n", type);
			break;
		}
}	/* END OF WHILE LOOP */
	eoexpr = expptr;
	strcpy(result,cat);	/* put string in output */
	return 0;
}
/*
 *
 * ////// CALL STRING FUNCTION ////////
 *
 *
 *
 * calling format:
 *	value = strcall(strexp,&result);
 *
 *	where: strexp = string expression
 *	       result = pointer to a expanded null terminated string
 */

/*
 *
 * //////// BASIC INTERPRETER STRING FUNCTION TABLE ////////
 *
 */
char *chr(); /* convert integer to ascii char */
char *left(); /* left$(string,n) return first n chars of string */
char *ext(); /* ext$(string,p,n) ret substr. of string start at p length n */
char *right(); /* right$(string,n) ret last n chars of string */
char *str(); /* str$(expr) converts numeric expression to string */
char *string(); /* string$(n,char) repeats char n times */
char *loc();	/* loc$(str) change upper case to lower case chars */
char *upc();	/* upc$(str) change lower case to upper case chars */


/*
 *
 * This is the BASIC interpreter string function
 * dispatch table.
 *
 */

/*
 * each entry contains the text for the STRING FUNCTION
 * in question and the address of the routine which services it
 */
struct tbl
	{
		char *cmdtxt;
		char *(*func)();
	};
struct tbl sfuntbl[] = {
	{"chr",		chr},
	{"left",	left},
	{"ext",		ext},	/* GE MNEMONIC */
	{"mid",		ext},	/* For TRS-80 FANS */
	{"right",	right},
	{"str",		str},
	{"string",	string}, 
	{"upc",		upc},
	{"loc",		loc},
	{0,		0}	
	};
/*
 *
 */
strcall(strexp,result)
char	strexp[];
char	**result;
{
	char	resbuf[132];	/* result buffer */
	char	funnam[20]; /* func name copied from 's'
				used for string search */
	register char *x; /* pointer to paren enclosed expression */
	register  int i;		/* index reg. for expediency */
	x = strexp;	/* set pointer to func string */
	/*
	 * copy the func name part up to the '$'
	 * to use for a string search in the table of names
	 */
	for(i=0; *x != '$' ;)
	{
		funnam[i++] = *x++;
	}
	*x++;	/* skip ( */
	funnam[i] = '\0';	/* null terminate */
	/*
	 * compare each string in table with funnam
	 * when match is found, call using offset code
	 * if end-of-table (null) encountered return 0
	 */
	for(i=0; sfuntbl[i].cmdtxt != 0 ; i++)
	{
		if(!(strcmp(funnam, sfuntbl[i].cmdtxt)))
		{
			x++;
			strcpy(resbuf,(*sfuntbl[i].func)(x));
			*result = resbuf;
			return(0);
		}
	}
	error(inst.thing.linno, 54); /* unknown string function */
	return(0);	/* oops not in this table, pal */
}
