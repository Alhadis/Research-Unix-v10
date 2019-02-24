/*
 * static char ID_strcth[] = "@(#) struct.h: 1.1 1/7/82";
 */

/*
 * Each manager keeps track of information relevant to a particular
 * frame. There is one manager for each frame.
 *
 *	forward, backward: link the managers into a doubly-linked list of
 *			   available (e.g., free) page frames
 *	frame_pntr:	   the address of the start of the
 *			   associated frame.
 *
 * These three fields are initialized by function sdpinit(), and do
 * not change.
 *
 *	page_id:    the itemid of the start of the page
 *		    which resides in the associated frame.
 *	ref_chng:   a flag which indicates whether or not
 *		    the page which resides in the associated frame
 *		    has been changed.
 *	lock_count: the number of lock calls made on items on the resident
 *		    page without a corresponding unlock call being issued
 *	partition:  the number of the page-frame partition
 *		    to which this manager belongs
 *
 * The value of these last three members is transient.
 */

struct frame_manager {
	long	page_id;	/* itemid to start of resident page	*/
	int	lock_count;	/* number of locks without matching 
					unlocks				*/
	int	forward;	/* forward link to next manager		*/
	int	backward;	/* backward link to previous manager	*/
	int	ref_chng;	/* reference/change flag		*/
	int	partition;	/* page frame partition			*/
	char   *frame_pntr;	/* address of associated frame		*/
	};

typedef struct frame_manager MANAGER;
/*eject*/
/*
 * A structure header_buffer exists for each open address space.
 * Each structure contains information which descirbe the status
 * of the associated address space.
 */

struct header_buffer {
	long	maxpage;	/* largest page number allocated	*/
	long	curpage;	/* page number of current page		*/
	int	curoffset;	/* offset into current page		*/
	int	numbfiles;	/* number of files composing space	*/
	long	filesize[MAXFILES];	/* size (in pages) of files	*/
	int	filedes[MAXFILES];	/* file descriptors of files	*/
	};

typedef struct header_buffer HEADER;
