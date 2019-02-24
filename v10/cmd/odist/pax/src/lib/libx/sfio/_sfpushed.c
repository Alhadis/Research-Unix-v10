#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
Sfile_t* sfpushed(reg Sfile_t* f)
#else
Sfile_t* sfpushed(f)
reg Sfile_t	*f;
#endif
{
	return _SFPUSHED(f);
}
