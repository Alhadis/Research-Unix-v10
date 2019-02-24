#include	"sfhdr.h"

/*	Print data with a given format
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
sfprintf(Sfile_t *f, const char *form, ...)
#else
sfprintf(f,form,va_alist)
reg Sfile_t	*f;	/* file stream to print to */
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
	rv = sfvprintf(f,form,args);

	va_end(args);
	return rv;
}

#if __STDC__ || __cplusplus
sfsprintf(char *s, int n, const char *form, ...)
#else
sfsprintf(s,n,form,va_alist)
reg char	*s;
reg int		n;
reg char	*form;
va_dcl
#endif
{
	va_list	args;
	Sfile_t	f;
	reg int	rv;

	if(!s || n <= 0)
		return -1;

	/* make a fake stream */
	SFCLEAR(&f);
	f.flags = SF_STRING|SF_WRITE;
	f.mode = SF_STRING;
	f.size = n-1;
	f.data = f.next = f.endr = (uchar*)s;
	f.endb = f.endw = f.data+f.size;
	sfset(&f,f.flags,1);

#if __STDC__ || __cplusplus
	va_start(args,form);
#else
	va_start(args);
#endif
	rv = sfvprintf(&f,form,args);
	*f.next = '\0';
	_Sfi = f.next - f.data;

	va_end(args);

	return rv;
}
