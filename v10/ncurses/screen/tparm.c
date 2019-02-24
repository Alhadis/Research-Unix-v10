static  char sccsid[] = "@(#)tparm.c	1.1	(1.9	3/4/83)";
/* Copyright (c) 1979 Regents of the University of California */

#include "curses.h"
#include "term.h"

#ifdef	NONSTANDARD
#include	"ns_curses.h"
#endif

#define	CTRL(c)	('c' & 037)
/* #define _err(msg) 0 */
static char *_err();
char *_branchto();

/*
 * Routine to perform parameter substitution.
 * instring is a string containing printf type escapes.
 * The whole thing uses a stack, much like an HP 35.
 * The following escapes are defined for substituting row/column:
 *
 *	%d	print pop() as in printf
 *	%[0]2d	print pop() like %[0]2d
 *	%[0]3d	print pop() like %[0]3d
 *	%c	print pop() like %c
 *	%s	print pop() like %s
 *	%l	pop() a string and push its length.
 *
 *	%p[1-0]	push ith parm
 *	%P[a-z] set variable
 *	%g[a-z] get variable
 *	%'c'	char constant c
 *	%{nn}	integer constant nn
 *
 *	%+ %- %* %/ %m		arithmetic (%m is mod): push(pop() op pop())
 *	%& %| %^		bit operations:		push(pop() op pop())
 *	%= %> %<		logical operations:	push(pop() op pop())
 *	%! %~			unary operations	push(op pop())
 *	%b			unary BCD conversion
 *	%d			unary Delta Data conversion
 *
 *	%? expr %t thenpart %e elsepart %;
 *				if-then-else, %e elsepart is optional.
 *				else-if's are possible ala Algol 68:
 *				%? c1 %t %e c2 %t %e c3 %t %e c4 %t %e %;
 *
 * all other characters are ``self-inserting''.  %% gets % output.
 */

/* #define tparmdebug */
#ifdef tparmdebug
char *tparm();
#include <stdio.h>
#define DEBUG
FILE *outf;
main()
{
	int so, ul, rev, bl, dim, bold, blank, prot, alt;
	setupterm(getenv("TERM"), 1, 0);
	outf = stdout;
	for (;;) {
		printf("so, ul, rev, bl, dim, bold, blank, prot, alt: ");
		scanf("%d %d %d %d %d %d %d %d %d",
		&so, &ul, &rev, &bl, &dim, &bold, &blank, &prot, &alt);
		printf("-->%s<--\n", tparm(set_attributes,
			so, ul, rev, bl, dim, bold, blank, prot, alt));
	}
}

_prstr(result)
char *result;
{
	register char *cp;

	for (cp=result; *cp; cp++)
		if (*cp >= ' ' && *cp <= '~')
			putchar(*cp);
		else
			printf("\\%o", *cp&0377);
}
#endif

#define push(i) (stack[++top] = (i))
#define pop()	(stack[top--])

