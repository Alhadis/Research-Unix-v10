/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	/////////////////////////////////////
 *	/////////////////////////////////////
 *	////////////// evalx.c //////////////
 *	/// J. P. Hawkins WH X4610 8C-001 ///
 *	///// Fri Aug 24 16:40:38 1979 //////
 *	/////////////////////////////////////
 *	/////////////////////////////////////

 *
 * calling format:
 *
 *	double value;
 *	double evalx();
 *	value = evalx(expstr)
 *
 *	where:	dummy is now unused
 *		expstr is the char string representing the expression
 *			to be evaluated. The string must be terminated
 *			by a character between values 0 -> 37(8).
 *			0 is the usual ascii string terminator. Values
 *			between 001(8) and 37(8) are keyword codes
 *			for secondary operations like "then" in an "if"
 *			statement or "to" in a "for" statement.
 *
 */
/*   "@(#) evalx.c:  V 1.9  4/21/81" */
/*
 * MOD,J.P.Hawkins,31-JAN-81 detects strings and string vars. mixed in
 *		numerical expressions as reject.
 */
#include	"bas.h"
#define	STKSIZ	6	/* stack size */

int strflg;
/*
#define	skip00() {while(*expptr == ' ' || *expptr == '\t') *expptr++;}
*/
#define	skip00()	{}	/* do nothing */
extern	int	stpflg;		/* stop flag for fatal error */
	double	pow();
	double	atof1();
	double	mathcall();

	char	*eoexpr;	/* pointer to end of expression */
/*
 *
 *
 */
double
evalx(exps)
char	exps[];		/* pointer to expression string */
{
	char	*expptr;	/* expression string pointer */
	char	ostack[STKSIZ];	/* operator stack */
	double	nstack[STKSIZ];	/* number stack		*/
	int	opflag;		/* set when operator encountered
				    cleared when variable or non-op
				     encountered. Used to deal with
				      unaries by forcing a 0.0 on the
					number stack */
	int	o, n;		/* indicies for ostack & nstack */

	char	field[80];	/* expression field */
	int	type;		/* field type returned by class */
	expptr = exps;	/* init expression string pointer */
	o = n = STKSIZ;	/* init operator and
			    variable stack pointer */
	opflag = 0;	/* init opflag */
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
	if((type = class(&expptr, field)) < 0) /* get field and type */
	{
		error(inst.thing.linno, 8);	/* EXPR SYNTAX */
		error(inst.thing.linno, 15);		/* FATAL ERROR */
		stpflg = 1;
		return(0.0);/* return zero on error */
	}

	switch(type){
#ifdef	STRINGS
		case SVCLASS:
		case STCLASS:
		case SFCLASS:
		case SACLASS:
			error(inst.thing.linno, 49); /* ST. in NUM */
			stpflg = 1;
			return(0.0);
			break;
#endif
		case VRCLASS:	/* VARIABLE CLASS */
			getvar(field, &nstack[--n]);
			opflag = 0;
			break;
		case VACLASS:	/* SUBSCRIPTED VARIABLE */
			agetvar(field,&nstack[--n]);
			opflag = 0;
			break;
		case NMCLASS:	/* NUMERIC FIELD */
			nstack[--n] = atof1(field);
			opflag = 0;
			break;
		case FNCLASS:	/* FUNCTION REFERENCE */
			nstack[--n] = mathcall(field);
			opflag = 0;
			break;
		case OPCLASS:	/* OPERATOR */
			switch(*field) {
				case '(': /* begin of expres */
					nstack[--n] = evalx(expptr);
					opflag = 0;
					expptr = eoexpr; /* put pointer
							   to correct place */
					break;
				case ')': /* end of express */
					/*
					 * eval the rest of the expr
					 * between the parens
					 */
					cleanup(nstack,ostack,&n,&o);
					eoexpr = expptr;
					return(nstack[n]);
					break;
				case '+':
				case '-':
				case '*':
				case '/':
				case '^':
					/*
					 * handle unaries by forcing
					 * a 0.0 onto the nstack before
					 * stacking the operator.
					 * This occurs when two operators
					 * in a row are encountered or the
					 * variable stack is empty when
					 * an operator encountered.
					 */
					if(opflag || n == STKSIZ)
						nstack[--n] = 0.0;
					opflag = 1;
					stackop(*field,nstack,ostack,&n,&o);
					break;
				default:
					break;
			}
			break;
		default:
			printf("WHAT HAPPENED? TYPE = %d\n", type);
			break;
		}
		/*
		 * skip trailing white space so that
		 * test at top of this loop fails when
		 * it's supposed to
		 */
		skip00();
}	/* END OF WHILE LOOP */

	cleanup(nstack,ostack,&n,&o);
	eoexpr = expptr;	/* save end of expr pointer for
				    "for", "if" and other commands */
	return(nstack[n]);
}
/*
 *
 * //// STACK OPERATOR IF PRECIDENCE TEST IS PASSED ////
 */
