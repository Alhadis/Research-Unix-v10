/*
 * per-channel structure
 */

struct kdi {
	struct	queue *dkrq;
	struct	block *ibp;
	struct	block *obp;
	ubm_t	imap;
	ubm_t	omap;
	unsigned short	rsize;		/* # bytes in last read */
	unsigned char	chan;
	unsigned char	ostate;
	unsigned char	kno;
	unsigned char lastctl;
};

/*
 * per-kmc structure
 */

struct kdikmc {
	struct	kdi *kdi;	/* per-channel structures */
	char	*state;		/* per-channel state */
	struct buf *buf;	/* where the data above is stashed */
	ubm_t	umap;	/* map token for kmc stuff below */
	uaddr_t	ubad;	/* unibus addr of device data */
	struct	dkmodule *modp;
	time_t	badtime;
	char	init;
	char	bad;
	char	ubno;
};

/*
 * format of UB addresses sent to KMC
 */
struct	kmaddr {
	unsigned short	hi;
	unsigned short	lo;
};

/*
 * KMC init packet
 */
struct	kinit {
	struct	kmaddr	cmdaddr;	/* UB addr of cmd buf */
	struct	kmaddr	stataddr;	/* UB addr of statbuf */
	struct	kmaddr	bufaddr;	/* UB addr of KMC workspace */
	struct	kmaddr	csraddr;	/* for DR11C - unused */
};

/*
 * command/status packets
 */
struct kin {				/* KMC command buffer */
	unsigned char	type;		/* command type */
	unsigned char	serno;		/* probably seq number */
	unsigned char	chan;		/* channel number */
	unsigned char	fill2;		/* probably seq number */
	unsigned short	len;		/* byte count */
	char	ctl;			/* possible control byte */
	char	mode;			/* command variant */
	struct	kmaddr addr;		/* UB location of buffer */
};

/*
 * Big structure with stuff that needs to be accessible on the unibus
 * one per kmc
 */

#define	NKMB	10		/* number of cmd/status packets */

struct kmcdk {
	struct	kinit	kinit;			/* the init packet */
	struct	kin	cmd[NKMB];		/* KMC command buffer */
	struct	kin	stat[NKMB];		/* KMC status buffer */
	char	kmcbuf[16*1024];		/* temp space for KMC */
};
