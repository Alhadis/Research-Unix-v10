#include	"_sfhdr.h"

#if __STDC__ || __cplusplus
sfputc(reg Sfile_t* f, reg int c)
#else
sfputc(f,c)
reg Sfile_t	*f;
reg int		c;
#endif
{
	return _SFPUTC(f,c);
}
