/*	@(#) _sputc.c: 1.1 10/15/83	(1.2	4/7/82)	*/
/*
 * 1/26/81 (Berkeley) @(#)standout.c	1.1
 */

# include	"curses.ext"

#ifdef DEBUG
_sputc(c, f)
chtype c;
FILE *f;
{
	int so;

	so = c & A_ATTRIBUTES;
	c &= 0177;
	if (so) {
		putc('<', f);
		fprintf(f, "%o,", so);
	}
	putc(c, f);
	if (so)
		putc('>', f);
}
#endif
