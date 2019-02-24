/*	@(#) tgoto.c: 1.1 10/15/83	(1.2	4/7/82)	*/
/*
 * tgoto: function included only for upward compatibility with old termcap
 * library.  Assumes exactly two parameters in the wrong order.
 */
char *
tgoto(cap, col, row)
char *cap;
int col, row;
{
	char *cp;
	char *tparm();

	cp = tparm(cap, row, col);
	return cp;
}
