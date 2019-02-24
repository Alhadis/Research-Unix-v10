#include	"sfhdr.h"

/*	Close a given file stream. If the file stream is associated with
**	a process via sfpopen(), both streams will be closed. The closed
**	stream is synced.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
sfclose(reg Sfile_t *f)
#else
sfclose(f)
reg Sfile_t	*f;
#endif
{
	reg int		rv, flags;

	if(!f)
	{	/* close-all */
		while(1)
		{	reg Sfpool_t	*p;

			/* the "goto" is because the lists change after each sfclose */
			for(p = &_Sfpool; p != NIL(Sfpool_t*); p = p->fore)
			for(f = p->head; f; f = f->fore)
				if(!SFFROZEN(f))
					goto close_f;
		close_f:
			if(!f)
				return 0;
			(void) sfclose(f);
		}
	}

	if(SFFROZEN(f))
		return -1;
	flags = f->flags;

	/* closing a stack of streams */
	while(f->push)
		if(sfclose((*_Sfstack)(f,NIL(Sfile_t*))) < 0)
			return -1;

	/* tell the register function */
	if(_Sfnotify)
		(*_Sfnotify)(f,SF_EOF);

	/* this is from popen */
	if(_Sfpclose && (flags&SF_PROCESS))
		return (*_Sfpclose)(f);

	/* sync file pointer */
	f->mode &= ~SF_POOL;
	rv = sfsync(f);

	/* terminate discipline */
	if(f->disc && f->disc->exceptf)
		(*(f->disc->exceptf))(f,SF_EOF,f->disc->handle);

	/* free space used */
	if(!(flags&SF_REUSE))
	{	/* remove from the list of active streams */
		if(f->pool)
		{	if(_Sfpmove)
				(*_Sfpmove)(f,-1);
			else
			{	if(f->fore)
					f->fore->back = f->back;
				if(f->back)
					f->back->fore = f->fore;
				else	f->pool->head = f->fore;
			}
			f->pool = NIL(Sfpool_t*);
			f->fore = f->back = NIL(Sfile_t*);
		}

		f->disc = NIL(Sfdisc_t*);

		if(f->data && (flags&SF_MALLOC))
		{	free((char*)f->data);
			f->data = NIL(uchar*);
			f->size = 0;
		}
	}

	if(!(flags&(SF_KEEPFD|SF_STRING)))
		while(close(f->file) < 0 && errno == EINTR)
			errno = 0;
	f->file = -1;

	if(!(flags&SF_REUSE) && f != sfstdin && f != sfstdout && f != sfstderr)
		SFFREE(f);
	else
	{	f->mode = 0;
		f->flags = 0;
		f->here = f->orig = 0L;
		f->endb = f->endr = f->endw = f->next = f->data;
	}

	return rv;
}
