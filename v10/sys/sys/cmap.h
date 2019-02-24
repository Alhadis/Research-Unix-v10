/*
 * core map entry
 *
 * the 20-bit fields imply a maximum of 1GB with 1K pages
 * which is scant compensation for the enormous space this
 * damned structure takes.
 */
struct cmap
{
unsigned int 	c_next:20,	/* index of next free list entry */
		c_lock:1,	/* locked for raw i/o or pagein */
		c_want:1,	/* wanted */
		c_intrans:1,	/* intransit bit */
		c_free:1,	/* on the free list */
		c_gone:1,	/* associated page has been released */

		c_prev:20,	/* index of previous */
		c_type:2,	/* type CSYS or CTEXT or CSTACK or CDATA */

		c_page:20,	/* virtual page number in segment */
		c_ndx:12,	/* index of owner proc or text */

		c_hlink:20,	/* hash link for <blkno,mdev> */

		c_blkno:20,	/* disk block this is a copy of */
		c_mdev:6;	/* which mounted dev this is from (NMOUNT) */
};

#define	CMHEAD	0

#ifdef	KERNEL
struct	cmap *cmap;
struct	cmap *ecmap;
int	ncmap;
struct	cmap *mfind();
int	firstfree, maxfree;
int	ecmx;			/* cmap index of ecmap */
#endif

/* bits defined in c_type */

#define	CSYS		0		/* none of below */
#define	CTEXT		1		/* belongs to shared text segment */
#define	CDATA		2		/* belongs to data segment */
#define	CSTACK		3		/* belongs to stack segment */

#define	pgtocm(x)	((((x)-firstfree) / CLSIZE) + 1)
#define	cmtopg(x)	((((x)-1) * CLSIZE) + firstfree)

/*
 * magic for c_mdev
 */
#define	MSWAPX	63	/* the swap device */
