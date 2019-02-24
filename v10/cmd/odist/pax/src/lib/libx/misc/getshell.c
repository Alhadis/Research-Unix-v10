/*
 * G. S. Fowler
 * D. G. Korn
 * AT&T Bell Laboratories
 *
 * shell library support
 */

extern char*	getenv();
extern char*	strrchr();

extern int	access();
extern int	geteuid();
extern int	getegid();
extern int	getgid();
extern int	getuid();
extern int	strmatch();

/*
 * return pointer to the full path name of the shell
 *
 * SHELL is read from the environment and must start with /
 *
 * if set-uid or set-gid then the executable and its containing
 * directory must not be writable by the real user
 *
 * /bin/sh is returned by default
 *
 * NOTE: csh is rejected because the bsh/csh differentiation is
 *       not done for `csh script arg ...'
 */

char*
getshell()
{
	register char*	s;
	register char*	sh;
	register int	i;

	if ((sh = getenv("SHELL")) && *sh == '/' && strmatch(sh, "*/(sh|*[!cC]sh)"))
	{
		if (!(i = getuid()))
		{
			if (!strmatch(sh, "?(/usr)?(/local)/?(l)bin/?([a-z])sh")) goto defshell;
		}
		else if (i != geteuid() || getgid() != getegid())
		{
			if (!access(sh, 2)) goto defshell;
			s = strrchr(sh, '/');
			*s = 0;
			i = access(sh, 2);
			*s = '/';
			if (!i) goto defshell;
		}
		return(sh);
	}
 defshell:
	return("/bin/sh");
}
