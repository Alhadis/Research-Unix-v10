/*
 * for datakit simple cure driver
 */

/*
 * per channel
 */
struct	kc {
	struct	queue *dkrq;
	char	flag;
	u_char	chan;
	struct	kc *link;
};

#define	NCTL	4
struct	cmd {
	long	ubaddr;
	unsigned short count;
	unsigned char chan;
	unsigned char flag;
	unsigned short ctl[NCTL];
};

/*
 * per interface
 */

#define	NCBUF	4

struct kccure {
	char	flags;
	char	kno;			/* which cure is ours */
	char	ubno;			/* which unibus has the cure */
	struct	device *addr;		/* cure registers */
	struct	cmd xcbuf[NCBUF];	/* xcommand buffers */
	struct	cmd rcbuf[NCBUF];	/* rcommand buffers */
	struct	block *obp[NCBUF];	/* current output blocks */
	struct	block *ibp[NCBUF];	/* current input blocks */
	ubm_t	imap[NCBUF];		/* unibus map cookies */
	ubm_t	omap[NCBUF];
	struct kc *xfirst, *xlast;	/* pending transmit channels */
	struct dkmodule *modp;
};
