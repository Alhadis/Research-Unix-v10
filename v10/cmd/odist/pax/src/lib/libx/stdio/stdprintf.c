#include	"sfhdr.h"
#include	"stdio.h"


/*	printf function
**
**	Written by Kiem-Phong Vo (12/10/90)
*/

#if __STDC__ || __cplusplus
_stdprintf(const char *form, ...)
#else
_stdprintf(form,va_alist)
reg char	*form;	/* print format */
va_dcl
#endif
{
	va_list	args;
	reg int	rv;

#if __STDC__ || __cplusplus
	va_start(args,form);
#else
	va_start(args);
#endif
	rv = sfvprintf(sfstdout,form,args);

	va_end(args);
	return rv;
}
