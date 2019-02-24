/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * pax common definitions
 */

#include <ls.h>
#include <stdio.h>
#include <sig.h>
#include <ftwalk.h>
#include <ctype.h>
#include <limits.h>
#include <hash.h>
#include <re.h>
#include <error.h>
#include <time.h>
#include <swap.h>
#include <align.h>

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

#ifndef S_IFIFO
#define S_IFIFO		0010000
#endif
#ifndef S_IFLNK
#define S_IFLNK		0120000
#endif
#ifndef S_IFSOCK
#define S_IFSOCK	0140000
#endif

#define PANIC		ERROR_PANIC|ERROR_SOURCE,__FILE__,__LINE__

#define allocate(x)	(x*)calloc(1,sizeof(x))
#define bcount(io)	(io.last-io.next)
#define bsave()		(state.backup=state.in)
#define elements(x)	(sizeof(x)/sizeof(x[0]))
#define message		if(state.trace<0)error
#define streq(a,b)	((*(a))==(*(a))&&!strcmp(a,b))
#define strneq(a,b,n)	((*(a))==(*(a))&&!strncmp(a,b,n))
#define round(x,y)	(((long)(x)+((y)-1))&~((y)-1))

#define RE_VERBOSE	(1<<RE_EXTERNAL)/* list re substitution		*/

/*
 * info per archive format
 *
 * NOTES:
 *
 *	format indices must agree with format[] table below
 *
 *	the *_NAMESIZE maximum file name lengths include the trailing \0
 *
 *	PATH_MAX must be >= max(*_NAMESIZE)
 */

#define ALAR		0
#define ALAR_NAME	"ansi"
#define ALAR_REGULAR	4
#define ALAR_SPECIAL	ALAR_REGULAR
#define ALAR_LABEL	2048
#define ALAR_HEADER	80
#define ALAR_VARHDR	9
#define ALAR_NAMESIZE	17
#define ALAR_RECSIZE	(BLOCKSIZE>>2)
#define ALAR_RECFORMAT	'D'
#define ALAR_ALIGN	0
#define ALAR_FLAGS	IN|OUT
#define ALAR_ID		"SLT"

#define BINARY		1
#define BINARY_NAME	"binary"
#define BINARY_REGULAR	DEFBUFFER
#define BINARY_SPECIAL	DEFBLOCKS
#define BINARY_HEADER	26
#define BINARY_NAMESIZE	256
#define BINARY_ALIGN	2
#define BINARY_FLAGS	IN|OUT

struct binary_header
{
	short		magic;
	unsigned short	dev;
	unsigned short	ino;
	unsigned short	mode;
	unsigned short	uid;
	unsigned short	gid;
	short		links;
	unsigned short	rdev;
	unsigned short	mtime[2];
	short		namesize;
	unsigned short	size[2];
};

#define CPIO		2
#define CPIO_NAME	"cpio"
#define CPIO_REGULAR	DEFBUFFER
#define CPIO_SPECIAL	DEFBLOCKS
#define CPIO_HEADER	76
#define CPIO_MAGIC	070707
#define CPIO_EXTENDED	1
#define CPIO_TRAILER	"TRAILER!!!"
#define CPIO_TRUNCATE(x) ((x)&0177777)
#define CPIO_NAMESIZE	256
#define CPIO_ALIGN	0
#define CPIO_FLAGS	IN|OUT

#define CPIO_FMT	0170000
#define CPIO_FIFO	0010000
#define CPIO_DIR	0040000
#define CPIO_CHR	0020000
#define CPIO_BLK	0060000
#define CPIO_REG	0100000
#define CPIO_LNK	0120000
#define CPIO_SOCK	0140000

#define IBMAR		3
#define IBMAR_NAME	"ibm"
#define IBMAR_REGULAR	ALAR_REGULAR
#define IBMAR_SPECIAL	ALAR_SPECIAL
#define IBMAR_ALIGN	ALAR_ALIGN
#define IBMAR_FLAGS	IN|OUT

