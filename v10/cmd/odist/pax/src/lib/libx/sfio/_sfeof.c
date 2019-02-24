#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sfeof(reg Sfile_t* f)
#else
sfeof(f)
reg Sfile_t	*f;
#endif
{
	return _SFEOF(f);
}
