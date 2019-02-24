#include "sys/param.h"
#include "sys/systm.h"
#include "sys/filsys.h"
#include "sys/fblk.h"
#include "sys/buf.h"
#include "sys/inode.h"
#include "sys/ino.h"
#include "sys/user.h"

typedef	struct fblk *FP;

/*
 * alloc will obtain the next available
 * free disk block from the free list of
 * the specified filsys.
 * The super block has up to NICFREE remembered
 * free blocks; the last of these is read to
 * obtain NICFREE more . . .
 */
static daddr_t bitfsalloc(), oldfsalloc(), bigfsalloc();

struct buf *
alloc(fip, prev)
struct inode *fip;
daddr_t prev;
{
	daddr_t bno;
	register struct filsys *fp;
	register struct buf *bp;

	fp = getfs(fip);
	while (fp->s_flock)
		sleep((caddr_t)&fp->s_flock, PINOD);
	if(BITFS(fip->i_dev))	/* unfortunately device dependent */
		if(fp->U.N.S_flag)
			bno = bigfsalloc(fp, prev);
		else
			bno = bitfsalloc(fp, prev);
	else
		bno = oldfsalloc(fp, fip->i_dev);
	if (bno == 0) {
		/*fp->s_nfree = 0;	/* but it would be wrong FIX*/
		fp->s_tfree = 0;
		fserr(fip->i_dev, fp, "file system full");
		tsleep((caddr_t)&u, PRIBIO, 5);	/* slow things down */
		u.u_error = ENOSPC;
		return (NULL);
	}
	bp = getblk(fip->i_dev, bno);
	clrbuf(bp);
	fp->s_fmod = 1;
	fp->s_tfree--;
	return (bp);
}

/*
 * bitmap fs alloc:
 * given the previous block in the file,
 * try to pick one in the same cylinder,
 * preferably in a nice rotational place
 * if we can't, pick one in the next couple of cylinders
 * if we can't, just pick any block
 */

static daddr_t
bitfsalloc(fp, prev)
register struct filsys *fp;
daddr_t prev;
{
	register daddr_t bno;
	register long i;
	daddr_t cyl, end, best;

	best = -1;
	if (fp->s_cylsize == 0) {	/* safety for the root.  ugh. */
		fp->s_cylsize = 40;
		fp->s_aspace = 4;	/* good for comets */
	}
	if (prev >= fp->s_isize) {
		bno = prev / fp->s_cylsize;
		bno *= fp->s_cylsize;
		if (bno < fp->s_isize)
			bno = fp->s_isize;
		cyl = bno + fp->s_cylsize;
		end = bno + 3 * fp->s_cylsize;
		if (end > fp->s_fsize)
			end = fp->s_fsize;
		if (cyl > fp->s_fsize)
			cyl = fp->s_fsize;
		i = bno - fp->s_isize;
		best = findbit(fp->s_bfree, i, cyl-fp->s_isize, prev-fp->s_isize,
			fp->s_aspace);	/* in same cylinder */
		if(best == -1)
			best = findbit(fp->s_bfree, cyl-fp->s_isize,
				end-fp->s_isize, 0L, 0);
	}
	if(best == -1)
		best = findbit(fp->s_bfree, 0L, fp->s_fsize-fp->s_isize, 0L, 0);
	if(best == -1)
		return(0);
	BITALLOC(fp->s_bfree, best);
	if (fp->s_valid) {
		fp->s_valid = 0;
		update();		/* ugh */
	}
	return (best+fp->s_isize);
}

/*
 * new-style, multi-segment bitmap
 * take care not to cross segment boundaries
 */

