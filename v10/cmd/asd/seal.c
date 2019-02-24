/*
 *	seal - prepare a package for shipment
 */

#include "asd.h"
#include "seal.h"
#include <ctype.h>

static long length;
static unsigned long checksum;

main (argc, argv)
	int argc;
	char **argv;
{
	int rc = 0;
	static char stdbuf[BUFSIZ];

	setbuf (stdout, stdbuf);

	length = 0;
	checksum = 0;

	rc = getargs (argc, argv, "kK:", seal);

	printf ("!end %ld %lu\n", length, checksum);

	return rc;
}


seal (f, fname)
	register FILE *f;
	char *fname;
{
	static int first = 1;

	if (first) {
		first = 0;
		printf ("!<seal>\n");
	}

	do {
		char line[MAXLINE];
		char outline[MAXLINE*3+10];
		register char *p, *endl;
		register int ch;

		/* read a line, possibly short */
		p = line;
		endl = line + MAXLINE;

		do {
			ch = getc (f);
			if (ch == EOF)
				break;
			*p++ = ch;
		} while (ch != '\n' && p < endl);

		endl = p;

		/*
		 * endl now points one past the last char
		 * in the line we have read.  If we didn't
		 * get any characters at all, we're done.
		 */

		if (line == endl)
			return ferror (f) != 0;

		length += endl - line;

		/* now convert the line to external form */
		ch = hrform (line, endl, outline);
		if (ch > (endl - line + INCPW - 1) / INCPW * OUTCPW + 2)
			ch = mrform (line, endl, outline);
		p = outline;

		/* if encrypting, do so */
		if (kflag || Kflag)
			mangle (line, endl);

		/* accumulate the (possibly encrypted) checksum */
		checksum = mkcsum (checksum, line, endl);

		while (--ch >= 0)
			putchar (*p++);
	} while (!feof (f) && !ferror (f));

	return ferror (f) != 0;
}

/*
 *	convert the characters in the range (in,inend] to
 *	storage starting at "out" -- return the number of
 *	output characters.
 */

int
hrform (in, inend, out)
	register char *in, *inend;
	char *out;
{
	register char *r = out;

	/* special handling for the first character(s) */
	if (in < inend) {
		if (*in == '.' || *in == '!' ||
		    (*in == 'F' && strncmp (in, "From", 4) == 0))
			*r++ = '\\';

		do {
			register int ch = (unsigned char) *in++;

			if (isprint (ch)) {
				if (ch == '\\')
					*r++ = '\\';
				*r++ = ch;
			} else {
				if (ch == ' ' || ch == '\t' || ch == '\n')
					*r++ = ch;
				else {
					*r++ = '\\';
					*r++ = hextab[(ch >> 4) & 0xf];
					*r++ = hextab[ch & 0xf];
				}
			}
		} while (in < inend);

		if (r[-1] != '\n') {
			*r++ = '\\';
			*r++ = '\n';
		}
	}

	return r - out;
}

int
mrform (in, inend, out)
	register char *in;
	char *inend, *out;
{
	char *r;
	register int len;

	len = inend - in;
	if (len <= 0)
		return 0;

	r = out;

	*r++ = '.';

	do {
		register unsigned long bits;
		register int n, outn;
		register char *rr;

		n = len;
		if (n > INCPW)
			n = INCPW;
		len -= n;
		outn = n + OUTCPW - INCPW;

		bits = 0;
		do	bits = (bits << BPC) | (*in++ & BYTEMASK);
		while (--n > 0);

		rr = r = r + outn;
		do {
			*--rr = RADBASE + bits % RADIX;
			bits /= RADIX;
		} while (--outn > 0);
	} while (len > 0);

	*r++ = '\n';

	return r - out;
}
