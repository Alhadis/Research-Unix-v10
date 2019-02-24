#include <stdio.h>

char	buf[BUFSIZ];			/* current line */

int	line;				/* current line number */

/*
 * take a line with a set of comma blank separated page numbers and
 * replace sequences of them with ||.  || is used so as not to get confused with
 * negative line number.  Negative line number are the representation for roman
 * page numbers.
 */
main()
{

	register char
		*in,			/* in pointer to current line */
		*out			/* out pointer to current line */
	;

	char
		*strchr(),		/* find a character in a string */
		*gpage(),		/* return a page number */
		*itoa()			/* convert an integer to ascii */
	;

	int
		fpage,			/* first page number in a sequence */
		lpage,			/* last page number in a sequence */
		cpage			/* current page number */
	;


/*
 * *** read each line ***
 */
	while (fgets(buf, sizeof(buf), stdin) != NULL) {
		++line;
		if ((in = strchr(buf, '\t')) == NULL)
			error("missing \\t on: %s", buf);
		if (strchr(in, '\n') == NULL)
			error("missing \\n on: %s", buf);

/*
 * *** get the first page number ***
 */
		in = out = gpage(in, &fpage);
		lpage = fpage;

/*
 * *** check each page number ***
 */
		while (*in != '\n') {
			in = gpage(in, &cpage);

			if ((lpage + 1) == cpage)	/* begin sequential list */
				lpage = cpage;
			else {
				if (fpage == lpage) {	/* not sequential */
					*out++ = ',';
					*out++ = ' ';
					out = itoa(out, cpage);
				} else {		/* end of sequential list */
					*out++ = '|';
					*out++ = '|';
					out = itoa(out, lpage);
					*out++ = ',';
					*out++ = ' ';
					out = itoa(out, cpage);
				}

				fpage = lpage = cpage;
			}
		}

/*
 * *** end last sequential list ***
 */
		if (fpage != lpage) {
			*out++ = '|';
			*out++ = '|';
			out = itoa(out, lpage);
		}

/*
 * *** output the line ***
 */
		*out++ = '\n';
		*out = '\0';
		fputs(buf, stdout);
	}
}
					
/*
 * look for a page number ignoring blanks tabs and commas.
 * return the page number in *i and the next character position.
 */
char *
gpage(p, i)
register char *p;			/* string to scan */
int *i;					/* place to return the page number */
{
	register int n;			/* temporary page number */

/*
 * *** ignore junk ***
 */
	for (;;++p) {
		switch (*p) {
		case ' ':
		case '\t':
		case ',':
			continue;
		}
		break;
	}

/*
 * *** convert page number ***
 */
	n = 0;
	while (*p >= '0' && *p <= '9')
		n = n*10 + *p++ - '0';

	*i = n;
	return(p);
}

/*
 * integer to characters
 * returns pointer to next position
 */
char *
itoa(p, i)
register char *p;			/* string to put digits in */
register int i;				/* integer to convert */
{

	register int t;			/* temporary */

/*
 * *** negative page numbers for roman numbering ***
 */
	if (i < 0) {
		i = -i;
		*p++ = '-';
	}

/*
 * *** recurse ***
 */
	if (t = i/10)
		p = itoa(p, t);

/*
 * *** store them ***
 */
	*p++ = i%10 + '0';

	return(p);
}

/*
 * output and error message and exit
 */
error(s1, s2, s3, s4)
{
	fprintf(stderr, "error at line %d\n ", line);
	fprintf(stderr, s1, s2, s3, s4);
	fprintf(stderr, "\n");
	exit(1);
}
