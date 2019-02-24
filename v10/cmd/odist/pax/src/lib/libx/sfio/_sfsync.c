#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
_sfsync(reg Sfile_t* f)
#else
_sfsync(f)
reg Sfile_t	*f;	/* stream to be synchronized */
#endif
{
	return _SFSYNC(f);
}
