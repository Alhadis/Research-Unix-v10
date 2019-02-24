#include	"sfhdr.h"

/*	Put out a nul-terminated string
**
**	Written by Kiem-Phong Vo
*/
#if __STDC__ || __cplusplus
sfputs(reg Sfile_t* f, const char* s, int t)
#else
sfputs(f,s,t)
reg Sfile_t	*f;
char		*s;
int		t;
#endif
{
	reg int		p;
	reg uchar	*os, *ps;
	reg int		n;

	if(f->mode != SF_WRITE && SFMODE(f,SF_WRITE) < 0)
		return -1;

	SFLOCK(f);
	if(f->size <= 0 || (uchar*)s == f->next)
	{	/* unbuffered stream or writing after sfpeek() */
		n = strlen((char*)s);
		SFopen(f);
		if((p = sfwrite(f,(uchar*)s,n)) == n && t != 0 && sfputc(f,t) >= 0)
			p += 1;
		goto done;
	}

	os = (uchar*)s;
	while(1)
	{	/* peek buffer for space */
		if(SFWPEEK(f,ps,p) <= 0)
			break;

		/* fast copy loop */
		while((*ps++ = *os++) != '\0' && --p > 0)
			;

		if(ps[-1] == '\0')
		{	/* done */
			p = -1;
			if(t != 0)	/* add the last char if any */
				ps[-1] = t;
			else	{ ps -= 1; os -= 1; }
		}

		/* move the write pointer */
		f->next = ps;
		if(p < 0)
			break;
	}

	p = (char*)os - s;

	if((f->flags&SF_LINE) && !(f->flags&SF_STRING))
	{	/* see if we need to flush the buffer */
		if(t == '\n')
			t = HIFORLINE;
		else if((t = (ps = f->next) - (os = f->data)) < HIFORLINE && t > 0)
		{	/* long check for embedded \n */
			n = os[0]; os[0] = '\n';
			while(*--ps != '\n')
				;
			if(ps > os || n == '\n')
				t = HIFORLINE;
			os[0] = n;
		}
		if(t >= HIFORLINE)
			{ SFopen(f); (void) _sfflsbuf(f,-1); }
	}

done :
	SFOPEN(f);
	return p;
}
