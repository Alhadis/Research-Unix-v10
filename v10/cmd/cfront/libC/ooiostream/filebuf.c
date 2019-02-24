/*ident	"@(#)ctrans:lib/stream/filebuf.c	1.1.2.2" */
/**************************************************************************
                        Copyright (c) 1984 AT&T
                          All Rights Reserved   

        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
      
        The copyright notice above does not evidence any        
        actual or intended publication of such source code.

filebuf.c:

*****************************************************************************/

#include <iostream.h>
#include <fstream.h>
#include <osfcn.h>
#include "streamdefs.h"

#ifndef O_RDONLY
#	include <fcntl.h>
#endif

#ifndef O_CREAT
#	define O_CREAT 00400
#endif

#ifndef O_TRUNC
#	define O_TRUNC 01000
#endif

#ifndef O_EXCL
#	define O_EXCL 02000
#endif


#ifndef O_APPEND
#	define O_APPEND 0
#endif

#include <errno.h>
extern int errno ;

static const int input=ios::in ;
static const int output=ios::out ;
static const int append=ios::app ;
static const int atend=ios::ate ;
static const int tcate=ios::trunc ;
static const int nocr=ios::nocreate ;
static const int norep=ios::noreplace ;

static const int seek_beg=ios::beg ;
static const int seek_end=ios::end ;
static const int seek_cur=ios::cur ;

int filebuf::last_op() 
{
	return (pptr()?output:(gptr()?input:0)) ;
}

inline void save_errno(int& orig)
{
	orig = ::errno ;
	::errno = 0 ;
}

inline int restore_errno(int& orig)
{
	if ( ::errno == 0 ) ::errno = orig ;
	return EOF ;
}
	
/*
 *	Open a file with the given mode.
 *	Return:		NULL if failure
 *			this if success
 */
filebuf* filebuf::open (const char *name, open_mode om, int prot)
{
	int errno_orig ;
	save_errno(errno_orig) ;
	if ( om&append ) om |= output ;

	if ( opened ) {
		restore_errno(errno_orig) ;
		return 0 ;
		}

        int flags = 0 ;
	if ( om&append ) flags |= O_APPEND ;

	switch (om&(input|output)) {
	case input : 
		flags |=  O_RDONLY ;
		xfd = ::open(name,flags) ;
		break ;

	case output :
		flags |= O_WRONLY|O_CREAT|O_TRUNC ;
		if ( om&nocr ) flags &= ~O_CREAT ;
		if ( om&norep ) flags |= O_CREAT|O_EXCL ;
		if ( om&append ) flags |= O_APPEND ;
		if ( flags == (O_WRONLY|O_CREAT|O_TRUNC) ) {
			xfd = ::creat(name,prot) ;
			}
		else {
			xfd = ::open(name,flags,prot) ;
			}

		if ( xfd >= 0 && (om&(atend|append))) lseek(xfd,0,2) ;
		break ;

	case input|output:
		flags |= O_RDWR|O_CREAT ;
		if ( om&tcate ) flags |= O_TRUNC ;
		if ( om&append ) flags |= O_APPEND ;
		if ( om&nocr ) flags &= ~O_CREAT ;
		xfd = ::open(name,flags,prot) ;
		if ( xfd >= 0 && (om&(atend|append))) lseek(xfd,0,2) ;
		break;
	}

	if (xfd < 0) return 0;

	opened = 1;
	setp(0,0) ;
	setg(0,0,0) ;
	mode = om ;
	last_seek = EOF ;
	restore_errno(errno_orig) ;
	return this;
}

filebuf* filebuf::attach (int f)
{
	if ( opened ) return 0 ;
	xfd = f;
	opened = 1;
	setp(0,0) ;
	setg(0,0,0) ;
	mode=0 ;
	last_seek= EOF;
	return this;
}

/*
 *	Empty an output buffer.
 *	Returns:	EOF on error
 *			0 on success
 */
