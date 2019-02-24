/*
 * Definitions needed to perform bad sector
 * revectoring ala DEC STD 144.
 *
 * The bad sector information is located in the
 * first 5 even numbered sectors of the last
 * track of the disk pack.  There are five
 * identical copies of the information, described
 * by the bad144 structure.
 *
 * Replacement sectors are allocated starting with
 * the first sector before the bad sector information
 * and working backwards towards the beginning of
 * the disk.  A maximum of 126 bad sectors are supported.
 * The position of the bad sector in the bad sector table
 * determines which replacement sector it corresponds to.
 *
 * The bad sector information and replacement sectors
 * are conventionally only accessible through the
 * 'c' file system partition of the disk.  If that
 * partition is used for a file system, the user is
 * responsible for making sure that it does not overlap
 * the bad sector information or any replacement sector.s
 */

#define	NBADDESC 126

/*
 * known to be 512 bytes long
 */
struct bad144 {
	long	bt_csn;				/* cartridge serial number */
	unsigned short	bt_mbz;			/* unused; should be 0 */
	unsigned short	bt_flag;		/* -1 => alignment cartridge */
	struct bt_bad {
		unsigned short	bt_cyl;		/* cylinder number of bad sector */
		unsigned short	bt_trksec;	/* track and sector number */
	} bt_bad[126];
};

#if KERNEL
int bad144rep();
#endif
