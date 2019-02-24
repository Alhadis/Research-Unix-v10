/*
 *	getpath (file) - read a path name
 *	putpath (file, path) - write a path name
 *
 *	These subroutines cater to the possibility of unprintable
 *	characters in the path names being read or written, by
 *	using the same sort of \ conventions commonly found in
 *	C character constants.  The result of getpath is a pointer
 *	to a static buffer whose contents will stay around no longer
 *	than the next call to getpath.  When getpath is called, the
 *	character about to be read from the input file must be the
 *	first character of the path name.
 *
 *	There are a few problems, mostly relating to bugs and language
 *	changes, to watch out for in these routines.  First of all,
 *	we assume that \v is known by the C compiler, even though
 *	it is not mentioned in Kernighan and Ritchie.  The reason for
 *	this is that if we do not make this assumption, we run into
 * 	a common bug in the handling of iscntrl().  Although the
 *	manuals all say that if c is a white-space character that is
 *	not a blank, then iscntrl(c) is true, several versions of the
 *	C library disagree with the documentation.  Thus we try to
 *	list all the white-space characters explicitly.
 */

#include <stdio.h>
#include <ctype.h>
#include "asd.h"

#define CHUNK 64

static char *r;
static unsigned size;

char *
getpath (file)
	register FILE *file;
{
	register int c;
	register int len = 0;

	c = getc (file);

	while (!isspace(c) && c != EOF) {
		register int i = 0, n = 0;

		/* determine the next input character */
		if (c == '\\') {
			c = getc (file);
			switch (c) {

			case '\\':
				break;
			
			case 'n':
				c = '\n';
				break;

			case 'r':
				c = '\r';
				break;

			case 't':
				c = '\t';
				break;

			case 'b':
				c = '\b';
				break;
			
			case 'f':
				c = '\f';
				break;
			
			case 'v':
				c = '\v';
				break;
			
			case ' ':
			     /* c = ' '; */
				break;

			default:
				while (c >= '0' && c <= '7' && i < 3) {
					n = (n << 3) + c - '0';
					i++;
					c = getc (file);
				}
				ungetc (c, file);
				c = n;
				break;
			}
		}

		/* ensure there's room in the buffer */
		if (len >= size)
			r = ralloc (r, size += CHUNK);

		/* put the character in the buffer */
		r[len++] = c;

		/* read the next character */
		c = getc (file);
	}

	/* unless we hit eof, we read one character too far. */
	if (c != EOF)
		ungetc (c, file);
	
	/* put a final null into the buffer */
	if (len >= size)
		r = ralloc (r, size += CHUNK);
	r[len] = '\0';

	return r;
}

void
putpath (file, path)
	register FILE *file;
	char *path;
{
	register char *p = path;
	register int c;
	
	while ((c = *p++) != NULL) {
		switch (c) {

		case '\n':
			fprintf (file, "\\n");
			break;

		case '\r':
			fprintf (file, "\\r");
			break;

		case '\b':
			fprintf (file, "\\b");
			break;

		case '\t':
			fprintf (file, "\\t");
			break;

		case '\f':
			fprintf (file, "\\f");
			break;
		
		case '\v':
			fprintf (file, "\\v");
			break;
		
		case '\\':
			fprintf (file, "\\\\");
			break;
		
		case ' ':
			fprintf (file, "\\ ");
			break;
		
		default:
			if (iscntrl (c))
				fprintf (file,
				    *p >= '0' && *p <= '7'? "\\%.3o": "\\%o",
				    c);
			else
				putc (c, file);
			break;
		}
	}
}
