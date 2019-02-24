#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
Sfile_t* sftmp(reg int n)
#else
Sfile_t* sftmp(n)
reg int	n;
#endif
{
	return _SFTMP(n);
}
