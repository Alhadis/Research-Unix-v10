#include	"sfhdr.h"

#if __STDC__ || __cplusplus
_stdvsscanf(char *s, const char *form, va_list args)
#else
_stdvsscanf(s,form,args)
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
	f.flags = SF_STRING|SF_READ;
	f.mode = SF_STRING;
	f.size = strlen((char*)s);
	f.data = f.next = f.endr = (uchar*)s;
	f.endb = f.endw = f.data+f.size;
	sfset(&f,f.flags,1);

	return sfvscanf(&f,form,args);
}
