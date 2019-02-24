/* "@(#) bedutil.c: V 1.12 8/19/83" */

#include <stdio.h>
#include "bas.h"

extern struct pages	*index;


/* initialize page pointer array */
clear_pages()
{
	int	i;

	for (i=0; i<NMPAG; i++)
	{
		index[i].maxlin = 0;
		index[i].begin = NULL;
	}
}


/* update program page pointer from fromptr on down */
update(fromptr)
char	*fromptr;
{
	int		i,
			pg;
	char		*tptr;
	LINNO		line_advance();
	union bascmd	loc;

	pg = (fromptr - txtbuf) / PGSIZ;
	if (pg == 0)				/* no program */
		tptr = txtbuf;
	else if (index[pg].maxlin == 0)		/* first line on page */
		tptr = fromptr;
	else
		tptr = index[pg].begin;
	for (i=pg; i<NMPAG; i++)
	{
		index[i].maxlin = 0;
		index[i].begin = NULL;
	}
	for (i=0; i<CMDSIZE; i++)
		loc.byte[i] = tptr[i];
	do {
		pg = (tptr - txtbuf) / PGSIZ;
		if (index[pg].maxlin == 0)
		{
			index[pg].maxlin = loc.thing.linno;
			index[pg].begin = tptr;
		}
		if (loc.thing.linno > index[pg].maxlin)
			index[pg].maxlin = loc.thing.linno;
	} while (loc.thing.linno = line_advance(&tptr));
}


dmppages(msg)
char	msg[];
{
	int	i;

	printf("Page Table Dump %s\n", msg);
	for (i=0; index[i].maxlin != 0; i++)
		printf("starts at character %d        maxlin=%d\n",
				index[i].begin - txtbuf, index[i].maxlin);
}


extern UCHAR	nonl;

extern short    margin,
		startpos;

/*
 *	lclfgets gets input from the user and echos it back to
 *	the terminal.
 */

char *
lclfgets(s,n,stream)
char	*s;
int	n;
FILE	*stream;
{
	int	c,
		i = 0,
		camefrom;
	char	*t;

	camefrom = startpos;
	t = s;
	while ((c = fgetc(stream)) != '\n' && c != EOF)
	{
		{
			if (i < n)
			{
				*t++ = c;
				++i;
			}
		}
	}
	if (i == 0 && c != '\n')
		return(NULL);
	if (c == '\n')
	{
		if (i < n)
			*t++ = c;
		nonl = 0;
	}
	*t = NULL;
	return(s);
}
