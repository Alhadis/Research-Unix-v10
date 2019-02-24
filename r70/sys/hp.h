/*
 * MASSBUS SMD disk data structures
 * one hpdisk per drive
 */

#define	NHPPART	8

struct hpdisk {
	struct hpdevice *addr;
	char flags;
	char type;	/* index into hpst */
	char errcnt;
	char recal;
	short badsec;
	char pinit;
	struct buf *actf, *actl;	/* queue of buffers waiting for xfer */
	long blkoff[NHPPART];
	long nblocks[NHPPART];
};

/*
 * device size info
 * here so hpdump can get it
 */
struct	hptype {
	short	type;	/* hpdt drive type */
	char	nsect;	/* sectors per track */
	char	ntrak;	/* tracks per cylinder */
	char	sdist;	/* sectors twixt search and io */
	char	rdist;	/* sectors close enough not to search */
	short	nspc;	/* sectors per cylinder */
	short	ncyl;	/* cylinders per pack */
	struct	size *sizes;
};
