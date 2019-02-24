/*
 * defs for the kdb50 driver
 */

/*
 * per-controller structure
 */

#define	BDACMD	32	/* no more than this many command packets */
#define	BDANMAP	20	/* number of chunks of map */

struct bd {
	struct biic *bd_addr;		/* device register address */
	short	bd_flags;		/* misc flag bits */
	short	bd_credits;		/* number of flow control credits */
	struct bdcomm *bd_comm;		/* virt addr of comm area */
	struct bdcmd *bd_cpkt;		/* virt addr of command packets */
	struct bdrsp *bd_rpkt;		/* virt addr of response packets */
	long	*bd_pmap;		/* page table for mapped transfers */
	struct buf *bd_cbuf;		/* buffer for comm/cmds */
	struct buf *bd_rbuf;		/* buffer for responses */
	struct buf *bd_mbuf;		/* buffer for page table */
	short	bd_cnext;		/* index of next cmd descriptor */
	short	bd_rnext;		/* ditto response */
	char	bd_cbusy[BDACMD];	/* busy flags for commands */
	char	bd_back[BDACMD];	/* back-index from cmd ring to packets */
	unsigned short bd_mbase[BDACMD];	/* base addr of allocated map */
	unsigned short bd_msize[BDACMD];	/* size of allocated map */
	struct map bd_map[BDANMAP];	/* and how it is allocated */
};
