/*
 * for datakit simple kmc driver
 */

/*
 * per channel
 */
struct	kb {
	struct	queue *dkrq;
	char	flag;
	unsigned char	chan;
	struct	kb *link;
};

/*
 * buffer descriptor
 */
struct kbdesc {
	short loaddr;	/* low buffer address */
	short len;	/* negative buffer length */
	unsigned char chan;	/* channel */
	char flag;	/* flags and high address */
};

/*
 * per interface
 */

#define	XRING	40
#define	RRING	20

struct kbkmc {
	char	flags;
	char	kno;		/* which kmc is ours */
	char	ubno;		/* which unibus has the kmc */
	struct device *addr;	/* kmc registers */
	struct dkmodule *modp;
	struct {	/* bundle for convenient unibus mapping */
		struct kbdesc _xring[XRING+1];
		struct kbdesc _rring[RRING+1];
	} rings;
#define	xring	rings._xring
#define	rring	rings._rring
	ubm_t	ringmap;
	uaddr_t	xrua, rrua;	/* unibus address of descriptor rings */
	struct block *xblock[XRING];	/* blocks in transmit descriptors */
	ubm_t	xmap[XRING];	/* and their map cookies */
	struct block *rblock[RRING];	/* likewise receive */
	ubm_t	rmap[RRING];
	char xrp, xwp;		/* read, write positions in xring */
	char rrp, rwp;		/* and in rring */
	int	rbytes;		/* bytes of receive buffer outstanding */
};
