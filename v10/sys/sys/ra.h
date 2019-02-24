/*
 * data structures for the MSCP disk class driver
 */

#define	NRAPART	8	/* eight partitions per drive */

struct radisk {
	struct msportsw *port;
	char open;		/* partition open */
	char pinit;		/* partition initialized */
	char flags;
	char rplret;
	long cmdcrf;		/* crf of pending non-xfer command */
	char cmdopc;		/* opcode too */
	struct buf *actf;	/* next buffer to send */
	struct buf *actl;	/* end of list of unsent buffers */
	daddr_t nblocks[NRAPART];	/* size of each partition */
	daddr_t blkoff[NRAPART];	/* starting block for each partition */
	struct ud_unit di;	/* hardware parameters */
};
