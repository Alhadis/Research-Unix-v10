#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sfulen(reg ulong v)
#else
sfulen(v)
reg ulong	v;
#endif
{
	return _SFULEN(v);
}
