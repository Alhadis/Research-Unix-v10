#include <stdio.h>

#define	MAXLINES	100

char	buf[BUFSIZ];			/* temporary buffer */

int	line;				/* current line number */

char	*readahead[MAXLINES];		/* pointers to readahead lines */

int	ahead;				/* how many lines have we read ahead */

/*
 * Take a set of index lines and produce a set of index lines with subindex lines.
 * Subindex lines are formed whenever n index lines have a common blank-separated
 * leading string.  Index lines are output as they are, subindex lines are
 * prefaced by a tab.
 */
main()
{
	char
		*rline(),		/* read a line into the readahead pool */
		*first,			/* the first line */
		*current,		/* the current input line */
		*index(),		/* main index part */
		*subindex(),		/* sub index part */
		*p			/* temporary pointer */
	;

	register int
		match,			/* how many tokens matched */
		min,			/* min number of tokens matched */
		i			/* temporary array index */
	;


/*
 * *** initalize ***
 */
	first = rline();
	if (*first == '\0')
		exit(0);

/*
 * *** main loop ***
 */
	while (feof(stdin) == 0) {
		min = sizeof(buf);
		current = rline();

/*
 * *** gather matching lines ***
 */
		while (match = substrings(first, current)) {
			min = min < match ? min : match;
			current = rline();
		}

		if (min == sizeof(buf))
			min = 0;

/*
 * *** output matching lines ***
 */
		if (min) {
			printf("\t%s", index(readahead[0], min));

			for (i = 0; readahead[i] != readahead[ahead-1]; ++i) {
				if (p = subindex(readahead[i], min))
					printf("\t\t%s", p);
				free(readahead[i]);
			}
		} else
			for (i = 0; readahead[i] != readahead[ahead-1]; ++i) {
				printf("%s", readahead[i]);
				free(readahead[i]);
			}

/*
 * *** reset and do it again ***
 */
		first = readahead[0] = readahead[ahead-1];
		ahead = 1;
	}
}
					
/*
 * read a line into the readahead pool and check that each line has a \n
 * on end of file return a line with just a \n in it to flush out the
 * read ahead queue
 */
char *
rline()
{
	char *malloc();

/*
 * *** get a line handeling end of file ***
 */
	if (feof(stdin))
		error("rline called too many times");

	if (fgets(buf, sizeof(buf), stdin) == NULL)
		strcpy(buf, "\n");
	++line;

/*
 * *** check for too many lines or missing \n ***
 */
	if (strchr(buf, '\n') == NULL)
		error("missing \\n on: %s", buf);

	if (ahead == MAXLINES)
		error("%d is too many lines to read ahead!", ahead);

/*
 * *** stuff it away and return it ***
 */
	if ((readahead[ahead] = malloc(strlen(buf) + 1)) == NULL)
		error("no space for line");

	strcpy(readahead[ahead], buf);

	return(readahead[ahead++]);
}

/*
 * return the number of blank separated tokens that are the same between
 * the two strings ignoring commas
 */
substrings(s1, s2)
register char *s1, *s2;
{
	register int blankcnt;

	for (blankcnt = 0; *s1 == *s2 && *s1 != '\n'; ++blankcnt) {

/*
 * *** see how far we match ***
 */
		while (*s1 == *s2 && *s1 != ' ') {
			while (*++s1 == ',')
				;
			while (*++s2 == ',')
				;
		}

/*
 * *** do we still match ignoring commas, blanks and tabs ***
 */
		if ((*s1 != *s2) &&
		    (*s1 != ' ' || *s2 != '\t') && (*s1 != '\t' || *s2 != ' '))
			return(blankcnt);

/*
 * *** trim off blanks ***
 */
		while (*s1 == ' ')
			++s1;
		while (*s2 == ' ')
			++s2;
	}

	return(blankcnt);
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

/*
 * take a string and return a substring of just the first i blank separated tokens
 * removing a trailing comma.  If only the page numbers remain,
 * add this to the string.
 */
char *
index(s, i)
register char *s;
register int i;
{
	static char tmp[BUFSIZ];

	register char *p;

/*
 * *** gather first i blank separated tokens ***
 */
	p = &tmp[0];

	while (i--) {
		while (*s != ' ' && *s != '\t')
			*p++ = *s++;
		while (*s == ' ')
			++s;
		*p++ = ' ';
	}

/*
 * *** trim off trailing blanks and commas ***
 */
	while (p > &tmp[1] && (p[-1] == ' ' || p[-1] == ','))
		--p;

/*
 * *** if nothing left it must have been just a comma so put it back ***
 */
	if (p == &tmp[0])
		*p++ = ',';

/*
 * *** end entry with either page list or \n ***
 */
	if (*s == '\t')
		strcpy(p, s);
	else {
		*p++ = '\n';
		*p = '\0';
	}

	return(&tmp[0]);
}

/*
 * take a string and return a substring ignoring the first i blank separated tokens
 * If only the page numbers remain, return NULL.
 */
char *
subindex(s, i)
register char *s;
register int i;
{

/*
 * *** ignore i blank separated tokens ***
 */
	while (i--) {
		while (*s != ' ' && *s != '\t')
			++s;
		while (*s == ' ')
			++s;
	}

/*
 * *** return subindex or nothing if only page numbers remain ***
 */
	return(*s == '\t' ? NULL : s);
}