/*
 * NOTE: since USTAR is an extension of TAR they share the same header
 */

#define TAR		4
#define TAR_NAME	"tar"
#define TAR_REGULAR	DEFBUFFER
#define TAR_SPECIAL	DEFBLOCKS
#define TAR_HEADER	TBLOCK
#define TAR_NAMESIZE	100
#define TAR_ALIGN	BLOCKSIZE
#define TAR_FLAGS	IN|OUT

#define header		tar_header_info
#define hblock		tar_header_block
#include <tar.h>
#undef header
#undef hblock

#define USTAR		5
#define USTAR_NAME	TMAGIC
#define USTAR_REGULAR	TAR_REGULAR
#define USTAR_SPECIAL	TAR_SPECIAL
#define USTAR_HEADER	TAR_HEADER
#define USTAR_ALIGN	TAR_ALIGN
#define USTAR_FLAGS	IN|OUT

#define tar_header	tar_header_block.dbuf
#define tar_block	tar_header_block.dummy

/*
 * s5r4 expanded cpio(5) char format
 * pax won't even bother with the binary counterpart
 */

#define ASC		6
#define ASC_NAME	"asc"
#define ASC_REGULAR	CPIO_REGULAR
#define ASC_SPECIAL	CPIO_SPECIAL
#define ASC_HEADER	110
#define ASC_MAGIC	070701
#define ASC_NAMESIZE	1024
#define ASC_ALIGN	4
#define ASC_FLAGS	IN|OUT

/*
 * s5r4 expanded cpio(5) char format with checksum
 * (misnamed ``crc'' -- the check is a simple sum of the header)
 * pax won't even bother with the binary counterpart
 */

#define ASCHK		7
#define ASCHK_NAME	"aschk"
#define ASCHK_REGULAR	ASC_REGULAR
#define ASCHK_SPECIAL	ASC_SPECIAL
#define ASCHK_MAGIC	070702
#define ASCHK_ALIGN	ASC_ALIGN
#define ASCHK_FLAGS	IN|OUT

/*
 * backup saveset format
 */

#define SAVESET		8
#define SAVESET_NAME	"vmsbackup"
#define SAVESET_REGULAR	0
#define SAVESET_SPECIAL	0
#define SAVESET_ALIGN	0
#define SAVESET_FLAGS	IN
#define SAVESET_ID	"DECVMS"
#define SAVESET_IMPL	"BACKUP"

#define BLKHDR_SIZE	256
#define BLKHDR_hdrsiz	0
#define BLKHDR_blksiz	40

#define FILHDR_SIZE	4
#define FILHDR_MAGIC	257
#define FILHDR_namelen	128
#define FILHDR_size	0
#define FILHDR_type	2
#define FILHDR_data	4

#define FILATT_blocks	10
#define FILATT_frag	12
#define FILATT_recatt	1
#define FILATT_recfmt	0
#define FILATT_reclen	2
#define FILATT_recvfc	15

#define RECHDR_SIZE	16
#define RECHDR_size	0
#define RECHDR_type	2

#define REC_file	3
#define REC_vbn		4

/*
 * the proposed POSIX IEEE Std 1003.1B-1990 interchange format
 */

#define PAX		9
#define PAX_NAME	"posix"
#define PAX_REGULAR	10
#define PAX_SPECIAL	10
#define PAX_ALIGN	0
#define PAX_FLAGS	IN|OUT
#define PAX_RECSIZE	(state.blocksize)
#define PAX_RECFORMAT	'F'
#define PAX_ID		"POSIX1"

/*
 * portable (object) archive
 */

