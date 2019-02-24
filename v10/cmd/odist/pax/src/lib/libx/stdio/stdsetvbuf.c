#include	"sfhdr.h"
#include	"stdio.h"

/*	Stdio function setvbuf()
**
**	Written by Kiem-Phong Vo (12/10/90)
*/

#if __STDC__ || __cplusplus
_stdsetvbuf(Sfile_t* f, char *buf, int type, int size)
#else
_stdsetvbuf(f,buf,type,size)
Sfile_t	*f;
char	*buf;
int	type;
int	size;
#endif
{
	if(type == _IOLBF)
		sfset(f,SF_LINE,1);
	else if((f->flags&SF_STRING))
		return -1;
	else if(type == _IONBF)
	{	sfsync(f);
		sfsetbuf(f,NIL(uchar*),0);
	}
	else if(type == _IOFBF)
	{	if(size == 0)
			size = SF_BUFSIZE;
		sfsync(f);
		sfsetbuf(f,(uchar*)buf,size);
	}

	return 0;
}