int filebuf::overflow(int c)
{
	int errno_orig ;
	save_errno(errno_orig) ;
	if ( !opened ) return restore_errno(errno_orig) ;
	if ( allocate() == EOF ) return restore_errno(errno_orig) ;
	if ( last_op() == input ) {
		if ( sync()==EOF ) return restore_errno(errno_orig) ;
	}

	register char* p = base() ;
	// pptr()==NULL does not imply p < pptr(),  so we need separate
	// test.
	while ( pptr() && p < pptr() )  {
		if ( O_APPEND==0 && (mode&append) ) {
			// System doesn't have an append mode, so approximate
			// as best we can.
			lseek(xfd,0,2) ;
			}
		/* Partial writes are sometimes possible in peculiar
		 * circumstances */
		register int count = write(xfd,p,pptr()-p) ;
		if ( count < 0 ) {
			last_seek = EOF ;
			return restore_errno(errno_orig) ;
		}
		p += count ;

		if ( SEEK_ARITH_OK	
				&& last_seek != EOF
				&& mode && !(mode&append)
				&& count >= 0 ) {
			last_seek += count ;
		} else {
			last_seek = EOF ;
		}

		if ( count < 0  ) return restore_errno(errno_orig) ;
	}

	setp(base(),ebuf()) ;
	setg(0,0,0);

	if ( c == EOF ) /* don't do anything */ ;
	else if ( unbuffered() ) {
		char ch = c;
		last_seek = EOF ;
		while ( write(xfd,&ch,1)!=1 && ::errno==0 ) ;
		if ( ::errno != 0 ) return restore_errno(errno_orig) ;
		}
	else {
		sputc(c) ;
		}

	restore_errno(errno_orig) ;
	return zapeof(c) ;
}


/*
 *	Fill an input buffer.
 *	Returns:	EOF on error or end of input
 *			next character on success
 */
int filebuf::underflow()
{
	int count;

	if ( !opened ) return EOF ;
	if ( allocate() == EOF ) return EOF ;
	if ( last_op() == output ) {
		if ( sync()==EOF ) return EOF ;
	}
	int orig_errno ;
	save_errno(orig_errno) ;
	setp(0,0) ;
	if ( unbuffered() ) {
		last_seek=EOF ;
		count = read(xfd,&lahead[0],1) ;
		setg(&lahead[0],&lahead[0],&lahead[count]) ;
		if ( count <= 0 ) return EOF ;
		} 
	else {
		register int	rdsize ;
		if ( blen() > 2*sizeof(long) ) {
			/* gptr must be set greater than base to
			 * guarantee at least 1 char of pushback.
			 * putting it farther will tend in many common
			 * cases to keep things aligned. 
			 */
			in_start = base()+sizeof(long) ;
			rdsize = blen()-sizeof(long) ;
		} else {
			in_start = base()+1 ;
			rdsize = blen()-1 ;
		}
		count = read(xfd,in_start,rdsize) ;
		while ( count<=0 && ::errno==EINTR ) {
			/*
			 * Signal caught and returned before any data
			 * transfered. 	
			 */
			::errno = 0 ;
			count = read(xfd,in_start,rdsize) ;
		}

		if ( SEEK_ARITH_OK	
				&& last_seek != EOF
				&& mode && !(mode&append) 
				&& count >= 0 ) {
			last_seek += count ;
		} else {
			last_seek = EOF ;
		}

		if ( count <= 0 ) {
			setg(0,0,0) ;
			return restore_errno(orig_errno) ;
			}
		setg(base(),in_start,in_start+count) ;
	}
	
	restore_errno(orig_errno) ;	
	return zapeof(*gptr());
}