stackop(op,nstack,ostack,n,o)
char	op;
double	nstack[];
char	ostack[];
int	*n;
int	*o;
{

	/*
	 * if precidence of current operator is
	 * HIGHER than the operator on top of the stack
	 * or the OPERATOR stack is EMPTY,
	 * the precidence test has "passed" so just push
	 * the current operator onto the operator stack.
	 */
	if(*o == STKSIZ)	/* if stack is empty */
	{
		ostack[--(*o)] = op;	/* stack operator
					    precidence done only
					     if stack NOT empty */
		return 0;
	}
	if(precid(op) > precid(ostack[*o]))
	{
		ostack[--(*o)] = op;
	}
	else
	{
		/*
		 * OTHERWISE if the current operator
		 * precidence is LESS than or EQUAL to the
		 * operator on the top of the stack
		 * "unstack" the operators from the operator
		 * stack until either the stack is empty
		 * or the precidence test passes
		 */
		while((precid(op) <= precid(ostack[*o])) && *o < STKSIZ)
			if(unstack(nstack,ostack,n,o) < 0)
			{
				return(-1); /* break loop if bad operator
						encountered */
			}

		ostack[--(*o)] = op;
	}
	return 0;
}
/*
 *
 * //// RETURN PRECIDENCE OF OPERATOR IN c ////
 */
precid(c)
char c;
{
static char olist[] = "+1-1*2/2^3"; /* list of operators & precidence */
	register int p;	/* scanning register */

	for(p=0; olist[p] != '\0'; p += 2)
	{
		if(c == olist[p])
			return(olist[p+1] - '0');
	}
	return(-1);	/* if we get here, we're in trouble! */
}




/*
 * //// CLEANUP THE REST OF THE EXPRESSION ////
 *
 * "pop and do" operations on opstack until
 * stack empty
 */
cleanup(nstack,ostack,n,o)
double	nstack[];
char	ostack[];
int	*n;
int	*o;
{
	while(*o < STKSIZ)
		if(unstack(nstack,ostack,n,o) < 0)
			return(-1);	/* break loop if bad operator
						encountered */
	return 0;
}
/*
 *
 * //// POP OPERATER OFF TOP OF OPSTACK ////
 * //// AND PERFORM THAT OPERATION ON //////
 * //// TOP TWO NUMBERS ON NSTACK //////////
 * //// NSTACK GETS POPPED AND RESULT //////
 * //// IS LEFT ON TOP /////////////////////
 * //// THOSE OPERATIONS ON TOP TWO NUMS /////
 */
unstack(nstack,ostack,n,o)
double	nstack[];
char	ostack[];
int	*n;
int	*o;
{
	switch(ostack[*o])
	{
		case '^':
			nstack[*n+1] = pow(nstack[*n+1],nstack[*n]);
			break;
		case '*':
			nstack[*n+1] = nstack[*n+1] * nstack[*n];
			break;
		case '/':
			if(nstack[*n] == 0.0) /* catch overflow */
			{
				error(inst.thing.linno, 29); /* OVERFLOW ERR */
				break;
			}
			nstack[*n+1] = nstack[*n+1] / nstack[*n];
			break;
		case '+':
			nstack[*n+1] = nstack[*n+1] + nstack[*n];
			break;
		case '-':
			nstack[*n+1] = nstack[*n+1] - nstack[*n];
			break;
		default:
			error(inst.thing.linno, 8);
			return(-1);
			break;
	}
	(*n)++;		/* pop number stack */
	(*o)++;		/* pop operator stack */
	return(0);
}
/*
 *
 * ////// CALL MATH FUNCTION ////////
 *
 *
 *
 * callin format:
 *	value = mathcall(string);
 *
 *	where: value = floating point value returned by call
 *	       string = pointer to a null terminated string containong
 *			the func text and expression within () - 
 *			i.e. "sin(EXPRESSION TEXT)"
 */

