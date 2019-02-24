#include	"sfhdr.h"


/*	Set the function to be called when a stream is opened or closed
**
**	Written by Kiem-Phong Vo (01/06/91)
*/
#if __STDC__ || __cplusplus
sfnotify(void (*notify)(Sfile_t*, int))
#else
sfnotify(notify)
void	(*notify)();
#endif
{
	_Sfnotify = notify;
}
