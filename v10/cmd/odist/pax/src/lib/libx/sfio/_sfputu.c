#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sfputu(reg Sfile_t* f, reg ulong v)
#else
sfputu(f,v)
reg Sfile_t	*f;
reg ulong	v;
#endif
{
	return _SFPUTU(f,v);
}
