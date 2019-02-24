/*ident	"@(#)ctrans:src/error.c	1.1.3.15" */
/**************************************************************************

	C++ source for cfront, the C++ compiler front-end
	written in the computer science research center of Bell Labs

	Copyright (c) 1984 AT&T, Inc. All rigths Reserved
	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T, INC.

 error.c :

	write error messages

	Until scan_started != 0 no context can be assumed

***************************************************************************/

#ifdef __cplusplus
#include <stdlib.h>
#endif

#include "cfront.h"
#include "size.h"

int error_count;
static int no_of_warnings;
char scan_started;
extern char emode;

#define ERRTRACE    20

static char* abbrev_tbl[] = {
	" argument",
	" base",
	" class",
	" declaration",
	" expression",
	" function",
	"G",
	"H",
	" initialize",
	"J",
	" constructor",	// 'K' !
	" list",
	" member",
	" name",
	" object",
	" pointer",
	" qualifie",
	" reference",
	" statement",
	" type",
	" undefined",
	" variable",
	" with",
	" expected", // 'X'
	"Y",
	"Z",
};

ea* ea0;

void error_init()
{
	static char errbuf[BUFSIZ];
	setbuf(stderr,errbuf);
	ea0 = new ea;
}

#define INTERNAL 127

void ext(int n)
{
	int useit=n; // to avoid n not used warning during build
// for testing only
//	if (n == INTERNAL)
//	abort();
	exit(error_count?error_count:1);
}

void print_loc()
{
	loc* sl = (Cstmt) ? &Cstmt->where : 0;
	loc* dl = (Cdcl && (Cdcl->base==NAME || Cdcl->base==TNAME)) ? &Cdcl->where : 0;
	if (sl && dl && sl->file==dl->file) {	// Cstmt and Cdcl in same file
		if (sl->line<=dl->line) {
			if (curloc.file==dl->file && curloc.line<dl->line)
				// hack to compensate for YACC's
				// bad manners in the use of line numbers
				sl->put(out_file);
			else
				dl->put(out_file);
		}
		else
			sl->put(out_file);
	}
	else if (sl && sl->file==curr_file)	// Cstmt in current file
		sl->put(out_file);
	else if (dl && dl->file==curr_file)	// Cdcl in current file
		dl->put(out_file);
	else
		curloc.put(out_file);
}

static void print_context()
{
	putc('\n',out_file);
}

static char in_error = 0;
loc dummy_loc;

void yyerror(char* s)
{
	error(s);
}

int error(const char* s)
{
	return error(0,s);
}

int error(int t, const char* s)
{
	return error(t,&dummy_loc,s,*ea0,*ea0,*ea0,*ea0);
}

int error(const char* s, const ea& a0, const ea& a1, const ea& a2, const ea& a3)
{
	return error(0,&dummy_loc,s,a0,a1,a2,a3);
}

int error(loc* lc, const char* s, const ea& a0, const ea& a1, const ea& a2, const ea& a3)
{
	return error(0,lc,s,a0,a1,a2,a3);
}

int error(int t, const char* s, const ea& a0, const ea& a1, const ea& a2, const ea& a3)
{
	return error(t,&dummy_loc,s,a0,a1,a2,a3);
}

int suppress_error;

int error(int t, loc* lc, const char* s, const ea& a0, const ea& a1, const ea& a2, const ea& a3)
/*
	"int" not "void" because of "pch" in lex.c

	legal error types are:
		'w'		warning	 (not counted in error count)
		'd'		debug
		's'		"not implemented" message
		'l'		"compiler limit exceeded" message
    		0		error 
    		'i'		internal error (causes abort)
		't'		error while printing error message
*/
{
	if (suppress_error) return 0;

	if (in_error++)
		if (t == 't')
			t = 'i';
		else if (4 < in_error) {
			fprintf(stderr,"\nOops!, error while handling error\n");
			ext(13);
		}

	FILE * of = out_file;
	out_file = stderr;

	if (!scan_started || t=='t')
		putch('\n');
	else if (lc != &dummy_loc)
		lc->put(out_file);
	else
		print_loc();

	switch (t) {
    	case 0:
		putstring("error: ");
		break;
        case 'w':
		no_of_warnings++;
		putstring("warning: ");
		break;
        case 'l':
		putstring("compiler limit exceeded: ");
		break;
        case 's':
		putstring("sorry, not implemented: ");
		break;
        case 'i':
		if (error_count++) {
			fprintf(out_file,"sorry, %s cannot recover from earlier errors\n",prog_name);
			ext(INTERNAL);
		}
		else
			fprintf(out_file,"internal %s error: ",prog_name);
        }

	ea argv[4];
	ea* a = argv;
	argv[0] = a0;
	argv[1] = a1;
	argv[2] = a2;
	argv[3] = a3;

	int c;

	while (c = *s++) {
		if ('A'<=c && c<='Z')
			putstring(abbrev_tbl[c-'A']);
		else if (c == '%') {
			switch (c = *s++) {
			case 'k':	// TOK assumed passed as an int
			{	TOK x = TOK(a->i);
				if (0<x && x<=MAXTOK && keys[x])
					fprintf(out_file," %s",keys[x]);
				else
					fprintf(out_file," token(%d)",x);
				break;
			}
			case 't':	// Ptype 
			{	Ptype tt = Ptype(a->p);
				if (tt == 0) break;

				putch(' ');
			
				extern int ntok;
				int nt = ntok;
				emode = 1;
				tt->dcl_print(0);
				emode = 0;
				ntok = nt;
				break;
			}
			case 'n':	// Pname
			{	Pname nn = Pname(a->p);
				if (nn && nn->string) {
					// suppress generated class names:
					if (nn->string[0]=='_'
					&& nn->string[1]=='_'
					&& nn->string[2]=='C') break;
					emode = 1;
					putch(' ');
					nn->print();
					emode = 0;
				}
				else
					putstring(" ?");
				break;
			}
			case 'p':	// pointer
			{	char* f = sizeof(char*)==sizeof(int)?" %d":" %ld";
				fprintf(out_file,f,a->p);
				break;
			}
			case 'c':	// char assumed passed as an int
				putch(a->i);
				break;

			case 'd':	// int
				fprintf(out_file," %d",a->i);
				break;

			case 's':	// char*
char *s = ((char *)a->p);
if ( s )
				putst((char*)a->p);
				break;
			}
			a++;
		}
		else
			putch(c);
	}

	if (!scan_started && t!='d') ext(4);
/*
	switch (t) {
	case 'd':
	case 't':
	case 'w':
		putch('\n');
		break;
	default:
*/
		print_context();
/*
	}
*/
	fflush(stderr);
    /* now we may want to carry on */

	out_file = of;

	switch (t) {
	case 't':
		if (--in_error) return 0;
	case 'i': 
		ext(INTERNAL);
	case 0:
	case 'l':
	case 's':
		if (MAXERR<++error_count) {
			fprintf(stderr,"Sorry, too many errors\n");
			ext(7);
		}
	}

	in_error = 0;
	return 0;
}
