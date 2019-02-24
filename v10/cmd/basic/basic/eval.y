/*	"@(#) eval.y: V 1.19 6/16/83"	*/

/*
*********EXPRESSION EVALUATOR******************
*/

%token STRCONST
%token FIXCONST
%token SNGCONST
%token DBLCONST
%token INTCONST
%token OCTCONST
%token DECCONST
%token HEXCONST
%token STRID
%token INTID
%token SNGID
%token DBLID
%token STRAID
%token INTAID
%token SNGAID
%token DBLAID
%token STRUID
%token INTUID
%token SNGUID
%token DBLUID
%token FUNCID
%token LPAR
%token RPAR
%token COMMA
%token LPOUND
%token LDOT
%token LSEMI
%token LBANG
%token LEXP
%token LMINUS
%token LPLUS
%token LMULT
%token LDIV
%token LIDIV
%token LMOD
%token LEQ
%token LNE
%token LLE
%token LLT
%token LGE
%token LGT
%token LNOT
%token LAND
%token LOR
%token LXOR
%token LIMP
%token LEQV
%token GLITCH	/* code for the function input$ which has an optional
		   pound sign in front of the option second argument */
%token TOKERR	/* must be last token.  > TOKERR signals embedded keyword */

%{
#include	<errno.h>
#include	<math.h>
#include	<setjmp.h>
#include	<stdio.h>
#include	<values.h>
#include	<bas.h>
#include	<ctype.h>

char	tokstr[LINMAX+1]; 		/* from lexical analyzer - yylex() */

/***** GLOBALS SET BY EVAL **************/

char		*eoexpr;  /* points to end of expression - set by yylex() */
short		exprtype; /* type of return value */
struct value	exprval;  /* return value */

/************* EXTERNALS *****************/

jmp_buf	overenv;	  		/* for overflow trapping */
extern	int		errno;
extern	char		*strget();
extern struct sym	*stinsert(),
			*stsrch(),
			*paramptr;
extern struct FUNCTAB	functab[];
extern double 		atof1(),
			pow();

/*********** SEMANTIC STACK **************/

#define		STAKMAX		20

STAKREC semstack[STAKMAX],
	*semtop = semstack - 1;

char	strstack[STRMAX*4],	/* string stack */
	*strtop  = strstack;

/*******MACROS FOR ERROR CHECKING*******/

#define	NUMCHEK if ((semtop-1)->type == STRING || semtop->type == STRING)\
		{\
			error(inst.thing.linno,inst.thing.stmtno, 49);\
			return(-1);\
		}

#define MIXCHEK if (((semtop-1)->type == STRING && semtop->type != STRING)\
	  	     || ((semtop-1)->type != STRING && semtop->type == STRING))\
		{\
			error(inst.thing.linno,inst.thing.stmtno, 80);\
			return(-1);\
		}

#define STAKCHEK if (semtop >= (semstack + STAKMAX - 1))\
		 {\
			error(inst.thing.linno,inst.thing.stmtno, 48);\
			return(-1);\
		 }

#define STRCHEK(STRPTR)	if ((strtop + strlen(STRPTR)) > (strstack + STRMAX * 4))\
			{\
				error(inst.thing.linno,inst.thing.stmtno, 48);\
				return(-1);\
			}
%}

%left LIMP
%left LEQV
%left LXOR
%left LOR
%left LAND
%left LNOT
%left LEQ LNE LLE LLT LGE LGT
%left LPLUS LMINUS
%left LMOD
%left LIDIV
%left LMULT LDIV
%left UMINUS
%left LEXP

%type <ival> exprlist FUNCID GLITCH INTCONST FIXCONST
%type <symptr> id STRID INTID SNGID DBLID STRAID INTAID SNGAID DBLAID
%type <symptr> STRUID INTUID SNGUID DBLUID

%start start

