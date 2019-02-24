/*
**	Set up access groups.
*/

#include	<sys/param.h>
#include	<setjmp.h>
#include	<grp.h>

#define	SYSERROR	(-1)

static int	catchsys();
static jmp_buf	Sigsysbuf;

int
setupgroups(name, gid)
	char *			name;
	int			gid;
{
	register char **	cpp;
	register short *	gp;
	register struct group *	grp;
	register int		n;
	register int		(*oldsig)();
	int			on;
	short			groups[NGROUPS];

	oldsig = signal(SIGSYS, catchsys);

	if
	(
		setjmp(Sigsysbuf)
		||
		(on = getgroups(NGROUPS, groups)) == SYSERROR
	)
	{
		(void)signal(SIGSYS, oldsig);
		return 0;	/* Not installed */
	}

	(void)signal(SIGSYS, oldsig);

	(void)setgrent();

	gp = groups;

	while ( gp < &groups[NGROUPS] && (grp = getgrent()) != (struct group *)0 )
		for ( cpp = grp->gr_mem ; *cpp != (char *)0 ; cpp++ )
			if ( strcmp(*cpp, name) == 0 && grp->gr_gid != gid )
			{
				*gp++ = grp->gr_gid;
				break;
			}

	(void)endgrent();

	if ( (n = gp-groups) == 0 && on == 0 )
		return 0;

	return setgroups(n, groups);
}

static int
catchsys(sig)
{
	longjmp(Sigsysbuf, 1);
	abort();
}
