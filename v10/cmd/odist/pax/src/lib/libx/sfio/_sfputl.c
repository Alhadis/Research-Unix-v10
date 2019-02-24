#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sfputl(reg Sfile_t* f, reg long v)
#else
sfputl(f,v)
reg Sfile_t	*f;
reg long	v;
#endif
{
	return _SFPUTL(f,v);
}