%%
start		:	expr
			{
			exprtype = semtop->type;

			switch (semtop->type)
			{
			case STRING  : strcpy(exprval.sval, semtop->val.sval);
				       break;
			case INTEGER : exprval.ival = semtop->val.ival;
				       break;
			case SINGLE  : exprval.fval = semtop->val.fval;
				       break;
			case DOUBLE  : exprval.dval = semtop->val.dval;
				       break;
			}
			}

		|	error
			{
			return(-1);
			}
		;

expr		:	expr LEQV expr
			{
			if (eqv() < 0)
				return(-1);
			}

		|	expr LIMP expr
			{
			if (imp() < 0)
				return(-1);
			}

		|	expr LOR expr
			{
			if (or() < 0)
				return(-1);
			}

		|	expr LXOR  expr
			{
			if (xor() < 0)
				return(-1);
			}

		|	expr LAND expr
			{
			if (and() < 0)
				return(-1);
			}

		|	LNOT expr
			{
			if (semtop->type == STRING)
			{
				error(inst.thing.linno,inst.thing.stmtno, 49);
				return(-1);
			}
			switch (semtop->type)
			{
			case DOUBLE:
				semtop->val.ival = ~mkint(semtop->val.dval);
				break;
			case SINGLE:
				semtop->val.ival = 
					       ~mkint((double)semtop->val.fval);
				break;
			case INTEGER:
				semtop->val.ival = ~ semtop->val.ival;
				break;
			}
			semtop->type = INTEGER;
			}

		|	expr LEQ expr
			{
			if (eq() < 0)
				return(-1);
			}

		|	expr LNE expr
			{
			if (ne() < 0)
				return(-1);
			}

		|	expr LLE expr
			{
			if (le() < 0)
				return(-1);
			}

		|	expr LLT expr
			{
			if (lt() < 0)
				return(-1);
			}

		|	expr LGE expr
			{
			if (ge() < 0)
				return(-1);
			}

		|	expr LGT expr
			{
			if (gt() < 0)
				return(-1);
			}

		|	expr LPLUS  expr
			{
			if (plus() < 0)
				return(-1);
			}

		|	expr LMINUS expr
			{
			if (minus() < 0)
				return(-1);
			}

		|	expr LMOD expr
			{
			if (mod() < 0)
				return(-1);
			}

		|	expr LIDIV expr
			{
			if (idiv() < 0)
				return(-1);
			}

		|	expr LMULT expr
			{
			if (mult() < 0)
				return(-1);
			}

		|	expr LDIV  expr
			{
			if (div() < 0)
				return(-1);
			}

		|	LMINUS expr	%prec UMINUS
			{
			switch (semtop->type)
			{
			case DOUBLE:
				semtop->val.dval = -semtop->val.dval;
				break;
			case SINGLE:
				semtop->val.fval = -semtop->val.fval;
				break;
			case INTEGER:
				semtop->val.ival = -semtop->val.ival;
				break;
			default:
				error(inst.thing.linno,inst.thing.stmtno, 49);
				return(-1);
			}
			}

		|	LPLUS expr	%prec UMINUS

		|	expr LEXP expr
			{
			if (raise() < 0)
				return(-1);
			}

		|	atom
		;