/*
 *
 * //////// BASIC INTERPRETER MATH TABLE ////////
 *
 */
#ifndef TEST
double	sin();
double	cos();
double	atan();
double	log();
double	tan();
double	fact();
#ifdef	STRINGS
double	len();
double	val();
double	asc();
#endif
#endif
/*
 * //// INSTRUMENT FUNCTIONS ////
 */
#ifdef	TEST
double	dvmr();
double	button();
double	berror();
#endif
double exp();
double _int(num)
double num;
{
	long	trunc;	
	trunc=num;
	num=trunc;
	return(num);
}
#ifndef TEST
double sqroot(num)
double num;
{
	if(num < 0.0)	
		error(inst.thing.linno, 19);
	return(pow(num, 0.5));	
}

#endif
double absolute(num)
double num;
{
	double	fabs();
	return(fabs(num));
}

double rndgen(num)
double	num;
{
	double	rndnum;
	rndnum = rand();	
	return((rndnum/32767.0) * num + 1);	
}
/*
 *
 * This is the BASIC interpreter math func strin and routine
 * dispatch table.
 *
 */

/*
 * each entry contains the text for the MATH FUNCTION
 * in question and the address of the routine which services it
 */
struct tbl
	{
		char *cmdtxt;
		double (*func)();
	};
#ifndef TEST
struct tbl mathtbl[] = {
	{"sin",		sin},
	{"cos",		cos},
	{"exp",		exp},
	{"log",		log},
	{"int",		_int},
	{"sqr",		sqroot}, 
	{"abs",		absolute},
	{"atn",		atan},
	{"rnd",		rndgen},	
	{"tan",		tan},
	{"fact",	fact},
#ifdef	STRINGS
	{(char *)1,	0},	/* String function fence */
	{"len",		len},
	{"val",		val},
	{"asc",		asc},
#endif
/* THIS STUFF NOT INCLUDED YET
	{"cot",		cotang},
	{"clg",		log10},
	{"sgn",		sign},
*/
	{0,		0}	
	};
#endif
#ifdef TEST
struct tbl mathtbl[] {
	{"abs",		absolute},
	{"rnd",		rndgen},
	{"int",		_int},
	{"exp",		exp},
	{"dvmr",	dvmr},
	{"btn",		button},
	{"error",	berror},
	{0,		0}
	};
#endif
/*
 *
 */
double
mathcall(s)
char	s[];
{
	char	funnam[10]; /* func name copied from 's'
				used for string search */
	register char *x; /* pointer to paren enclosed expression */
	double	evalx();
	double	value;	/* final value returned */
	double	exvalue; /* value of expression before being operated
			     upon by the math func */
	register  int i;		/* index reg. for expediency */
	x = s;		/* set pointer to func string */
	/*
	 * copy the func name part up to the '('
	 * to use for a string search in the table of names
	 */
	for(i=0; *x != '(' ;)
	{
		funnam[i++] = *x++;
	}
	funnam[i] = '\0';	/* null terminate */
	/*
	 * compare each string in table with funnam
	 * when match is found, call using offset code
	 * if end-of-table (null) encountered return 0
	 */
#ifdef	STRINGS
	strflg = 0;
#endif
	for(i=0; mathtbl[i].cmdtxt != 0 ; i++)
	{
#ifdef	STRINGS
		if(mathtbl[i].cmdtxt == (char *)1)
		{
			i++;
			strflg = 1;	/* its a string type */
		}
#endif
		if(!(strcmp(funnam, mathtbl[i].cmdtxt)))
		{
			x++;
#ifdef	STRINGS
			if(strflg == 0)
			{
#endif
				exvalue = evalx(x); 
				value = (*mathtbl[i].func)(exvalue);
				return(value);
#ifdef	STRINGS
			}
			else
			{
				value = (*mathtbl[i].func)(x);
				return(value);
			}
#endif
		}
	}
	error(inst.thing.linno, 17);	/* unknown math function */
	return(0.0);	/* oops not in this table, pal */
}
