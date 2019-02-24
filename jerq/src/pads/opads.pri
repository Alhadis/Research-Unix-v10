#ifndef PADS_H
#define PADS_H

typedef unsigned short Attrib;
#define PADS_VERSION 0x851021		/* YYMMDD */
#define CARTE    0x80
#define NUMERIC  1
#undef major
#undef minor
#ifndef PADS_TERM
int	strlen(char*),
	strcmp(char*,char*),
	read(int,char*,int),
	write(int,char*,int),
	ioctl(int,int ...);
char	*strcpy(char*,char*),
	*sbrk(int);

#define PRINTF_TYPES char * ...
#define PRINTF_ARGS char *fmt ...
#define FMT(i) ( ((int*)&fmt)[i] )
#define PRINTF_COPY\
	(fmt?fmt:""), FMT(1),FMT(2),FMT(3),FMT(4),FMT(5),FMT(6),FMT(7),FMT(8)

#ifdef TRACE
#define OK(x)		if( !ok() ) PadsError(__SRC__);
#define VOK		if( !ok() ) PadsError(__SRC__);
#define IF_LIVE(x)	{ if( x )   PadsError(__SRC__); } if(0)
#define SRCFILE(f)	static __SRC__ = f;
typedef int  (*PFI)();
PFI trace_fcn(char*,int), trace_ptr;
#define trace  (!(trace_ptr=trace_fcn(__SRC__,__LINE__))) ? 0 : (*trace_ptr)
#else
#define OK(x)		{ if( !ok() ) return x; }
#define VOK		{ if( !ok() ) return ; 	}
#define IF_LIVE(x)	  if( x )
#define SRCFILE(f)
#define trace  if( 0 )
#endif

class Index {
public:
unsigned char	major;
unsigned char	minor;
		Index()			{ major = minor = 0;		}
	 	Index(int i)		{ major = i>>8; minor = i&0xFF; }
		Index(int j, int n )	{ major = j;	minor = n;	}
	short	sht();		/* don't inline sht() - sht() etc */
	int	null();
};
extern Index ZIndex;
class Carte {
public:
	unsigned char	size;		/* host.size != term.size */
	unsigned char	attrib;
	unsigned char	items;
	unsigned char	width;
	struct Index	bin[1];
			Carte() {};
};
#else
typedef unsigned short Index;
#define MJR(i)	((i)>>8)
#define MNR(i)	((i)&0xFF)
typedef struct Carte Carte;
typedef enum Protocol Protocol;
struct Carte {
	unsigned char	size;		/* host.size != term.size */
	unsigned char	attrib;
	short		width;
	Index		bin[1];
};
#endif

#define	SELECTLINE	((Attrib)0x0001)
#define SORTED		((Attrib)0x0002)
#define ACCEPT_KBD	((Attrib)0x0004)
#define FOLD		((Attrib)0x0008)
#define TRUNCATE	((Attrib)0x0010)
#define USERCLOSE	((Attrib)0x0020)
#define DONT_CUT	((Attrib)0x0040)
#define FLUSHLINE	((Attrib)0x0080)	/* should not be required */
#define FAKELINE	((Attrib)0x0100)
#define USERCUT		((Attrib)0x0200)
#define DONT_CLOSE	((Attrib)0x0400)
#define NO_TILDE	((Attrib)0x0800)
#define DONT_DIRTY	((Attrib)0x1000)

#ifndef PADS_TERM
char *sf(PRINTF_TYPES);
char *PadsInit(char* = 0);
void PadsServe(long = 0);
void NewHelp();
void NewPadStats();
class PadRcv;
typedef void (PadRcv::*Action)(...);
void Pick(char*,Action,long);
long UniqueKey();
Index NumericRange(short,short);
void PadsWarn(PRINTF_TYPES);
//extern char *TapTo;
void PadsError(PRINTF_TYPES);

class PadRcv {	friend Remote; friend Pad;
	short	oid;
	short	magic;
	int	isvalid();
public:
		PadRcv();
		~PadRcv();
	void	invalidate();
virtual	int	disc();
virtual	char	*kbd(char*);
virtual	char	*help();
virtual	void	numeric(long);
virtual	void	userclose();
virtual	void	cycle();
virtual	void	linereq(long,Attrib=0);
virtual	int	accept(Action);
virtual	void	usercut();
};

class Pad {
	PadRcv	*_object;
	char	*_name;
	char	*_banner;
	Attrib	_attributes;
	long	_lines;
	void	termop(enum Protocol);
	long	errorkey;
	void	nameorbanner(enum Protocol, PRINTF_TYPES);
public:
	int	ok();
		Pad(PadRcv *);
		~Pad();
	void	alarm(short=0);
	void	banner(PRINTF_TYPES);
	void	clear();
	void	dump();
	void	error(PRINTF_TYPES);
	void	insert(class Line&);
	void	insert(long, Attrib, PadRcv*, Index, PRINTF_TYPES);
	void	insert(long, Attrib, PadRcv*, class Menu&, PRINTF_TYPES);
	void	insert(long, Attrib, PRINTF_TYPES);
	void	insert(long, PRINTF_TYPES);
	void	lines(long);
	void	makecurrent();
	void	makegap(long,long);
	void	menu(Index);
	void	menu(class Menu&);
	void	name(PRINTF_TYPES);
	void	options(Attrib, Attrib=0);
	void	tabs(short);
	void	removeline(long);
	void	createline(long,long);
	void	createline(long);
};

