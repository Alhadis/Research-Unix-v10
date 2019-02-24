
#ifndef STREAMH
#define STREAMH

extern char* memcpy(char*,const char*,int) ;
			/* Of course this should come in from a header.
			 * But there is variation in where
			 * that header is found. 			 * 
			 */

#include <stdio.h>

			/* I'd like to not include <stdio.h>.  But for
			 * compatibility with older versions there is
			 * an iostream constructor that expects a
			 * FILE* argument, stdiobuf, 
			 * and iostream::sync_with_stdio.
			 */

#ifndef NULL
#define NULL	0
#endif
#ifndef EOF
#define	EOF	(-1)
#endif
#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

#define cin		(*iocin)
#define cout		(*iocout)
#define cerr		(*iocerr)
#define cdebug		(*iocdebug)

#define	zapeof(c) (unsigned char)(c)
                                        /* extracts char from c. The
                                         * critical requirement is
                                         *      zapeof(EOF)!=EOF
                                         * (c)&0377 is an alternative
                                         * in case the cast has a bug
                                         * in your compiler.
					 */


enum state_value { _good=0, _eof=1, _fail=2, _bad=4 };

static const int iostatebits = _eof|_fail|_bad ;
	
enum open_mode { input=1, output=2, atend=4, append=010, binaryio=020 } ;

enum seek_dir { seek_beg=0, seek_cur=1, seek_end=2 } ;

typedef long int streampos ;

class streambuf {
	short		alloc;	
	short		x_unbuf;
	char* 		x_base;	
	char*		x_pbase;
	char*		x_pptr;	
	char* 		x_epptr;
	char* 		x_gptr;
	char*		x_egptr;
	char*		x_eback;
	int		x_blen;	
	int		xsputn(const char*  s,int n);
	int		xsgetn(char*  s,int n);
    public:
	void		dbp();
    protected:
	char*		base() 		{ return x_base ; }
	char*		pbase()		{ return x_pbase ; }
	char*		pptr() 		{ return x_pptr ; }
	char*		epptr() 	{ return x_epptr ; }
	char*		gptr() 		{ return x_gptr ; }
	char*		egptr() 	{ return x_egptr ; }
	char*		eback() 	{ return x_eback ; }
	char* 		ebuf()		{ return x_base+x_blen ; }
	int		blen()		{ return x_blen; }
	void		setp(char*  p, char*  ep)
	{
		x_pbase=x_pptr=p ; x_epptr=ep ;
	}
	void		setg(char*  eb,char*  g, char*  eg)
	{
		x_eback=eb; x_gptr=g ; x_egptr=eg ;
	}
	void		pbump(int n) 
	{ 
		x_pptr+=n ;
	}

	void		gbump(int n) 
	{ 
		x_gptr+=n ;
		}

	void		setb(char* b, char* eb, int a = 0 )
	{
		if ( alloc && x_base ) delete x_base ;
		x_base = b ;
		if (eb < b )	{ x_unbuf = 1 ; x_blen = 0 ; }
		else		{ x_unbuf = 0 ; x_blen = eb-b ; }
		alloc = a ;
		}
	int		unbuffered() { return x_unbuf; }
	int		allocate()
	{
		if ( x_base== 0 && !unbuffered() ) return doallocate() ;
		else			  	 return 0 ;
	}
	virtual int 	doallocate();
	virtual void	destructor() ;
    public : 
	int		in_avail()
	{
		return x_gptr<x_egptr ? x_egptr-x_gptr : 0 ;
	}
	int		out_waiting() { return x_pptr && x_pptr>x_pbase ; } 
	virtual int	overflow(int c=EOF);
	virtual int	underflow();
	virtual int	pbackfail(int c);
	virtual int	sync();
	virtual streampos
			seek(streampos,seek_dir); /*** should be protected ***/
	int		sgetc()
	{
		/***WARNING: sgetc does not bump the pointer ***/
		return (x_gptr>=x_egptr) ? underflow() : zapeof(*x_gptr);
	}
	int		snextc()
	{
		return (x_gptr>=(x_egptr-1))
				? underflow()
				: zapeof(*++x_gptr);
	}
	int		sbumpc()
	{
		return  ( x_gptr>=x_egptr && underflow()==EOF ) 
				? EOF 
				: zapeof(*x_gptr++) ;
	}
	void		stossc()
	{
		if ( x_gptr++ > x_egptr ) underflow() ;
	}

