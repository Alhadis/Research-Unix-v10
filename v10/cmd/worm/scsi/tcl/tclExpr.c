/* 
 * tclExpr.c --
 *
 *	This file contains the code to evaluate expressions for
 *	Tcl.
 *
 * Copyright 1987 Regents of the University of California
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies.  The University of California
 * makes no representations about the suitability of this
 * software for any purpose.  It is provided "as is" without
 * express or implied warranty.
 */

#ifndef lint
static char rcsid[] = "$Header: /sprite/src/lib/tcl/RCS/tclExpr.c,v 1.13 90/03/22 15:24:59 ouster Exp $ SPRITE (Berkeley)";
#pragma ref rcsid
#endif not lint

#define	_POSIX_SOURCE

#include <stdio.h>
#include <ctype.h>
#include "tcl.h"
#include "tclInt.h"

/*
 * The data structure below describes the state of parsing an expression.
 * It's passed among the routines in this module.
 */

typedef struct {
    Tcl_Interp *interp;		/* Intepreter to use for command execution
				 * and variable lookup. */
    char *originalExpr;		/* The entire expression, as originally
				 * passed to Tcl_Expr. */
    char *expr;			/* Position to the next character to be
				 * scanned from the expression string. */
    int token;			/* Type of the last token to be parsed from
				 * expr.  See below for definitions.
				 * Corresponds to the characters just
				 * before expr. */
    int number;			/* If token is NUMBER, gives value of
				 * the number. */
} ExprInfo;

/*
 * The token types are defined below.  In addition, there is a table
 * associating a precedence with each operator.  The order of types
 * is important.  Consult the code before changing it.
 */

#define NUMBER		0
#define OPEN_PAREN	1
#define CLOSE_PAREN	2
#define END		3
#define UNKNOWN		4

/*
 * Binary operators:
 */

#define MULT		8
#define DIVIDE		9
#define MOD		10
#define PLUS		11
#define MINUS		12
#define LEFT_SHIFT	13
#define RIGHT_SHIFT	14
#define LESS		15
#define GREATER		16
#define LEQ		17
#define GEQ		18
#define EQUAL		19
#define NEQ		20
#define BIT_AND		21
#define BIT_XOR		22
#define BIT_OR		23
#define AND		24
#define OR		25
#define QUESTY		26
#define COLON		27

/*
 * Unary operators:
 */

#define	UNARY_MINUS	28
#define NOT		29
#define BIT_NOT		30

/*
 * Precedence table.  The values for non-operator token types are ignored.
 */

int precTable[] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    11, 11, 11,				/* MULT, DIVIDE, MOD */
    10, 10,				/* PLUS, MINUS */
    9, 9,				/* LEFT_SHIFT, RIGHT_SHIFT */
    8, 8, 8, 8,				/* LESS, GREATER, LEQ, GEQ */
    7, 7,				/* EQUAL, NEQ */
    6,					/* BIT_AND */
    5,					/* BIT_XOR */
    4,					/* BIT_OR */
    3,					/* AND */
    2,					/* OR */
    1, 1,				/* QUESTY, COLON */
    12, 12, 12				/* UNARY_MINUS, NOT, BIT_NOT */
};