static daddr_t
bigfsalloc(fp, prev)
register struct filsys *fp;
daddr_t prev;
{
	register int fblk;
	register long bs;
	register long best, beg, cyl;
	int nblk;

	bs = fp->U.N.S_bsize;
	if(prev > 0) {
		beg = prev/fp->s_cylsize;
		beg *= fp->s_cylsize;
		if(beg < fp->s_isize)
			beg = fp->s_isize;	/* unnecessary */
		cyl = beg + fp->s_cylsize;
		if(cyl > fp->s_fsize)
			cyl = fp->s_fsize;
		fblk = beg/bs;
		if (fblk != (cyl-1)/bs)
			cyl = (fblk+1)*bs;	/* bound to this block */
		best = findbit(fp->U.N.S_blk[fblk]->b_un.b_words, beg%bs,
			cyl%bs, prev%bs, fp->s_aspace);
		if (best != -1)
			goto done;
		beg = cyl;
		cyl = beg + 2 * fp->s_cylsize;
		if (cyl >= fp->s_fsize)
			cyl = fp->s_fsize;
		fblk = beg/bs;
		if (fblk != (cyl - 1)/bs)
			cyl = (fblk+1)*bs;
		best = findbit(fp->U.N.S_blk[fblk]->b_un.b_words, beg%bs,
			cyl%bs, 0L, 0);
		if(best != -1)
			goto done;
	}
	/*
	 * find any free bit at all
	 */
	nblk = (fp->s_fsize + bs - 1) / bs;
	for (fblk = 0; fblk < nblk; fblk++) {
		best = findbit(fp->U.N.S_blk[fblk]->b_un.b_words, 0L, bs, 0L, 0);
		if(best != -1)
			goto done;
	}
	/*
	 * found nothing nowhere
	 */
	return(0);
	/*
	 * found bit best in block fblk
	 */
done:
	BITALLOC((fp->U.N.S_blk[fblk]->b_un.b_words), best);
	if(fp->s_valid) {
		fp->s_valid = 0;
		update();
	}
	return(best+fblk*bs);
}

/* find a bit between bot and top, preferably at least space from prev */
findbit(ptr, bot, top, prev, space)
long *ptr;
long bot, top, prev;
{
	register long *p;
	register int j;
	register long bno, best;

	best = -1;
	j = bot/BITCELL;
	p = ptr + j;
	bno = j*BITCELL;	/* start on long boundary */
	for(; bno < top; p++) {
		if(*p == 0) {	/* none free in whole long */
			bno += BITCELL;	/* so onward */
			continue;
		}
		for(j = 0; j < BITCELL; j++, bno++) {
			if((*p & (1<<j)) == 0)
				continue;
			if(bno < bot || bno >= top)	/* out of the loop? */
				continue;	/* first and last longs */
			if(best == -1)
				best = bno;
			if(bno - prev >= space)
				return(bno);
		}
	}
	return(best);	/* -1 or whatever we found */
}

static daddr_t
oldfsalloc(fp, dev)
register struct filsys *fp;
{
	register daddr_t bno;
	register struct buf *bp;

	do {
		if (fp->s_nfree <= 0)
			return (0);
		if (fp->s_nfree > NICFREE) {
			fserr(dev, fp, "bad free count");
			return (0);
		}
		bno = fp->s_free[--fp->s_nfree];
		if (bno == 0)
			return (0);
	} while (badblock(dev, fp, bno));
	if (fp->s_nfree <= 0) {
		fp->s_flock++;
		bp = bread(dev, bno);
		if ((bp->b_flags&B_ERROR) == 0) {
			fp->s_nfree = ((FP)(bp->b_un.b_addr))->df_nfree;
			bcopy((caddr_t)((FP)(bp->b_un.b_addr))->df_free,
			    (caddr_t)fp->s_free, sizeof(fp->s_free));
		}
		brelse(bp);
		fp->s_flock = 0;
		wakeup((caddr_t)&fp->s_flock);
		if (fp->s_nfree <= 0)
			return (0);
	}
	return (bno);
}

/*
 * place the specified disk block
 * back on the free list of the
 * specified device.
 */
