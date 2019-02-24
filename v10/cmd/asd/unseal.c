/*
 *	unseal - disclose and verify the contents of a sealed package
 */

#include "asd.h"
#include "seal.h"
#include <ctype.h>

static FILE *outfd;
static char outf[] = "/usr/tmp/usXXXXXX";
static char line[MAXULINE];

main (argc, argv)
	int argc;
	char **argv;
{
	int rc = 0;
	int c;
	char *keyfile;
	static char stdbuf[BUFSIZ];

	setbuf (stdout, stdbuf);

	umask (077);
	mktemp (outf);
	outfd = fopen (outf, "w+");
	if (outfd == NULL) {
		fprintf (stderr, "unseal: can't create temp file\n");
		exit (1);
	}

	/* don't leave dregs */
	unlink (outf);

	rc = getargs (argc, argv, "kK:", unseal);
	
	/* if successful, copy the temp file to the output */
	if (rc == 0) {
		register FILE *in = outfd, *out = stdout;
		register int ch;

		rewind (in);

		while ((ch = getc (in)) != EOF)
			putc (ch, out);
		
		fflush (out);
		
		if (ferror (in)) {
			fprintf (stderr, "unseal: error reading temp file\n");
			rc++;
		}

		if (ferror (out)) {
			fprintf (stderr, "unseal: error writing stdout\n");
			rc++;
		}
	}

	return rc;
}

static unsigned long length, checksum;

unseal (f, fname)
	FILE *f;
	char *fname;
{
	int intext = 0, rc = 0, nulltext = 1;

	length = checksum = 0;
	resetN12();

	while (fgets (line, MAXULINE, f) != NULL) {

		/* check for a control line, which starts with "!" */
		if (line[0] == '!') {
			if (intext) {
				/* only thing allowed is !end */
				if (strncmp (line + 1, "end ", 4) == 0) {
					rc += docksum (line, length, checksum);
					intext = 0;
				} else {
					fprintf (stderr, "invalid control line %s", line);
					rc++;
				}
			} else {
				/* look for header, else quietly ignore */
				if (strcmp (line + 1, "<seal>\n") == 0) {
					intext++;
					nulltext = 0;
				}
			}
		/* data lines are quietly ignored if not in text */
		} else if (intext) {
			rc += dodata (line);
		}
	}

	if (nulltext) {
		fprintf (stderr, "unseal: no contents\n");
		rc++;
	}

	if (intext) {
		fprintf (stderr, "unseal: no checksum\n");
		rc++;
	}

	return rc;
}

static char *
csverify (p, value)
	register char *p;
	register unsigned long value;
{
	register unsigned long n = 0;

	while (isspace (*p))
		p++;

	if (!isdigit (*p))
		return NULL;

	do	n = n * 10 + *p++ - '0';
	while (isdigit (*p));

	if (!isspace (*p))
		return NULL;

	if (n != value)
		return NULL;

	return p;
}

docksum (line, length, checksum)
	char *line;
	unsigned long length, checksum;
{
	register char *p = line + 5;

	p = csverify (p, length);
	if (p == NULL) {
		fprintf (stderr, "unseal: invalid length\n");
		return 1;
	}
	p = csverify (p, checksum);
	if (p == NULL) {
		fprintf (stderr, "unseal: invalid checksum\n");
		return 1;
	}
	return 0;
}

dodata (line)
	char *line;
{
	int hrout(), mrout();
	char out[MAXULINE * 2];
	int r;

	r = (*(line[0] == '.'? mrout: hrout)) (line, out);
	if (r < 0)
		return 1;
	fwrite (out, sizeof(*out), r, outfd);
	length += r;

	if (kflag || Kflag)
		mangle (out, out + r);

	checksum = mkcsum (checksum, out, out + r);

	return 0;
}

int
hrout (line, out)
	char *line, *out;
{
	register char *p, *q;
	register int ch;

	p = line;
	q = out;

	while ((ch = *p++) != '\0') {
		if (ch != '\\') {
			*q++ = ch;
		} else {
			ch = *p++;
			if (ch == '\\' || ch == '.' || ch == '!' || ch == 'F') {
				*q++ = ch;
			} else if (ch != '\n') {
				register int n = 0, i = 1;

				do {
					ch = unhex (ch);
					if (ch < 0) {
						fprintf (stderr,
							"bad hex char: %s", line);
						return -1;
					}
					n = (n << 4) | ch;
					if (i)
						ch = *p++;
				} while (--i >= 0);

				*q++ = n;
			}
		}
	}

	return q - out;
}

int
unhex (c)
	register int c;
{
	register char *p;

	p = hextab;
	while (*p && *p != c)
		p++;
	if (*p)
		return p - hextab;
	return -1;
}

int
mrout (line, out)
	char *line, *out;
{
	register char *p = line + 1;
	char *q = out;

	while (*p != '\n' && *p != '\0') {
		register unsigned long bits;
		register int n;
		register char *rq;

		n = 0;
		bits = 0;
		do {
			register int ch = *p++;
			if (ch == '\n')
				break;
			if (ch < RADBASE || ch >= RADBASE + RADIX) {
				fprintf (stderr, "bad input char: %s", line);
				return -1;
			}
			bits = bits * RADIX + ch - RADBASE;
			n++;
		} while (n < OUTCPW);

		n -= OUTCPW - INCPW;
		if (n <= 0) {
			fprintf (stderr, "non-positive output count: %s", line);
			return -1;
		}

		rq = q = q + n;
		do {
			*--rq = bits;
			bits >>= BPC;
		} while (--n > 0);
	}

	return q - out;
}