/*
 *----------------------------------------------------------------------
 *
 * ExprGetNum --
 *
 *	Parse off a number from a string.
 *
 * Results:
 *	The return value is the integer value corresponding to the
 *	leading digits of string.  If termPtr isn't NULL, *termPtr
 *	is filled in with the address of the character after the
 *	last one that is part of the number.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
ExprGetNum(string, termPtr)
    register char *string;		/* ASCII representation of number.
					 * If leading digit is "0" then read
					 * in base 8;  if "0x", then read in
					 * base 16. */
    register char **termPtr;		/* If non-NULL, fill in with address
					 * of terminating character. */
{
    int result, sign;
    register char c;

    c = *string;
    result = 0;
    if (c == '-') {
	sign = -1;
	string++; c = *string;
    } else {
	sign = 1;
    }
    if (c == '0') {
	string++; c = *string;
	if (c == 'x') {
	    while (1) {
		string++; c = *string;
		if ((c >= '0') && (c <= '9')) {
		    result = (result << 4) + (c - '0');
		} else if ((c >= 'a') && (c <= 'f')) {
		    result = (result << 4) + 10 + (c - 'a');
		} else if ((c >= 'A') && (c <= 'F')) {
		    result = (result << 4) + 10 + (c - 'A');
		} else {
		    break;
		}
	    }
	} else {
	    while ((c >= '0') && (c <= '7')) {
		result = (result << 3) + (c - '0');
		string++;  c = *string;
	    }
	}
    } else {
	while ((c >= '0') && (c <= '9')) {
	    result = (result*10) + (c - '0');
	    string++;  c = *string;
	}
    }
    if (termPtr != NULL) {
	*termPtr = string;
    }
    return result*sign;
}

