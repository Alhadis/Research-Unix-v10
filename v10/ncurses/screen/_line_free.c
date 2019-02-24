/*	@(#) _line_free.c: 1.1 10/15/83	(1.9	10/27/82)	*/

#include "curses.ext"

/* '_line_free' returns a line object to the free list */
_line_free (p)
register struct line   *p;
{
	register int i, sl, n=0;
	register struct line **q;

	if (p == NULL)
		return;
#ifdef DEBUG
	if(outf) fprintf(outf, "mem: Releaseline (%x), prev SP->freelist %x", p, SP->freelist);
#endif
	sl = lines;
	for (i=sl,q = &SP->cur_body[sl]; i>0; i--,q--)
		if (p == *q)
			n++;
	for (i=sl,q = &SP->std_body[sl]; i>0; i--,q--)
		if (p == *q)
			n++;
#ifdef DEBUG
	if(outf) fprintf(outf, ", count %d\n", n);
#endif
	if (n > 1)
		return;
	p -> next = SP->freelist;
	p -> hash = -888;
	SP->freelist = p;
}
