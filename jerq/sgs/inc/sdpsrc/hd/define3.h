/*
 * static char ID_def3h[] = "@(#) define3.h: 1.1 1/7/82";
 */

/*
 * No user serviceable parts here; internal defined constants only
 *
 *
 *
 *	NOTCALLEDBEFORE	sdpinit() has not been called previously
 *	CALLEDBEFORE	sdpinit() has been called previously
 *	CLOSED		initial value of file descriptors
 *	INVALID		value of linked-list pointers in managers not on free
 *				frame list
 *	LAMDA		value of backward link of first manager;
 *				forward link of last
 *	LRU		indicates "least-recently-used" page frame
 *	MRU		indicates "most-recently-used" page frame
 *	NOFRAMES	mnemonic for delist; no frames linked into the free
 *				frame list
 *	NOPAGE		alias for fatal() from getnxtpage; it is not a
 *				valid itemid
 *	NOTFOUND	mnemonic for file not present
 *	NOTMADE		initial value for file descriptors
 *	RW		read/write permissions for open;  See open(II)
 *	SUCCESS		return value for successful function
 *	SYSERR		return value for unsuccessful return from
 *				system function
 */

#define NOTCALLEDBEFORE	0
#define CALLEDBEFORE	1
#define CLOSED		(-1)
#define INVALID		(-2)
#define LAMDA		(-1)
#define LRU		0
#define MRU		1
#define NO		0
#define YES		1
#define NOFRAMES	(-1)
#define NOPAGE		(-1)
#define NOTFOUND	(-1)
#define NOTMADE		(-2)
#define RW		2
#define SUCCESS		1
#define SYSERR		(-1)
