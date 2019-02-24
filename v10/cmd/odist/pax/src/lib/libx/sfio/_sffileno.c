#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sffileno(reg Sfile_t* f)
#else
sffileno(f)
reg Sfile_t	*f;
#endif
{
	return _SFFILENO(f);
}