#define PORTAR		10
#define PORTAR_NAME	"portarch"
#define PORTAR_REGULAR	0
#define PORTAR_SPECIAL	0
#define PORTAR_HEADER	sizeof(portar_header)
#define PORTAR_MAG	"!<arch>\n"
#define PORTAR_MAGSIZ	8
#define PORTAR_SYM	"(/|__________E???_)*"
#define PORTAR_END	"`\n"
#define PORTAR_ENDSIZ	2
#define PORTAR_TERM	'/'
#define PORTAR_ALIGN	2
#define PORTAR_FLAGS	IN

struct portar_header
{
	char	ar_name[16];
	char	ar_date[12];	/* left-adj; decimal char*; blank fill	*/
	char	ar_uid[6];	/*	"				*/
	char	ar_gid[6];	/*	"				*/
	char	ar_mode[8];	/* left-adj; octal char*; blank fill	*/
	char	ar_size[10];	/* left-adj; decimal char*; blank fill	*/
	char	ar_fmag[2];	/* PORTAR_END				*/
};

/*
 * ranlib (object) archive -- almost PORTAR
 */

#define RANDAR		11
#define RANDAR_NAME	"randarch"
#define RANDAR_REGULAR	0
#define RANDAR_SPECIAL	0
#define RANDAR_SYM	"(__.SYMDEF|__________E???X)*"
#define RANDAR_TERM	' '
#define RANDAR_ALIGN	PORTAR_ALIGN
#define RANDAR_FLAGS	IN

#define FILE_DEFAULT	"-"		/* default in/out/delta file	*/

#define IN_DEFAULT	CPIO		/* first getheader() state	*/
#define OUT_DEFAULT	CPIO		/* default output format	*/

#define IN		(1<<0)		/* copy in			*/
#define OUT		(1<<1)		/* copy out			*/

#define COLLECT		(1<<2)		/* collect input member info	*/
#define COMPRESS	(1<<3)		/* output is self delta		*/
#define CONVERT		(1<<4)		/* convert archive format	*/

#define SETIDS		(1<<0)		/* set explicit uid and gid	*/

#define INFO_SEP	'!'		/* info header field separator	*/

#define ID		"PAX"		/* info header id		*/
#define IDLEN		(sizeof(ID)-1)	/* strlen(ID)			*/

#define IMPLEMENTATION	"ATTPAX1"	/* implementation id		*/

#define TYPE_COMPRESS	'C'		/* compress encoding type	*/
#define TYPE_DELTA	'D'		/* delta encoding type		*/
#define VERSION		'A'		/* encoding type version	*/

#define DELTA_create	'c'		/* delta create data op		*/
#define DELTA_delete	'd'		/* delta delete data op		*/
#define DELTA_pass	'p'		/* delta pass pseudo op		*/
#define DELTA_update	'u'		/* delta update data op		*/
#define DELTA_verify	'v'		/* delta verify data op		*/

#define DELTA_LO(c)	((c)&0xffff)	/* lo order checksum bits	*/
#define DELTA_HI(c)	DELTA_LO(c>>16)	/* hi order checksum bits	*/

#define NOLINK		0		/* not a link			*/
#define HARDLINK	'1'		/* hard link to previous entry	*/
#define SOFTLINK	'2'		/* soft link to previous entry	*/

#define BLOCKSIZE	512		/* block size			*/
#define IOALIGN		BOUND1		/* io buffer alignment		*/
#define MINBLOCK	1		/* smallest block size		*/
#define DEFBLOCKS	20		/* default blocking		*/
#define DEFBUFFER	16		/* default io buffer blocking	*/
#define MAXBLOCKS	40		/* largest blocking		*/
#define MAXUNREAD	BLOCKSIZE	/* max bunread() count		*/

struct bio				/* buffered io info		*/
{
	char*		next;		/* next char pointer		*/
	char*		last;		/* last char+1 pointer		*/
	char*		buffer;		/* io buffer			*/
	unsigned long	count;		/* char transfer count		*/
	unsigned long	offset;		/* volume offset		*/
	unsigned int	blocked:1;	/* blocked device io		*/
	unsigned int	blok:1;		/* BLOK io file			*/
	unsigned int	blokflag:1;	/* io file BLOK flag		*/
	unsigned int	empty:1;	/* last read was empty		*/
	unsigned int	eof:1;		/* hit EOF			*/
	unsigned int	unblocked:1;	/* set unblocked device io	*/
};