/*
 *----------------------------------------------------------------------
 *
 * ExprLex --
 *
 *	Lexical analyzer for expression parser.
 *
 * Results:
 *	TCL_OK is returned unless an error occurred while doing lexical
 *	analysis or executing an embedded command.  In that case a
 *	standard Tcl error is returned, using interp->result to hold
 *	an error message.  In the event of a successful return, the token
 *	and (possibly) number fields in infoPtr are updated to refer to
 *	the next symbol in the expression string, and the expr field is
 *	advanced.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
ExprLex(interp, infoPtr)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting. */
    register ExprInfo *infoPtr;		/* Describes the state of the parse. */
{
    register char *p, c;
    char *var, *term;
    int result;

    /*
     * The next token is either:
     * (a)	a variable name (indicated by a $ sign plus a variable
     *		name in the standard Tcl fashion);  lookup the value
     *		of the variable and return its numeric equivalent as a
     *		number.
     * (b)	an embedded command (anything between '[' and ']').
     *		Execute the command and convert its result to a number.
     * (c)	a series of decimal digits.  Convert it to a number.
     * (d)	space:  skip it.
     * (d)	an operator.  See what kind it is.
     */

    p = infoPtr->expr;
    c = *p;
    while (isspace(c)) {
	p++;  c = *p;
    }
    infoPtr->expr = p+1;
    switch (c) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	    infoPtr->token = NUMBER;
	    infoPtr->number = ExprGetNum(p, &infoPtr->expr);
	    return TCL_OK;

	case '$':
	    infoPtr->token = NUMBER;
	    var = Tcl_ParseVar(infoPtr->interp, p, &infoPtr->expr);
	    if (var == NULL) {
		return TCL_ERROR;
	    }
	    if (((Interp *) infoPtr->interp)->noEval) {
		infoPtr->number = 0;
		return TCL_OK;
	    }
	    infoPtr->number = ExprGetNum(var, &term);
	    if ((term == var) || (*term != 0)) {
		c = *infoPtr->expr;
		*infoPtr->expr = 0;
		Tcl_Return(interp, (char *) NULL, TCL_STATIC);
		sprintf(interp->result,
			"variable \"%.50s\" contained non-numeric value \"%.50s\"",
			p, var);
		*infoPtr->expr = c;
		return TCL_ERROR;
	    }
	    return TCL_OK;

	case '[':
	    infoPtr->token = NUMBER;
	    result = Tcl_Eval(infoPtr->interp, p+1, TCL_BRACKET_TERM,
		    &infoPtr->expr);
	    if (result != TCL_OK) {
		return result;
	    }
	    infoPtr->expr++;
	    if (((Interp *) infoPtr->interp)->noEval) {
		infoPtr->number = 0;
		Tcl_Return(interp, (char *) NULL, TCL_STATIC);
		return TCL_OK;
	    }
	    infoPtr->number = ExprGetNum(interp->result, &term);
	    if ((term == interp->result) || (*term != 0)) {
		char string[200];
		infoPtr->expr[-1];
		infoPtr->expr[-1] = 0;
		sprintf(string, "command \"%.50s\" returned non-numeric result \"%.50s\"",
			p+1, interp->result);
		infoPtr->expr[-1] = c;
		Tcl_Return(interp, string, TCL_VOLATILE);
		return TCL_ERROR;
	    }
	    Tcl_Return(interp, (char *) NULL, TCL_STATIC);
	    return TCL_OK;

	case '(':
	    infoPtr->token = OPEN_PAREN;
	    return TCL_OK;

	case ')':
	    infoPtr->token = CLOSE_PAREN;
	    return TCL_OK;

	case '*':
	    infoPtr->token = MULT;
	    return TCL_OK;

	case '/':
	    infoPtr->token = DIVIDE;
	    return TCL_OK;

	case '%':
	    infoPtr->token = MOD;
	    return TCL_OK;

	case '+':
	    infoPtr->token = PLUS;
	    return TCL_OK;

	case '-':
	    infoPtr->token = MINUS;
	    return TCL_OK;

	case '?':
	    infoPtr->token = QUESTY;
	    return TCL_OK;

	case ':':
	    infoPtr->token = COLON;
	    return TCL_OK;

	case '<':
	    switch (p[1]) {
		case '<':
		    infoPtr->expr = p+2;
		    infoPtr->token = LEFT_SHIFT;
		    break;
		case '=':
		    infoPtr->expr = p+2;
		    infoPtr->token = LEQ;
		    break;
		default:
		    infoPtr->token = LESS;
		    break;
	    }
	    return TCL_OK;

	case '>':
	    switch (p[1]) {
		case '>':
		    infoPtr->expr = p+2;
		    infoPtr->token = RIGHT_SHIFT;
		    break;
		case '=':
		    infoPtr->expr = p+2;
		    infoPtr->token = GEQ;
		    break;
		default:
		    infoPtr->token = GREATER;
		    break;
	    }
	    return TCL_OK;

	case '=':
	    if (p[1] == '=') {
		infoPtr->expr = p+2;
		infoPtr->token = EQUAL;
	    } else {
		infoPtr->token = UNKNOWN;
	    }
	    return TCL_OK;

	case '!':
	    if (p[1] == '=') {
		infoPtr->expr = p+2;
		infoPtr->token = NEQ;
	    } else {
		infoPtr->token = NOT;
	    }
	    return TCL_OK;

	case '&':
	    if (p[1] == '&') {
		infoPtr->expr = p+2;
		infoPtr->token = AND;
	    } else {
		infoPtr->token = BIT_AND;
	    }
	    return TCL_OK;

	case '^':
	    infoPtr->token = BIT_XOR;
	    return TCL_OK;

	case '|':
	    if (p[1] == '|') {
		infoPtr->expr = p+2;
		infoPtr->token = OR;
	    } else {
		infoPtr->token = BIT_OR;
	    }
	    return TCL_OK;

	case '~':
	    infoPtr->token = BIT_NOT;
	    return TCL_OK;

	case 0:
	    infoPtr->token = END;
	    infoPtr->expr = p;
	    return TCL_OK;

	default:
	    infoPtr->expr = p+1;
	    infoPtr->token = UNKNOWN;
	    return TCL_OK;
    }
}

/*
 *----------------------------------------------------------------------
 *
 * ExprGetValue --
 *
 *	Parse a "value" from the remainder of the expression in infoPtr.
 *
 * Results:
 *	Normally TCL_OK is returned.  The value of the parsed number is
 *	returned in infoPtr->number.  If an error occurred, then
 *	interp->result contains an error message and TCL_ERROR is returned.
 *
 * Side effects:
 *	Information gets parsed from the remaining expression, and the
 *	expr and token fields in infoPtr get updated.  Information is
 *	parsed until either the end of the expression is reached (null
 *	character or close paren), an error occurs, or a binary operator
 *	is encountered with precedence <= prec.  In any of these cases,
 *	infoPtr->token will be left pointing to the token AFTER the
 *	expression.
 *
 *----------------------------------------------------------------------
 */

