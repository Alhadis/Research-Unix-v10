/*ident	"@(#)ctrans:incl/fstream.h	1.1.1.2" */
/**************************************************************************
                        Copyright (c) 1984 AT&T
                          All Rights Reserved   

        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
      
        The copyright notice above does not evidence any        
        actual or intended publication of such source code.

*****************************************************************************/
#ifndef FSTREAMH
#define FSTREAMH

#include <iostream.h>

class  filebuf : public streambuf {	/* a stream buffer for files */
public:
			filebuf() ;
			filebuf(int fd);
			filebuf(int fd, char*  p, int l) ;

	int		is_open() { return opened ; }
	int		fd() { return xfd ; }
	filebuf*	open(const char *name, open_mode om, int prot=0664);
	filebuf*	attach(int fd) ;
	filebuf* 	close() ;
			~filebuf() ;
public: /* virtuals */
	virtual int	overflow(int=EOF);
	virtual int	underflow();
	virtual int	sync() ;
	virtual streampos
			seekoff(streamoff,seek_dir,open_mode) ;
	virtual streambuf*
			setbuf(char*  p, int len) ;
protected:
	int		xfd;	
	open_mode	mode ;
	char		opened;
	streampos	last_seek ;
	char* 		in_start;
	int		last_op();
	char		lahead[2] ;
};

class fstreambase : virtual public ios { 
public:
			fstreambase() ;
	
			fstreambase(const char* name, 
					open_mode mode,int prot=0664) ;
			fstreambase(int fd) ;
			fstreambase(int fd, char*  p, int l) ;
			~fstreambase() ;
	void		open(const char* name, open_mode mode, int prot=0664) ;
	void		attach(int fd);
	void		close() ;
	void		setbuf(char*  p, int l) ;
	filebuf*	rdbuf() { return &buf ; }
private:
	filebuf		buf ;
protected:
	void		verify(int) ;
} ;

class ifstream : public fstreambase, public istream {
public:
			ifstream() ;
			ifstream(const char* name, 
					open_mode mode,int prot=0664) ;
			ifstream(int fd) ;
			ifstream(int fd, char*  p, int l) ;
			~ifstream() ;

	filebuf*	rdbuf() { return fstreambase::rdbuf(); }
	void		open(const char* name, open_mode mode, int prot=0664) ;
} ;

class ofstream : public fstreambase, public ostream {
public:
			ofstream() ;
			ofstream(const char* name, 
					open_mode mode,int prot=0664) ;
			ofstream(int fd) ;
			ofstream(int fd, char*  p, int l) ;
			~ofstream() ;

	filebuf*	rdbuf() { return fstreambase::rdbuf(); }
	void		open(const char* name, open_mode mode, int prot=0664) ;
} ;

class fstream : public fstreambase, public iostream {
public:
			fstream() ;
	
			fstream(const char* name, 
					open_mode mode,int prot=0664) ;
			fstream(int fd) ;
			fstream(int fd, char*  p, int l) ;
			~fstream() ;
	filebuf*	rdbuf() { return fstreambase::rdbuf(); }
	void		open(const char* name, open_mode mode, int prot=0664) ;
} ;

#endif