struct fileinfo				/* common internal file info	*/
{
	int		magic;		/* header magic number		*/
	char*		id;		/* archive file id		*/
	char*		name;		/* archive file name		*/
	int		namesize;	/* name size with null byte	*/
	char*		path;		/* local file name for reading	*/
	struct
	{
	int		op;		/* op				*/
	struct deltainfo* base;		/* base file pointer		*/
	long		size;		/* target file size		*/
	int		version;	/* encoding type version	*/
	}		delta;		/* delta info			*/
	struct stat*	st;		/* stat() info from ftwalk()	*/
	int		type;		/* st_mode type (S_IFMT bits)	*/
	int		linktype;	/* NOLINK, HARDLINK, SOFTLINK	*/
	char*		linkname;	/* link to this path		*/
	int		linknamesize;	/* linkname size with null byte	*/
	char*		uidname;	/* user id name			*/
	char*		gidname;	/* group id name		*/
	struct
	{
	int		blocks;		/* io block count		*/
	int		format;		/* format			*/
	int		section;	/* file section number		*/
	}		record;		/* record format info		*/
	long		checksum;	/* checksum			*/
	unsigned int	skip:1;		/* skip this entry		*/
};

struct deltainfo			/* delta entry info		*/
{
	struct fileinfo* info;		/* deltapass() file info	*/
	short		dev;		/* dev				*/
	short		ino;		/* ino				*/
	long		mtime;		/* modify time			*/
	long		offset;		/* data offset			*/
	long		size;		/* data size			*/
	unsigned int	mark:1;		/* visit mark			*/
};

struct fileid				/* unique file identifier	*/
{
	int		dev;		/* device			*/
	int		ino;		/* inode			*/
};

struct formatinfo			/* format info			*/
{
	char*		name;		/* name				*/
	int		regular;	/* default regular blocking	*/
	int		special;	/* default special blocking	*/
	int		align;		/* trailer alignment		*/
	int		flags;		/* io info			*/
};

struct linkinfo				/* link info			*/
{
	char*		name;		/* name				*/
	int		namesize;	/* name size with null byte	*/
	struct fileid	id;		/* generated link file id	*/
};

struct maplist				/* file name map list		*/
{
	struct maplist*	next;		/* next in list			*/
	reprogram*	re;		/* compiled match re		*/
	char*		into;		/* map into this		*/
	int		flags;		/* resub() flags		*/
};

struct postinfo				/* post processing restoration	*/
{
	time_t		mtime;		/* modify time			*/
	int		mode;		/* permissions			*/
	int		uid;		/* user id			*/
	int		gid;		/* group id			*/
};

union integral				/* byte|half swap probe		*/
{
	unsigned long	l;
	unsigned short	s[2];
	unsigned char	c[4];
};

