#define	NBLOCK		512
#define	BLOCKSIZE	(4096)

/*
 * Descriptor pointing to a disk block
 */
typedef struct{
	short	nbytes;	/* number of bytes used in this block */
	short	bnum;	/* which disk block it is */
}Block;
/*
 * Strings all live in a common buffer and are nicely garbage compacted.
 */
typedef struct{
	char	*s;	/* pointer to string */
	ulong	n;	/* number used; s[n]==0 */
	ulong	size;	/* size of allocated area */
}String;
String *bldstring();	/* (temporary String)-building function */
#define	DUBIOUS	1L	/* An unlikely date; means we don't know origin of buffer */
/*
 * A File is a local buffer for the in-core block
 * and an array of block pointers.  The order of the block pointers
 * in the file structure determines the order of the true data,
 * as opposed to the order of the bits in the file (c.f. ed temp files *).
 */
typedef struct File{
	String	name;		/* name of associated real file, "" if none */
	String	str;		/* storage for in-core data */
	long	date;		/* date file was read in */
	long	origin;		/* file location of first char on screen */
	long	selloc;		/* start location of selected text */
	long	nsel;		/* number of chars selected */
	int	curblk;		/* block associated with File.str */
	short	id;		/* id number for communication */
	char	changed;	/* changed since last write */
	short	nblocks;	/* number of blocks in File.block */
	short	nalloc;		/* number of blocks allocated */
	Block	*block;		/* array of block pointers */
	struct File *next;	/* list chain */
}File;
File *file;
String buffer;		/* Place to save squirreled away text */
String transmit;	/* String to send to Jerq */

File	*Fcreat();
File	*Fnew();
File	*Fload();
File	*Fileid();
long	length(), Fforwnl(), Fbacknl(), Fcountnl();
#define	YMAX	1024	/* as in jerq.h */
#define	DIAG	file
#define	TRUE	1
#define	FALSE	0
long	loc1, loc2;	/* location of searched-for string */
char	*strcpy();
int	fileschanged;
