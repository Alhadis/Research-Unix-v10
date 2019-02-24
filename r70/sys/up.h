/*
 * UNIBUS SMD disk data structures
 * one updisk per drive
 * one scctl per controller
 */

#define	NUPPART	8

struct updisk {
	struct scctl *ctl;
	char flags;
	char type;		/* type of drive; magic code */
	char unit;		/* physical unit number on this controller */
	char pinit;
	struct buf *actf;	/* list of buffers waiting to be xfered */
	struct buf *actl;
	struct updisk *next;
	long blkoff[NUPPART];
	long nblocks[NUPPART];
};

#define	NSCUP	8	/* max units per controller */

struct scctl {
	struct updevice *addr;
	struct updisk *actf;	/* list of active drives */
	struct updisk *actl;
	char flags;
	char ubno;		/* unibus number -- only for mapping */
	char errcnt;
	char recal;		/* current place in retry machine */
	char softas;		/* non-transfer attention required */
	struct updisk *drives[NSCUP];	/* back-pointers to drives */
};

/*
 * device size
 * here so updump can get it
 */
struct	upst {
	char	nsect;	/* sectors per track */
	char	ntrak;	/* tracks per cylinder */
	char	sdist;	/* sectors twixt search and io */
	char	rdist;	/* sectors close enough not to search */
	short	nspc;	/* sectors per cylinder */
	short	ncyl;	/* cylinders per pack */
	struct	size *sizes;
};
