/*
 * first block of rct: random crap
 * longwords are in VAX order
 */

struct rct {
	long	rc_volser[2];	/* volume serial number */
	short	rc_flags;
	short	rc_res0;
	long	rc_lbn;		/* block being replaced */
	long	rc_rbn;		/* replacement block */
	long	rc_badrbn;	/* bad replacement block */
	short	rc_junk[256-12];	/* reserved */
};

/*
 * flags
 */

#define	RPROT	02		/* volume should be write protected */
#define	RFE	0200		/* force error in replacement block */
#define	RBADRBN	020000		/* bad rbn being replaced */
#define	RPH2	040000		/* in `phase 2' */
#define	RPH1	0100000		/* in `phase 1' */

/*
 * replacement block descriptor
 */

struct rbd {
	unsigned rb_lbn: 28;
	unsigned rb_code: 4;
};

/*
 * codes
 */

#define	RFREE	0
#define	RALT	01	/* alternate */
#define	RALLOC	02	/* allocated */
#define	RBAD	04	/* unusable replacement block */
#define	RNULL	010	/* null entry (no such RBN) */

#define	RBNPB	128	/* RBNs per block */
#define	RBNSEC	512	/* bytes per RBN sector */

/*
 * blocks in the RCT
 */

#define	RCTHDR	0	/* header stuff */
#define	RCTDATA	1	/* saved data from block under replacement */
#define	RCTTAB	2	/* beginning of the table */