struct stateinfo			/* program state		*/
{
	int		acctime;	/* reset file access times	*/
	int		append;		/* append -- must be 0 or 1 !!!	*/
	struct bio	backup;		/* backup() position		*/
	int		blocksize;	/* explicit buffer size		*/
	int		buffersize;	/* io buffer size		*/
	int		complete;	/* files completely in volume	*/
	int		current;	/* current file[] index		*/
	struct
	{
	char*		base;		/* base archive for delta	*/
	char*		buffer;		/* temporary buffer		*/
	int		buffersize;	/* temporary buffer size	*/
	long		checksum;	/* archive running checksum	*/
	long		count;		/* delta file count		*/
	int		fd;		/* file read descriptor		*/
	char*		hdr;		/* header pointer		*/
	char*		hdrbuf;		/* header buffer		*/
	long		membersum;	/* archive member checksum	*/
	long		offset;		/* base file offset		*/
	int		op;		/* delta op state		*/
	long		size;		/* base file size		*/
	int		sum;		/* compute input checksum if >0	*/
	HASHTABLE*	tab;		/* entry table			*/
	}		delta;		/* delta sub state		*/
	char*		destination;	/* pass mode destination dir	*/
	dev_t		dev;		/* . device number		*/
	unsigned short	devcnt;		/* dev assignment count		*/
	HASHTABLE*	restore;	/* post proc restoration table	*/
	int		drop;		/* drop a `.' for each file	*/
	int		dropcount;	/* current line drop count	*/
	int		entries;	/* total archive entry count	*/
	int		entry;		/* archive entry number		*/
	int		errors;		/* error count			*/
	int		exact;		/* exact archive read		*/
	char**		files;		/* alternate file name list	*/
	char*		file;		/* io file name			*/
	char*		filter;		/* file output filter command	*/
	int		flags;		/* flags			*/
	int		formatin;	/* input archive format index	*/
	int		formatout;	/* output archive format index	*/
	int		ftwflags;	/* ftwalk() flags		*/
	int		gid;		/* current group id		*/
	struct
	{
	char		volume[7];	/* volume id			*/
	char		format[7];	/* format id			*/
	char		implementation[8];/* implementation id		*/
	char		owner[15];	/* owner id			*/
	}		id;		/* id strings (including '\0')	*/
	struct bio	in;		/* buffered input info		*/
	unsigned short	inocnt;		/* ino assignment count		*/
	int		intermediate;	/* make intermediate dirs	*/
	int		interrupt;	/* this signal caused exit	*/
	int		keepgoing;	/* keep going on error		*/
	int		(*linkf)();	/* use when possible for -rw	*/
	HASHTABLE*	linktab;	/* hard link table		*/
	int		list;		/* full file trace		*/
	struct maplist*	maps;		/* file name maps		*/
	int		matchsense;	/* pattern match sense		*/
	unsigned long	maxout;		/* max volume/part output count	*/
	int		modemask;	/* and with mode for chmod()	*/
	int		modtime;	/* retain mtime			*/
	int		newer;		/* append only if newer		*/
	long		offset;		/* relative archive byte offset	*/
	int		operation;	/* IN|OUT operation mode	*/
	struct bio	out;		/* buffered output info		*/
	int		owner;		/* set owner info		*/
	int		part;		/* device change count		*/
	char**		patterns;	/* name match patterns		*/
	char		pwd[PATH_MAX];	/* full path of .		*/
	int		pwdlen;		/* pwd length sans null		*/
	time_t		present;	/* for long file listing	*/
	struct
	{
	int		charset;	/* convert record charset	*/
	struct fileinfo* file;		/* current output file		*/
	int		format;		/* output format		*/
	int		line;		/* convert records<->lines	*/
	int		pad;		/* pad output record blocks	*/
	char*		pattern;	/* format match pattern		*/
	int		offset;		/* data buffer offset		*/
	int		size;		/* io size			*/
	}		record;		/* record info			*/
	FILE*		rtty;		/* tty file read pointer	*/
#if SAVESET
	struct
	{
	char*		block;		/* current block		*/
	int		blocksize;	/* max block size		*/
	char*		bp;		/* block pointer		*/
	int		recatt;		/* record attributes		*/
	int		recfmt;		/* record format		*/
	int		reclen;		/* record length		*/
	int		recvfc;		/* record fixed control length	*/
	int		lastsize;	/* size of last record		*/
	time_t		time;		/* backup time			*/
	}		saveset;	/* backup saveset state		*/
#endif
	int		selected;	/* number of selected files	*/
	int		setgid;		/* set file gid to this value	*/
	int		setuid;		/* set file uid to this value	*/
	int		(*statf)();	/* -L=lpstat() -P=lstat()	*/
	int		summary;	/* output summary info		*/
	int		swap;		/* {BYTE,HALF} swap operation	*/
	int		test;		/* debug test bits		*/
	struct
	{
	char*		buffer;		/* temporary buffer		*/
	int		buffersize;	/* temporary buffer size	*/
	char*		file;		/* tmp file name		*/
	}		tmp;		/* temporary stuff		*/
	int		trace;		/* error() debug trace level	*/
	int		uid;		/* current user id		*/
	int		update;		/* copy file only if newer	*/
	int		verbose;	/* trace files when acted upon	*/
	int		verify;		/* verify action on file	*/
	int		volume;		/* archive volume number	*/
	FILE*		wtty;		/* tty file write pointer	*/
	int		xdev;		/* don't cross device boundaries*/
	int		yesno;		/* interactive answer is yes/no	*/
};

