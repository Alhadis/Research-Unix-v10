#include "errors.h"

#define	BLOCKSIZE	4096
#define	NDISC		5
#define	NBUFFILES	3+2*NDISC	/* unix+undo+snarf+NDISC*(transcript+buf) */
#define	NSUBEXP		10

#define	TRUE		1
#define	FALSE		0

#define	INFINITY	((Posn)0x7FFFFFFFL)
#define	INCR		25
#define	SS		2	/* shorts are hardwired to two bytes in files */

#define	new(t, n)	((t *)alloc(((ulong)(n)*sizeof(t))))
#define	gcnew(p, n)	((void)gcalloc((ulong)(n)*sizeof((p)[0]), (long **)&(p)))
#define	gcrenew(p, n)	((void)gcrealloc((uchar *)p, ((ulong)(n)*sizeof((p)[0]))))

typedef unsigned short	ushort;
typedef unsigned long	ulong;
typedef unsigned char	uchar;
typedef long		Posn;		/* file position or address */
typedef	ushort		Mod;		/* modification number */

#include "mesg.h"

enum State{
	Clean=' ',
	Dirty='\'',
	Unread='-'
};
typedef struct File File;
typedef struct Range{
	Posn	p1, p2;
}Range;
typedef struct Rangeset{
	Range	p[NSUBEXP];
}Rangeset;
typedef struct Address{
	Range	r;
	File	*f;
}Address;
typedef struct List{
	int	nalloc;
	int	nused;
	long	*ptr;		/* code depends on ptrs being same size as Posns */
}List;
typedef struct Block{
	short	bnum;		/* absolute number on disk */
	short	nbytes;		/* bytes stored in this block */
}Block;
typedef struct Discdesc{
	int	fd;		/* unix file descriptor of temp file */
	int	nbk;		/* high water mark */
	List	free;		/* array of free blocks */
}Discdesc;
typedef struct Disc{
	Discdesc *desc;		/* descriptor of temp file */
	Posn	nbytes;		/* bytes on disc file */
	struct{
		int	nalloc;
		int	nused;
		Block	*ptr;
	}block;			/* array of used blocks (same shape as List) */
}Disc;
typedef struct String{
	short	n;
	short	size;
	uchar	*s;
}String;
typedef struct Buffer{
	Disc	*disc;		/* disc storage */
	Posn	nbytes;		/* total length of buffer */
	String	cache;		/* in-core storage for efficiency */
	Posn	c1, c2;		/* cache start and end positions in disc */
				/* note: if dirty, cache is really c0, c0+cache.n */
	int	dirty;		/* cache dirty */
}Buffer;
#define	NGETC	128
struct File{
	Buffer	*buf;		/* cached disc storage */
	Buffer	*transcript;	/* what's been done */
	Posn	markp;		/* file pointer to start of latest change */
	Mod	mod;		/* modification stamp */
	Posn	nbytes;		/* total length of file */
	Posn	hiposn;		/* highest address touched this Mod */
	Address	dot;		/* current position */
	Range	tdot;		/* what terminal thinks is current range */
	Range	mark;		/* tagged spot in text (don't confuse with Mark) */
	List	*rasp;		/* map of what terminal's got */
	String	name;		/* file name */
	short	tag;		/* for communicating with terminal */
	char	state;		/* Clean, Dirty, Unread */
	char	closeok;	/* ok to close file? */
	char	marked;		/* file has been Fmarked at least once; once
				 * set, this will never go off as undo doesn't
				 * revert to the dawn of time */
	long	inumber;	/* file from which it was read */
	long	date;		/* time stamp of unix file */
	Posn	cp1, cp2;	/* Write-behind cache positions and */
	String	cache;		/* string */
	uchar	getcbuf[NGETC];
	int	ngetc;
	int	getci;
	Posn	getcp;
};
typedef struct Filelist{
	int	nalloc;
	int	nused;
	File	**ptr;
}Filelist;
typedef struct Mark{
	Posn	p;
	Range	dot;
	Range	mark;
	Mod	m;
	short	s1;
}Mark;
typedef struct Regexp Regexp;
struct Regexp{
	String	text;
};
#define	Fgetc(f)  ((--(f)->ngetc<0)? Fgetcload(f, (f)->getcp) : (f)->getcbuf[(f)->getcp++, (f)->getci++])
#define	Fbgetc(f) (((f)->getci<=0)? Fbgetcload(f, (f)->getcp) : (f)->getcbuf[--(f)->getcp, --(f)->getci])

extern uchar	*alloc();
extern uchar	*gcalloc();
extern uchar	*gcrealloc();
extern char	*brk();
extern char	*sbrk();
extern Address	address();
extern Address	lineaddr();
extern Buffer	*Bopen();
extern Disc	*Dopen();
extern Discdesc	*Dstart();
extern File	*Fopen();
extern File	*newfile();
extern File	*lookfile();
extern File	*current();
extern Posn	getnum();
extern Posn	readio();
extern Posn	writeio();
extern Range	rdata();
extern String	*tempstr();
extern uchar	genbuf[];
extern char	*home;
extern int	io;
extern int	patset;
extern int	quitok;
extern Address	addr;
extern Buffer	*undobuf;
extern Buffer	*snarfbuf;
extern Buffer	*unixbuf;
extern Filelist	file;
extern Filelist	tempfile;
extern File	*cmd;
extern File	*curfile;
extern Mod	modnum;
extern Posn	cmdpt;
extern Posn	cmdptadv;
extern Rangeset	sel;
extern String	cmdstr;
extern String	genstr;
extern String	lastpat;
extern String	lastregexp;
extern String	unixcmd;
extern int	dontcompact;
extern int	downloaded;
extern int	dounlock;

#define	compactok()	dontcompact=FALSE
#define	nocompact()	dontcompact=TRUE

extern char	*strcat();
extern char	*strchr();

#if	vax|mc68020		/* longs are addressable at byte boundaries */
#define	PUTPOSN(d,s)		*(Posn *)d = *(s), d+=sizeof(Posn)
#define GETPOSN(p,s)		p = *(Posn *)(s)
#else
#define	PUTPOSN(d,s)	{ register char *P=(char *)(s);\
			  *d++= *P++;*d++= *P++;*d++= *P++;*d++= *P++; }
#define GETPOSN(p,s)	{ long L; register uchar *P1=(s),*P2=(uchar *)&L;\
			*P2++= *P1++;*P2++= *P1++;*P2++= *P1++;*P2++= *P1++;\
			p = L; }
#endif
extern void exit();

#define	HIGHBIT	0x00