	int		sputbackc(char c)
	{
		if (x_gptr > x_eback ) return zapeof(*--x_gptr = c);
		else return pbackfail(c) ;
	}

	int		sputc(int c)
	{
		return (x_pptr>=x_epptr) ? overflow(zapeof(c))
				      : zapeof(*x_pptr++=c);
	}
	int		sputn(const char*  s,int n)
	 {
		if ( n <= (x_epptr-x_pptr) ) {
			memcpy(x_pptr,s,n) ;
			pbump(n);
			return n ;
		} else {
			return xsputn(s,n) ;
		}
	}
	int		sgetn(char*  s,int n)
	{
		if ( n <= (x_egptr-x_gptr) ) {
			memcpy(s,x_gptr,n) ;
			gbump(n);
			return n ;
		} else {
			return xsgetn(s,n) ;
		}
	}
	virtual streambuf*
			setbuf(char*  p, int len, int count =0) ;
   	streambuf*	setbuf(unsigned char*  p, int len, int count =0) ;

			/*** Constructors -- should be protected ***/
			streambuf() ;
			streambuf(char*  p, int l,int c=0) ;
			~streambuf() ;
};

class  filebuf : public streambuf {	/* a stream buffer for files */
protected:
	int		xfd;	
	open_mode	mode ;
	char		opened;
	streampos	last_seek ;
	char* 		in_start;
	int		last_op();
	char		lahead[2] ;
public:
			filebuf() ;
			filebuf(int fd);
			filebuf(int fd, char*  p, int l) ;

	virtual int	overflow(int=EOF);
	virtual int	underflow();
	virtual int	sync() ;
	virtual streampos
			seek(streampos,seek_dir) ;
	virtual streambuf*
			setbuf(char*  p, int len, int count =0) ;
	int		is_open() { return opened ; }
	int		fd() { return xfd ; }
	filebuf*	open(char *name, open_mode om, int prot=0664);
	filebuf*	attach(int fd) ;
	int 		close() ;
	void		destructor() ;
};


class stdiobuf : public streambuf {
	/*** stdiobuf is obsolete, should be avoided ***/
	FILE*		fp ;			
	int		last_op ;
	char		buf[2];
	virtual int	overflow(int=EOF);
	virtual int	underflow();
	virtual int	sync() ;
	virtual streampos
			seek(streampos,seek_dir) ;
	virtual int	pbackfail(int c);
	void		destructor() ;
public:
			stdiobuf(FILE* f) ;
	FILE*		stdiofile() { return fp ; }
};

class pipebuf : public streambuf {
	virtual int	overflow(int);	
	virtual int	underflow();
	virtual int	pbackfail(int);
	void 		normalize() ;
public:
			pipebuf() ;
			pipebuf(char*  p, int l, int c=0) ;
	int		full() ;
	int		empty() ;
	virtual streambuf*
			setbuf(char*  p, int l, int c) ;
	
};

class iostream;
typedef iostream&	(*iostreamf)(iostream&) ;

#define IOMANIP(mtype,ftype,atype)				\
class mtype {							\
	iostream& (*fct)(iostream&,atype) ;			\
	atype	arg ;						\
	friend iostream& operator<<(iostream&, mtype&) ;	\
	friend iostream& operator>>(iostream&, mtype&) ;	\
public:								\
	mtype(iostream& (*f)(iostream&,atype), atype a) :	\
		fct(f),  arg(a) { } 				\
	} ;							\
