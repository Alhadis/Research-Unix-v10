/*
 * defs for the uda50 driver
 */

/*
 * per-controller structure
 */

#define	MAXCMD	32	/* no more than this many command packets */

struct ud {
	struct udcomm *ud_comm;		/* virt addr of comm area */
	uaddr_t	ud_pcomm;		/* unibus addr of ring base */
	struct udcmd *ud_cpkt;		/* virt addr of command packets */
	uaddr_t ud_pcpkt;		/* unibus addr of command packets */
	short	ud_cnext;		/* index of next cmd descriptor */
	short	ud_rnext;		/* ditto response */
	ubm_t	ud_cmap[MAXCMD];	/* ub mapping info for transfers */
	char	ud_cbusy[MAXCMD];	/* busy flags for commands */
	char	ud_back[MAXCMD];	/* back-index from cmd ring to packets */
	struct udrsp *ud_rpkt;		/* virt addr of response packets */
	struct buf *ud_cbuf;		/* buffer for comm/cmds */
	ubm_t	ud_cbm;			/* and its map info */
	struct buf *ud_rbuf;		/* buffer for responses */
	ubm_t	ud_rbm;			/* and its map info */
	int	ud_bdpno;		/* bdp for transfers */
	short	ud_flags;		/* misc flag bits */
	short	ud_credits;		/* number of flow control credits */
	struct device *ud_addr;		/* device register address */
};

/*
 * ud_flags
 */

#define	UINIT	01		/* already did trivial init */
#define	UIDONE	02		/* initialization all done */
#define	UPWAIT	04		/* waiting for command packet */
#define	UFIRST	010		/* let first packet go even if no credits */
#define	UTIMER	020		/* timer will kick on next go */
#define	UCWAIT	040		/* waiting for credits */
#define	UISTART	0100		/* initialization started */

/*
 * ud_cbusy
 */

#define	FREE	0
#define	NABBED	01
#define	SENT	02
#define	MAPPED	04

#define	NOBACK	(-1)
