/*
 * Seek sort for disks.
 *
 * actf and actl are the ends of an ordered list of buffers
 * bp is a new buffer, to be inserted in the list in a good place
 * the buffers are ordered by `cylinder' which is expected in b_resid.
 *
 * the list is two ascending lists: those greater than the head end
 * (the block being read or written now), and those less.
 * the idea is to sweep across the disk slowly doing io, then
 * seek quickly back and sweep again
 */
#include "sys/param.h"
#include "sys/buf.h"

#define	b_cylin	b_resid

disksort(actf, actl, bp)
register struct buf **actf, **actl, *bp;
{
	register struct buf *ap;
	register long cyl;

	if((ap = *actf) == NULL) {
		*actf = *actl = bp;
		bp->av_forw = NULL;
		return;
	}
	if(bp->b_cylin < ap->b_cylin) {
		while(ap->av_forw) {
			if(ap->av_forw->b_cylin < ap->b_cylin) {
				cyl = 0;
				break;
			}
			ap = ap->av_forw;
		}
	}
	else
		cyl = ap->b_cylin;
	while(ap->av_forw) {
		if(ap->av_forw->b_cylin < cyl ||
		   bp->b_cylin < ap->av_forw->b_cylin)
			break;
		ap = ap->av_forw;
		cyl = ap->b_cylin;
	}
	bp->av_forw = ap->av_forw;
	ap->av_forw = bp;
	if(ap == *actl)
		*actl = bp;
}
