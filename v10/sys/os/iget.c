#include "sys/param.h"
#include "sys/user.h"
#include "sys/inode.h"
#include "sys/conf.h"

#define	INOHSZ	63
/* INOHASH can't depend on fstype as long as unmount changes fstyp to err-fs */
#define	INOHASH(dev, ino)	(((dev)+(ino))%INOHSZ)
struct inode *inohash[INOHSZ];
struct inode *ifreel;

/*
 * Initialize hash links for inodes
 * and build inode free list.
 */
ihinit()
{
	register int i;
	register struct inode *ip = inode;

	ifreel = inode;
	for (i = 0; i < inodecnt-1; i++, ip++)
		ip->i_hlink = ip + 1;
	ip->i_hlink = NULL;
	for (i = 0; i < INOHSZ; i++)
		inohash[i] = NULL;
}

/*
 * Find an inode if it is incore.
 */
struct inode *
ifind(hp, ino)
register struct inode *hp;
register ino_t ino;
{
	register struct inode *ip;

	for (ip = inohash[INOHASH(hp->i_dev,ino)]; ip; ip = ip->i_hlink)
		if (ino==ip->i_number && hp->i_dev==ip->i_dev && hp->i_fstyp==ip->i_fstyp)
			return (ip);
	return ((struct inode *)0);
}

/*
 * see if there are any inodes save the given one
 * in the same filesystem
 * used by unmount routines to decide whether fs is busy
 * try to unmount anything mounted atop this fs
 * -- botch: deeply nested filesystems may run the system
 * out of stack and cause a crash
 */

int
ifsbusy(rip)
register struct inode *rip;
{
	register struct inode *ip;

	for (ip = inode; ip < inodeNINODE; ip++) {
		if (ip->i_count == 0)
			continue;
		if (ip->i_mpoint != rip->i_mpoint)
			continue;
		if (ip->i_mroot) {	/* something mounted here? */
			ip->i_count++;
			(*fstypsw[ip->i_mroot->i_fstyp]->t_mount)((struct inode *)NULL,
				ip, 0, 0, ip->i_mroot->i_fstyp);
			if (ip->i_count == 1) {	/* just our reference left? */
				idec(ip);
				continue;
			}
			idec(ip);
		}
		if (ip != rip)
			return (1);
	}
	return (0);
}

/*
 * mark inaccessible all i-nodes in this filesystem
 * used for forced unmounting
 * any further IO through those inodes will produce an error
 * -- it's annoying that ERRFS must be manifest
 */

#define	ERRFS	5
ifsinacc(rip)
register struct inode *rip;
{
	register struct inode *ip;

	rip = rip->i_mpoint;
	for (ip = inode; ip < inodeNINODE; ip++) {
		if (ip->i_count == 0)
			continue;
		if (ip->i_mpoint != rip)
			continue;
		ip->i_fstyp = ERRFS;	/* yes, always */
		ip->i_mpoint = rootdir;		/* hmm */
		if (ip->i_mroot)
			(*fstypsw[ip->i_mroot->i_fstyp]->t_mount)((struct inode *)NULL,
				ip, 0, 0, ip->i_mroot->i_fstyp);
	}
}

#ifndef plock		/* done inline */
/*
 * Lock an inode
 * If its already locked,
 * set the WANT bit and sleep.
 */
plock(ip)
register struct inode *ip;
{

	while(ip->i_flag&ILOCK) {
		ip->i_flag |= IWANT;
		sleep((caddr_t)ip, PINOD);
	}
	ip->i_flag |= ILOCK;
}

/*
 * Unlock an inode.
 * If WANT bit is on,
 * wakeup.
 */
prele(ip)
register struct inode *ip;
{
	ip->i_flag &= ~ILOCK;
	if(ip->i_flag&IWANT) {
		ip->i_flag &= ~IWANT;
		wakeup((caddr_t)ip);
	}
}
#endif


/*
 * Look up an inode by filsys, i-number.
 * filsys is denoted by some inode in that filesystem.
 * If the inode is mounted on, perform
 * the indicated indirection.
 * If it is in core (in the inode structure),
 * honor the locking protocol.
 * If it is not in core, fill in the fields that
 * signify the inode`s `name' (number, dev, filsys, fstyp)
 * and leave the rest blank, to be filled in by the caller.
 * A `blank' inode has i_count == 1.
 * In all cases, a pointer to a locked
 * inode structure is returned.
 */
