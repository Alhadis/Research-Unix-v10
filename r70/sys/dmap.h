/*
 * Definitions for the mapping of vitual swap
 * space to the physical swap area - the disk map.
 */

#define	NDMAP 		64	/* size of the swap area map */

struct	dmap
{
	swblk_t	dm_size;	/* current size used by process */
	swblk_t	dm_alloc;	/* amount of physical swap space allocated */
	swblk_t	dm_map[NDMAP];	/* first disk block number in each chunk */
};
#ifdef KERNEL
extern struct	dmap zdmap;
extern int dmmin;		/* size in clicks of dm_map[0] */
extern int dmmax;		/* max size of swap alloc, in clicks */
#endif

/*
 * The following structure is that ``returned''
 * from a call to vstodb().
 */
struct	dblock
{
	swblk_t	db_base;	/* base of physical contig drum block */
	swblk_t	db_size;	/* size of block */
};
