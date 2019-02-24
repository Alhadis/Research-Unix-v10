/*LINTLIBRARY*/

/*
**	Install lnode pointed at by arg.
**
**	If this lnode is a member of a group, install the group first, recursively.
**
**	Returns 0 for success, or SYSERROR.
*/ 

#include	<shares.h>



setlimits(lp)
	register struct lnode *	lp;
{
	struct lnode		gl;

	if ( lp->l_group != 0 )
	{
		if ( getshares(&gl, lp->l_group, 0) == 0 )
			lp->l_group = 0;
		else
		if ( setlimits(&gl) == SYSERROR )
			return SYSERROR;
	}

	return limits(lp, L_SETLIM);
}
