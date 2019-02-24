#include	"sfhdr.h"

/*	External variables used by sfio
**	Written by Kiem-Phong Vo (06/27/90)
*/

Sfile_t	_Sfstdin =
{	NIL(uchar*),		/* next */
	NIL(uchar*),		/* endw */
	NIL(uchar*),		/* endr */
	NIL(uchar*),		/* endb */
	NIL(Sfile_t*),		/* push */
	SF_READ,		/* flags */
	0,			/* file */
	0L,			/* orig */

	0,			/* size */
	NIL(uchar*),		/* data */
	0L,			/* here */
	0,			/* mode: will be set by sfmode() */
	"",			/* tiny */
	NIL(Sfdisc_t*),		/* disc */
	&_Sfpool,		/* pool */
	NIL(Sfile_t*),		/* back */
	&_Sfstdout		/* fore */
};
Sfile_t _Sfstdout =
{	NIL(uchar*),		/* next */
	NIL(uchar*),		/* endw */
	NIL(uchar*),		/* endr */
	NIL(uchar*),		/* endb */
	NIL(Sfile_t*),		/* push */
	SF_WRITE,		/* flags */
	1,			/* file */
	0L,			/* orig */

	0,			/* size */
	NIL(uchar*),		/* data */
	0L,			/* here */
	0,			/* mode: will be set by sfmode() */
	"",			/* tiny */
	NIL(Sfdisc_t*),		/* disc */
	&_Sfpool,		/* pool */
	&_Sfstdin,		/* back */
	&_Sfstderr		/* fore */
};
Sfile_t _Sfstderr =
{	NIL(uchar*),		/* next */
	NIL(uchar*),		/* endw */
	NIL(uchar*),		/* endr */
	NIL(uchar*),		/* endb */
	NIL(Sfile_t*),		/* push */
	SF_WRITE,		/* flags */
	2,			/* file */
	0L,			/* orig */

	0,			/* size */
	NIL(uchar*),		/* data */
	0L,			/* here */
	0,			/* mode: will be set by sfmode() */
	"",			/* tiny */
	NIL(Sfdisc_t*),		/* disc */
	&_Sfpool,		/* pool */
	&_Sfstdout,		/* back */
	NIL(Sfile_t*)		/* fore */
};

Sfpool_t _Sfpool =		/* the discrete pool of streams */
	{ 0, &_Sfstdin, NIL(Sfpool_t*), NIL(Sfpool_t*) };

Sfile_t	*_Sffree;		/* free list of streams */
Fa_t	*_Fafree;		/* free list of format/arglist stack objects */

int	_Sfi;					/* for fast integer decoding */
int	(*_Sfpclose)_SFA_((Sfile_t*));		/* to close a sfpopen-stream */
int	(*_Sfpmove)_SFA_((Sfile_t*, int));	/* to move a stream in a pool */
Sfile_t	*(*_Sfstack)_SFA_((Sfile_t*, Sfile_t*));/* to manipulate a stack of streams */
void	(*_Sfnotify)_SFA_((Sfile_t*, int));	/* to register streams */
