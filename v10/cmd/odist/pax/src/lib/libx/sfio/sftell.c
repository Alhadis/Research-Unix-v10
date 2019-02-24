#include	"sfhdr.h"

/*	Tell the current location in a given stream
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
long sftell(reg Sfile_t* f)
#else
long sftell(f)
reg Sfile_t	*f;
#endif
{	
	reg long	here;

	/* set the stream to the right mode */
	if((!f->mode || (f->mode&~(SF_RDWR|SF_STRING))) && _sfmode(f,0) < 0)
		return -1;

	if(f->mode&SF_STRING)
		return (long)(f->next-f->data);

	/* let sfseek() handle the hard case */
	if(f->orig >= 0 && (f->flags&(SF_SHARE|SF_APPEND)))
		return sfseek(f,0L,1);

	here = f->here + ((f->mode&SF_WRITE) ? f->next-f->data : f->next-f->endb);
	return here - (f->orig > 0 ? f->orig : 0L);
}
