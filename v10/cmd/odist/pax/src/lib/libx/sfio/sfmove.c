#include	"sfhdr.h"

#if sparc
#include	<sys/mman.h>
#define MAPSIZE	(256*1024)
#endif

/*	Move data from one stream to another.
**	This code is written so that it'll work even in the presence
**	of stacking streams, pool, and discipline.
**	If you must change it, be gentle.
**
**	Written by Kiem-Phong Vo (12/07/90)
*/

#if __STDC__ || __cplusplus
sfmove(Sfile_t* fr, Sfile_t* fw, reg long n, const char* rs)
#else
sfmove(fr,fw,n,rs)
Sfile_t		*fr;	/* moving data from this stream */
Sfile_t		*fw;	/* moving data to this stream */
reg long	n;	/* number of bytes/records to move. <0 for unbounded move */
reg char	*rs;	/* record separators */
#endif
{
	reg uchar	*cp, *next;
	reg int		r, w, rc;
	reg int		direct;
	reg long	n_move;
	uchar		rbuf[SF_GRAIN];
	char		rsv[(uchar)(~0)];
#if sparc
	caddr_t		mbuf = (caddr_t)(-1);
	off_t		msize = 0;
#endif

	if(!fr)
		return 0;

	if(rs)
	{	/* set up the mapping to determine which byte is a separator */
		if(*rs)
		{	memclear(rsv,sizeof(rsv));
			if(rs[0] == '\\' && rs[1] == '0')
			{	/* coding of the zero byte */
				rsv[rc = 0] = 1;
				rs += 2;
			}
			else	rc = (uchar)(*rs);

			/* coding of the rest of the bytes */
			while(r = (uchar)(*rs++))
				rsv[r] = 1;
			rs = rsv;
		}
		else	rs = NIL(char*);
	}

	for(n_move = 0; n != 0; )
	{	/* get the input stream into the right mode */
		if(fr->mode != SF_READ && SFMODE(fr,SF_READ) < 0)
			break;

		/* try reading a block of data */
		direct = 0;
		if((r = fr->endb - (next = fr->next)) <= 0)
		{	/* sffilbuf handles stack and discipline */
			if(fr->disc || fr->push)
			{	(void)_sffilbuf(fr,-1);
				if((r = fr->endb - (next = fr->next)) <= 0)
					break;
			}
			/* no more data */
			else if(fr->flags&SF_STRING)
				break;
			/* try direct read */
			else
			{	
#if sparc			/* try memory mapping for fast read */
				struct stat	st;

				if(mbuf != (caddr_t)(-1))
				{	/* release previous mapped space */
					munmap(mbuf,msize);
					mbuf = (caddr_t)(-1);
				}

				if(fr->orig >= 0 && (n < 0 || n >= SF_BUFSIZE) &&
				   fstat(fr->file,&st) >= 0 &&
				   (st.st_mode&S_IFMT) == S_IFREG)
				{	if((msize = st.st_size - fr->here) > MAPSIZE)
						msize = MAPSIZE;
					if(msize > n && n > 0)
						msize = n;
					mbuf = mmap(NIL(caddr_t),msize,PROT_READ,
					 	    MAP_SHARED,fr->file,fr->here);
					if(mbuf != (caddr_t)(-1))
					{	/* advance read pointer */
						(void)lseek(fr->file,msize,1);
						next = (uchar*)mbuf;
						r = msize;
						goto r_done;
					}
				}
#endif
				/* get a decent size buffer to work with */
				if((w = fr->size) >= sizeof(rbuf))
					next = fr->data;
				else
				{	next = rbuf;
					w = sizeof(rbuf);
				}

				if(!rs && n > 0 && n < w)
					w = (int)n;
				while((r = read(fr->file,next,w)) < 0 && errno == EINTR)
					errno = 0;
				if(r <= 0)
					break;

				/* for future sfseek's correctness */
			r_done:
				fr->next = fr->endb = fr->endr = fr->data;
				direct = 1;
			}
		}

		if(!rs)
		{	if(n > 0)
			{	if(n < r)
					r = (int)n;
				n -= r;
			}
			n_move += r;
		}
		else
		{	/* count lines */
			reg uchar	*endb = next+r;
			reg int		string = (fr->flags&SF_STRING);

			if(!direct)
				SFlock(fr);
			if(!string)
			{	w = endb[-1];
				endb[-1] = rc;
			}
			else	w = 0;
			for(cp = next; cp < endb; )
			{	/* find the line extent */
				if(string)
					while(cp < endb && !rs[*cp++])
						;
				else	while(!rs[*cp++])
						;
				if(cp < endb || w == rc)
				{	n_move += 1;
					if(n > 0 && (n -= 1) == 0)
						break;
				}
			}
			if(!string)
				endb[-1] = w;
			r = cp-next;
			if(!direct)
				SFopen(fr);
			else if(cp < endb)
				(void) lseek(fr->file,(long)(cp-endb),1);
		}

		/* advance read pointer */
		if(direct)
			fr->here += r;
		else	fr->next += r;

		/* write out data */
		if(fw)
		{	if(fw->mode != SF_WRITE && SFMODE(fw,SF_WRITE) < 0)
				goto w_error;
			if((fw->endb - (cp = fw->next)) > r)
			{	SFlock(fw);
				MEMCPY(cp,next,r);
				fw->next = cp;
				SFopen(fw);
			}
			else if((w = sfwrite(fw,next,r)) != r)
			{	/* write error */
				if(w > 0)
				{	r -= w;
					if(!rs)
						n_move -= r;
				}
			w_error :
				(void) sfseek(fr,(long)(-r),1);
				break;
			}
		}
	}

#if sparc
	if(mbuf != (caddr_t)(-1))
		munmap(mbuf,msize);
#endif

	return n_move;
}
