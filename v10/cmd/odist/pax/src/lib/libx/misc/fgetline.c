/*
 * Glenn Fowler
 * AT&T Bell Laboratories
 */

#include <stdio.h>

#define LINECHUNK	256		/* line resize increment	*/

static char*		buf;		/* current line buffer		*/
static int		siz;		/* current buf size		*/

extern char*		malloc();
extern char*		realloc();

/*
 * return next line from fp
 * if new!=0 then the old buffer space is not reclaimed and
 * the new buffer space is returned with size strlen(line)+new
 * otherwise an internal buffer is used for each call
 *
 * newline is replaced by 0
 *
 * 0 returned if first char is EOF
 * otherwise EOF is treated as a newline
 */

char*
fgetline(fp, new)
register FILE*	fp;
int		new;
{
	register int	c;
	register char*	s;
	register char*	end;

	if ((!buf || new) && !(buf = malloc(siz = LINECHUNK))) return(0);
	s = buf;
	end = s + siz;
	for (;;)
	{
		if (s >= end)
		{
			if (!(buf = realloc(buf, siz += LINECHUNK))) return(0);
			s = buf + siz - LINECHUNK;
			end = buf + siz;
		}
		switch (c = getc(fp))
		{
		case EOF:
			if (s == buf)
			{
				if (new)
				{
					free(buf);
					buf = 0;
				}
				return(0);
			}
			/*FALLTHROUGH*/
		case '\n':
			*s = 0;
			return(new ? realloc(buf, siz = buf - s + new) : buf);
		}
		*s++ = c;
	}
}

/*
 * free fgetline() buffer
 */

void
ffreeline()
{
	if (buf)
	{
		free(buf);
		buf = 0;
	}
}
