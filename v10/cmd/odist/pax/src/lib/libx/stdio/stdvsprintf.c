#include	"sfhdr.h"

#if __STDC__ || __cplusplus
_stdvsprintf(char *s, const char *form, va_list args)
#else
_stdvsprintf(s,form,args)
register char	*s;
register char	*form;
va_list		args;
#endif
{
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

	rv = sfvprintf(&f,form,args);
	*f.next = '\0';
	_Sfi = f.next - f.data;

	return rv;
}