atom		:	STRCONST
			{
			register char	*tptr;

			STAKCHEK
			(++semtop)->val.sval = strtop;
			semtop->type = STRING;
			tptr = tokstr;
			STRCHEK(tptr);
			while (*strtop++ = *tptr++) ;
			if ((strtop - semtop->val.sval) > STRMAX)
			{
				error(inst.thing.linno,inst.thing.stmtno, 87);
				return(-1);
			}
			}

		|	INTCONST
			{
			STAKCHEK
			++semtop;
			if (setjmp(overenv) != 0)
				semtop->val.dval = MAXDOUBLE;
			else if (( semtop->val.dval = atof1(tokstr))
							== MAXDOUBLE)
				error(inst.thing.linno,inst.thing.stmtno, 60);
			if (semtop->val.dval >= -MAXINT 
				&& semtop->val.dval <= MAXINT)
			{
				semtop->val.ival = semtop->val.dval;
				semtop->type = INTEGER;
			} 
			else if ((semtop->val.dval >= -maxfloat 
				&& semtop->val.dval <= maxfloat)
				&& $1 <= 7)
			{
				semtop->val.fval = semtop->val.dval;
				semtop->type = SINGLE;
			} 
			else
				semtop->type = DOUBLE;
			}

		|	FIXCONST
			{
			STAKCHEK
			++semtop;
			if (setjmp(overenv) != 0)
				semtop->val.dval = MAXDOUBLE;
			else if (( semtop->val.dval = atof1(tokstr))
							== MAXDOUBLE)
				error(inst.thing.linno,inst.thing.stmtno, 60);
			if ($1 > 7 || semtop->val.dval < -maxfloat 
					       || semtop->val.dval > maxfloat)
				semtop->type = DOUBLE;
			else
			{
				semtop->val.fval = semtop->val.dval;
				semtop->type = SINGLE;
			} 
			}

		| 	DBLCONST
			{
			STAKCHEK
			++semtop;
			if (setjmp(overenv) != 0)
				semtop->val.dval = MAXDOUBLE;
			else if ((semtop->val.dval = atof1(tokstr))
							== MAXDOUBLE)
				error(inst.thing.linno,inst.thing.stmtno, 60);
			semtop->type = DOUBLE;
			}

		| 	SNGCONST
			{
			STAKCHEK
			++semtop;
			if (setjmp(overenv) != 0)
				semtop->val.fval = maxfloat;
			else 
				semtop->val.fval = mkfloat(atof1(tokstr));
			semtop->type = SINGLE;
			}

		|	OCTCONST
			{
			STAKCHEK
			(++semtop)->type = INTEGER;
			if (octtoi(tokstr, &(semtop->val.ival)) < 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 8);
				return(-1);
			}
			}

		|	DECCONST
			{
			STAKCHEK
			(++semtop)->type = INTEGER;
			if (dectoi(tokstr, &(semtop->val.ival)) < 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 8);
				return(-1);
			}
			}

		|	HEXCONST
			{
			STAKCHEK
			(++semtop)->type = INTEGER;
			if (hextoi(tokstr, &(semtop->val.ival)) < 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 8);
				return(-1);
			}
			}

		|	FUNCID
			{
			errno = 0;
			if (((*(functab[$1 - 1].handler))(0)) < 0)
				return(-1);
			}

		|	FUNCID LPAR exprlist RPAR
			{
			errno = 0;
			if (((*(functab[$1 - 1].handler))($3)) < 0)
				return(-1);
			}

		|	GLITCH LPAR expr RPAR
			{
			errno = 0;
			if (((*(functab[$1 - 1].handler))(1)) < 0)
				return(-1);
			}

		|	GLITCH LPAR expr COMMA expr RPAR
			{
			errno = 0;
			if (((*(functab[$1 - 1].handler))(2)) < 0)
				return(-1);
			}

		|	GLITCH LPAR expr COMMA LPOUND expr RPAR
			{
			errno = 0;
			if (((*(functab[$1 - 1].handler))(2)) < 0)
				return(-1);
			}

		|	id
			{
			register char	*tptr;

			STAKCHEK
			switch ($1->symtype)
			{
			case DBLID:
				if ($1->isbufvar)
				{
					if (getdouble($1->data.bufvar.start,
						&((++semtop)->val.dval)) < 0)
						return(-1);
				} else
					(++semtop)->val.dval = $1->data.dval;
				semtop->type = DOUBLE;
				break;
			case SNGID:
				if ($1->isbufvar)
				{
					float	tmp;

					if (getfloat($1->data.bufvar.start,
						& tmp) < 0)
						return(-1);
					(++semtop)->val.fval = tmp;
				} else
					(++semtop)->val.fval = $1->data.fval;
				semtop->type = SINGLE;
				break;
			case INTID:
				if ($1->isbufvar)
				{
					int	i;
					char	*to,
						*from;

					to = (char *) & (++semtop)->val.ival;
					from = $1->data.bufvar.start;
					for (i=0; i<$1->data.bufvar.length; i++)
						*to++ = *from++;
				} else
					(++semtop)->val.ival = $1->data.ival;
				semtop->type = INTEGER;
				break;
			case STRID:
				(++semtop)->val.sval = strtop;
				semtop->type = STRING;
				if ($1->isbufvar)
				{
					int	i;
					char	*from;

					if ((strtop + $1->data.bufvar.length+1) 
						> (strstack+STRMAX*4))
					{
						error(inst.thing.linno,inst.thing.stmtno, 48);
						return(-1);
					}
					from = $1->data.bufvar.start;
					for (i=0; i<$1->data.bufvar.length; i++)
						*strtop++ = *from++;
					*strtop++ = '\0';
				} else {
					tptr = $1->data.string;
					STRCHEK(tptr);
					while (*strtop++ = *tptr++) ;
				}
				break;
			case DBLUID:
			case SNGUID:
			case INTUID:
			case STRUID:
				if (ufunceval($1, 0) < 0)
					return(-1);
			}
			}

		|	id LPAR exprlist RPAR
			{
			short		dimlist[MAXDIM],
					offset,
					*makeheader();
			register 	i;
			long		size;
			register char	*tptr;

			switch ($1->symtype)
			{
			case DBLAID:
				if ($1->data.darr.header == NULL) 
				/* auto dimension */
				{
					for (i=0; i < $3; i++)
						dimlist[i] = DEFDIM;
					if (($1->data.darr.header = makeheader($3, dimlist, &size)) == NULL)
					{
						stremove($1->name, $1->symtype);
						return(-1);
					}
					if (($1->data.darr.arblok = (double *) calloc((unsigned) size, (unsigned) sizeof(double))) == NULL)
					{
						stremove($1->name, $1->symtype);
						error(inst.thing.linno,inst.thing.stmtno, 84);
						return(-1);
					}
					for (i=0; i < size; i++)
						*($1->data.darr.arblok + i) = 0.0;
				}
				if (mkdimlist($3, dimlist) < 0)
					return(-1);
				if ((offset = getoffset($1->data.darr.header, $3, dimlist)) < 0)
					return(-1);
				semtop -= $3 - 1;
				semtop->type = DOUBLE;
				semtop->val.dval = *(($1->data.darr.arblok) + offset);
				break;

			case SNGAID:
				if ($1->data.farr.header == NULL) 
				/* auto dimension */
				{
					for (i=0; i < $3; i++)
						dimlist[i] = DEFDIM;
					if (($1->data.farr.header = makeheader($3, dimlist, &size)) == NULL)
					{
						stremove($1->name, $1->symtype);
						return(-1);
					}
					if (($1->data.farr.arblok = (float *) calloc((unsigned) size, (unsigned) sizeof(float))) == NULL)
					{
						stremove($1->name, $1->symtype);
						error(inst.thing.linno,inst.thing.stmtno, 84);
						return(-1);
					}
					for (i=0; i < size; i++)
						*($1->data.farr.arblok + i) = 0.0;
				}
				if (mkdimlist($3, dimlist) < 0)
					return(-1);
				if ((offset = getoffset($1->data.farr.header, $3, dimlist)) < 0)
					return(-1);
				semtop -= $3 - 1;
				semtop->type = SINGLE;
				semtop->val.fval = *(($1->data.farr.arblok) + offset);
				break;

			case INTAID:
				if ($1->data.iarr.header == NULL) 	
				/* auto dimension */
				{
					for (i=0; i < $3; i++)
						dimlist[i] = DEFDIM;
					if (($1->data.iarr.header = makeheader($3, dimlist, &size)) == NULL)
					{
						stremove($1->name, $1->symtype);
						return(-1);
					}
					if (($1->data.iarr.arblok = (int *) calloc((unsigned) size, (unsigned) sizeof(int))) == NULL)
					{
						stremove($1->name, $1->symtype);
						error(inst.thing.linno,inst.thing.stmtno, 84);
						return(-1);
					}
					for (i=0; i < size; i++)
						*($1->data.iarr.arblok + i) = 0;
				}
				if (mkdimlist($3, dimlist) < 0)
					return(-1);
				if ((offset = getoffset($1->data.iarr.header, $3, dimlist)) < 0)
					return(-1);
				semtop -= $3 - 1;
				semtop->type = INTEGER;
				semtop->val.ival = *(($1->data.iarr.arblok) + offset);
				break;
			case STRAID:
				if ($1->data.sarr.header == NULL)
				/* auto dimension */
				{
					for (i=0; i < $3; i++)
						dimlist[i] = DEFDIM;
					if (($1->data.sarr.header = makeheader($3, dimlist, &size)) == NULL)
					{
						stremove($1->name, $1->symtype);
						return(-1);
					}
					if (($1->data.sarr.arblok = (char **) calloc((unsigned) size, (unsigned) sizeof(char *))) == NULL)
					{
						stremove($1->name, $1->symtype);
						error(inst.thing.linno,inst.thing.stmtno, 84);
						return(-1);
					}
					for (i=0; i < size; i++)
						*($1->data.sarr.arblok + i) = strget("");
				}
				if (mkdimlist($3, dimlist) < 0)
					return(-1);
				if ((offset = getoffset($1->data.sarr.header, $3, dimlist)) < 0)
					return(-1);
				semtop -= $3 - 1;
				semtop->type = STRING;
				tptr = *(($1->data.sarr.arblok) + offset);
				STRCHEK(tptr);
				semtop->val.sval = strtop;
				while (*strtop++ = *tptr++) ;
				break;
			case DBLUID:
			case SNGUID:
			case INTUID:
			case STRUID:
				if (ufunceval($1, $3) < 0)
					return(-1);
			}
			}

		|	LPAR expr RPAR
		;

