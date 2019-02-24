/*
 *	getflds - break a line into fields
 *
 *	General strategy: ptrs is a pointer to an array of
 *	character pointers.  This array is used to hold the
 *	results of each call to getflds.  Expandptrs() uses
 *	realloc to grow this array as needed.  Each pointer in
 *	the array points into the buffer addressed by str,
 *	so all the strings returned by getflds are really
 *	contiguous.  Expandstr() uses realloc to grow this buffer
 *	as needed; after it has done so it must relocate all
 *	the pointers into the buffer because it may have moved.
 *
 *	Both expandstrs and expandptrs return nonzero for failure.
 *
 *	Ptrs and str are initially null.  In the (unlikely)
 *	event that an attempt to grow one of them fails, the
 *	corresponding pointer is set back to null.
 */

#include <stdio.h>
#include <setjmp.h>

/* how many elements to expand str and ptrs at a time */
#define STRCHUNK 512
#define PTRCHUNK 128

char *malloc(), *realloc();
static char **ptrs = NULL, *str = NULL;
static unsigned ptrsize = 0, strsize = 0;

static
expandptrs()
{
	ptrsize += PTRCHUNK;
	ptrs = (char **) (ptrs? realloc (ptrs, ptrsize * sizeof (char *)):
				malloc (ptrsize * sizeof (char *)));
	if (ptrs == NULL) {
		ptrsize = 0;
		return 1;
	}
	return 0;
}

static
expandstr()
{
	register char *newstr;
	register unsigned n;

	strsize += STRCHUNK;
	newstr = str? realloc (str, strsize): malloc (strsize);
	if (newstr == NULL) {
		str = NULL;
		strsize = 0;
		return 1;
	}

	/* relocate the ptrs array */
	for (n = 0; n < ptrsize; n++)
		ptrs[n] = newstr + (ptrs[n] - str);

	str = newstr;
	return 0;
}

char **
getflds (f)
	register FILE *f;
{
	register int c;
	register unsigned nptr = 0, nstr = 0;

	/* try to read the first character of the line */
	c = getc (f);

	/* a line beginning with # is a comment */
	while (c == '#') {
		do c = getc (f);
		while (c != '\n' && c != EOF);
		if (c == '\n')
			c = getc (f);
	}

	/* EOF at the beginning of the line is real */
	if (c == EOF)
		return NULL;

	/* one iteration per field */
	do {
		/* 0 outside a quoted string, otherwise the type of quote */
		register quote = 0;

		/* skip leading white space */
		while (c == ' ' || c == '\t')
			c = getc (f);

		/* maybe it wan't a field after all */
		if (c == '\n' || c == EOF)
			break;

		/* make room for the field pointer and plant it */
		if (nptr >= ptrsize)
			if (expandptrs())
				return NULL;
		ptrs[nptr++] = str + nstr;

		/* one iteration per character or escape sequence */
		while ((c != ' ' && c != '\t' || quote) && c != '\n' && c != EOF) {

			/* check for escape sequence */
			if (c == '\\') {
				c = getc (f);
				switch (c) {

				/* C escapes for newline, etc. */
				case 'b': c = '\b'; break;
				case 'f': c = '\f'; break;
				case 'n': c = '\n'; break;
				case 'r': c = '\r'; break;
				case 't': c = '\t'; break;
				case 'v': c = '\v'; break;

				/* these characters stand for themselves */
				case '\'': case '"': case '#':
				case ' ': case '\t': case '\\':
					break;

				/* backslash, newline is ignored */
				case '\n':
					c = getc (f);
					continue;

				/* backslash, octal digits */
				case '0': case '1': case '2': case '3':
				case '4': case '5': case '6': case '7':
					{
						register r = c - '0';
						register n = 2;
						do {
							c = getc (f);
							if (c < '0' || c > '7') {
								ungetc (c, f);
								break;
							}
							r = (r<<3) + c - '0';
						} while (--n > 0);
						c = r;
					}
					break;

				/* else treat the backslash as an ordinary char */
				default:
					ungetc (c, f);
					c = '\\';
					break;
				}

			/* not a backslash, check for a quote */
			} else if (c == '\'' || c == '"') {

				/* beginning of a quoted string */
				if (!quote) {
					quote = c;
					c = getc (f);
					continue;

				/* end of a quoted string */
				} else if (c == quote) {
					quote = 0;
					c = getc (f);
					continue;
				}

				/*
				 * otherwise we're inside a quoted string
				 * and we've seen the "other" quote, which
				 * is now just an ordinary character.
				 */
			}

			/* we have a character; put it in the string */
			if (nstr >= strsize)
				if (expandstr())
					return NULL;
			str[nstr++] = c;
			c = getc (f);
		}

		/* end of field; plant a null char at the end of the string */
		if (nstr >= strsize)
			if (expandstr())
				return NULL;
		str[nstr++] = '\0';

	} while (c != '\n' && c != EOF);

	/* end of line; put a null pointer at the end of the array */
	if (nptr >= ptrsize)
		if (expandptrs())
			return NULL;
	ptrs[nptr++] = NULL;

	return ptrs;
}