struct inode *
iget(fip, dev, ino)
register struct inode *fip;
dev_t dev;
register ino_t ino;
{
	register struct inode *ip;
	register int slot;

loop:
	slot = INOHASH(dev, ino);
	for (ip = inohash[slot]; ip; ip = ip->i_hlink) {
		if(ino == ip->i_number && dev == ip->i_dev
			&& fip->i_fstyp == ip->i_fstyp) {
mloop:
			if((ip->i_flag&ILOCK) != 0) {
				ip->i_flag |= IWANT;
				sleep((caddr_t)ip, PINOD);
				goto loop;
			}
			if(ip->i_mroot != NULL) {
				ip = ip->i_mroot;
				goto mloop;
			}
			ip->i_count++;
			ip->i_flag |= ILOCK;
			return(ip);
		}
	}
	if(ifreel == NULL) {
		tablefull("inode");
		u.u_error = ENFILE;
		return(NULL);
	}
	ip = ifreel;
	ifreel = ip->i_hlink;
	ip->i_hlink = inohash[slot];
	inohash[slot] = ip;
	ip->i_dev = dev;
	ip->i_fstyp = fip->i_fstyp;
	ip->i_number = ino;
	ip->i_flag = ILOCK;
	ip->i_count = 1;
	ip->i_sptr = NULL;
	ip->i_mroot = NULL;
	ip->i_mpoint = fip->i_mpoint;
	return (ip);
}

/*
 * Decrement reference count of
 * an inode structure.
 * On the last reference,
 * call closei to close the file.
 * Don't unlock unless we were the only reference.
 */
idec(ip)
register struct inode *ip;
{
	register int i;
	register struct inode *jp;

	if (ip->i_count == 0)
		panic("i_count==0");
	if (ip->i_count == 1) {
		ip->i_flag |= ILOCK;
		closei(ip);
		(*fstypsw[ip->i_fstyp]->t_put)(ip);
		i = INOHASH(ip->i_dev, ip->i_number);
		if (inohash[i] == ip)
			inohash[i] = ip->i_hlink;
		else {
			for (jp = inohash[i]; jp; jp = jp->i_hlink)
				if (jp->i_hlink == ip) {
					jp->i_hlink = ip->i_hlink;
					goto done;
				}
			panic("idec");
		}
done:
		prele(ip);
		ip->i_flag = 0;
		ip->i_number = 0;
		ip->i_hlink = ifreel;
		ifreel = ip;
	}
	ip->i_count--;
}

/*
 * decrement the count for a locked inode.
 * unlock.
 * perhaps this should be a macro later
 */

iput(ip)
register struct inode *ip;
{
#if NOTDEF
	if ((ip->i_flag & ILOCK) == 0)
		panic("iput");
#endif
	if (ip->i_count == 0)
		panic("i_count==0");
	if (ip->i_count == 1)
		idec(ip);	/* will take care of lock too */
	else {
		ip->i_count--;
		prele(ip);
	}
}

/*
 * Check accessed and update flags on
 * an inode structure.
 * If any is on, update the inode
 * with the current time.
 * If waitfor is given, then must insure
 * i/o order so wait for write to complete.
 */
iupdat(ip, ta, tm, waitfor)
register struct inode *ip;
time_t *ta, *tm;
int waitfor;
{

	if((ip->i_flag&(IUPD|IACC|ICHG)) != 0)
		(*fstypsw[ip->i_fstyp]->t_updat)(ip, ta, tm, waitfor);
}

/*
 * allocate an in-core-only inode with no filesystem counterpart
 * used for pipes and similar things
 * a locked inode is returned, filled in and useable,
 * but not a member of any filesystem
 */
struct inode *
iuniq(fstyp)
	int fstyp;
{
	struct inode *ip;
	static ino_t ino=0;
	ino_t inostart;
	struct inode pi;		/* primer */

	pi.i_dev = NODEV;
	pi.i_fstyp = fstyp;
	pi.i_mpoint = NULL;
	for (inostart = ino++; ino != inostart; ino++) {
		if (ifind(&pi, ino))	/* already in use */
			continue;
		if ((ip = iget(&pi, NODEV, ino)) == NULL)
			return (NULL);	/* no inodes, or something */
		if (ip->i_count != 1) {	/* already in use -- safety */
			iput(ip);
			continue;
		}
		ip->i_mode = IFREG | (0666 & ~u.u_cmask);
		ip->i_uid = u.u_uid;
		ip->i_gid = u.u_gid;
		ip->i_nlink = 0;
		return(ip);
	}
	tablefull("fake inode");
	u.u_error = ENFILE;
	return(NULL);
}
