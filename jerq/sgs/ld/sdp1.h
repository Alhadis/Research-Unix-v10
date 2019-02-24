/*
 *  static char ID[] = "@(#) sdp1.h: 1.1 1/6/82";
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
 *	HITPARTITION	page frame partition for hash table collisions
 *	ITEMID		the type of C data structure for an itemid
 *	ITEMIDSIZE	inumber of bytes in an itemid
 *	ITEMSIZE	number of bits in an itemid
 *	MAXATTACH	maximum number of concurrently opened address
 *				spaces
 *	MAXFILES	maximum number of files comprising an address
 *				space
 *	MISPARTITION	page frame partition for initial hash table
 *				symbols and aux entries
 *	MODE		mode for open() calls
 *	NAMELENG	maximum size of an address space file name
 *	NUMBERFRAMES	number of SDP paging frames
 *	PAGEBITS	2**PAGEBITS == size of an address space, in
 *				pages
 *	SVSPACE		SDP address space id for slot vectors
 *	SVPARTITION	page frame partition for slot vectors
 *	SYMSPACE	SDP address space id for global symbol table
 *	AUXSPACE	SDP address space id for aux entries
 *	TEMPLATE	template for all address space file names
 */

#define BYTEBITS	9
#define DFILELENG	{40000}
#define DFILES		1
#if AR16WR
#define DPARTITIONS	{20}
#else
#define DPARTITIONS	{40}
#endif
#define DPARTSIZE	1
#define DPARTRESET	-1
#define HITPARTITION	0
#define ITEMID		long
#define ITEMSIZE	32
#define ITEMIDSIZE	4
#define MAXATTACH	3	/* > largest address space id */
#define MAXFILES	1
#define MISPARTITION	0
#define MODE		0600
#define NAMELENG	30
#if !AR16WR
#define NUMBERFRAMES	40
#else
#define NUMBERFRAMES	20	/* DPARTSIZE * DPARTITIONS == NUMBERFRAMES */
#endif
#define PAGEBITS	15
#define SVSPACE		1
#define SVPARTITION	0
#define SYMSPACE	0
#define AUXSPACE	2
#define TEMPLATE	"sdp__"
