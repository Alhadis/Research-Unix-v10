/*LINTLIBRARY*/

/*
**	Write a new ``shares'' file record from details in lnode.
**
**	Returns 0 for illegal uid,
**		SYSERROR for bad system call,
**		else sizeof(Share).
*/ 

#include	<shares.h>
#include	<libc.h>


int
putshares(lp, extime)
	register struct lnode *	lp;
	unsigned long		extime;
{
	register long		posn;
	Share			share;

	if ( lp->l_uid > MAXUID )
		return 0;

	if ( !openshares(1) )
		return SYSERROR;

	share.l = *lp;
	share.extime = extime;

	posn = sizeof(Share) * lp->l_uid;

	if ( lseek(ShareFd, posn, 0) == SYSERROR )
		return SYSERROR;

	return write(ShareFd, &share, sizeof(Share));
}
