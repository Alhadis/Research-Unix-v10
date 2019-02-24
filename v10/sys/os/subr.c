#include "sys/param.h"
#include "sys/systm.h"
#include "sys/inode.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/proc.h"

/*
 * Bmap defines the structure of file system storage
 * by returning the physical block number on a device given the
 * inode and the logical block number in a file.
 * When convenient, it also leaves the physical
 * block number of the next block of the file in rablock
 * for use in read-ahead.
 */
daddr_t
bmap(ip, bn, rwflg)
register struct inode *ip;
daddr_t bn;
{
	register daddr_t i;
	struct buf *bp, *nbp;
	int j, sh;
	daddr_t prev;
	daddr_t nb, *bap;
	dev_t dev;

	if(bn < 0) {
		u.u_error = EFBIG;
		return((daddr_t)0);
	}
	dev = ip->i_dev;
	rablock = 0;

	/*
	 * blocks 0..NADDR-4 are direct blocks
	 */
	if(bn < NADDR-3) {
		i = bn;
		nb = ip->i_un.i_addr[i];
		if(nb == 0) {
			prev = (i > 0? ip->i_un.i_addr[i-1]: 0);
			if(rwflg==B_READ || (bp = alloc(ip, prev))==NULL)
				return((daddr_t)-1);
			nb = dbtofsb(dev, bp->b_blkno);
			if ((ip->i_mode&IFMT) == IFDIR)
				/*
				 * Write directory blocks synchronously
				 * so they never appear with garbage in
				 * them on the disk.
				 */
				bwrite(bp);
			else
				bdwrite(bp);
			ip->i_un.i_addr[i] = nb;
			ip->i_flag |= IUPD|ICHG;
		}
		if(i < NADDR-4)
			rablock = ip->i_un.i_addr[i+1];
		return(nb);
	}

	/*
	 * addresses NADDR-3, NADDR-2, and NADDR-1
	 * have single, double, triple indirect blocks.
	 * the first step is to determine
	 * how many levels of indirection.
	 */
	sh = 0;
	nb = 1;
	bn -= NADDR-3;
	for(j=3; j>0; j--) {
		sh += NSHIFT(dev);
		nb <<= NSHIFT(dev);
		if(bn < nb)
			break;
		bn -= nb;
	}
	if(j == 0) {
		u.u_error = EFBIG;
		return((daddr_t)0);
	}

	/*
	 * fetch the first indirect block
	 */
	nb = ip->i_un.i_addr[NADDR-j];
	if(nb == 0) {
		prev = ip->i_un.i_addr[NADDR-j-1];
		if(rwflg==B_READ || (bp = alloc(ip, prev))==NULL)
			return((daddr_t)-1);
		nb = dbtofsb(dev, bp->b_blkno);
		/*
		 * Write synchronously so that indirect blocks
		 * never point at garbage.
		 */
		bwrite(bp);
		ip->i_un.i_addr[NADDR-j] = nb;
		ip->i_flag |= IUPD|ICHG;
	}

	/*
	 * fetch through the indirect blocks
	 */
	for(; j<=3; j++) {
		bp = bread(dev, nb);
		if(bp->b_flags & B_ERROR) {
			brelse(bp);
			return((daddr_t)0);
		}
		bap = bp->b_un.b_daddr;
		sh -= NSHIFT(dev);
		i = (bn>>sh) & NMASK(dev);
		nb = bap[i];
		if(nb == 0) {
			prev = (i>0? bap[i-1]: 0);
			if(rwflg==B_READ || (nbp = alloc(ip, prev))==NULL) {
				brelse(bp);
				return((daddr_t)-1);
			}
			nb = dbtofsb(dev, nbp->b_blkno);
			if (j < 3 || (ip->i_mode&IFMT) == IFDIR)
				/*
				 * Write synchronously so indirect blocks
				 * never point at garbage and blocks
				 * in directories never contain garbage.
				 */
				bwrite(nbp);
			else
				bdwrite(nbp);
			bap[i] = nb;
			bdwrite(bp);
		} else
			brelse(bp);
	}

	/*
	 * calculate read-ahead.
	 */
	if(i < NINDIR(dev)-1)
		rablock = bap[i+1];
	return(nb);
}

