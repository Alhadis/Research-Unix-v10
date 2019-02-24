#include	"sfhdr.h"

/*	Function to clear a locked stream.
**	This is useful for programs that longjmp from the mid of an sfio function.
**	There is no guarantee on data integrity in such a case.
**
**	Written by Kiem-Phong Vo (07/20/90).
*/
#if __STDC__ || __cplusplus
sfclrlock(reg Sfile_t* f)
#else
sfclrlock(f)
reg Sfile_t	*f;
#endif
{
	f->mode &= ~SF_LOCK;
	return sfset(f,f->flags,1);
}
