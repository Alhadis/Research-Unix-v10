#include	"sfhdr.h"
#include	"stdio.h"


/*	sprintf function
**
**	Written by Kiem-Phong Vo (12/10/90)
*/


#if __STDC__ || __cplusplus
_stdsprintf(char *s, const char *form, ...)
#else
_stdsprintf(s,form,va_alist)
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
	f.flags = SF_STRING|SF_WRITE;
	f.mode = SF_STRING;
	f.size = 4*SF_BUFSIZE;
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
