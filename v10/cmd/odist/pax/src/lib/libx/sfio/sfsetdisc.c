#include	"sfhdr.h"

/*	Set a new discipline for a stream.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
Sfdisc_t* sfsetdisc(reg Sfile_t* f, reg Sfdisc_t* disc)
#else
Sfdisc_t* sfsetdisc(f,disc)
reg Sfile_t	*f;
reg Sfdisc_t	*disc;
#endif
{
	reg Sfdisc_t	*old;

	/* set the stream to the right mode */
	if((!f->mode || (f->mode&~(SF_RDWR|SF_STRING))) && _sfmode(f,0) < 0)
		return NIL(Sfdisc_t*);

	/* now set the new discipline */
	if((old = f->disc) && old->exceptf)
		(*(old->exceptf))(f,0,old->handle);
	f->disc = disc;

	return old;
}