id		:	STRID
			{
			if (($$ = stsrch(tokstr, STRID)) == NULL)
				if (($$ = stinsert(tokstr, STRID)) == NULL)
					return(-1);
				else
					$$->data.string = strget("");
			}

		|	STRAID
			{
			if (($$ = stsrch(tokstr, STRAID)) == NULL)
				if (($$ = stinsert(tokstr, STRAID)) == NULL)
					return(-1);
			}

		|	STRUID
			{
			if (($$ = stsrch(tokstr, STRUID)) == NULL)
			{
				error(inst.thing.linno,inst.thing.stmtno, 10);
				return(-1);
			}
			}

		|	INTID
			{
			if (($$ = stsrch(tokstr, INTID)) == NULL)
				if (($$ = stinsert(tokstr, INTID)) == NULL)
					return(-1);
				else
					$$->data.ival = 0;
			}

		|	INTAID
			{
			if (($$ = stsrch(tokstr, INTAID)) == NULL)
				if (($$ = stinsert(tokstr, INTAID)) == NULL)
					return(-1);
			}

		|	INTUID
			{
			if (($$ = stsrch(tokstr, INTUID)) == NULL)
			{
				error(inst.thing.linno,inst.thing.stmtno, 10);
				return(-1);
			}
			}

		|	SNGID
			{
			if (($$ = stsrch(tokstr, SNGID)) == NULL)
				if (($$ = stinsert(tokstr, SNGID)) == NULL)
					return(-1);
				else
					$$->data.fval = 0.0;
			}

		|	SNGAID
			{
			if (($$ = stsrch(tokstr, SNGAID)) == NULL)
				if (($$ = stinsert(tokstr, SNGAID)) == NULL)
					return(-1);
			}

		|	SNGUID
			{
			if (($$ = stsrch(tokstr, SNGUID)) == NULL)
			{
				error(inst.thing.linno,inst.thing.stmtno, 10);
				return(-1);
			}
			}

		|	DBLID
			{
			if (($$ = stsrch(tokstr, DBLID)) == NULL)
				if (($$ = stinsert(tokstr, DBLID)) == NULL)
					return(-1);
				else
					$$->data.dval = 0.0;
			}

		|	DBLAID
			{
			if (($$ = stsrch(tokstr, DBLAID)) == NULL)
				if (($$ = stinsert(tokstr, DBLAID)) == NULL)
					return(-1);
			}

		|	DBLUID
			{
			if (($$ = stsrch(tokstr, DBLUID)) == NULL)
			{
				error(inst.thing.linno,inst.thing.stmtno, 10);
				return(-1);
			}
			}
		;

