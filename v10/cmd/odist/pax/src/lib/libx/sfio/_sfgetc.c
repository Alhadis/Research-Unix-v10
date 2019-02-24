#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sfgetc(reg Sfile_t* f)
#else
sfgetc(f)
reg Sfile_t	*f;
#endif
{
	return _SFGETC(f);
}