free(fip, bno)
	struct inode *fip;
	daddr_t bno;
{	int bs;
	register struct filsys *fp;
	register struct buf *bp;

	fp = getfs(fip);
	fp->s_fmod = 1;
	while (fp->s_flock)
		sleep((caddr_t)&fp->s_flock, PINOD);
	if (badblock(fip->i_dev, fp, bno))
		return;
	if(BITFS(fip->i_dev)) {
	/* paranoia suggests checking it's not already free */
		if(fp->U.N.S_flag) {
			bs = fp->U.N.S_bsize;
			BITFREE((fp->U.N.S_blk[bno/bs]->b_un.b_words), bno%bs);
		}
		else {
			bno -= fp->s_isize;
			BITFREE(fp->s_bfree, bno);
		}
		if(fp->s_valid) {	/* not any more */
			fp->s_valid = 0;
			update();	/* even GROSSER */
		}
	}
	else {
		if (fp->s_nfree <= 0) {
			fp->s_nfree = 1;
			fp->s_free[0] = 0;
		}
		if (fp->s_nfree >= NICFREE) {
			fp->s_flock++;
			bp = getblk(fip->i_dev, bno);
			((FP)(bp->b_un.b_addr))->df_nfree = fp->s_nfree;
			bcopy((caddr_t)fp->s_free,
			    (caddr_t)((FP)(bp->b_un.b_addr))->df_free,
			    sizeof(fp->s_free));
			fp->s_nfree = 0;
			bwrite(bp);
			fp->s_flock = 0;
			wakeup((caddr_t)&fp->s_flock);
		}
		fp->s_free[fp->s_nfree++] = bno;
	}
	fp->s_tfree++;
	fp->s_fmod = 1;
}

/*
 * Check that a block number is in the
 * range between the I list and the size
 * of the device.
 * This is used mainly to check that a
 * garbage file system has not been mounted.
 */
badblock(dev, fp, bn)
	dev_t dev;
	register struct filsys *fp;
	daddr_t bn;
{

	if (bn < fp->s_isize || bn >= fp->s_fsize) {
		fserr(dev, fp, "bad block");
		return(1);
	}
	return(0);
}

/*
 * allocate an unused disk inode in the specified filesystem
 * up to NICINOD possible i-numbers are kept in a list
 * in s_inode; try those first.  If the list empties,
 * scan the i-list and fill it again.
 * To speed things up, pick up the scan where it last
 * left off (s_lasti) unless there are believed to be
 * many free i-nodes with lower numbers (s_nbehind).
 * s_ilock is there only to avoid having two processes
 * in the list-filling code.
 */

int dupinod;		/* debug */

struct inode *
ialloc(fip)
	struct inode *fip;
{
	register struct filsys *fp;
	register struct buf *bp;
	register struct inode *ip;
	register int i;
	struct dinode *dp;
	ino_t ino;
	int first;
	int inopb;
	daddr_t adr;

	fp = getfs(fip);
	if (fp->s_ninode > NICINOD || fp->s_ninode < 0) {
		fserr(fip->i_dev, fp, "bad inode count");
		fp->s_ninode = 0;
	}
loop:
	if (fp->s_ninode > 0) {
		ino = fp->s_inode[--fp->s_ninode];
		if (ifind(fip, ino)) {	/* already in use */
			dupinod++;	/* debug */
			goto loop;
		}
		ip = iget(fip, fip->i_dev, ino);
		if (ip == NULL)		/* probably table full */
			return(NULL);
		if (ip->i_count == 1 && fsiread(fip, ip) < 0)
			goto loop;
		if (ip->i_mode != 0) {	/* already allocated */
			iput(ip);
			goto loop;
		}
		for (i=0; i<NADDR; i++)
			ip->i_un.i_addr[i] = 0;
		fp->s_tinode--;
		fp->s_fmod = 1;
		return(ip);
	}
	/*
	 * nothing left in super-block; restock
	 */
	while (fp->s_ilock)
		sleep((caddr_t)&fp->s_ilock, PINOD);
	if (fp->s_ninode > 0)	/* someone beat us to it? */
		goto loop;
	fp->s_ilock++;
	first = 1;
	inopb = INOPB(fip->i_dev);	/* optimization */
fromtop:
	ino = fp->s_lasti;
	ino = ((ino / inopb) * inopb) + 1;	/* round down to start of block */
	adr = itod(fip->i_dev, ino);
	if (fp->s_nbehind > 4 * NICINOD
	||  adr <= SUPERB || adr >= fp->s_isize
	||  first == 0) {
		first = 0;
		ino = 1;
		adr = SUPERB + 1;
		fp->s_nbehind = 0;
	}
	for(; adr < fp->s_isize; adr++) {
		bp = bread(fip->i_dev, adr);
		if (bp->b_flags & B_ERROR) {
			brelse(bp);
			ino += inopb;
			continue;
		}
		dp = bp->b_un.b_dino;
		for (i=0; i<inopb; i++, ino++, dp++) {
			if (dp->di_mode != 0 || ifind(fip, ino))
				continue;
			fp->s_inode[fp->s_ninode++] = ino;
			if (fp->s_ninode >= NICINOD)
				break;
		}
		brelse(bp);
		if (fp->s_ninode >= NICINOD)
			break;
	}
	/*
	 * if we found nothing,
	 * try again from the beginning of the i-list
	 */
	if (fp->s_ninode <= 0 && first) {
		first = 0;
		goto fromtop;
	}
	if (fp->s_ninode == NICINOD)
		fp->s_lasti = ino;
	else {		/* hit the end, but found something */
		fp->s_lasti = 1;
		fp->s_nbehind = 0;
	}
	fp->s_ilock = 0;
	wakeup((caddr_t)&fp->s_ilock);
	if (fp->s_ninode > 0)
		goto loop;
	fserr(fip->i_dev, fp, "out of inodes");
	u.u_error = ENOSPC;
	return (NULL);
}