exprlist	:	exprlist COMMA expr
			{
			$$ = $1 + 1;
			}

		|	expr
			{
			$$ = 1;
			}
		;
%%
eval(evstring)
char	*evstring;
{
	STAKREC	*oldsem;
	char	*oldstr;

	eoexpr = evstring;
	oldsem = semtop;
	oldstr = strtop;
	if (yyparse())
	{
		semtop = oldsem;
		strtop = oldstr;
		return(-1);
	}
	else
	{
		semtop = oldsem;
		strtop = oldstr;
		return(0);
	}
}


yyerror(errstring)
char	*errstring;
{
	error(inst.thing.linno,inst.thing.stmtno, 8);
}



yylex()
{
	extern	int	rinput();
	int		retval;
	register char	*tptr;
	register	trailing;

	switch (retval = lex(&eoexpr, tokstr))
	{
	case FUNCID:
		yylval.ival = tokstr[1] & 0377;
		if (functab[yylval.ival-1].handler == rinput)
			return(GLITCH);
		else
			return(FUNCID);
	case INTCONST:
			yylval.ival = trailing = 0;
			tptr = tokstr;
			while (*tptr == '0') 
				tptr++;
			while (1)
				switch (*tptr++)
				{
				case '\0' :
					yylval.ival -= trailing;
					return(INTCONST);
				case '0'  :
					yylval.ival++;
					trailing++;
					break;
				default   :
					yylval.ival++;
					trailing = 0;
					break;
				}
	case FIXCONST:
			yylval.ival = trailing = 0;
			tptr = tokstr;
			while (*tptr == '0' || *tptr == '.') 
				tptr++;
			while (1)
				switch (*tptr++)
				{
				case '\0':
					yylval.ival -= trailing;
					return(FIXCONST);
				case '0' :
					yylval.ival++;
					trailing++;
					break;
				case '.' :
					break;
				default  :
					yylval.ival++;
					trailing = 0;
					break;
				}
	default:
			break;
	}
	if (retval > TOKERR)	/* keyword encountered, back up and stop */
	{
		eoexpr--;
		return(0);
	}
	return(retval);
}


