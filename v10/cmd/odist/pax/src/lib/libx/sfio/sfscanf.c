#include	"sfhdr.h"

/*	Read formated data from a stream
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
sfscanf(Sfile_t *f, const char *form, ...)
#else
sfscanf(f,form,va_alist)
reg Sfile_t	*f;	/* file to be scanned */
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
	rv = sfvscanf(f,form,args);
	va_end(args);
	return rv;
}

#if __STDC__ || __cplusplus
sfsscanf(const char *s, const char *form,...)
#else
sfsscanf(s,form,va_alist)
reg char	*s;
reg char	*form;
va_dcl
#endif
{
	va_list	args;
	Sfile_t	f;
	reg int	rv;

	if(!s)
		return -1;

	/* make a fake stream */
	SFCLEAR(&f);
	f.flags = SF_STRING|SF_READ;
	f.mode = SF_STRING;
	f.size = strlen((char*)s);
	f.data = f.next = f.endw = (uchar*)s;
	f.endb = f.endr = f.data+f.size;
	sfset(&f,f.flags,1);

#if __STDC__ || __cplusplus
	va_start(args,form);
#else
	va_start(args);
#endif
	rv = sfvscanf(&f,form,args);
	va_end(args);

	return rv;
}
