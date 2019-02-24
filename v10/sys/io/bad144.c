/*
 * DEC STD 144 bad block replacement
 * given a cylinder, track, sector
 * return a replacement block
 * the return is an offset:
 * lbn = (size of disk) - (size of a track) - repno
 */

#include "sys/bad144.h"

int
bad144rep(bt, cyl, trk, sec)
	register struct bad144 *bt;
{
	register int i;
	register long blk, bblk;

	blk = ((long)cyl << 16) + (trk << 8) + sec;
	for (i = 0; i < NBADDESC; i++) {
		bblk = ((long)bt->bt_bad[i].bt_cyl << 16) + bt->bt_bad[i].bt_trksec;
		if (blk == bblk)
			return (i);
		if (blk < bblk || bblk < 0)
			break;
	}
	return (-1);
}