extern char*			definput;
extern char*			defoutput;
extern char*			eomprompt;
extern char			uidname[];
extern char			gidname[];

extern struct formatinfo	format[];

extern struct fileinfo		file;
extern struct stateinfo		state;

extern char			alar_header[];
extern struct binary_header	binary_header;
extern union tar_header_block	tar_header_block;
extern struct portar_header	portar_header;

extern void		finish();
extern void		usage();
extern void		copyin();
extern void		copy();
extern int		copyout();
extern void		fileout();
extern void		filein();
extern void		filepass();
extern void		fileskip();
extern int		copyinout();
extern void		append();
extern void		initdelta();
extern void		deltabase();
extern int		deltafd();
extern void		deltaverify();
extern void		deltapass();
extern void		deltadelete();
extern int		getprologue();
extern void		putprologue();
extern void		getepilogue();
extern void		putepilogue();
extern int		getheader();
extern void		getdeltaheader();
extern void		adddelstr();
extern void		adddelnum();
extern void		putheader();
extern void		putdeltaheader();
extern void		gettrailer();
extern void		puttrailer();
extern int		getlabel();
extern void		putlabels();
#if SAVESET
extern int		getsaveset();
extern int		mkbyte();
extern int		mkhalf();
extern long		mklong();
#endif
extern int		getfile();
extern int		selectfile();
extern int		getnumber();
extern int		getformat();
extern int		verify();
extern int		openin();
extern int		openout();
extern int		validout();
extern int		addlink();
extern char*		map();
extern void		listentry();
extern void		getidnames();
extern void		setidnames();
extern void		initfile();
extern char**		initmatch();
extern int		match();
extern int		dirprefix();
extern char*		strlower();
extern char*		strupper();
extern void		swap();
extern void		setfile();
extern void		settime();
extern int		missdir();
extern int		restore();
extern long		cpio_long();
extern void		cpio_short();
extern int		cpio_mode();
extern int		tar_checksum();
extern long		asc_checksum();
extern long		getlabnum();
extern char*		getlabstr();
extern void		binit();
extern void		balloc();
extern int		bread();
extern void		bunread();
extern char*		bget();
extern void		backup();
extern void		bflushin();
extern void		bflushout();
extern void		bwrite();
extern void		bput();
extern void		interactive();
extern void		newio();
extern int		dodelta();
extern int		doupdate();
#if DEBUG
extern char*		operations();
#endif

extern char*		ctime();
extern char*		fgetline();
extern char*		fmtmode();
extern int		fstat();
extern int		link();
extern long		lseek();
extern int		lstat();
extern int		lpstat();
extern char*		malloc();
extern char*		memcpy();
extern char*		memdup();
extern unsigned long	memsum();
extern char*		pathcanon();
extern char*		pathtemp();
extern int		remove();
extern int		stat();
extern char*		strcpy();
extern char*		strncpy();
extern char*		strchr();
extern char*		strrchr();
extern char*		strtape();
extern long		strtol();
extern long		strton();
extern char*		strdup();
extern int		strmatch();
extern int		struid();
extern int		strgid();
extern int		getsymlink();
extern int		putsymlink();

#if __STDC__

extern FILE*		fdopen(int, char*);

#endif
