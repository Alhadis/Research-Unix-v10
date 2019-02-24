/* bit fields strike again!! there seems to be an indirection problem */

struct cmap
{
unsigned int 	c_next:13,	/* index of next free list entry */
		c_prev:13,	/* index of previous (6700 for 8Mb) */
		c_lock:1,	/* locked for raw i/o or pagein */
		c_want:1,	/* wanted */
		c_page:16,	/* virtual page number in segment */
		c_hlink:13,	/* hash link for <blkno,mdev> */
		c_intrans:1,	/* intransit bit */
		c_free:1,	/* on the free list */
		c_gone:1,	/* associated page has been released */
		c_type:2,	/* type CSYS or CTEXT or CSTACK or CDATA */
		c_blkno:20,	/* disk block this is a copy of */
		c_ndx:10,	/* index of owner proc or text */
		c_mdev:6;	/* which mounted dev this is from (NMOUNT) */
};

extern struct cmap *cmap;

foo(i)
register i;
{
	cmap[i-1].c_next = i;
}

