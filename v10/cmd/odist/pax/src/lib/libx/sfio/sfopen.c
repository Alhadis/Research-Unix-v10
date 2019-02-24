#include	"sfhdr.h"

/*	Open a file/string for IO.
**	If f is not nil, it is taken as an existing stream that should be
**	closed and its structure reused for the new stream.
**
**	Written by Kiem-Phong Vo (06/27/90)
*/

#if __STDC__ || __cplusplus
Sfile_t *sfopen(reg Sfile_t* f, const char* file, const char* mode)
#else
Sfile_t *sfopen(f,file,mode)
reg Sfile_t	*f;		/* old stream structure */
char		*file;		/* file/string to be opened */
reg char	*mode;		/* mode of the stream */
#endif
{
	reg int		fd, oflags, sflags, oldfd;

	if((f && SFFROZEN(f)) || !file || !mode)
		return NIL(Sfile_t*);
	oldfd = -1;

	if(f)
	{	/* reuse the space of this stream */
		f->flags |= SF_REUSE;
		f->flags &= ~SF_KEEPFD;	/* force closing of f->file */
		oldfd = f->file;
		(void) sfclose(f);
	}

	/* construct the open flags */
	sflags = 0;
	while(1) switch(*mode++)
	{
	case 'w' :
		oflags = O_WRONLY | O_TRUNC | O_CREAT;
		sflags = SF_WRITE;
		break;
	case 'a' :
		oflags = O_WRONLY | O_APPEND | O_CREAT;
		sflags = SF_WRITE | SF_APPEND;
		break;
	case 'r' :
		oflags = O_RDONLY;
		sflags = SF_READ;
		break;
	case '+' :
		oflags = (oflags & ~(O_RDONLY|O_WRONLY))|O_RDWR;
		sflags |= SF_READ|SF_WRITE;
		break;
	case 's' :
		sflags = SF_STRING|SF_READ;
		break;
	case 'b' :
		break;
	case  0  :
		goto e_mode;
	default :
		return NIL(Sfile_t*);
	}
e_mode :;

	if(sflags&SF_STRING)
		return sfnew(f,(uchar*)file,strlen(file),-1,sflags);
	else if(!sflags || !file[0])
		return NIL(Sfile_t*);

	/* open the file */
#ifndef NO_OFLAGS
	fd = open((char*)file,oflags,0666);
#else
	if((fd = open(file,oflags&03)) < 0 &&
	   (oflags&O_CREAT) && (fd = creat(file,0666)) >= 0 && !(oflags&O_WRONLY))
	{	/* the file now exists, reopen it for read/write */
		(void) close(fd);
		fd = open(file,oflags&03);
	}
	else if(fd >= 0)
	{	/* truncate file */
		if(oflags&O_TRUNC)
		{	reg int	tf = creat(file,0666);
			(void)close(tf);
		}

		/* make sure file pointer is at the top */
		if((oflags&O_RDWR) && !(oflags&O_APPEND))
			(void)lseek(fd,0L,0);
	}
#endif
	if(fd < 0)
		return NIL(Sfile_t*);

	if(oldfd >= 0 && fd != oldfd && (oldfd = dup(fd)) >= 0)
	{	/* freopen desires fd == oldfd */
		(void) close(fd);
		fd = oldfd;
	}

	if(!(f = sfnew(f,NIL(uchar*),-1,fd,sflags)) )
		(void) close(fd);
	else if(mode[0] != 'r' && f->mode != SF_WRITE)
		_sfmode(f,SF_WRITE);

	return f;
}
