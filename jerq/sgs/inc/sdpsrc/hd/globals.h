/*
 * static char ID_glblsh[] = "@(#) globals.h: 1.1 1/7/82";
 */

/*
 * Global SDP data structures
 *
 *
 *
 *	callstatus		sdpinit() "called already?" flag
 *	direct_names		root directory to files making up an
 *					address space
 *	file_size		maximum sizes of files making up an
 *					address space
 *	frames			SDP page frmes
 *	global_frames		control information for each of the address
 *					spaces
 *	lru			pointer to least recently used frame
 *	manager			control information on each SDP frame
 *	mru			pointer to most recently used frame
 *	num_linked		number of unlocked, free, frames
 *	num_partitions		number of page frame partitions
 *	num_spaces		number of open address spaces
 *	open_spacenumbers	space ids of the open address spaces
 *	partitions		number of page frames in each partition
 *	partreset		usage count at which page frame partition
 *					sizes are recomputed
 *	partusage		usage count for each partition
 */

char	frames[NUMBERFRAMES][PAGESIZE];
MANAGER	manager[NUMBERFRAMES];
HEADER	global_frames[MAXATTACH];
int	lru[DPARTSIZE];
int	mru[DPARTSIZE];

char    direct_names[MAXATTACH][NAMELENG];
long	file_size[MAXFILES]	= DFILELENG;

int	partitions[DPARTSIZE]	= DPARTITIONS;
int	partreset		= DPARTRESET;
int	partusage[DPARTSIZE+1];

int	callstatus		= NOTCALLEDBEFORE;

int	num_linked[DPARTSIZE];
int	num_partitions		= DPARTSIZE;
int	num_spaces		= 0;
int	open_spacenumbers[MAXATTACH];
