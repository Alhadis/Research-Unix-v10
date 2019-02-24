/* util.c: utility routines */

#include "fdevelop.h"
#include <ctype.h>

error(f, s)
	int f;
	char *s;
{
	fprintf(stderr, "%s: %s\n", cmdname, s);
	if (lineno)
		fprintf(stderr, " source line number %d\n", lineno);
	if (f)
		exit(2);
}

int strisnum(p)		/* 1 if string p represents a float */
	register char *p;
{
	register int digits = 0;
	register int n;
						/* REG EXPR:	*/
	if (*p == '-')				/* -?		*/
		p++;
	while (isdigit(*p)) {
		digits++;			/* [0-9]*	*/
		p++;
	}
	if (*p == '.') {			/* (.[0-9]*)?	*/
		p++;
		while (isdigit(*p)) {
			digits++;
			p++;
		}
	}
	if (digits == 0)			/* >0 digits	*/
		return 0;
	if (tolower(*p) == 'e') {		/* ([eE]	*/
		*p++;
		if (*p == '+' || *p == '-')	/*  [+-]?	*/
			p++;
		digits = 1;
		if (!isdigit(*p))		/*  [0-9]	*/
			return 0;
		n = *p++ - '0';
		while (isdigit(*p)) {		/*  [0-9]+	*/
			digits++;
			n = 10*n + *p++ - '0';
			if (n > 30)
				return 0;
		}
		if (digits == 0)
			return 0;		/*        )?	*/
	}
	return (*p == '\0');
}

int moreinput(fp)
	FILE *fp;
{
	int c;

	if (lexsaweof)
		return 0;
	c = getc(fp);
	if (c == EOF)
		return 0;
	ungetc(c, fp);
	return 1;
}

#define CHECKEOF		if (c == EOF) {\
		lexsaweof = 1;\
		error(WARN, "file does not end with newline");\
		return;\
	}


lex(fp)		/* put next string of non-white into buf */
	register FILE *fp;
{
	register int c;
	register char *p, *danger;

	danger = &buf[MAXSTR-1];
	while ((c = getc(fp)) == ' ' || c == '\t')
		;
	CHECKEOF
	if (c == '\n') {
		ungetc(c, fp);
		buf[0] = '\0';
		return;
	}
	p = buf;
	*p++ = c;
	while ((c = getc(fp)) != EOF && c != ' ' && c != '\t' && c != '\n') {
		if (p < danger) 
			*p++ = c;
		else if (p == danger)
			error(WARN, "string too long -- truncated");
	}
	*p = '\0';
	CHECKEOF
	if (c == '\n')
		ungetc(c, fp);
/* fprintf(stderr, "lex returning: %s\n", buf); */
}


lexstr(fp)		/* like lex, but go til newline and handle quotes */
	FILE *fp;
{
	int c;
	int quoted = 0;
	char *p, *danger;

	while ((c = getc(fp)) == ' ' || c == '\t')
		;
	CHECKEOF
	if (c == '\"') {
		quoted = 1;
		c = getc(fp);
		CHECKEOF
	}
	if (c == '\n') {
		buf[0] = '\0';
		return;
	}
	p = buf;
	*p++ = c;
	danger = &buf[MAXBUF-1];
	while ((c = getc(fp)) != EOF && c != '\n') {
		if (p < danger) 
			*p++ = c;
		else if (p == danger)
			error(WARN, "text string too long -- truncated");
	}
	CHECKEOF
	*p = '\0';
	if (quoted && *(--p) == '\"')
		*p = '\0';
}

lexrest(fp)		/* get rest of line into buf */
			/* no error checking; error free from Pass 1 */
	FILE *fp;
{
	int c;
	char *p;

	p = buf;
	while ((c = getc(fp)) != EOF && c != '\n') {
		*p++ = c;
	}
	*p = '\0';
}

gobble(fp)		/* chew space til EOF; complain if nonwhite */
	FILE *fp;
{
	int c;

	while ((c = getc(fp)) == ' ' || c == '\t')
		;
	CHECKEOF
	if (c != '\n') {
		error(WARN, "garbage at end of line");
		while ((c = getc(fp)) != EOF && c != '\n')
			;
		CHECKEOF
	}
}

gobble2(fp)		/* chew space til EOF, no complaints */
	FILE *fp;
{
	int c;

	while ((c = getc(fp)) != EOF && c != '\n')
		;
	CHECKEOF
}

#define WANTPROF 0
#if WANTPROF
	int	hmallocinit;
	FILE	*hmallocfp;
#endif

char *emalloc(n)	/* check return from malloc */
	int n;
{
	char *p;
	extern char *malloc();

#if WANTPROF
	if (hmallocinit == 0) {
		hmallocinit = 1;
		if ((hmallocfp = fopen("/tmp/malloc.hist", "w")) == NULL)
			error(FATAL, "malloc history bug: can't open file");
	}
#endif
	p = malloc((unsigned) n);
	if (p == NULL)
		error(FATAL, "out of memory");
#if WANTPROF
	fprintf(hmallocfp, "m\t%d\t%d\n", (int) p, n);
#endif
	return p;
}

efree(p)		/* personal version of free to match emalloc */
	char *p;
{
#if WANTPROF
	if (hmallocinit == 0)
		error(FATAL, "malloc history bug: first free before malloc");
	fprintf(hmallocfp, "f\t%d\n", (int) p);
#endif
	free(p);
}
