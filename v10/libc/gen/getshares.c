/*LINTLIBRARY*/

/*
**	Fill in details for lnode from ``shares'' record.
**
**	Returns 0 for failure, or last active time of user.
*/ 

#include	<shares.h>
#include	<libc.h>


unsigned long
getshares(lp, uid, lock)
	register struct lnode *	lp;
	int			uid;
	int			lock;
{
	register long		posn;
	Share			share;

	lp->l_uid = uid;
	lp->l_flags = 0;
	lp->l_shares = 0;
	lp->l_group = 0;
	lp->l_usage = 0;
	lp->l_charge = 0;

	if ( uid > MAXUID )
		return 0;

	if ( !openshares(lock) )
		return 0;

	posn = sizeof(Share) * uid;

	if ( lseek(ShareFd, posn, 0) == SYSERROR )
		return 0;

	if ( read(ShareFd, &share, sizeof(Share)) != sizeof(Share) )
		return 0;

	if ( share.l.l_uid != uid )
		return 0;

	*lp = share.l;

	if ( share.extime == 0 )
		return 1;

	return share.extime;
}
