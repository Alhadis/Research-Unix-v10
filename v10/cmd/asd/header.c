/*
 *	various subroutines to deal with archive headers
 */

#include <stdio.h>
#include <ctype.h>
#include "asd.h"

/*
 *	convert p to a long value.  maximum input length is len,
 *	input is to be interpreted in base b.  No negative values.
 */
long
cvlong (p, len, base)
	register char *p;
	register int len, base;
{
	register int i;
	register long r;

	r = 0;
	i = len;

	do {
		register int c = *p++;
		if (isdigit (c))
			r = r * base + c - '0';
	} while (--i > 0);

	return r;
}

long
read_header (name, file)
	register char *name;
	FILE *file;
{
	register int n;
	register int i;
	register char *p, *q;

	n = fread ((char *) &ar_hdr, sizeof (ar_hdr), 1, file);
	if (n != 1) {
		fprintf (stderr, "can't read %s\n", name);
		exit (1);
	}

	if (strncmp (ar_hdr.ar_fmag, ARFMAG, sizeof (ar_hdr.ar_fmag)) != 0) {
		fprintf (stderr, "input phase error on %s\n", name);
		exit (1);
	}

	/* check the component name, allowing for trailing blanks */
	p = name;
	q = ar_hdr.ar_name;
	for (i = 0; i < sizeof (ar_hdr.ar_name); i++) {
		if (*q++ != (*p? *p++: ' ')) {
			fprintf (stderr, "expected %s, got %.*s\n",
			    name, sizeof (ar_hdr.ar_name), ar_hdr.ar_name);
		}
	}

	/* crack the archive header and put the information in "hdr" */
	hdr.size = cvlong (ar_hdr.ar_size, sizeof (ar_hdr.ar_size), 10);
	hdr.mode = cvlong (ar_hdr.ar_mode, sizeof (ar_hdr.ar_mode), 8);
	hdr.date = cvlong (ar_hdr.ar_date, sizeof (ar_hdr.ar_date), 10);

	return hdr.size;
}

/* advance to the start of the next archive header */
void
next_header(f)
	register FILE *f;
{
	if (hdr.size & 1)
		getc(f);
}

/* skip leading white space, return a field */
char *
getfield (f)
	register FILE *f;
{
	register char c;

	/* skip leading white space */
	do c = getc (f);
	while (isspace (c) && c != '\n');

	/* if we hit a newline, something's wrong */
	if (c == '\n') {
		fprintf (stderr, "unexpected newline\n");
		exit (1);
	}

	/* return the nonblank, read a "pathname" and return it */
	ungetc (c, f);
	return getpath (f);
}

/* insist on an end of line right here */
void
geteol (f)
	register FILE *f;
{
	register int c;

	c = getc (f);
	if (c != '\n') {
		fprintf (stderr, "expected newline, got %c\n", c);
		exit (1);
	}
}
