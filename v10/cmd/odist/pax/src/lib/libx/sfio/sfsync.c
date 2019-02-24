#include	"sfhdr.h"

static char	*Version = "\n@(#)sfio (AT&T Bell Laboratories) 12/07/90\0\n";

/*	Synchronize data in buffers with the file system.
**	If f is nil, all streams are sync-ed
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
_sfsync(reg Sfile_t* f)
#else
_sfsync(f)
reg Sfile_t	*f;	/* stream to be synchronized */
#endif
{
	reg int		n, rv, flag;

	if(!f)
	{	/* use recursion to handle the sync-all case */
		reg Sfpool_t	*p;

		rv = 0;
		for(p = &_Sfpool; p != NIL(Sfpool_t*); p = p->fore)
			for(f = p->head; f != NIL(Sfile_t*); f = f->fore)
			{	/* can't sync these */	
				if(SFFROZEN(f))
					continue;

				/* make sure pool list is not rearranged */
				flag = f->mode&SF_POOL;
				f->mode &= ~SF_POOL;
				if(sfsync(f) < 0)
					rv = -1;
				f->mode |= flag;
			}
		return rv;
	}

	if(f->mode == 0 || (f->mode&SF_SYNC))
		return 0;

	if((f->mode&~(SF_RDWR|SF_STRING)) && _sfmode(f,0) < 0)
		return -1;

	rv = 0;
	for(; f; f = f->push)
	{	/* pretend that this stream is not on a stack */
		SFLOCK(f);
		flag = f->mode&SF_PUSH;
		f->mode &= ~SF_PUSH;

		if(f->mode&SF_WRITE)
		{	/* sync the buffer, make sure pool don't move */
			SFopen(f);
			flag = f->mode&SF_POOL;
			f->mode &= ~SF_POOL;
			if(_sfflsbuf(f,-1) < 0)
				rv = -1;
			f->mode |= flag;
			SFLOCK(f);
		}
		else if((f->mode&SF_READ) && !(f->flags&SF_SHARE) &&
			f->orig >= 0 && (n = f->endb-f->next) > 0)
		{	/* make sure the file pointer is at the right place */
			reg long	(*seekf)_SFA_((Sfile_t*,long,int,char*));
			reg char	*handle;

			f->mode = SF_SYNC|SF_LOCK;
			seekf = f->disc ? f->disc->seekf :
					NIL(long(*)_SFA_((Sfile_t*,long,int,char*)));
			handle = f->disc ? f->disc->handle : NIL(char*);
			f->here -= n;
			if(SEEKF(seekf,f,f->here,0,handle) < 0)
				rv = -1;
		}

		f->mode |= flag;
		SFOPEN(f);
	}

	return rv;
}
