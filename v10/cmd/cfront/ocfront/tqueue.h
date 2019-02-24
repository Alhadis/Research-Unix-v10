/*ident	"@(#)ctrans:src/tqueue.h	1.1.1.6" */
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

struct toknode {
	TOK      tok;			/* token for parser */
	bit      used;			/* token has been processed by lalex() */
	YYSTYPE  retval;			/* $arg */
	loc      place;
	toknode* next;
	toknode* last;
		toknode(TOK,YYSTYPE,loc);
		~toknode();
};
extern toknode* front;
extern toknode* rear;

extern void addtok(TOK,YYSTYPE,loc);	/* add tok to rear of Q */
extern TOK deltok(int);			/* take tok from front of Q */
extern TOK tlex();

// interface to parser
extern TOK lalex();
extern int yychar;
extern YYSTYPE yylval;
extern TOK	la_look();
extern void	la_backup( TOK, YYSTYPE );
extern int	la_cast();
extern void	check_decl();
extern void	check_cast();
extern TOK	lalex();

// save / restore implicit inline functions
extern toknode*	save_text();
extern void	restore_text();
