/*
 * for datakit simple kmc driver
 */

/*
 * per channel
 */
struct	kb {
	struct	queue *dkrq;
	char	flag;
	u_char	chan;
	struct	kb *link;
};

/*
 * per interface
 */

struct kbkmc {
	char	flags;
	char	kno;		/* which kmc is ours */
	char	ubno;		/* which unibus has the kmc */
	struct device *addr;	/* kmc registers */
	struct block *ibuf;	/* pending input buffer */
	struct block *obuf;	/* pending output buffer */
	uaddr_t iaddr, oaddr;	/* their unibus addresses */
	ubm_t	imap, omap;	/* and map cookies */
	struct kb *xfirst, *xlast;	/* pending transmit channels */
	struct dkmodule *modp;
};
