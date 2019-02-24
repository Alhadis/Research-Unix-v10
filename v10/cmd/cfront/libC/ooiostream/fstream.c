/*ident	"@(#)ctrans:lib/stream/fstream.c	1.1.2.2" */
/**************************************************************************
                        Copyright (c) 1984 AT&T
                          All Rights Reserved   

        THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
      
        The copyright notice above does not evidence any        
        actual or intended publication of such source code.

fstream.c:

*****************************************************************************/

#include <iostream.h>
#include <fstream.h>

static const open_mode ios_atend = ios::ate ;
static const open_mode ios_input = ios::in ;
static const open_mode ios_output = ios::out ;
static const open_mode ios_append = ios::app ;
static const int seek_cur = ios::cur ;
static const int seek_end = ios::end ;
static const int seek_beg = ios::beg ;

fstreambase::fstreambase() { init(&buf) ; }

fstreambase::fstreambase(const char* name, open_mode mode, int prot)
{
	init(&buf) ;
	open(name,mode,prot) ;
	}

fstreambase::fstreambase(int fd) 
	: buf(fd) 
{
	init(&buf) ;
	}

fstreambase::fstreambase(int fd, char* p, int l)
	: buf(fd,p,l) 
{
	init(&buf) ;
	}

static inline void fstreambase::verify(int ok)
{
	if ( ok ) clear(0) ;
	else 	  setstate(ios::failbit) ;
	}

void fstreambase::setbuf(char* p, int len) 
{
	verify(buf.setbuf(p,len) != 0 ) ;
	}

void fstreambase::open(const char* name, open_mode mode, int prot)
{
	verify(buf.open(name,mode,prot) != 0 ) ;
	}

void fstreambase::attach(int fd ) 
{
	verify(buf.attach(fd) != 0 ) ;
	}

void fstreambase::close() 
{
	verify(buf.close() != (filebuf*)0 ) ;
	}

fstreambase::~fstreambase() { }

ifstream::ifstream() { }
ifstream::ifstream(const char* name, open_mode mode, int prot)
	: fstreambase(name,mode|ios_input,prot) { }
ifstream::ifstream(int fd) : fstreambase(fd) { }
ifstream::ifstream(int fd, char* p, int l) : fstreambase(fd,p,l) { }
ifstream::~ifstream() { }

void ifstream::open(const char* name, open_mode mode, int prot)
{
	verify(rdbuf()->open(name,mode|ios_input,prot) != (filebuf*)0 ) ;
	}

ofstream::ofstream() { }
ofstream::ofstream(const char* name, open_mode mode, int prot)
	: fstreambase(name,mode|ios_output,prot) { }
ofstream::ofstream(int fd) : fstreambase(fd) { }
ofstream::ofstream(int fd, char* p, int l) : fstreambase(fd,p,l) { }
ofstream::~ofstream() { }
void ofstream::open(const char* name, open_mode mode, int prot)
{
	verify(rdbuf()->open(name,mode|ios_output,prot) != (filebuf*)0 ) ;
	}

fstream::fstream() { }
fstream::fstream(const char* name, open_mode mode, int prot)
	: fstreambase(name,mode,prot) { }
fstream::fstream(int fd) : fstreambase(fd) { }
fstream::fstream(int fd, char* p, int l) : fstreambase(fd,p,l) { }
fstream::~fstream() { }void fstream::open(const char* name, open_mode mode, int prot)
{
	verify(rdbuf()->open(name,mode,prot) != (filebuf*)0 ) ;
	}