int
ExprGetValue(interp, infoPtr, prec)
    Tcl_Interp *interp;			/* Interpreter to use for error
					 * reporting. */
    register ExprInfo *infoPtr;		/* Describes the state of the parse
					 * just before the value (i.e. ExprLex
					 * will be called to get first token
					 * of value). */
    int prec;				/* Treat any un-parenthesized operator
					 * with precedence <= this as the end
					 * of the expression. */
{
    Interp *iPtr = (Interp *) interp;
    int result, operator, operand;
    int gotOp;				/* Non-zero means already lexed the
					 * operator (while picking up value
					 * for unary operator).  Don't lex
					 * again. */

    /*
     * There are two phases to this procedure.  First, pick off an initial
     * value.  Then, parse (binary operator, value) pairs until done.
     */

    gotOp = 0;
    result = ExprLex(interp, infoPtr);
    if (result != TCL_OK) {
	return result;
    }
    if (infoPtr->token == OPEN_PAREN) {

	/*
	 * Parenthesized sub-expression.
	 */

	result = ExprGetValue(interp, infoPtr, -1);
	if (result != TCL_OK) {
	    return result;
	}
	if (infoPtr->token != CLOSE_PAREN) {
	    Tcl_Return(interp, (char *) NULL, TCL_STATIC);
	    sprintf(interp->result,
		    "unmatched parentheses in expression \"%.50s\"",
		    infoPtr->originalExpr);
	    return TCL_ERROR;
	}
    } else {
	if (infoPtr->token == MINUS) {
	    infoPtr->token = UNARY_MINUS;
	}
	if (infoPtr->token >= UNARY_MINUS) {

	    /*
	     * Process unary operators.
	     */

	    operator = infoPtr->token;
	    result = ExprGetValue(interp, infoPtr, precTable[infoPtr->token]);
	    if (result != TCL_OK) {
		return result;
	    }
	    switch (operator) {
		case UNARY_MINUS:
		    infoPtr->number = -infoPtr->number;
		    break;
		case NOT:
		    infoPtr->number = !infoPtr->number;
		    break;
		case BIT_NOT:
		    infoPtr->number = ~infoPtr->number;
		    break;
	    }
	    gotOp = 1;
	} else if (infoPtr->token != NUMBER) {
	    goto syntaxError;
	}
    }

    /*
     * Got the first operand.  Now fetch (operator, operand) pairs.
     */

    if (!gotOp) {
	result = ExprLex(interp, infoPtr);
	if (result != TCL_OK) {
	    return result;
	}
    }
    while (1) {
	operand = infoPtr->number;
	operator = infoPtr->token;
	if ((operator < MULT) || (operator >= UNARY_MINUS)) {
	    if ((operator == END) || (operator == CLOSE_PAREN)) {
		return TCL_OK;
	    } else {
		goto syntaxError;
	    }
	}
	if (precTable[operator] <= prec) {
	    return TCL_OK;
	}

	/*
	 * If we're doing an AND or OR and the first operand already
	 * determines the result, don't execute anything in the
	 * second operand:  just parse.  Same style for ?: pairs.
	 */

	if (((operator == AND) && !operand)
		|| ((operator == OR) && operand)) {
	    iPtr->noEval++;
	    result = ExprGetValue(interp, infoPtr, precTable[operator]);
	    iPtr->noEval--;
	} else if (operator == QUESTY) {
	    if (operand != 0) {
		result = ExprGetValue(interp, infoPtr, precTable[operator]);
		operand = infoPtr->number;
		if (result != TCL_OK)
		    return result;
		if (infoPtr->token != COLON)
		    goto syntaxError;
		iPtr->noEval++;
		result = ExprGetValue(interp, infoPtr, precTable[operator]);
		iPtr->noEval--;
	    } else {
		iPtr->noEval++;
		result = ExprGetValue(interp, infoPtr, precTable[operator]);
		iPtr->noEval--;
		if (result != TCL_OK)
		    return result;
		if (infoPtr->token != COLON)
		    goto syntaxError;
		result = ExprGetValue(interp, infoPtr, precTable[operator]);
		operand = infoPtr->number;
	    }
	    infoPtr->number = operand;
	} else {
	    result = ExprGetValue(interp, infoPtr, precTable[operator]);
	}
	if (result != TCL_OK) {
	    return result;
	}
	if ((infoPtr->token < MULT) && (infoPtr->token != NUMBER)
		&& (infoPtr->token != END)
		&& (infoPtr->token != CLOSE_PAREN)) {
	    goto syntaxError;
	}
	switch (operator) {
	    case MULT:
		infoPtr->number = operand * infoPtr->number;
		break;
	    case DIVIDE:
		if (infoPtr->number == 0) {
		    Tcl_Return(interp, "divide by zero", TCL_STATIC);
		    return TCL_ERROR;
	        }
		infoPtr->number = operand / infoPtr->number;
		break;
	    case MOD:
		if (infoPtr->number == 0) {
		    Tcl_Return(interp, "divide by zero", TCL_STATIC);
		    return TCL_ERROR;
	        }
		infoPtr->number = operand % infoPtr->number;
		break;
	    case PLUS:
		infoPtr->number = operand + infoPtr->number;
		break;
	    case MINUS:
		infoPtr->number = operand - infoPtr->number;
		break;
	    case LEFT_SHIFT:
		infoPtr->number = operand << infoPtr->number;
		break;
	    case RIGHT_SHIFT:
		infoPtr->number = operand >> infoPtr->number;
		break;
	    case LESS:
		infoPtr->number = operand < infoPtr->number;
		break;
	    case GREATER:
		infoPtr->number = operand > infoPtr->number;
		break;
	    case LEQ:
		infoPtr->number = operand <= infoPtr->number;
		break;
	    case GEQ:
		infoPtr->number = operand >= infoPtr->number;
		break;
	    case EQUAL:
		infoPtr->number = operand == infoPtr->number;
		break;
	    case NEQ:
		infoPtr->number = operand != infoPtr->number;
		break;
	    case BIT_AND:
		infoPtr->number = operand & infoPtr->number;
		break;
	    case BIT_XOR:
		infoPtr->number = operand ^ infoPtr->number;
		break;
	    case BIT_OR:
		infoPtr->number = operand | infoPtr->number;
		break;
	    case AND:
		infoPtr->number = operand && infoPtr->number;
		break;
	    case OR:
		infoPtr->number = operand || infoPtr->number;
		break;
	}
    }

    syntaxError:
    Tcl_Return(interp, (char *) NULL, TCL_STATIC);
    sprintf(interp->result, "syntax error in expression \"%.50s\"",
	    infoPtr->originalExpr);
    return TCL_ERROR;
}

