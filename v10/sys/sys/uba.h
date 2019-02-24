/*
 * data structures for the unibus adapter code
 */

#define	UBNMAP	100

struct uba {
	short flags;
	short path;		/* data paths available */
	struct ubadev *addr;	/* our registers and address window */
	ubm_t *sbmap;		/* stream block maps, per buffer */
	uaddr_t *sbaddr;	/* and unibus addresses */
	ubm_t shmap;		/* stream block header map */
	uaddr_t shaddr;		/* stream block header unibus address */
	struct map map[UBNMAP];
};

/*
 * flags
 */

#define	UBINIT	01	/* this adapter has been set up */
#define	UBMWANT	02	/* map needed */
#define	UBQBUS	04	/* this is not a unibus */ 
