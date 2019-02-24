
/*
 *
 * Simple scanner and recursive descent parser used to handle raster file
 * editing expressions.
 *
 * These were the last routines I wrote and by the time I got done it was
 * pretty clear that I should have written writevalue(), readvalue(), and
 * getvalue() a little differently. Anyway things seem to work OK even
 * though everything could have been better, so I'm not going to change
 * things right now.
 *
 */


#include <stdio.h>
#include <ctype.h>

#include "gen.h"			/* general purpose definitions */
#include "rast.h"			/* Imagen raster file definitions */
#include "buildrast.h"			/* really just for Charinfor def */
#include "editrast.h"			/* defs primarily used by scanner */


FILE		*fpin;			/* everything comes from this FILE */
char		buf[50];		/* input buffer */


Tokens		tokens[] = {		/* tokens recognized by scanner */

	"=",		ASSIGNOP,
	"(",		LPAREN,
	")",		RPAREN,
	"+",		PLUS,
	"-",		MINUS,
	"*",		TIMES,
	"/",		DIVIDE,
	"xref",		XREF,
	"yref",		YREF,
	"height",	HEIGHT,
	"width",	WIDTH,
	"chwidth",	CHWIDTH,
	"endedit",	ENDEDIT,
	"build",	BUILD

};


int		lasttoken;		/* code identifying last token read */


extern Charinfo	charinfo[];		/* used as the symbol table - sort of */
extern		first, last;		/* valid entries are in this closed interval */


/*
 *
 * Everything's done in floating point, so all the routines that handle the
 * parsing need to be declared as float.
 *
 */


float	expr(), exprprime(), term(), termprime(), factor();


/*****************************************************************************/


edit(fp)


    FILE	*fp;			/* reading stuff from this file */


{


    char	name[20];		/* of the character we're editing */
    int		lhs_index;		/* location in charinfo[] */
    int		lhs_field;		/* field we want to change */
    int		value;			/* of the expression */


/*
 *
 * Called when we want to process editing expressions from file *fp.
 *
 */


    fpin = fp;				/* nexttoken() reads from fpin */

    nexttoken();			/* loop assumes we've read first token */

    while ( lasttoken != ENDEDIT  &&  lasttoken != BUILD )  {

	if ( (lhs_field = lasttoken) != XREF && lhs_field != YREF && lhs_field != CHWIDTH )
	    error(FATAL, "illegal assignment");

	fscanf(fpin, "%s", name);
	if ( (lhs_index = lookup(name)) == -1 )
	    error(FATAL, "can't find character %s", name);

	nexttoken();			/* better be an assignment operator */
	if ( lasttoken != ASSIGNOP )
	    error(FATAL, "missing assignment operator");

	nexttoken();			/* first token in the expression */
	value = expr() + .5;

	if ( lhs_field == XREF )
	    writevalue(value, G_XREF, charinfo[lhs_index].glydir + rst[G_XREF].offset);
	else if ( lhs_field == YREF )
	    writevalue(value, G_YREF, charinfo[lhs_index].glydir + rst[G_YREF].offset);
	else if ( lhs_field == CHWIDTH )
	    charinfo[lhs_index].chwidth = value;

    }   /* End while */

}   /* End of edit */


/*****************************************************************************/


lookup(name)


    char	*name;			/* find this guy in charinfo[] */


{


    int		i;			/* loop index for lookups */


/*
 *
 * Looks for character *name in the charinfo[] array. If it's found its
 * index is returned to the caller, otherwise -1 is returned. We need to
 * know the values of first and last so we can properly restrict the search.
 * They're must be declared and set in whatever files use these routines.
 * For now it only includes buildrast.c, but we may want to make the editing
 * stuff more general.
 *
 */


    for ( i = first; i <= last; i++ )
	if ( strcmp(name, charinfo[i].name) == 0 )
	    return(i);

    return(-1);

}   /* End of lookup */


/*****************************************************************************/


nexttoken()


{


    int		i;			/* loop index for lookups in tokens[] */


/*
 *
 * Reads the input file *fpin looking for the next token. A code identifying
 * it is saved in lasttoken and the actual token string is saved in buf[].
 * If we reach EOF lasttoken will be set to ENDEDIT.
 *
 */


    if ( fscanf(fpin, "%1s", buf) == EOF )  {
	lasttoken = ENDEDIT;
	return;
    }	/* End if */

    if ( isdigit(buf[0]) )  {		/* must be a constant - integers only */
	fscanf(fpin, "%[0-9]", &buf[1]);
	lasttoken = CONSTANT;
	return;
    }	/* End if */

    if ( isalpha(buf[0]) )		/* get the rest of the token */
	fscanf(fpin, "%[a-zA-Z]", &buf[1]);

    for ( i = 0; i < sizeof(tokens); i++ )
	if ( strcmp(buf, tokens[i].str) == 0 )  {
	    lasttoken = tokens[i].code;
	    return;
	}   /* End if */

    error(FATAL, "don't recognize token %s", buf);

}    /* End of nexttoken */