/*
 *----------------------------------------------------------------------
 *
 * Tcl_Expr --
 *
 *	Parse and evaluate an expression.
 *
 * Results:
 *	The return value is TCL_OK if the expression was correctly parsed;
 *	if there was a syntax error or some other error during parsing,
 *	then another Tcl return value is returned and Tcl_Result points
 *	to an error message.  If all went well, *valuePtr is filled in
 *	with the result corresponding to the expression string.
 *
 * Side effects:
 *	None.
 *
 *----------------------------------------------------------------------
 */

int
Tcl_Expr(interp, string, valuePtr)
    Tcl_Interp *interp;		/* Intepreter to use for variables etc. */
    char *string;		/* Expression to evaluate. */
    int *valuePtr;		/* Where to store result of evaluation. */
{
    ExprInfo info;
    int result;

    info.interp = interp;
    info.originalExpr = string;
    info.expr = string;
    result = ExprGetValue(interp, &info, -1);
    if (result != TCL_OK) {
	return result;
    }
    if (info.token != END) {
	Tcl_Return(interp, (char *) NULL, TCL_STATIC);
	sprintf(interp->result, "syntax error in expression \"%.50s\"", string);
	return TCL_ERROR;
    }
    *valuePtr = info.number;
    return TCL_OK;
}
