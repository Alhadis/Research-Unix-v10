/*
 *	static char ID_tgitmh[] = "@(#)tagitem.h	1.5 1/3/84";
 */

/*
 * SDP user-dependent constants
 *
 *	BYTEBITS	2**BYTEBITS == sze of a page, in bytres
 *	DFILELENG	maximum size of a SDP paging file, in pages
 *	DFILES		initial number of files in an addres space
 *	DPARTITIONS	number of page frames in each partition
 *	DPARTSIZE	number of page frame partitions
 *	DPARTRESET	usage count at which partition sizes are
 *				recomputed
 *	long		the type of C data structure for an itemid
 *	ITEMIDSIZE	inumber of bytes in an itemid
 *	ITEMSIZE	number of bits in an itemid
 *	MAXATTACH	maximum number of concurrently opened address
 *				spaces
 *	MAXFILES	maximum number of files comprising an address
 *				space
 *	PARTITION	page frame partition
 *	MODE		mode for open() calls
 *	NAMELENG	maximum size of an address space file name
 *	NUMBERFRAMES	number of SDP paging frames
 *	PAGEBITS	2**PAGEBITS == size of an address space, in
 *				pages
 *	CREATE		tells sdpinit() when the address space paging
 *				files do not exist
 *	TEMPLATE	template for all address space file names
 */

#define BYTEBITS	9
#define DFILELENG	{40000}
#define DFILES		1
#ifndef pdp11
#define DPARTITIONS	{40}
#else
#define DPARTITIONS	{20}
#endif
#define DPARTSIZE	1
#define DPARTRESET	-1
#define PARTITION	0
#define ITEMSIZE	32
#define ITEMIDSIZE	4
#define MAXATTACH	2	/* > largest address space id */
#define MAXFILES	1
#define MODE		0600
#define NAMELENG	30
#ifndef pdp11
#define NUMBERFRAMES	40
#else
#define NUMBERFRAMES	20	/* DPARTSIZE * DPARTITIONS == NUMBERFRAMES */
#endif
#define PAGEBITS	15
#define CREATE		1
#define TEMPLATE	"sdp__"

#define TMPDIR "/usr/tmp"
#define TAGITEM struct tagitem
#define SYMENTRY struct symbol
#define ERROR  0

/*	tagitem.h contains structure that is used for comparing tags
 *	in the compress utility.  it also defines a macro to verity
 *	that a file contains the long IS25 object file format.
 */

struct tagitem {
	long		oldloc;		/*  old loc in symbol table  */
	unsigned	nentrys;	/*  # of entries of tag      */
	long		next_ti;	/*  ptr. to next tagitem     */
	long		next_col;	/*  next item in collision chain */
	long		tagbuf;		/* ptr. to symbol table info*/
	long		p_realtag;	/*  ptr to "real" struct     */
	long		newloc;		/*  loc of tag after compression */
	SYMENT		*tmpsym;	/* temp storage of structure mems */
};

struct symbol {

	SYMENT		symbuf;
	long		auxbuf;
	long		nxtsym;
};
