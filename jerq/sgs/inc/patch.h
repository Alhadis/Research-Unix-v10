/*
 * static char ID_patch[] = "@(#) patch.h: 1.3 7/23/82";
 */

struct plist
{
	long blk_cnt;			/* the number of blocks */
	union pentry
	{
		struct
		{
			long		blk_addr;	/* block address    */
			long		blk_size;	/* size of block    */
			unsigned short	blk_scnum;	/* section of block */
			char		blk_type;	/* type of block    */
			char		blk_pad;	/* padding	    */
		} type01;				/* FREE or OLD FCN  */
		struct
		{
			long		blk_addr;	/* block address    */
			long		blk_ndx;	/* fcn tv index     */
			unsigned short	blk_scnum;	/* section of block */
			char		blk_type;	/* type of block    */
			char		blk_size;	/* size of block    */
		} type2;				/* DECF		    */
	} block[1];
};

#define PLIST  struct plist
#define PENTRY union pentry

#define PLSIZE sizeof(PLIST)
#define PESIZE sizeof(PENTRY)

/*
 * There are three types of patch list items:
 *
 *	type0	: a block of FREE, unallocated patch space. This space
 *			can be used for overwrites
 *	type1	: a block of text space belonging to a function which
 *			has been replaced. The OLD Function can still
 *			be active
 *	type2	: a DECision Function, currently in use
 */

#define FREE_patch	0
#define OLDF_patch	1
#define DECF_patch	2

/*
 * Specify the number of bytes to leave at the start of any replaced
 * function, when tv are not being used
 *
 * This space holds the explicit branch instruction - a "pseudo tv" -
 * and must be an integral number of words, to insure proper function
 * alignment
 */

#define BRANCH	8