/*
 * Pass back  c  to the user at his location u_base;
 * update u_base, u_count, and u_offset.  Return -1
 * on the last character of the user's read.
 * u_base is in the user address space unless u_segflg is set.
 */
passc(c)
register c;
{
	register id;

	if((id = u.u_segflg) == SEGSYS)
		*u.u_base = c;
	else
		if(id==SEGUINST?suibyte(u.u_base, c):subyte(u.u_base, c) < 0) {
			u.u_error = EFAULT;
			return(-1);
		}
	u.u_count--;
	u.u_offset = Lladd(u.u_offset, 1);
	u.u_base++;
	return(u.u_count == 0? -1: 0);
}

/*
 * Pick up and return the next character from the user's
 * write call at location u_base;
 * update u_base, u_count, and u_offset.  Return -1
 * when u_count is exhausted.  u_base is in the user's
 * address space unless u_segflg is set.
 */
cpass()
{
	register c, id;

	if(u.u_count == 0)
		return(-1);
	if((id = u.u_segflg) == SEGSYS)
		c = *u.u_base;
	else
		if((c = id==SEGUDATA?fubyte(u.u_base):fuibyte(u.u_base)) < 0) {
			u.u_error = EFAULT;
			return(-1);
		}
	u.u_count--;
	u.u_offset = Lladd(u.u_offset, 1);
	u.u_base++;
	return(c&0377);
}

/*
 * Routine which sets a user error; placed in
 * illegal entries in the bdevsw and cdevsw tables.
 */
nodev()
{

	u.u_error = ENODEV;
}

/*
 * Null routine; placed in insignificant entries
 * in the bdevsw and cdevsw tables.
 */
nulldev()
{

}

imin(a, b)
{

	return (a < b ? a : b);
}

imax(a, b)
{

	return (a > b ? a : b);
}

/*
 * this should probably come out eventually
 */
struct proc *
pfind(pid)
	register int pid;
{
	register struct proc *p;

	for (p = proc; p < procNPROC; p++)
		if (p->p_stat && p->p_pid == pid)
			return (p);
	return (NULL);
}

strncmp(s1, s2, len)
register char *s1, *s2;
register len;
{
	do {
		if (*s1 != *s2++)
			return(1);
		if (*s1++ == '\0')
			return(0);
	} while (--len);
	return(0);
}

strcpy(to, fr)
register char *to, *fr;
{
	while (*to++ = *fr++)
		;
}

/*
 * long-long support
 */

#define	M 0x80000000

unsigned
Lshift(ll, l)
llong_t ll;
long l;
{
	return (ll.hi<<(32-l)) | (ll.lo>>l);
}

llong_t
ltoL(l)
long l;
{
	llong_t t;

	t.hi = 0;
	t.lo = l;
	return t;
}

llong_t
Lladd(ll, l)
llong_t ll;
long l;
{
	llong_t t;
	long cin;

	t = ll;
	t.lo += l;
	cin = ll.lo^t.lo;
	if (l>=0) {
		if ((ll.lo&cin)&M)
			t.hi++;
	} else {
		if ((~ll.lo&cin)&M)
			t.hi--;
	}
	return t;
}

llong_t
Luadd(ll, u)
llong_t ll;
unsigned long u;
{
	llong_t t;
	long cin;

	t = ll;
	t.lo += u;
	cin = ll.lo^t.lo;
	if ((ll.lo&cin)&M)
		t.hi++;
	return t;
}

llong_t
LLadd(lla, llb)
llong_t lla, llb;
{
	llong_t t;

	t.hi = lla.hi+llb.hi;
	t.lo = lla.lo+llb.lo;
	if ((lla.lo&llb.lo | lla.lo&~t.lo | llb.lo&~t.lo)&M)
		t.hi++;
	return t;
}
