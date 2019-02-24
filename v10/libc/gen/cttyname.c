/*
 * on success:
 *	returns the pathname ("/dev/...") of the controlling terminal.
 *	bug: does not actually find controlling terminal.
 *	bug: returns pointer to static area.
 * on failure:
 *	returns 0.
 */

char	*_ttyname();
static char cttybuf[32];

char *
cttyname()
{
	register char *s = &cttybuf[0];

	if (_ttyname(s, 0) || _ttyname(s, 2) || _ttyname(s, 1))
		return(s);
	return(0);
}
