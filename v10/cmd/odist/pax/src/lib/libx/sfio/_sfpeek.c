#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sfpeek(reg Sfile_t* f, uchar** bp)
#else
sfpeek(f,bp)
reg Sfile_t	*f;	/* file to peek */
uchar		**bp;	/* start of data area */
#endif
{
	return _SFPEEK(f,bp);
}