filebuf* filebuf::close()
{
	int f = xfd ;
	if ( !opened ) {
		return 0 ;
	}
	if (last_op()==output) overflow();
	setg(0,0,0) ;
	setp(0,0) ;
	opened = 0 ;
	xfd = -1 ;
	last_seek = EOF ;
	if ( mode != 0 ) {
		mode = 0 ;
		int orig_errno ;
		save_errno(orig_errno) ;
		int ok = ::close(f);
		restore_errno(orig_errno) ;
		return ( ok==EOF ? 0 : this ) ;
	} else {
		return this ; 
	}
}

int filebuf::sync() 
{
	int ok = 0 ;
	int orig_errno ;
	save_errno(orig_errno) ;
	switch(last_op()) {
	case input: 
		last_seek = lseek(xfd,gptr()-egptr(),seek_cur) ;
		if ( last_seek < 0 ) {
			ok = EOF ;
			last_seek = EOF ;
			}
		break ;
	case output:
		ok = overflow() ;
		/* This does not result in infinite recursion even though
		 * under some circumstances overflow might call sync.
		 * it explicitly does not when last_op==output
		 */
		break;
	}
	setp(0,0) ;
	setg(0,0,0) ;
	restore_errno(orig_errno) ;
	return (ok==EOF ? EOF : 0 ) ;
}

streampos filebuf::seekoff(streamoff p, seek_dir d, open_mode m)
{
	int orig_errno ;
	save_errno(orig_errno) ;
	if( SEEK_ARITH_OK && last_seek >= 0 && !unbuffered() ) {
		char* refptr = 0 ;
		streampos sneed ;
		streampos sref, minavail, maxavail ;

		switch ( last_op() ) {
		case input : {
			refptr = gptr() ;
			sref = last_seek-(egptr()-gptr()) ;
			minavail = last_seek-(egptr()-in_start) ; 
			maxavail = last_seek-1 ;
			}
		case output : {
			// only allowable seek during output is
			// to present position. 
			refptr = pptr() ;
			sref = last_seek+pptr()-pbase() ;
			minavail = maxavail = sref ;
			}
		}

		switch( d ) {
		case seek_beg : sneed = p ; break ;
		case seek_cur : sneed = sref+p ;break ;
		case seek_end : refptr = 0 ; break ;
				/* Can't do seek_end */
		}

		if ( refptr && sneed >= minavail && sneed <= maxavail ) {
			switch( last_op() ) {
			case input : {
				setg(eback(),refptr+(sneed-sref),egptr());
				} break ;
			case output : {
				// Seeking to current position.  Nothing to
				// do.
				} break ;
			default : { 
				// shouldn't get here.  Try to recover somehow
				sync() ;
				seekoff(p,d,m);
				} break ;
			}
			return sneed ;
		}
		if ( refptr && sneed < sref && sneed+blen()/2 > sref 
			&& last_op() == input ) {
			// looks like we are stepping backward through
			// a file.  Performance may be improved by
			// backing up a little extra. 

			streampos toofar = sneed-blen()/2 ;
			if ( toofar < 0 ) toofar = 0 ;
			sync() ;
			last_seek=lseek(xfd,toofar,seek_beg) ;
			overflow();
			seekoff(p,d,m);
		}
	}

	restore_errno(orig_errno) ;
	if ( sync()==EOF )	return EOF ;
	else	{
		last_seek=lseek(xfd,p,d) ;
		return last_seek ;
		}
}

filebuf::filebuf()
	: xfd(-1), opened(0), mode(0), last_seek(EOF)
{

}

filebuf::filebuf(int f)
	: xfd(f), opened(1), mode(0), last_seek(EOF)
{

}

filebuf::filebuf(int f, char* p, int l) 
	: streambuf(p,l), xfd(f), opened(1), mode(0), last_seek(EOF)
{

}
	
filebuf::~filebuf() {
	close() ;
	}

streambuf* filebuf::setbuf(char* p , int len)
{

	if ( is_open() ) return 0 ;
	setb(0,0) ;
	return streambuf::setbuf(p,len) ;
}
