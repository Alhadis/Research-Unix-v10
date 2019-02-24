/*ident	"@(#)ctrans:src/tqueue.h	1.1.1.2" */
/**************************************************************************

        C++ source for cfront, the C++ compiler front-end
        written in the computer science research center of Bell Labs

        Copyright (c) 1984 AT&T, Inc. All Rights Reserved
        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

tqueue.h:

*****************************************************************************/

#ifndef EOF
#include <stdio.h>
#endif

extern int queue_toks;	// TRUE if tokens should be queued
struct toknode {
	TOK      tok;			/* token for parser */
	YYSTYPE  retval;			/* $arg */
	toknode* next;
	toknode* last;
		toknode(TOK,YYSTYPE);
		~toknode();
};
extern toknode* front;
extern toknode* rear;

//extern int  deltok(int );		/* take tok from front of Q */

extern void addtok(TOK,YYSTYPE);	/* add tok to rear of Q */
extern TOK deltok(int);			/* take tok from front of Q */
extern TOK tlex();
extern TOK lalex();
extern YYSTYPE yylval;
