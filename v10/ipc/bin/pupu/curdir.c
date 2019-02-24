/*
	current directory.
	Places the full pathname of the current directory in `str'.
	Handles file systems not mounted on a root directory
	via /etc/mtab (see mtab(V)).
	NOTE: PWB systems don't use mtab(V), but they don't mount
	file systems anywhere but on a root directory (so far, at least).

	returns 0 on success
	< 0 on failure.

	Current directory on return:
		success: same as on entry
		failure: UNKNOWN!
*/
curdir(str)
char *str;
{
	register int n;

	n = chdir(str);
	getwd(str);
	return n;
}
