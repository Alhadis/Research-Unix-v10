/* Copyright Bell Telephone Laboratories Whippany, N.J.

 *	***********************************
 *	***********************************
 *	****** ERROR MESSAGE PRINTER ******
 *	*** R. B. Drake WH 8C-005 X4163 ***
 *	**** Fri Aug 24 17:21:10 1979 *****
 *	***********************************
 *	***********************************
 *
 * MOD,J.P.Hawkins,17-SEP-79, added error 44 and 45
 * MOD,J.P.Hawkins,31-JAN-81, String handling errors

*/
/*   "@(#) error.c:  V 1.6  11/29/81" */

#include	"bas.h"
#ifndef	LSX
static char *mesg[] =
{
	"REFERS TO A NON EXISTING LINE NUMBER",		/* 0 */
	"UNRECOGNIZABLE OPERATION",			/* 1 */
	"CANNOT OPEN FILE",				/* 2 */
	"ILLEGAL VARIABLE NAME",			/* 3 */
	"BAD FILENAME",					/* 4 */
	"WORKING STORAGE AREA EMPTY",			/* 5 */
	"RUNS NESTED TOO DEEPLY",			/* 6 */
	"UNASSIGNED VARIABLE",				/* 7 */
	"EXPRESSION SYNTAX",				/* 8 */
	"BAD KEYWORD IN STATEMENT",			/* 9 */
	"IMPROPER OR NO RELATIONAL OPERATOR",		/* 10 */
	"UNBALANCED QUOTES",				/* 11 */
	"FILE EDITING NOT PERMITTED IN SINGLE STEP MODE",	/* 12 */
	"MISSING OR ILLEGAL DELIMITER",			/* 13 */
	"GOSUB WITH NO RETURN",				/* 14 */
	"IS FATAL",					/* 15 */
	"UNBALANCED PARENTHESIS",			/* 16 */
	"UNKNOWN MATH FUNCTION",			/* 17 */
	"NEXT WITH NO OR WRONG FOR IN PROGRESS",	/* 18 */
	"CANNOT PROCESS IMAGINARY NUMBER",		/* 19 */
	"WHAT ?",					/* 20 */
	"BAD DIMENSION SYNTAX",				/* 21 */
	"TOO MANY DIMENSIONS",				/* 22 */
	"REDUNDANT DIM STATEMENT",			/* 23 */
	"NOT ENOUGH WORKING STORAGE SPACE",		/* 24 */
	"VARIABLE NOT DIMENSIONED",			/* 25 */
	"WRONG NUM OF DIMS",				/* 26 */
	"ONE OR MORE DIMS LARGER THAN ASSIGNED",	/* 27 */
	"NEG. OR ZERO DIMENSION ILLEGAL",		/* 28 */
	"DIVIDE BY ZERO",				/* 29 */
	"BAD TAB SPEC. IN PRINT",			/* 30 */
	"SYS CALL FAILED",				/* 31 */
	"BAD FILE DECLARE SYNTAX",			/* 32 */
	"OUT OF DATA",					/* 33 */
	"FILE-NAME TOO LONG",				/* 34 */
	"FILE DES. USED UP",				/* 35 */
	"FILE NOT OPEN FOR OUTPUT",			/* 36 */
	"FILE NOT OPEN FOR INPUT",			/* 37 */
	"EXPRESSION YIELDS AN IMPOSSIBLE VALUE",	/* 38 */
	"PRINTF: ARG COUNT MISMATCH",			/* 39 */
	"PRINTF: MORE THAN 10 ARGS",			/* 40 */
	"LINE TOO LONG FOR STRIP PRINTER",		/* 41 */
	"MOV REQUIRES 3 LINE #'s SPACING IS OPTIONAL",	/*42*/
	"BAD NAME OR LINE NUMBER AT BEGINNING OF SUBROUTINE", /* 43 */
	"ELSE WITH NO IF",				/* 44 */
	"ENDIF WITH NO IF",				/* 45 */
	"GOTO NOT ALLOWED INSIDE STRUCTURED 'IF'",	/* 46 */
	"BREAK OR CONTINUE WITH NO FOR IN PROGRESS",		/* 47 */
	"FOR WITHOUT MATCHING NEXT",			/* 48 */
#ifdef	STRINGS
	"STRING VARIABLE IN NUMERICAL EXPR.",		/* 49 */
	"NON-STRING IN STRING ASSIGNMENT",		/* 50 */
	"NUMERIC IN STRING EXPRESSION",			/* 51 */
	"INVALID STRING OPERATOR",			/* 52 */
	"CANNOT COMPARE STRING WITH NUM. TYPES",	/* 53 */
	"UNKNOWN STRING FUNCTION",			/* 54 */
	"OUT OF STRING RANGE",				/* 55 */
#else
	0,						/* 49 */
	0,						/* 50 */
	0,						/* 51 */
	0,						/* 52 */
	0,						/* 53 */
	0,						/* 54 */
	0,						/* 55 */
#endif
	"FACTORIAL NEG. OR TOO LARGE",			/* 56 */
	"SEEK: SYNTAX ERR.",				/* 57 */
	"SEEK FAILED",					/* 58 */
	"ILLEGAL SEEK MODE",				/* 59 */
	"REWIND: SYNTAX ERR.",				/* 60 */
};
#endif

/*
 *
 * //////// TEST SET AND INSTRUMENT ERRORS ////////
 */
/*  	"MISSING ' DELIMITER BEFORE BUS ADDR",		   100 */
/*  	"PS: VOLTAGE OUT OF RANGE",			   101 */
/*  	"PS: CURRENT OUT OF RANGE",			   102 */
/*  	"PS: OVERVOLTAGE OUT OF RANGE",			   103 */
/*  	"RELAY ERR",					   104 */
/*  	"RELAY: INVALID FUNC.",				   105 */
/*  	"RELAY: INVALID NUMBER",			   106 */
/*  	"DVM: INVALID MODE",				   107 */
/*  	"DVM: INVALID RANGE",				   108 */
/*  	"LODSET: IMPROPER NUMBER OF ARGUMENTS",		   109 */
/*  	"LODSET: IMPROPER MODE",			   110 */
/*  	"LODSET: UNABLE TO SET LOAD",			   111 */

error(line,num)
{
	error2(line,num,'\n');
}

error2(line,num,c)
char	c;
int line,num;
{
#ifndef	LSX
	char	*msgadr;

	msgadr = mesg[num];
#endif

	if(line != 0)
#ifdef	LSX
		printf("ERROR #%d, LINE %d.\n",num,line);
#endif
#ifndef	LSX
		printf("ERROR LINE %d: %s%c",line,msgadr,c);
#endif
	else
#ifdef	LSX
		printf("ERROR #%d.\n",num);
#endif
#ifndef	LSX
		printf("ERROR %s%c",msgadr,c);
#endif
}
