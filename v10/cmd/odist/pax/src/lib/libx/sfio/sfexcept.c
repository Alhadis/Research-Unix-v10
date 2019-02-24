#include	"sfhdr.h"

/*	Function to handle io exceptions.
**	Written by Kiem-Phong Vo (8/18/90)
*/
#if __STDC__ || __cplusplus
_sfexcept(reg Sfile_t* f, reg int type, reg int io)
#else
_sfexcept(f,type,io)
reg Sfile_t	*f;	/* stream where the exception happened */
reg int		type;	/* io type that was performed */
reg int		io;	/* the io return value that indicated exception */
#endif
{
	reg int		d;
	reg uchar	*data;
	reg int		(*exceptf)_SFA_((Sfile_t*,int,char*));
	reg char	*handle;

	f->flags |= io < 0 ? SF_ERROR : SF_EOF;

	exceptf = f->disc ? f->disc->exceptf : NIL(int(*)_SFA_((Sfile_t*,int,char*)));
	handle = f->disc ? f->disc->handle : NIL(char*);
	if(exceptf)
	{	/* check with discipline function */
		SFOPEN(f); d = (*exceptf)(f,type,handle); SFLOCK(f);
		if(d < 0)
			return SF_EDONE;
		else if(d > 0)
			return SF_EDISC;
	}

	/* handle default cases for string streams */
	if((f->mode&SF_STRING) && io >= 0)
	{	if(type == SF_READ || (f->size >= 0 && !(f->flags&SF_MALLOC)))
			goto chk_stack;

		/* extend buffer */
		if((d = f->size) < 0)
			d = 0;
		d = ((d+io+SF_GRAIN)/SF_GRAIN)*SF_GRAIN;
		data = (uchar*)(f->size > 0 ? realloc((char*)f->data,d) : malloc(d));
		if(!data)
			goto chk_stack;
		f->flags |= SF_MALLOC;
		f->endr = f->endw = f->data = data;
		f->endb = data+d;
		f->next = data+(f->size > 0 ? f->size : 0);
		f->size = d;
		return SF_ECONT;
	}

	if(errno == EINTR)
	{	/* if just an interrupt, we can continue */
		errno = 0;
		f->flags &= ~(SF_EOF|SF_ERROR);
		return SF_ECONT;
	}

chk_stack:
	if(f->push &&
	   ((type == SF_READ  && f->next >= f->endb) ||
	    (type == SF_WRITE && f->next <= f->data)))
	{	/* pop the stack */
		SFOPEN(f);
		sfclose((*_Sfstack)(f,NIL(Sfile_t*)));
		return SF_ESTACK;
	}

	return SF_EDONE;
}