inline iostream& operator<<(iostream& ios, mtype& m)		\
{								\
	return (*m.fct)(ios,m.arg) ;				\
	}							\
inline iostream& operator>>(iostream& ios, mtype& m)		\
{								\
	return (*m.fct)(ios,m.arg) ;				\
	}							\
class ftype {							\
	iostream& (*fct)(iostream&,atype) ;			\
    public :							\
	ftype(iostream& (*f)(iostream&,atype)) :		\
		 fct(f) { }					\
	mtype operator() (atype a) { return mtype(fct,a) ; }	\
	} ;							\

#define IOMANIP2(mtype,ftype,ptype,atype)				\
class mtype {								\
	iostream& (*fct)(iostream&,ptype,atype) ;			\
	ptype	param ;							\
	atype	arg ;							\
	friend iostream& operator<<(iostream&, mtype&) ;		\
	friend iostream& operator<<(iostream&, mtype&) ;		\
public:									\
	mtype(iostream& (*f)(iostream&,ptype,atype), ptype p, atype a):	\
		fct(f), param(p), arg(a) { } 				\
	} ;								\
inline iostream& operator<<(iostream& ios, mtype& m)			\
{									\
	return (*m.fct)(ios,m.param,m.arg) ;				\
	}								\
inline iostream& operator>>(iostream& ios, mtype& m)			\
{									\
	return (*m.fct)(ios,m.param,m.arg) ;				\
	}								\
class ftype {								\
	iostream& (*fct)(iostream&,ptype,atype) ;			\
	ptype	param ;							\
    public :								\
	ftype(iostream& (*f)(iostream&,ptype,atype),ptype p) :		\
		 fct(f), param(p) { }					\
	mtype operator() (atype a) { return mtype(fct,param,a) ; }	\
	} ;								\


class iostream {
protected:
	streambuf*	bp;
	iostream*	tied_to;
	int		state;		
	enum { io_tied=010, io_skipws=020 } ;
	int		ispecial;
	int		ospecial;
	int		freewhat ;
	int		x_gcount ;
	void		eatwhite() ;
	int		sync()	{ return bp->sync() ; }
	void 		xget(char*  c,int sk) ;
	int		do_ipfx(int noskipws) ;
	int		do_opfx() ;
	void		setstate(int b) { state|=b ; ispecial|=b; ospecial|=b; }
					
private:
	short 		x_convbase ;
	short 		x_width ;
	short		x_precision;
	char		x_fill;
	char		x_ladjust;
	char		x_showbase;
public:
			iostream(streambuf* b,int sk=1, iostream *t=0) ;
			iostream(FILE*) ; /*** obsolete ***/
			iostream(int fd) ; /*** obsolete, use fstream */
			iostream(int size, char* p,int sk=1) ;
					/*** obsolete, use strstream ***/
			~iostream() ;

