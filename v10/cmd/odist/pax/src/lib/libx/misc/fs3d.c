/*
 * AT&T Bell Laboratories
 *
 * disable (0) / enable (1) / tell (2) 3d fs operations
 * the argument must be 0, 2 or a previous fs3d() return value
 * the return value is the previous state
 */

#define OP3D	010000
#define ON3D	001000

extern int	umask();

int
fs3d(n)
register int	n;
{
	register int	o;

	o = umask(0) | OP3D;
	switch (n)
	{
	case 0:
		(void)umask(o & ~ON3D);
		break;
	case 1:
		(void)umask(o | ON3D);
		break;
	default:
		(void)umask(o);
		break;
	}
	return((o & ON3D) != 0);
}
