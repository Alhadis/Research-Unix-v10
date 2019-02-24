/*
 * per-massbus structure
 */

#define	MBAUNITS	8	/* units per massbus */
struct mba {
	struct mbaregs *addr;
	int flags;
	short lastoff;		/* alignment offset for current xfer */
	char first, last;	/* units waiting for xfer */
	char mbq[MBAUNITS];
	struct buf *mbuf[MBAUNITS];	/* buffer pending */
	int (*xstart[MBAUNITS])();	/* code to start xfer */
};