/*
 * Free the specified inode on the specified device.
 * The algorithm stores up to NICINOD inodes in the super
 * block and throws away any more.  It keeps track of the
 * number of inodes thrown away which preceded the current
 * search point in the file system.  This lets us rescan
 * for more inodes from the beginning only when there
 * are a reasonable number of inodes back there to reallocate.
 */

ifree(ip)
register struct inode *ip;
{
	register struct filsys *fp;

	fp = getfs(ip);
	if (fp->s_ninode > NICINOD || fp->s_ninode < 0) {
		fserr(ip->i_dev, fp, "bad inode count");
		fp->s_ninode = 0;
	}
	fp->s_tinode++;
	if (fp->s_ilock)
		return;
	if (fp->s_ninode >= NICINOD) {
		if (fp->s_lasti > ip->i_number)
			fp->s_nbehind++;
		return;
	}
	fp->s_inode[fp->s_ninode++] = ip->i_number;
	fp->s_fmod = 1;
}

/*
 * getfs maps an inode into
 * a pointer to the incore super
 * block.
 *
 * panic: no fs -- the device is not mounted.
 *	this "cannot happen"
 */
struct filsys *
getfs(fip)
register struct inode *fip;
{

	if (fip->i_fstyp != 0 || fip->i_un.i_bufp == NULL)
		panic("no fs");
	return (fip->i_un.i_bufp->b_un.b_filsys);
}

/*
 * Fserr prints the name of a file system
 * with an error diagnostic, in the form
 *	filsys: error message
 */
fserr(dev, fp, cp)
	dev_t dev;
	struct filsys *fp;
	char *cp;
{

	printf("0%o,0%o %s: %s\n", major(dev), minor(dev), fp->s_fsmnt, cp);
}

/*
 * Update is the internal name of 'sync'.  It goes through the disk
 * queues to initiate sandbagged IO; goes through the inodes to write
 * modified nodes; and it goes through the mount table to initiate modified
 * super blocks.
 */
update()
{
	register struct inode *ip;
	static int updlock;

	if (updlock)
		return;
	updlock++;
	/*
	 * Write back each (modified) inode.
	 */
	for (ip = inode; ip < inodeNINODE; ip++)
		if((ip->i_flag&ILOCK)==0 && ip->i_count) {
			ip->i_flag |= ILOCK;
			ip->i_count++;
			iupdat(ip, &time, &time, 0);
			iput(ip);
		}
	updlock = 0;
	/*
	 * Force stale buffer cache information to be flushed,
	 * for all devices.
	 */
	bflush(NODEV);
}