dmpstak(msg)
{
	STAKREC	*currec;

	printf("Semantic Stack Dump %s\n", msg);
	currec = semtop;
	while (currec >= semstack)
		switch(currec->type)
		{
		case DOUBLE :   printf("DOUBLE: %.40g\n", (currec--)->val.dval);
				break;
		case SINGLE :   printf("SINGLE: %.40g\n", (currec--)->val.fval);
				break;
		case INTEGER :  printf("INTEGER: %d\n", (currec--)->val.ival);
				break;
		case STRING :   printf("STRING: %s\n", (currec--)->val.sval);
				break;
		}
}


/*	evaluates calls to user-defined functions	*/
ufunceval(stptr, numargs)
struct sym	*stptr;
int		numargs;
{
	struct sym	*oldparam,
			*curarg;
	char		savetok[LINMAX+1],
			*oldlexptr,
			*tptr;
	int		actargs,
			oldyylval;

	if (stptr->scratch)
	{	/* recursive call */
		stptr->scratch = 0;
		error(inst.thing.linno,inst.thing.stmtno, 65);
		return(-1);
	}
	/* save active parameter and globals */
	oldparam = paramptr;
	strcpy(savetok, tokstr);
	oldyylval = yylval.ival;
	oldlexptr = eoexpr;
	if (stptr->data.udef.arg == NULL)
	{
		if (numargs != 0)
		{
			error(inst.thing.linno,inst.thing.stmtno, 52);
			return(-1);
		} else
			paramptr = NULL;
	}
	else
	{
		curarg = paramptr = stptr->data.udef.arg;
		actargs = 1;
		while (actargs <= numargs)
		{
			if (curarg == NULL)
			{
				error(inst.thing.linno,inst.thing.stmtno, 52);
				return(-1);
			}
			switch (curarg->symtype)
			{
			case DBLID:
				switch (semtop->type)
				{
				case DOUBLE:
					curarg->data.dval = semtop->val.dval;
					break;
				case SINGLE:
					curarg->data.dval = semtop->val.fval;
					break;
				case INTEGER:
					curarg->data.dval = semtop->val.ival;
					break;
				case STRING:
					error(inst.thing.linno,inst.thing.stmtno, 50);
					return(-1);
				}
				break;
			case SNGID:
				switch (semtop->type)
				{
				case DOUBLE:
					curarg->data.fval 
						= mkfloat(semtop->val.dval);
					break;
				case SINGLE:
					curarg->data.fval = semtop->val.fval;
					break;
				case INTEGER:
					curarg->data.fval = semtop->val.ival;
					break;
				case STRING:
					error(inst.thing.linno,inst.thing.stmtno, 50);
					return(-1);
				}
				break;
			case INTID:
				switch (semtop->type)
				{
				case DOUBLE:
					curarg->data.ival 
					     = mkint(semtop->val.dval);
					break;
				case SINGLE:
					curarg->data.ival 
						= mkint(semtop->val.fval);
					break;
				case INTEGER:
					curarg->data.ival = semtop->val.ival;
					break;
				case STRING:
					error(inst.thing.linno,inst.thing.stmtno, 50);
					return(-1);
				}
				break;
			case STRID:
				switch (semtop->type)
				{
				case DOUBLE:
				case SINGLE:
				case INTEGER:
					error(inst.thing.linno,inst.thing.stmtno, 50);
					return(-1);
				case STRING:
					if((curarg->data.string = 
					    strget(semtop->val.sval)) == NULL)
						return(-1);
					strtop = semtop->val.sval;
					break;
				}
				break;
			}
			semtop--;
			actargs++;
			curarg = curarg->next;
		}
		if (curarg != NULL)
		{
			error(inst.thing.linno,inst.thing.stmtno, 52);
			return(-1);
		}
	}

	stptr->scratch = 1;
	if (eval(stptr->data.udef.funcexp) < 0)
	{
		paramptr = oldparam;
		strcpy(tokstr, savetok);
		eoexpr = oldlexptr;
		stptr->scratch = 0;
		return(-1);
	}

	/* restore old parameter and globals */
	curarg = paramptr;
	for (actargs = 1; actargs <= numargs; actargs++)
	{
		if (curarg->symtype == STRID)
			strfree(curarg->data.string);
		curarg = curarg->next;
	}
	paramptr = oldparam;
	strcpy(tokstr, savetok);
	yylval.ival = oldyylval;
	eoexpr = oldlexptr;
	stptr->scratch = 0;

	/* stack result */
	STAKCHEK
	if (stptr->symtype == DBLUID)
	{
		if (exprtype == STRING)
		{
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		}
		else if (exprtype == INTEGER)
			(++semtop)->val.dval = exprval.ival;
		else if (exprtype == SINGLE)
			(++semtop)->val.dval = exprval.fval;
		else 
			(++semtop)->val.dval = exprval.dval;
		semtop->type = DOUBLE;
	}
	if (stptr->symtype == SNGUID)
	{
		if (exprtype == STRING)
		{
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		}
		else if (exprtype == INTEGER)
			(++semtop)->val.fval = exprval.ival;
		else if (exprtype == SINGLE)
			(++semtop)->val.fval = exprval.fval;
		else 
			(++semtop)->val.fval = mkfloat(exprval.dval);
		semtop->type = SINGLE;
	}
	else if (stptr->symtype == INTUID)
	{
		if (exprtype == STRING)
		{
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		}
		else if (exprtype == INTEGER)
			(++semtop)->val.ival = exprval.ival;
		else if (exprtype == SINGLE)
			(++semtop)->val.ival = mkint(exprval.fval);
		else 
			(++semtop)->val.ival = mkint(exprval.dval);
		semtop->type = INTEGER;
	}
	else	/* STRUID */
	{
		if (exprtype != STRING)
		{
			error(inst.thing.linno,inst.thing.stmtno, 51);
			return(-1);
		}
		(++semtop)->val.sval = strtop;
		semtop->type = STRING;
		tptr = exprval.sval;
		STRCHEK(tptr);
		while (*strtop++ = *tptr++) ;
	}
	return(0);
}



