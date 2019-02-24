/* brutally hacked from ../pkg/path.c */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "dist.h"

#define CHUNK 64

static char *r;
static unsigned size;

static char *
ralloc(char *ptr, size_t size)
{
	char *result;

	if (!ptr)
		result = malloc(size);
	else
		result = realloc(ptr, size);
	if (!result) {
		eprintf("out of memory in getpath");
		exit(1);
	}
	return result;
}

char *
getpath (FILE *file)
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
putpath (FILE *file, char *path)
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
