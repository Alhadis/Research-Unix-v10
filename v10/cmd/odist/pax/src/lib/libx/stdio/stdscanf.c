#include	"sfhdr.h"
#include	"stdio.h"

/*	Read formatted data from a stream
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
_stdscanf(const char *form, ...)
#else
_stdscanf(form,va_alist)
reg char	*form;	/* scanning format */
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
	rv = sfvscanf(sfstdin,form,args);
	va_end(args);
	return rv;
}
