#include	"sfhdr.h"

/*	Switch the given stream to a desired mode
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
_sfmode(reg Sfile_t* f, reg int wanted)
#else
_sfmode(f,wanted)
reg Sfile_t	*f;	/* change r/w mode and sync file pointer for this stream */
reg int		wanted;	/* desired mode */
#endif
{
	reg int		n, rv;
	reg long	addr, (*seekf)_SFA_((Sfile_t*,long,int,char*));
	reg char	*handle;

	if(SFFROZEN(f))
		return -1;

	SFLOCK(f);
	rv = 0;

	if(f->mode&SF_POOL)
	{	/* move to head of pool */
		if(f != f->pool->head)
		{	if(SFFROZEN(f->pool->head))
			{	SFOPEN(f);
				return -1;
			}
			(*_Sfpmove)(f,0);
		}
		f->mode &= ~SF_POOL;
		if((n = f->mode&~SF_LOCK) != 0 && (wanted == 0 || n == wanted))
			goto done;
	}

	seekf = f->disc ? f->disc->seekf : NIL(long(*)_SFA_((Sfile_t*,long,int,char*)));
	handle = f->disc ? f->disc->handle : NIL(char*);
	switch(f->mode&~SF_LOCK)
	{
	case 0: /* initializing an unitialized std-stream */
		SFopen(f); (void) sfnew(f,NIL(uchar*),-1,f->file,f->flags);
		goto done;

	case SF_WRITE: /* switching to SF_READ */
		if(!(f->flags&SF_READ))
			rv = -1;
		else
		{	/* reset buffer */
			SFopen(f);
			if((rv = _sfflsbuf(f,-1)) < 0)
				goto done;
			SFLOCK(f);
			if(f->flags&SF_APPEND)
				f->here = SEEKF(seekf,f,0L,1,handle);
			if(f->size == 0)
			{	/* unbuffered */
				f->data = f->tiny;
				f->size = sizeof(f->tiny);
			}
			f->next = f->endr = f->endw = f->endb = f->data;
			f->mode = SF_READ;
			if(f->size > 1 && f->orig >= 0)
				(void) sfseek(f,f->here-f->orig,0);
		}
		goto done;

	case SF_SYNC: /* a previously sync-ed read stream */
		if(wanted == SF_WRITE)
		{	/* can't do it */
			if(!(f->flags&SF_WRITE))
				rv = -1;
			else	goto do_write;
		}
		else
		{	/* just reset the pointers */
			f->mode = SF_READ|SF_LOCK;
			addr = f->here + (f->endb - f->next);
			if((addr = SEEKF(seekf,f,addr,0,handle)) < 0)
				rv = -1;
			else	f->here = addr;
		}
		goto done;

	case SF_READ: /* switching to SF_WRITE */
		if(!(f->flags&SF_WRITE))
		{	rv = -1;
			goto done;
		}

		if((n = f->endb-f->data) > 0)
		{	/* reset file pointer */
			addr = f->here - n;
			if((addr = SEEKF(seekf,f,addr,0,handle)) < 0)
			{	rv = -1;
				goto done;
			}
			else	f->here = addr;
		}
	do_write:
		if(f->data == f->tiny)
		{
			f->endb = f->data = f->next = NIL(uchar*);
			f->size = 0;
		}
		else	f->endb = f->data + f->size;
		f->mode = SF_WRITE;
		goto done;
	}
done :
	SFopen(f);
	sfset(f,f->flags,1);
	return rv;
}