	int		convbase()	{ return x_convbase ; }
	void		convbase(int b)
	{
		if ( b==0 || b==10 || b==8 || b==16 )	x_convbase=b ;
		else					x_convbase=10 ;
	}
	int		width()		{ return x_width ; }
	void		width(int w)	{ x_width = w ; }
	char		fill()		{ return x_fill ; }
	void		fill(char f)	{ x_fill = f ; }	
	int		ladjust()	{ return x_ladjust ; }
	void		ladjust(int a)	{ x_ladjust = a ; }
	int		precision()	{ return x_precision ; }
	void		precision(int a) { x_precision = a ; }
	int		showbase()	{ return x_showbase ; }
	void		showbase(int s)	{ x_showbase = s ; }
	void		sync_with_stdio() ;
	iostream&	operator<< (iostreamf f) { return (*f)(*this) ; }
	iostream&	operator>> (iostreamf f) { return (*f)(*this) ; }
	int		rdstate()	{ return state; }
			operator int()  { return !(state&(_fail|_bad)) ; }
	int		operator!() { return state&(_fail|_bad) ; } 
	int		eof()		{ return state&_eof; }
	int		fail()		{ return state&(_fail|_bad) ; }
	int		bad()		{ return state&_bad ; }
	int		good()		{ return state == _good; }
	void		clear(state_value i =0) 
	{ 	
		state = i & iostatebits ; 
		ispecial = (ispecial&~iostatebits) | state ;
		ospecial = (ospecial&~iostatebits) | state ;
	}
	streambuf*	rdbuf() { return bp ;} 
	int		ipfx(int noskipws=0)
	{
		return ( !(noskipws?(ispecial&~io_skipws):ispecial)
				|| do_ipfx(noskipws) ) ;
	}
	int		opfx()	/* Output prefix */
	{
		return !ospecial || do_opfx() ;
	}
	iostream&	flush() ;
  	streampos	tell() 	{ return bp->seek(0,seek_cur) ; }
	iostream&	seek(streampos p, seek_dir d)
	{
		if ( bp->seek(p,d) == EOF ) setstate(_bad) ;
		return *this ;
	 }
	iostream&	operator<<(const char*);
	iostream&	operator<<(int a); /* beware: << 'a' writes 97 */
	iostream&	operator<<(long);	
	iostream&	operator<<(double);
	iostream&	operator<<(unsigned int a);
	iostream&	operator<<(unsigned long);
	iostream&	operator<<(void*);
	iostream&	operator<<(streambuf*);
	iostream&	put(char c)
	{
		if ( !ospecial ) {
			if (  bp->sputc(c) == EOF )  {
				setstate(_eof|_fail) ;
			}
		}
		return *this ;
	}

	iostream&	write(const char*  s,int n)	
	{
		if ( !state ) {
			if ( bp->sputn(s,n) != n ) setstate(_eof|_fail);
			}
		return *this ;
	}
	iostream&	write(const unsigned char* s, int n)
	{
		return write((const char*)s,n);
	}
	iostream&	operator>>(char*);
	iostream&	operator>>(unsigned char*);
	iostream&	operator>>(unsigned char& c)
	{
		if ( !ispecial && bp->in_avail() ) {
			c = bp->sbumpc() ;
		} else {
			xget((char*)&c,ispecial&io_skipws) ;
		}
		return *this ;
	}
	iostream&	operator>>(char& c)
	{
		if ( !ispecial && bp->in_avail() ) {
			c = bp->sbumpc() ;
		} else {
			xget(&c,ispecial&io_skipws) ;
		}
		return *this ;
	}

	iostream&	operator>>(short&);
	iostream&	operator>>(int&);
	iostream&	operator>>(long&);
	iostream&	operator>>(unsigned short&);
	iostream&	operator>>(unsigned int&);
	iostream&	operator>>(unsigned long&);
	iostream&	operator>>(float&);
	iostream&	operator>>(double&);
	iostream&	operator>>(streambuf*);
	iostream&	get(char* , int lim, char delim='\n');
	iostream&	get(unsigned char* b,int lim, char delim='\n');
	iostream&	getline(char* b, int lim, char delim='\n');
	iostream&	getline(unsigned char* b, int lim, char delim='\n');
	iostream&	get(streambuf& sb, char delim ='\n');
	iostream&	get(unsigned char& c)
	{
		if ( !(ispecial&~io_skipws) && bp->in_avail()) {
			x_gcount = 1 ;
			c = bp->sbumpc() ;
		} else {
			xget((char*)&c,0) ;
		}
		return *this ;
	}
	iostream&	get(char& c)
	{
		if ( !(ispecial&~io_skipws) && bp->in_avail()) {
			x_gcount = 1 ;
			c = bp->sbumpc() ;
		} else {
			xget(&c,0) ;
		}
		return *this ;
	}
	int 		get()
	{
		unsigned char c ;
		return get(c) ? zapeof(c) : EOF ;
	}

