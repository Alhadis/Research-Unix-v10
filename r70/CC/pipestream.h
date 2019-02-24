/*ident	"@(#)ctrans:incl/pipestream.h	1.1.1.1" */
/**************************************************************************
                        Copyright (c) 1984 AT&T
                          All Rights Reserved   

        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
      
        The copyright notice above does not evidence any        
        actual or intended publication of such source code.

*****************************************************************************/
#ifndef PIPESTREAMH
#define PIPESTREAMH

#include <iostream.h>

class pipebuf : public streambuf {
public:
			pipebuf() ;
			pipebuf(char*  p, int l) ;
	int		full() ;
	int		empty() ;
public: /* virtuals */
	virtual streambuf*
			setbuf(char*  p, int l) ;
	virtual int	overflow(int);	
	virtual int	underflow();
	virtual int	pbackfail(int);
protected:
	void 		normalize() ;
};

class pipestream : public iostream {
public:
			pipestream();
			pipestream(char*  p, int l) ;
			~pipestream() ;
	pipebuf*	rdbuf() ;
private:
	pipebuf		buf ;
	} ;

#endif
