#include	"sfhdr.h"

/*	Push back one byte to a given SF_READ stream
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
sfungetc(reg Sfile_t* f, reg int c)
#else
sfungetc(f,c)
reg Sfile_t	*f;	/* push back one byte to this stream */
reg int		c;	/* the value to be pushed back */
#endif
{
	reg uchar	*cp;

	if(c < 0 || (f->mode != SF_READ && SFMODE(f,SF_READ) < 0) || f->next <= f->data)
		return -1;

	SFLOCK(f);
	if(*(cp = --f->next) != (uchar)c)
	{
		if((f->flags&SF_STRING) && !(f->flags&SF_WRITE))
		{	/* avoid writing into a constant string */
			f->next += 1;
			c = -1;
		}
		else	*cp = (uchar)c;
	}
	SFOPEN(f);
	return c;
}
