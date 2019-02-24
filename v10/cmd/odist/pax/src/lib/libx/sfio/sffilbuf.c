#include	"sfhdr.h"

/*	Fill the buffer of a stream with data.
**	sffilbuf behaves differently based on the second argument 'type'.
**	If type > 0, the first byte read is returned.
**	If type == 0 and in line mode, sffilbuf tries to read up to \n and
**		if \n is detected, the number of bytes up to \n is returned.
**	In all other cases, the number of bytes read is returned.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
_sffilbuf(reg Sfile_t* f, int type)
#else
_sffilbuf(f,type)
reg Sfile_t	*f;	/* fill the read buffer of this stream */
int		type;	/* see above */
#endif
{
	reg int		r, n;
	reg uchar	*s, *data, *endb, *line, c;
	reg int		(*readf) _SFA_((Sfile_t*, uchar*, int, char*));
	reg long	(*seekf) _SFA_((Sfile_t*, long, int, char*));
	reg char	*handle;

redo_loop:
	if(f->mode != SF_READ && SFMODE(f,SF_READ) < 0)
		return -1;
	SFLOCK(f);

	readf = f->disc ? f->disc->readf : NIL(int(*) _SFA_((Sfile_t*,uchar*,int,char*)));
	seekf = f->disc ? f->disc->seekf : NIL(long(*) _SFA_((Sfile_t*,long,int,char*)));
	handle = f->disc ? f->disc->handle : NIL(char*);

	if(((endb = f->endb) - f->next) <= 0)
	{	/* no more string data */
		if(f->mode&SF_STRING)
		{	r = 0;
			goto at_eof;
		}
		endb = f->next = f->endb = f->endr = f->data;
	}
	else if(type > 0)
		goto done;

	line = NIL(uchar*);
	n = f->size - (endb - f->data);
	data = endb;
	while(n > 0)
	{
		if(f->flags&SF_SHARE)
			f->here = SEEKF(seekf,f,f->here,0,handle);
		if((r = READF(readf,f,data,n,handle)) > 0)
		{
			f->here += r;
			f->endb = (endb += r);
			if(type > 0) /* return a char */
				goto done;

			/* normal filbuf */
			if(type < 0 || !(f->flags&SF_LINE))
				goto done;

			/* must see a \n */
			c = endb[-1]; endb[-1] = '\n';
			s = data;
			while(*s++ != '\n')
				;
			endb[-1] = c;
			if(s < endb || c == '\n')
			{
				line = s;
				goto done;
			}

			/* read some more */
			n -= r;
			data += r;
		}
		else
		{	/* eof or error */
		at_eof :
			switch(_sfexcept(f,SF_READ,r))
			{
			case SF_ESTACK :
				if(f->next < (endb = f->endb))
					goto done;
				/* else fall through */
			case SF_EDISC :
				SFopen(f);
				goto redo_loop;
			case SF_EDONE :
				goto done;
			}
		}
	}

done:
	r = type > 0 ?	(f->next < endb ? (int)(*f->next++) : EOF) :
			((line ? line : endb) - f->next);
	SFOPEN(f);
	return r;
}
