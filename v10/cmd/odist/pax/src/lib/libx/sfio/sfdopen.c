#include	"sfhdr.h"

/*	Open a stream given a file descriptor.
**
**	Written by Kiem-Phong Vo (06/27/90).
*/

#if !__STDC__ && !__cplusplus
#ifndef const
#define const
#endif
#endif

Sfile_t *sfdopen(fd,mode)
reg int		fd;
reg const char	*mode;
{
	reg int		sflags;
	reg Sfile_t	*f;

	if(fd < 0 || !mode || !mode[0])
		return NIL(Sfile_t*);

	/* construct the open flags */
	sflags = 0;
	while(1) switch(*mode++)
	{
	case 'w' :
		sflags = SF_WRITE;
		break;
	case 'a' :
		(void) lseek(fd,0L,2);
		sflags = SF_WRITE | SF_APPEND;
		break;
	case 'r' :
		sflags = SF_READ;
		break;
	case '+' :
		sflags |= SF_READ|SF_WRITE;
	case 'b' :
		break;
	case  0  :
		goto e_mode;
	default :
		return NIL(Sfile_t*);
	}
e_mode:
	if(!sflags)
		return NIL(Sfile_t*);

	if((f = sfnew(NIL(Sfile_t*),NIL(uchar*),-1,fd,sflags)) != NIL(Sfile_t*))
		if(mode[0] != 'r' && f->mode != SF_WRITE)
			_sfmode(f,SF_WRITE);

	return f;
}
