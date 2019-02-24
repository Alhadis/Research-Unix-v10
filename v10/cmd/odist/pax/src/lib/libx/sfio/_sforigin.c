#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
long sforigin(reg Sfile_t* f)
#else
long sforigin(f)
reg Sfile_t	*f;
#endif
{
	return _SFORIGIN(f);
}