/*	makes list of dimensions from values on semantic stack	*/
mkdimlist(numdims, dimlist)
int	numdims;
short	dimlist[];
{
	register	i;

	for (i=0; i < numdims; i++)
		switch ((semtop - numdims + i + 1)->type)
		{
		case STRING:
			error(inst.thing.linno,inst.thing.stmtno, 49);
			return(-1);
		case DOUBLE:
			if ((semtop - numdims + i + 1)->val.dval < -.5)
			{
				error(inst.thing.linno,inst.thing.stmtno, 28);
				return(-1);
			}
			if ((semtop - numdims + i + 1)->val.dval >= MAXSHORT)
			{
				error(inst.thing.linno,inst.thing.stmtno, 86);
				return(-1);
			}
			dimlist[i] = (semtop - numdims + i + 1)->val.dval + .5;
			break;
		case SINGLE:
			if ((semtop - numdims + i + 1)->val.fval < -.5)
			{
				error(inst.thing.linno,inst.thing.stmtno, 28);
				return(-1);
			}
			if ((semtop - numdims + i + 1)->val.fval >= MAXSHORT)
			{
				error(inst.thing.linno,inst.thing.stmtno, 86);
				return(-1);
			}
			dimlist[i] = (semtop - numdims + i + 1)->val.fval + .5;
			break;
		case INTEGER:
			if ((semtop - numdims + i + 1)->val.ival < 0)
			{
				error(inst.thing.linno,inst.thing.stmtno, 28);
				return(-1);
			}
			if ((semtop - numdims + i + 1)->val.ival >= MAXSHORT)
			{
				error(inst.thing.linno,inst.thing.stmtno, 28);
				return(-1);
			}
			dimlist[i] = (semtop - numdims + i + 1)->val.ival;
			break;
		}
	return(0);
}



intify(value)
double	value;
{
	if (value > MAXINT)
		value = MAXINT;
	if (value < -MAXINT)
		value = -MAXINT;
	return((value > 0) ? (int) (value + .5) : (int) (value - .5));
}



mkint(value)
double	value;
{
	if (value > MAXINT)
	{
		error(inst.thing.linno,inst.thing.stmtno, 94);
		value = MAXINT;
	}
	if (value < -MAXINT)
	{
		error(inst.thing.linno,inst.thing.stmtno, 94);
		value = -MAXINT;
	}
	return((value > 0) ? (int) (value + .5) : (int) (value - .5));
}



float
mkfloat(value)
double	value;
{
	if (value > maxfloat)
	{
		error(inst.thing.linno,inst.thing.stmtno, 95);
		value = maxfloat;
	}
	if (value < -maxfloat)
	{
		error(inst.thing.linno,inst.thing.stmtno, 95);
		value = -maxfloat;
	}
	return(value);
}