	iostream&	read(char*  s,int n);
	iostream&	ignore(int n=1,int delim=EOF) ;
	iostream&	read(unsigned char* s,int n) 
	{
			return read((char*)s,n) ;
	}
	int		gcount() ;
	iostream&	putback(char c);
	iostream*	tie();
	iostream*	tie(iostream* s);
	int		skip() ;
	int		skip(int i) ;
 
};

typedef iostream ostream ;
typedef iostream istream ;

class fstream: public iostream {
	filebuf		buf ;
public:
			fstream() ;
	
			fstream(char* name, open_mode mode,int prot=0664) ;
			fstream(int fd) ;
			fstream(int fd, char*  p, int l) ;
			~fstream() ;
	int		open(char* name, open_mode mode, int prot=0664) ;
	int		attach(int fd);
	int		close() ;
	streambuf*	setbuf(char*  p, int l,int count) ;
	filebuf*	rdbuf() { return &buf ; }


} ;

class strstream ;
class strstreambuf : public streambuf
{
	void*		(*afct)(long) ;
	void		(*ffct)(void*) ;
	int		ignore_oflow ;
	int		froozen ;
	int		auto_extend ;
	int		doallocate() ;
	int		overflow(int) ;
	int 		underflow() ;
	void		destructor() ;
	streambuf*	setbuf(char*  p, int l, int c) ;
public: 
		strstreambuf(void* (*a)(long) = 0 , void (*f)(void*) = 0) ;
	void		init( char* b, int size, char* pstart) ;
	void		init( unsigned char* b, int size,
					unsigned char* pstart) ;
	void		freeze(int n=1) ;
	char*		str() ;
	} ;

class strstream : public iostream {
	strstreambuf	buf ;
public:
		strstream(char* str, open_mode mode, int size = 0);
		strstream(unsigned char* str, open_mode mode, int size = 0);
		strstream(void* (*allocator)(long), void (*freer)(void*) ) ;
		strstream() ;
		~strstream() ;

	char*		str() ;
	int		pcount() ;
	strstreambuf*	rdbuf() ;		
	} ;

extern istream* iocin;	/* buffered standard input (file descriptor 0) */
extern ostream* iocout;	/* buffered standard output (file descriptor 1) */
extern ostream* iocerr;	/* unbuffered error output (file descriptor 2) */
extern ostream* iocdebug; /* buffered error output (file descriptor 2) */

IOMANIP(iomanip,ioap,int) ;

iostream& 	WS(iostream& i) ; 
iostream&	ENDS(iostream& i) ;

iomanip 	setbase(int b) ;
iomanip		setw(int w) ;

iostream& HEX(iostream&) ;
iostream& DEC(iostream&) ; 
iostream& OCT(iostream&) ; 
iostream& FLUSH(iostream&) ;

static class Iostream_init {
	static int	stdstatus ; /* see cstreams.c */
	static int	initcount ; /* initialized to 0 automatically */
	friend		iostream ;
public:
	Iostream_init() ; 
	~Iostream_init() ; 
} iostream_init ;	/* The constructor is called once for
			 * every time this header file is #included.
			 * It initializes the package, including
			 * cin, cout, cerr and cdebug.
			 */

/***************************** output conversions *********************
 *	These are obsolete and included for compatibility
 *	with earlier stream package.
 *********************/

extern char*  oct(long, int =0);
extern char*  dec(long, int =0);
extern char*  hex(long, int =0);

extern char*  chr(int, int =0);	/* chr(0) is the empty string "" */
extern char*  str(const char*, int =0);
extern char*  form(const char* ...);
			/* printf format
                         * Things may go terribly wrong (maybe even core
                         * dumps, if form tries to create a string with
                         * more than "max_field_width" characters. */

#endif
