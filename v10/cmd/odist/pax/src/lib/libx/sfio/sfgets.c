#include	"sfhdr.h"

/*	Read a line into a buffer. The line length is saved in the
**	stream buffer and can be accessed via sfslen(). If the buffer
**	size n is <= 0, the string will be kept in a static area.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
char *sfgets(reg Sfile_t *f, char* us, reg int n)
#else
char *sfgets(f,us,n)
reg Sfile_t	*f;	/* stream to read from */
char		*us;	/* space to read into */
reg int		n;	/* max number of bytes to read */
#endif
{
	reg uchar	*ps, *s;
	reg int		p;
	static uchar	*Sfs;
	static int	Sfn;

	if(f->mode != SF_READ && SFMODE(f,SF_READ) < 0)
		return NIL(char*);
	SFLOCK(f);

	if(n <= 0 || !us)
	{	us = (char*)Sfs;
		n  = -1;
	}
	else	n -= 1;

	s = (uchar*)us;
	while(n)
	{	/* peek the read buffer for data */
		if(SFRPEEK(f,ps,p) <= 0)
			break;

		/* set amount copyable right now */
		if(p > n)
		{	if(n > 0)
				p = n;
			else
			{	/* make sure our buffer has space */
				n = s - Sfs;
				if(n >= Sfn-1)
				{	Sfn += SF_GRAIN;
					Sfs = Sfs ? (uchar*)realloc((char*)Sfs,Sfn) :
						    (uchar*)malloc(Sfn);
					if(!Sfs)
					{	/* can't get more space */
						Sfs = (uchar*)us;
						Sfn -= SF_GRAIN;
						break;
					}
					else
					{	us = (char*)Sfs;
						s = Sfs+n;
					}
				}

				/* copy at most this much */
				if(p > (n = (Sfn-1)-n))
					p = n;
				n = -1;
			}
		}

		if((f->flags&SF_STRING) && !(f->flags&SF_WRITE))
		{	reg uchar *endps = ps+p;
			while(ps < endps && (*s++ = *ps++) != '\n')
				;
		}
		else
		{	reg int	c = ps[p-1]; ps[p-1] = '\n';
			while((*s++ = *ps++) != '\n')
				;
			if((ps - f->next) == p)
				s[-1] = c;
			f->next[p-1] = c;
		}

		/* gobble up read data */
		f->next = ps;

		if(ps[-1] == '\n')
			break;
		else if(n > 0)
			n -= p;
	}

	if((_Sfi = s - ((uchar*)us)) <= 0)
		us = NIL(char*);
	else	*s = '\0';
done :
	SFOPEN(f);
	return us;
}
