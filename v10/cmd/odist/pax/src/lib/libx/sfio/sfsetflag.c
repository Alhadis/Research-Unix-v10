#include	"sfhdr.h"

/*	Set some control flags for the stream
**
**	Written by Kiem-Phong Vo (07/16/90)
*/
#define RDSTRING(f)	((f->flags&SF_RDSTR) == SF_RDSTR)
#define WRSTRING(f)	((f->flags&SF_WRSTR) == SF_WRSTR)
#define RDSTREAM(f)	(f->mode&SF_READ)
#define WRSTREAM(f)	((f->mode&SF_WRITE) && !(f->flags&SF_LINE))

#if __STDC__ || __cplusplus
sfsetflag(reg Sfile_t* f, reg int flags, reg int on)
#else
sfsetflag(f,flags,on)
reg Sfile_t	*f;
reg int		flags;
reg int		on;
#endif
{
	if(!f->mode) /* a std-stream */
		(void)sfnew(f,NIL(uchar*),-1,f->file,f->flags);
	else if(SFFROZEN(f))
		return 0;

	SFLOCK(f);

	/* can't touch this mode */
	if(!(f->flags&SF_BOTH))
		flags &= ~SF_RDWR;
	/* must have at least one read/write mode */
	else if(!on && (flags&SF_RDWR) && (flags&SF_RDWR) != SF_RDWR)
		f->flags |= SF_RDWR;

	if(on)
		f->flags |=  (flags&SF_SETS);
	else	f->flags &= ~(flags&SF_SETS);

	if(!(f->mode&(SF_PUSH|SF_POOL)) && (RDSTRING(f) || RDSTREAM(f)))
		f->flags |= SF_GETC;
	else	f->flags &= ~SF_GETC;

	if(!(f->mode&(SF_PUSH|SF_POOL)) && (WRSTRING(f) || WRSTREAM(f)))
		f->flags |= SF_PUTC;
	else	f->flags &= ~SF_PUTC;

	if(f->orig < 0)
		f->flags &= ~(SF_RELATIVE|SF_SHARE|SF_APPEND);

	SFOPEN(f);

	if(!(f->mode&SF_STRING))
	{	/* if mode is exclusive, f must be in the right mode */
		flags = f->flags&SF_RDWR;
		if((flags == SF_WRITE && !(f->mode&SF_WRITE)) ||
		   (flags == SF_READ && !(f->mode&(SF_READ|SF_SYNC))) )
			_sfmode(f,flags);
	}

	return f->flags;
}
