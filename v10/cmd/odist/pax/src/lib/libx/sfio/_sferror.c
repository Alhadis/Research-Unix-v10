#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sferror(reg Sfile_t* f)
#else
sferror(f)
reg Sfile_t	*f;
#endif
{
	return _SFERROR(f);
}
