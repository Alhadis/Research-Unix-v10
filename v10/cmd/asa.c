/*
 *	asa - interpret asa carriage control characters
 *
 *	This program is designed to make sense out of the output
 *	of fortran programs whose authors have used asa carriage
 *	control characters.  It processes either the files
 *	whose names are given as arguments or the standard input
 *	if no file names are given.  The first character of each
 *	line is assumed to be a control character: the meanings
 *	of the control characters are:
 *
 *	' '	single-space before printing
 *	'0'	double-space before printing
 *	'-'	triple-space before printing
 *	'1'	new page before printing
 *	'+'	do not space at all before printing
 *
 *	A line beginning with '+' will overprint the previous line.
 *
 *	Lines beginning with other than the above characters are
 *	treated as if they began with ' '; if any such lines appear,
 *	an appropriate diagnostic will appear on the standard error
 *	file before termination.  The return code will be zero unless
 *	one or more input files could not be opened.
 *
 *	The program forces the first line of each input file to
 *	start on a new page.
 */

#include <stdio.h>
typedef char bool;
#define true 1
#define false 0

/* program name, for diagnostics */
char *pgmname;

/* count of lines with bad control characters */
long badlines = 0;

/* buffer for the standard output */
char stdbuf[BUFSIZ];

/* number of errors detected */
int retcode = 0;

main (c, v)
	int c;
	char **v;
{
	setbuf (stdout, stdbuf);
	pgmname = v[0];

	/* were any files given, or do we process stdin? */
	if (c <= 1)

		/* process standard input */
		dofile ("standard input");
	else {
		register int i;

		/* one iteration per input file */
		for (i = 1; i < c; i++) {
			if (freopen (v[i], "r", stdin) == NULL) {
				fprintf (stderr, "%s: cannot open %s\n",
					pgmname, v[i]);
				retcode++;
			} else
				dofile (v[i]);
		}
	}

	/* report invalid input lines -- dofile increments badlines */
	if (badlines)
		fprintf (stderr, "%s: %ld invalid input line%s\n",
			pgmname, badlines, badlines>1? "s": "");

	if (ferror (stdout)) {
		fprintf (stderr, "%s: output error\n", pgmname);
		retcode++;
	}

	return retcode;
}

/*
 *	dofile - process the standard input.
 *
 *	This program is called once for each input file, with stdin
 *	redirected to the file.  The "fname" argument is used
 *	for writing diagnostic messages only.
 */

/* true only when the first output character has not been written */
bool firstout = true;

dofile (fname)
	char *fname;
{
	register int c;

	/* true if we are about to read the first character in a line */
	bool firstchar = true;

	/* true if we are about to write the first line for the file */
	bool firstline = true;

	while ((c = getchar()) != EOF) {

		/* separate input files by formfeeds */
		if (firstline && !firstout)
				putchar ('\f');

		if (firstchar) {
			switch (c) {

			/* new page */
			case '1':
				if (!firstline)
					putchar ('\f');
				break;

			/* triple space */
			case '-':
				if (!firstline)
					putchar ('\n');
				putchar ('\n');
				putchar ('\n');
				break;

			/* double space */
			case '0':
				if (!firstline)
					putchar ('\n');
				putchar ('\n');
				break;

			/* single space and invalid characters */
			default:
				badlines++;
			/* no break */
			case '\n':
			case ' ':
				if (!firstline)
					putchar ('\n');
				break;

			/* no space at all */
			case '+':
				if (!firstline)
					putchar ('\r');
				break;
			}
			firstline = false;
			firstout = false;
			if (c != '\n')
				firstchar = false;
		} else {
			/* not the first character of an input line */
			if (c == '\n')
				firstchar = true;
			else
				putchar (c);
		}
	}

	/* end the file with a newline if any output appeared */
	if (!firstline)
		putchar ('\n');

	if (ferror (stdin)) {
		fprintf (stderr, "%s: input error on %s\n", pgmname, fname);
		retcode++;
	}
}
