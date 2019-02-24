/*ident	"@(#)ctrans:incl/stdiostream.h	1.1.1.2" */
/**************************************************************************
                        Copyright (c) 1984 AT&T
                          All Rights Reserved   

        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
      
        The copyright notice above does not evidence any        
        actual or intended publication of such source code.

*****************************************************************************/
#ifndef STDSTREAMH
#define STDSTREAMH

#include <iostream.h>
#include <stdio.h>

class stdiobuf : public streambuf {
	/*** stdiobuf is obsolete, should be avoided ***/
public: // Virtuals
	virtual int	overflow(int=EOF);
	virtual int	underflow();
	virtual int	sync() ;
	virtual streampos
			seekoff(streamoff,ios::seek_dir,int) ;
	virtual int	pbackfail(int c);
public:
			stdiobuf(FILE* f) ;
	FILE*		stdiofile() { return fp ; }
	virtual		~stdiobuf() ;
private:
	FILE*		fp ;			
	int		last_op ;
	char		buf[2];
};

class stdiostream : public ios {
public:
			stdiostream(FILE*) ;
			~stdiostream() ;
	stdiobuf*	rdbuf() ;
private:
	stdiobuf	buf ;
};

#endif

