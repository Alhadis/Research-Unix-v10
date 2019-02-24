/*	@(#) line_alloc.c: 1.1 10/15/83	(1.9	10/27/82)	*/

#include "curses.ext"

/*
 * _line_alloc returns a pointer to a new line structure.
 */
struct line *
_line_alloc ()
{
	register struct line   *rv = SP->freelist;
	char *calloc();

#ifdef DEBUG
	if(outf) fprintf(outf, "mem: _line_alloc (), prev SP->freelist %x\n", SP->freelist);
#endif
	if (rv) {
		SP->freelist = rv -> next;
	} else {
#ifdef NONSTANDARD
		_ec_quit("No lines available in line_alloc", "");
#else
		rv = (struct line *) calloc (1, sizeof *rv);
		rv -> body = (chtype *) calloc (columns, sizeof (chtype));
#endif
	}
	rv -> length = 0;
	rv -> hash = 0;
	return rv;
}