/* VARARGS */
char *
tparm(instring, p1, p2, p3, p4, p5, p6, p7, p8, p9)
	char *instring;
	int p1, p2, p3, p4, p5, p6, p7, p8 ,p9;
{
	static char result[32];
	static char added[10];
	int vars[26];
	int stack[10], top = 0;
	register char *cp = instring;
	register char *outp = result;
	register int c;
	register int op;
	int sign;
	int onrow = 0;
	int leadzero = 0;	/* not having leading zero is unimplemented */
	char *xp;

	if (instring == 0)
		return _err("null arg");
	added[0] = 0;
#ifdef tparmdebug
	printf("'");
	_prstr(instring);
	printf("'\n");
#endif

	while (c = *cp++) {
#ifdef tparmdebug
printf("loop, c %c%c%c, top %d @ %d, 2nd %d, out '",
	c, *cp, *(cp+1), stack[top], top, stack[top-1]);
_prstr(result); printf("'\n");
#endif
		if (c != '%') {
			*outp++ = c;
			continue;
		}
		op = stack[top];
		if (*cp == '0') {
			leadzero = 1;
			cp++;
		}
		switch (c = *cp++) {

		/* PRINTING CASES */
		case 'd':
			if (op < 10)
				goto one;
			if (op < 100)
				goto two;
			/* fall into... */

		case '3':
three:
			if (c == '3' && *cp++ != 'd')
				return _err("bad char after %3");
			*outp++ = (op / 100) | '0';
			op %= 100;
			/* fall into... */

		case '2':
			if (op >= 100)
				goto three;
			if (c == '2' && *cp++ != 'd')
				return _err("bad char after %2");
two:	
			*outp++ = op / 10 | '0';
one:
			*outp++ = op % 10 | '0';
			(void) pop();
			continue;

		case 'c':
			/*
			 * This code is worth scratching your head at for a
			 * while.  The idea is that various weird things can
			 * happen to nulls, EOT's, tabs, and newlines by the
			 * tty driver, arpanet, and so on, so we don't send
			 * them if we can help it.  So we instead alter the
			 * place being addessed and then move the cursor
			 * locally using UP or RIGHT.
			 *
			 * This is a kludge, clearly.  It loses if the
			 * parameterized string isn't addressing the cursor
			 * (but hopefully that is all that %c terminals do
			 * with parms).  Also, since tab and newline happen
			 * to be next to each other in ASCII, if tab were
			 * included a loop would be needed.  Finally, note
			 * that lots of other processing is done here, so
			 * this hack won't always work (e.g. the Ann Arbor
			 * 4080, which uses %B and then %c.)
			 */
			switch (op) {
				/*
				 * Null.  Problem is that our output is, by
				 * convention, null terminated.
				 */
			case 0:
				op = 0200;   /* Parity should be ignored */
				break;
				/*
				 * Control D.  Problem is that certain very
				 * ancient hardware hangs up on this, so the
				 * current (!) UNIX tty driver doesn't xmit
				 * control D's.
				 */
			case CTRL(d):
				/*
				 * Newline.  Problem is that UNIX will expand
				 * this to CRLF.
				 */
			case '\n':
				xp = onrow ? cursor_down : cursor_right;
				if (onrow && xp && op < lines-1 && cursor_up) {
					op += 2;
					xp = cursor_up;
				}
				if (xp && instring == cursor_address) {
					strcat(added, xp);
					op--;
				}
				break;
				/*
				 * Tab used to be in this group too,
				 * because UNIX might expand it to blanks.
				 * We now require that this tab mode be turned
				 * off by any program using this routine,
				 * or using termcap in general, since some
				 * terminals use tab for other stuff, like
				 * nondestructive space.  (Filters like ul
				 * or vcrt will lose, since they can't stty.)
				 * Tab was taken out to get the Ann Arbor
				 * 4080 to work.
				 */
			}

			*outp++ = op;
			(void) pop();
			break;

		case 'l':
			xp = (char *) pop();
			push(strlen(xp));
			break;

		case 's':
			xp = (char *) pop();
			while (*xp)
				*outp++ = *xp++;
			break;

		case '%':
			*outp++ = c;
			break;

		/*
		 * %i: shorthand for increment first two parms.
		 * Useful for terminals that start numbering from
		 * one instead of zero (like ANSI terminals).
		 */
		case 'i':
			p1++; p2++;
			break;

		/* %pi: push the ith parameter */
		case 'p':
			switch (c = *cp++) {
			case '1': push(p1); break;
			case '2': push(p2); break;
			case '3': push(p3); break;
			case '4': push(p4); break;
			case '5': push(p5); break;
			case '6': push(p6); break;
			case '7': push(p7); break;
			case '8': push(p8); break;
			case '9': push(p9); break;
			default: return _err("bad parm number");
			}
			onrow = (c == '1');
			break;
		
		/* %Pi: pop from stack into variable i (a-z) */
		case 'P':
			vars[*cp++ - 'a'] = pop();
			break;
		
		/* %gi: push variable i (a-z) */
		case 'g':
			push(vars[*cp++ - 'a']);
			break;
		
		/* %'c' : character constant */
		case '\'':
			push(*cp++);
			if (*cp++ != '\'')
				return _err("missing closing quote");
			break;
		
		/* %{nn} : integer constant.  */
		case '{':
			op = 0; sign = 1;
			if (*cp == '-') {
				sign = -1;
				cp++;
			} else if (*cp == '+')
				cp++;
			while ((c = *cp++) >= '0' && c <= '9') {
				op = 10*op + c - '0';
			}
			if (c != '}')
				return _err("missing closing brace");
			push(sign * op);
			break;
		
		/* binary operators */
		case '+': c=pop(); op=pop(); push(op + c); break;
		case '-': c=pop(); op=pop(); push(op - c); break;
		case '*': c=pop(); op=pop(); push(op * c); break;
		case '/': c=pop(); op=pop(); push(op / c); break;
		case 'm': c=pop(); op=pop(); push(op % c); break; /* %m: mod */
		case '&': c=pop(); op=pop(); push(op & c); break;
		case '|': c=pop(); op=pop(); push(op | c); break;
		case '^': c=pop(); op=pop(); push(op ^ c); break;
		case '=': c=pop(); op=pop(); push(op = c); break;
		case '>': c=pop(); op=pop(); push(op > c); break;
		case '<': c=pop(); op=pop(); push(op < c); break;

		/* Unary operators. */
		case '!': stack[top] = !stack[top]; break;
		case '~': stack[top] = ~stack[top]; break;
		/* Sorry, no unary minus, because minus is binary. */

		/*
		 * If-then-else.  Implemented by a low level hack of
		 * skipping forward until the match is found, counting
		 * nested if-then-elses.
		 */
		case '?':	/* IF - just a marker */
			break;

		case 't':	/* THEN - branch if false */
			if (!pop())
				cp = _branchto(cp, 'e');
			break;

		case 'e':	/* ELSE - branch to ENDIF */
			cp = _branchto(cp, ';');
			break;

		case ';':	/* ENDIF - just a marker */
			break;

		default:
			return _err("bad % sequence");
		}
	}
#ifdef tparmdebug
printf("part a: '");
_prstr(result);
printf("', len %d, part b: '", outp-result);
_prstr(added);
printf("'\n");
#endif
	strcpy(outp, added);
	return (result);
}

static
char *
_err(msg)
char *msg;
{
#ifdef tparmdebug
	write(2,"tparm: ", 7);
	write(2, msg, strlen(msg));
	write(2, "\r\n", 2);
#endif
	return 0;
}

char *
_branchto(cp, to)
register char *cp;
char to;
{
	register int level = 0;
	register char c;

	while (c = *cp++) {
		if (c == '%') {
			if ((c = *cp++) == to || c == ';') {
				if (level == 0) {
					return cp;
				}
			}
			if (c == '?')
				level++;
			if (c == ';')
				level--;
		}
	}
	return _err("no matching ENDIF");
}
