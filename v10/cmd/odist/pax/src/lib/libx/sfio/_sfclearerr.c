#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sfclearerr(reg Sfile_t* f)
#else
sfclearerr(f)
reg Sfile_t	*f;
#endif
{
	return _SFCLEARERR(f);
}
