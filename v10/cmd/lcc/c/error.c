/* C compiler: error reporting */

#include "c.h"

int errcnt;		/* number of compilation errors */
int errlimit = 20;	/* compilation error limit */
int wflag;		/* != 0 to suppress warning messages */

dclproto(static void printtoken,(void));

/* error - issue error message */
#ifdef __STDC__
void error(char *fmt, ...) {
#else
void error(fmt, va_alist) char *fmt; va_dcl {
#endif
	va_list ap;

	va_init(ap, fmt);
	if (firstfile != file && firstfile && *firstfile)
		fprint(2, "%s: ", firstfile);		/* omit */
	fprint(2, "%w: ", &src);
	vfprint(2, fmt, ap);
	if (++errcnt >= errlimit) {
		errcnt = -1;
		error("too many errors\n");
		exit(1);
	}
	va_end(ap);
}

/* expect - advance if t is tok, otherwise issue message */
int expect(tok) {
	if (t == tok) {
		t = gettok();
		return t;
	}
	errcnt--;
	error("syntax error; found");
	printtoken();
	fprint(2, " expecting `%k'\n", tok);
	errcnt++;
	return 0;
}

/* fatal - issue fatal error message and exit */
int fatal(name, fmt, n) char *name, *fmt; {
	*bp++ = '\n';
	outflush();
	error("compiler error in %s--", name);
	fprint(2, fmt, n);
	exit(1);
	return 0;
}

/* printtoken - print current token preceeded by a space */
static void printtoken() {
	switch (t) {
	case ID: fprint(2, " `%s'", token); break;
	case ICON:
		if (*token == '\'') {
			char *s;
	case SCON:	fprint(2, " ");
			for (s = token; *s && s - token < 20; s++)
				if (*s < ' ' || *s >= 0177)
					fprint(2, "\\%o", *s);
				else
					fprint(2, "%c", *s);
			if (*s)
				fprint(2, " ...");
			else
				fprint(2, "%c", *token);
			break;
		} /* else fall thru */
	case FCON: {
		char c = *cp;
		*cp = 0;
		fprint(2, " `%s'", token);
		*cp = c;
		break;
		}
	case '`': case '\'': fprint(2, " \"%k\"", t); break;
	default: fprint(2, " `%k'", t);
	}
}

/* skipto - skip input up to tok U set, for a token where kind[t] is in set */
void skipto(tok, set) char set[]; {
	int n;
	char *s;

	for (n = 0; t != EOI && t != tok; t = gettok()) {
		if (set) {
			for (s = set; *s && kind[t] != *s; s++)
				;
			if (kind[t] == *s)
				break;
		}
		if (n++ == 0) {
			errcnt--;
			error("skipping", 0, 0, 0, 0);
		}
		if (n <= 8)
			printtoken();
		else if (n == 9)
			fprint(2, " ...\n");
	}
	if (n > 8) {
		errcnt--;
		error("up to", 0, 0, 0, 0);
		printtoken();
	}
	if (n > 0)
		fprint(2, "\n");
}

/* test - check for token tok, skip to tok U set, if necessary */
void test(tok, set) char set[]; {
	if (t == tok)
		t = gettok();
	else {
		expect(tok);
		skipto(tok, set);
		if (t == tok)
			t = gettok();
	}
}

/* warning - issue warning error message */
#ifdef __STDC__
void warning(char *fmt, ...) {
#else
void warning(fmt, va_alist) char *fmt; va_dcl {
#endif
	va_list ap;

	va_init(ap, fmt);
	if (wflag == 0) {
		errcnt--;	/* compensate for increment in error */
		error("warning: ");
		vfprint(2, fmt, ap);
	}
	va_end(ap);
}