class Line {
public:
	int	ok();
		Line();
	PadRcv	*object;
	char	*text;
	long	key;
	Attrib	attributes;
	Index	carte;
};
class IList {
	friend	Menu;
	Index	index;
	IList	*next;
public:
		IList(Index i, IList *n) { index = i; next = n; }
};

class Item {
public:
	char	*text;
	Action	action;
	long	opand;
		Item(char*,Action,long);
		Item();			/* ever used ? */
};
class Menu {
	IList	*list;
	int	size;
	void	dump();
public:
		Menu();
		~Menu();
		Menu( char*, Action=0, long=0 );
	Index	index( char* =0, Action=0, long=0 );
	void	first( char*, Action=0, long=0 );
	void	first( Index );
	void	last( char*, Action=0, long=0 );	
	void	last( Index );	
	void	sort( char*, Action=0, long=0 );
	void	sort( Index );
};

class Binary {
public:
	Binary	*left;
	Binary	*right;
	Index	index;
		Binary()	{}
};

class Cache {
	friend	ItemCache;
	friend	CarteCache;
	Binary	*root;
	Index	current;
	Index	SIZE;
public:
		Cache(unsigned char,unsigned char);
	int	ok();
};

class ItemCache : public Cache {
	Item	***cache;
public:
		ItemCache();
	Index	place(Item);
	Item	*take(Index);
};

class CarteCache : public Cache {
	Carte	***cache;
public:
		CarteCache();
	Index	place(Carte*);
	Carte	*take(Index);
	Index	numeric(short,short);
	void	cartelimits(Carte*);
};

extern ItemCache  *ICache;
extern CarteCache *CCache;
#endif

#define CARTESIZE(s) (sizeof(Carte) + (s)*sizeof(Index))

enum Protocol {
	P_UCHAR		= 1,
	P_SHORT		= 2,
	P_LONG		= 4,

	P_CACHEOP	= 0x10,
	P_I_DEFINE	= 0x11,
	P_I_CACHE	= 0x12,
	P_C_DEFINE	= 0x13,
	P_C_CACHE	= 0x14,

	P_STRING	= 0x20,
	P_INDEX		= 0x21,

	P_PADDEF	= 0x30,
	P_ATTRIBUTE	= 0x31,
	P_BANNER	= 0x32,
	P_CARTE		= 0x33,
	P_LINES		= 0x34,
	P_NAME		= 0x35,
	P_TABS		= 0x36,

	P_PADOP		= 0x40,
	P_ACTION	= 0x41,
	P_ALARM		= 0x42,
	P_CLEAR		= 0x43,
	P_CYCLE		= 0x44,
	P_DELETE	= 0x45,
	P_KBDSTR	= 0x46,
	P_LINE		= 0x47,
	P_LINEREQ	= 0x48,
	P_MAKECURRENT	= 0x49,
	P_MAKEGAP	= 0x4A,
	P_NEXTLINE	= 0x4B,
	P_NUMERIC	= 0x4C,
	P_USERCLOSE	= 0x4D,
	P_CREATELINE	= 0x4E,
	P_REMOVELINE	= 0x4F,

	P_HOSTSTATE	= 0x50,
	P_BUSY		= 0x51,
	P_IDLE		= 0x52,

	P_USERCUT	= 0x5F,

	P_PICK		= 0x60,

	P_HELPSTR	= 0x70,
	P_SHELL		= 0x71,

	P_VERSION	= 0x80,
};

#ifdef PADS_TERM
long		RcvLong();
short		RcvShort();
unsigned char	RcvUChar();
char		*RcvString();

void		SendLong();
void		SendShort();
void		SendUChar();
void		SendString();
#else
#include <CC/stdio.h>
FILE *Popen(char*,char*);
int Pclose(FILE*);

class Remote {
public:
	int	fd;
	int	pktbase;
	int	pktsize;
unsigned char	writebuffer[2048];

	long	shiftin(int);
	void	shiftout(int, long);
	int	writesize;
	void	err(char * = 0);
	int	get();
	void	checkproto(int p);
	void	put(char c);
	void	proto(int p);

	long	rcvlong();
	short	rcvshort();
unsigned char	rcvuchar();
	PadRcv	*rcvobj();

	void	sendlong(long);
	void	sendobj(PadRcv*);
	void	sendshort(short);
	void	senduchar(unsigned char);
	char	*rcvstring(char*);
	void	sendstring(char*);

	void	pktstart(char);
	void	pktend();
	void	pktflush();

	void	share();

		Remote(char*);
		Remote(int);
};

extern Remote *R;

#endif
#endif
