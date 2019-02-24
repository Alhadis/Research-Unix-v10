/*
**	Set up parameters for share scheduler
*/

#include <shares.h>
#include <sys/share.h>
#include <signal.h>
#include <setjmp.h>
#include <stdio.h>
#include <errno.h>

#define	DEFSHARES	1	/* Default number of shares for a group 0 user */
#define	SYSERROR	(-1)
#ifndef	ETOOMANYU
#define	ETOOMANYU	ENOSPC	/* Must go in errno.h or lnode.h */
#endif

static int	catchsys();
static jmp_buf	Sigsysbuf;

int
setupshares(uid, efp)
	int			uid;
	void			(*efp)();
{
	register int		(*oldsig)();
	register unsigned long	extime;
	struct sh_consts 	shconsts;
	struct lnode		share;
	extern unsigned long	time();

	if ( uid == 0 )
		return 0;	/* root needs no set-up */

	oldsig = signal(SIGSYS, catchsys);

	if
	(
		setjmp(Sigsysbuf)
		||
		limits((struct lnode *)&shconsts, L_GETCOSTS) == SYSERROR
		||
		(Shareflags & NOSHARE)
	)
	{
		(void)signal(SIGSYS, oldsig);
		return 0;	/* Share not installed/active */
	}

	(void)signal(SIGSYS, oldsig);

	if ( (extime = getshares(&share, uid, 0)) == 0 )
	{
		if ( uid == IDLEUID )
		{
			share.l_shares = IDLESHARES;
			share.l_group = IDLEGROUP;
		}
		else
		if ( uid == OTHERUID )
		{
			share.l_shares = OTHERSHARES;
			share.l_group = OTHERGROUP;
		}
		else
		{
			if ( efp != (void (*)())0 )
				(*efp)("Cannot get shares for uid %d -- using defaults.", uid);
			share.l_shares = DEFSHARES;
		}
	}
	else
	if ( limits(&share, L_OTHLIM) == SYSERROR )
	{
		/*
		**	Decay usage by time since last access.
		*/

		if ( (extime = (time((long *)0) - extime) / Delta) > 0 )
		{
			extern double	pow();

			share.l_usage *= pow(DecayUsage, (float)extime);
		}
	}

	if ( setlimits(&share) == SYSERROR )
	{
		if ( errno == ETOOMANYU )
		{
			char * cp = "other";

			share.l_uid = OTHERUID;

			if ( limits(&share, L_OTHLIM) != SYSERROR )
				(void)setlimits(&share);
			else
				cp = "root";

			if ( efp != (void (*)())0 )
				(*efp)("Warning: system out of share structures, using \"%s\".\n", cp);
		}
		else
		{
			if ( efp != (void (*)())0 )
				(*efp)("setlimits(3) error");
			closeshares();
			return 1;
		}
	}

	closeshares();

	return 0;
}

static int
catchsys(sig)
{
	longjmp(Sigsysbuf, 1);
	abort();
}
