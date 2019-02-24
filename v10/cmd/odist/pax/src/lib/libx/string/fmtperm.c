/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * return strperm() expression for perm
 */

#include <ls.h>

char*
fmtperm(perm)
register int	perm;
{
	register char*	s;
	register char*	p;
	char*		o;

	static char	buf[32];

	s = buf;

	/*
	 * ugo
	 */

	p = s;
	*s++ = '+';
	o = s;
	if (perm & S_ISVTX) *s++ = 't';
	if ((perm & (S_ISUID|S_ISGID)) == (S_ISUID|S_ISGID))
	{
		perm &= ~(S_ISUID|S_ISGID);
		*s++ = 's';
	}
	if ((perm & (S_IRUSR|S_IRGRP|S_IROTH)) == (S_IRUSR|S_IRGRP|S_IROTH))
	{
		perm &= ~(S_IRUSR|S_IRGRP|S_IROTH);
		*s++ = 'r';
	}
	if ((perm & (S_IWUSR|S_IWGRP|S_IWOTH)) == (S_IWUSR|S_IWGRP|S_IWOTH))
	{
		perm &= ~(S_IWUSR|S_IWGRP|S_IWOTH);
		*s++ = 'w';
	}
	if ((perm & (S_IXUSR|S_IXGRP|S_IXOTH)) == (S_IXUSR|S_IXGRP|S_IXOTH))
	{
		perm &= ~(S_IXUSR|S_IXGRP|S_IXOTH);
		*s++ = 'x';
	}
	if (s == o) s = p;

	/*
	 * ug
	 */

	p = s;
	if (s > buf) *s++ = ',';
	*s++ = 'u';
	*s++ = 'g';
	*s++ = '+';
	o = s;
	if ((perm & (S_IRUSR|S_IRGRP)) == (S_IRUSR|S_IRGRP))
	{
		perm &= ~(S_IRUSR|S_IRGRP);
		*s++ = 'r';
	}
	if ((perm & (S_IWUSR|S_IWGRP)) == (S_IWUSR|S_IWGRP))
	{
		perm &= ~(S_IWUSR|S_IWGRP);
		*s++ = 'w';
	}
	if ((perm & (S_IXUSR|S_IXGRP)) == (S_IXUSR|S_IXGRP))
	{
		perm &= ~(S_IXUSR|S_IXGRP);
		*s++ = 'x';
	}
	if (s == o) s = p;

	/*
	 * u
	 */

	p = s;
	if (s > buf) *s++ = ',';
	*s++ = 'u';
	*s++ = '+';
	o = s;
	if (perm & S_ISUID) *s++ = 's';
	if (perm & S_IRUSR) *s++ = 'r';
	if (perm & S_IWUSR) *s++ = 'w';
	if (perm & S_IXUSR) *s++ = 'x';
	if (s == o) s = p;

	/*
	 * go
	 */

	p = s;
	if (s > buf) *s++ = ',';
	*s++ = 'g';
	*s++ = 'o';
	*s++ = '+';
	o = s;
	if ((perm & (S_IRGRP|S_IROTH)) == (S_IRGRP|S_IROTH))
	{
		perm &= ~(S_IRGRP|S_IROTH);
		*s++ = 'r';
	}
	if ((perm & (S_IWGRP|S_IWOTH)) == (S_IWGRP|S_IWOTH))
	{
		perm &= ~(S_IWGRP|S_IWOTH);
		*s++ = 'w';
	}
	if ((perm & (S_IXGRP|S_IXOTH)) == (S_IXGRP|S_IXOTH))
	{
		perm &= ~(S_IXGRP|S_IXOTH);
		*s++ = 'x';
	}
	if (s == o) s = p;

	/*
	 * g
	 */

	p = s;
	if (s > buf) *s++ = ',';
	*s++ = 'g';
	*s++ = '+';
	o = s;
	if (perm & S_ISGID) *s++ = 's';
	if (perm & S_IRGRP) *s++ = 'r';
	if (perm & S_IWGRP) *s++ = 'w';
	if (perm & S_IXGRP) *s++ = 'x';
	if (s == o) s = p;

	/*
	 * o
	 */

	p = s;
	if (s > buf) *s++ = ',';
	*s++ = 'o';
	*s++ = '+';
	o = s;
	if (perm & S_IROTH) *s++ = 'r';
	if (perm & S_IWOTH) *s++ = 'w';
	if (perm & S_IXOTH) *s++ = 'x';
	if (s == o) s = p;
	if (s == buf) *s++ = '0';
	*s = 0;
	return(buf);
}
