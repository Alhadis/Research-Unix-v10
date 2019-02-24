#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sfputd(reg Sfile_t* f, reg double v)
#else
sfputd(f,v)
reg Sfile_t	*f;
reg double	v;
#endif
{
	return _SFPUTD(f,v);
}