/*****************************************************************************/


float expr()


{


/*
 *
 * Handles the productions:
 *
 *		EXPR -> TERM EXPR'
 *
 */


    return(exprprime(term()));

}   /* End of expr */


/*****************************************************************************/


float exprprime(val)


    float	val;			/* add stuff to this guy */


{


/*
 *
 * Handles the productions:
 *
 *		EXPR' -> + TERM EXPR'
 *		EXPR' ->
 *
 */


    if ( lasttoken == PLUS )  {
	nexttoken();
	return(exprprime(val + term()));
    } else if ( lasttoken == MINUS )  {
	nexttoken();
	return(exprprime(val - term()));
    } else return(val);

}   /* End of exprprime */


/*****************************************************************************/


float term()


{


/*
 *
 * Handles the production:
 *
 *		TERM -> FACTOR TERM'
 *
 */


    return(termprime(factor()));

}   /* End of term */


/*****************************************************************************/


float termprime(val)


    float	val;			/* value up to this point */


{


/*
 *
 * Handles the productions:
 *
 *		TERM' -> * FACTOR TERM'
 *		TERM' -> / FACTOR TERM'
 *		TERM' ->
 *
 */


    if ( lasttoken == TIMES )  {
	nexttoken();
	return(termprime(val * factor()));
    } else if ( lasttoken == DIVIDE )  {
	nexttoken();
	return(termprime(val / factor()));
    } else return(val);

}   /* End of termprime */


/*****************************************************************************/


float factor()


{


    float	val;			/* value of the factor */


/*
 *
 * Handles the productions:
 *
 *		FACTOR -> ( EXPR )
 *		FACTOR -> - EXPR
 *		FACTOR -> CONSTANT
 *		FACTOR -> IDENT
 *
 */


    if ( lasttoken == CONSTANT )  {
	val = atoi(buf);
	nexttoken();
    } else if ( lasttoken == MINUS )
	val = exprprime(0);
    else if ( lasttoken == LPAREN )  {
	nexttoken();
	val = expr();
	if ( lasttoken == RPAREN )
	    nexttoken();
	else error(FATAL, "expression syntax error - missing right paren");
    } else if ( lasttoken == XREF || lasttoken == YREF || lasttoken == HEIGHT || lasttoken == WIDTH || lasttoken == CHWIDTH )  {
	val = charvalue(lasttoken);
	nexttoken();
    } else error(FATAL, "syntax error - don't recognize factor");

    return(val);

}   /* End of factor */


/*****************************************************************************/


charvalue(tok)


    int		tok;			/* number of the last token */


{


    char	name[20];		/* name of the character to lookup */
    int		i;			/* its index in charinfo[] */


/*
 *
 * Called when we want to get the value of XREF, YREF, HEIGHT, WIDTH, or
 * CHWIDTH for a character. The guy we'll be looking up is the next string
 * in the input file.
 *
 * It's really clear that things, especially readvalue(), could have been
 * done better - sorry.
 *
 */


    fscanf(fpin, "%s", name);
    if ( (i = lookup(name)) == -1 )
	error(FATAL, "can't find character %s", name);

    if ( tok == XREF )
	return(readvalue(charinfo[i].glydir + rst[G_XREF].offset, rst[G_XREF].size, rst[G_XREF].type));
    else if ( tok == YREF )
	return(readvalue(charinfo[i].glydir + rst[G_YREF].offset, rst[G_YREF].size, rst[G_YREF].type));
    else if ( tok == HEIGHT )
	return(readvalue(charinfo[i].glydir + rst[G_HEIGHT].offset, rst[G_HEIGHT].size, rst[G_HEIGHT].type));
    else if ( tok == WIDTH )
	return(readvalue(charinfo[i].glydir + rst[G_WIDTH].offset, rst[G_WIDTH].size, rst[G_WIDTH].type));
    else error(FATAL, "CHWIDTH not implemented yet");

}   /* End of charvalue */


/*****************************************************************************/


