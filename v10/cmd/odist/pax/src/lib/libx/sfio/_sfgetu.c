#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
ulong sfgetu(reg Sfile_t* f)
#else
ulong sfgetu(f)
reg Sfile_t	*f;
#endif
{
	return _SFGETU(f);
}
