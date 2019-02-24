#include <stdio.h>
#include "filehdr.h"
#include "ldfcn.h"
#include "lddef.h"

LDLIST	*_ldhead = NULL;


LDFILE *
allocldptr()
{
	extern char *calloc();
	extern LDLIST *_ldhead;
	LDLIST *ldptr, *ldindx;
	static int last_fnum_ = 0;

	if ((ldptr = (LDLIST *)calloc(1, LDLSZ)) == NULL)
		return (NULL);
	ldptr->ld_next = NULL;
	if (_ldhead == NULL)
		_ldhead = ldptr;
	else
	{
		for (ldindx = _ldhead;
			ldindx->ld_next != NULL; ldindx = ldindx->ld_next)
		{
		}
		ldindx->ld_next = ldptr;
	}
	ldptr->ld_item._fnum_ = ++last_fnum_;
	return ((LDFILE *)ldptr);
}

static char ID[] = "@(#) allocldptr.c: 1.2 2/16/83";
