/*
 * TMSCP driver
 */

struct tatape {
	short flags;
	short dens;		/* current density */
	struct msportsw *port;
	struct buf *actf;	/* next buffer sent */
	struct buf *actl;	/* last buffer sent */
	struct mscmd *cmdp;	/* pending command this unit */
};
