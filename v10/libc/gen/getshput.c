/*LINTLIBRARY*/

/*
**	Reads, modifies, and writes the record in the ``shares'' file
**	whose uid matches "lp->l_uid".
**
**	The function indicated by the "func" argument is called with the address
**	of the original structure, and the address of the structure to be updated.
**
**	Returns value returned by passed function for success and 0 for error
**	But note, that if the function returns 0 itself, then the ``shares'' file
**	will not be updated.
**
**	Otherwise, the value returned by the function is used to update the
**	``extime'' field in the shares record.
*/ 

#include	<shares.h>


unsigned long
getshput(lp, func)
	struct lnode *	lp;
	unsigned long	(*func)();
{
	unsigned long	ri;
	struct lnode	p;

	(void)getshares(&p, lp->l_uid, 1);

	if ( (ri = (*func)(lp, &p)) != 0 )
		if ( putshares(&p, ri) <= 0 )
			return 0;

	return ri;
}
