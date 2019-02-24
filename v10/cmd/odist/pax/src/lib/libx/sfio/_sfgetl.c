#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
long sfgetl(reg Sfile_t* f)
#else
long sfgetl(f)
reg Sfile_t	*f;
#endif
{
	return _SFGETL(f);
}
