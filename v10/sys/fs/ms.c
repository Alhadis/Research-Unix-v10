/*
 *	This file contains the routines which support mounted
 *	streams (file system type 3).
 */
#include "sys/param.h"
#include "sys/systm.h"
#include "sys/inode.h"
#include "sys/stream.h"
#include "sys/user.h"
#include "sys/file.h"
#include "sys/stat.h"
#include "sys/conf.h"

int msupdat(), msread(), mswrite(), msstat();
int msmount(), msioctl();
struct inode *msopen();
struct fstypsw msfs =
fsinit(nulldev, msupdat, msread, mswrite, nulldev, msstat,
	nullnami, msmount, msioctl, msopen, nodev);

/*
 *  The following definitions apply to all comments:
 *	- `locked' means that an inode has had `plock' applied to it
 *	- `reserved' means that the inode's i_count has been incremented to
 *	  ensure that the inode will be freed during an operation.
 */

/*
 *  To ensure consistency
 */
#define SETMOUNT(d,r) d->i_mroot = r
#define CLRMOUNT(d) d->i_mroot = NULL

msmount(cip, dip, flag, mnt, fstyp)
struct inode *cip, *dip;
{

	if(accowner(dip) == 0)	/* only owner can mount */
		return;		/* errno set by accowner */

	if (mnt) {
		/*
		 * from fmount(), `dip' is locked and reserved
		 */
		mson(cip, dip, flag, fstyp);
		/*
		 * iput occurs in fmount();
		 */
	} else {
		/*
		 * from fmount(), nothing locked or reserved
		 */
		msoff(dip, 0);
	}
}

/*
 *  Create an in core root node, mount it, and attach a communications inode.
 *  On entry, `dip' is locked and reserved.  On exit, `dip' is still locked
 *  and reserved (i.e. shoud be iput).
 */
mson(cip, dip, flag, fstyp)
	register struct inode *cip;	/* stream being mounted */
	register struct inode *dip;	/* mount point */
{
	register struct inode *rip;	/* root inode for new fs */

	/* must be mounting a stream */
	if(cip->i_sptr == NULL) {
		u.u_error = ENXIO;
		return;
	}

	/* already mounted? */
	if(dip->i_fstyp == fstyp) {
		u.u_error = EBUSY;		/* in use */
		return;
	}

	/* create a new inode for the fs root */
	rip = iuniq(fstyp);
	if(rip == NULL)
		return;		/* errno already set */
	rip->i_un.i_cip = cip;
	rip->i_mpoint = dip;
	rip->i_mode &= IFMT;
	rip->i_mode |= dip->i_mode & ~IFMT;
	rip->i_flag |= IACC;	/* force update */
	prele(rip);
	cip->i_count++;

	/* mount the root */
	SETMOUNT(dip,rip);
	dip->i_count++;
}

/*
 *  Unmount the stream and close it if approriate.
 *  On entry, `dip' is neither reserved or locked.
 */
msoff(dip, rlocked)
	register struct inode *dip;	/* mount point */
{
	register struct inode *rip;	/* root inode */
	register struct inode *cip;	/* mounted stream */

	/* if not mounted, we're done */
	if (dip->i_mroot==NULL)
		return;

	/* 
	 *  To avoid deadlock, always lock the resources in the same order.
	 *  Since the root may already be locked on the way in, it comes
	 *  first.
	 */
	rip = dip->i_mroot;
	if (!rlocked)
		plock(rip);
	plock(dip);

	/* make sure nothing changed while we were locking the inodes */
	if (dip->i_mroot!=rip) {
		prele(dip);
		if (!rlocked)
			prele(rip);
		return;
	}

	/* disassociate the stream from the mount point */
	cip = rip->i_un.i_cip;
	rip->i_un.i_cip = NULL;
	if(cip != NULL) {
		if(cip->i_count==1)
			stclose(cip, 1);
		iput(cip);
	}

	/* unmount root from file system */
	rip->i_mpoint = NULL;
	CLRMOUNT(dip);
	iput(dip);
	iput(rip);
}

/*
 * If the node is still mounted and the stream has hung up,
 * unmount it.  On entry, rip is locked.
 * This is called by `update()'.
 *
 * This is called because IACC is set in rip in mson above.
 * update routines are meant to clear IACC|IUPD|ICHG, but
 * we don't, so the calls keep coming.
 *
 * stat and open should probably check the stream too.
 */
msupdat(rip, ta, tm, waitfor)
	register struct inode *rip;
	time_t *ta, *tm;
{
	register struct inode *cip;
	register struct inode *dip;

	cip = rip->i_un.i_cip;
	if (cip==NULL || cip->i_sptr==NULL || cip->i_sptr->flag&HUNGUP) {
		dip = rip->i_mpoint;
		if (dip != NULL) {
			if (!(rip->i_flag&ILOCK))
				panic("msupdate: inode not locked");
			msoff(dip, 1);
		}
	}
}

/*
 *  Pass read onto the stream's inode.
 *  `rip' is locked during this operation.
 */
msread(rip)
	struct inode *rip;
{
	register struct inode *cip;

	cip = rip->i_un.i_cip;
	if (cip!=NULL)
		readi(cip);
	else
		u.u_error = EPIPE;
}

/*
 *  Pass write onto the stream's inode.
 *  `rip' is locked during this operation.
 */
mswrite(rip)
	struct inode *rip;
{
	register struct inode *cip;

	cip = rip->i_un.i_cip;
	if (cip!=NULL)
		writei(cip);
	else
		u.u_error = EPIPE;
}

/*
 *  Pass ioctl onto the stream's inode.
 *  `rip' is locked during this operation.
 */
msioctl(rip, cmd, cmarg, flag)
register struct inode *rip;
caddr_t cmarg;
{
	register struct inode *cip;

	cip = rip->i_un.i_cip;
	if (cip!=NULL)
		stioctl(cip, cmd, cmarg);
	else
		u.u_error = EPIPE;
}

/*
 * stat `mounted' inode
 */
msstat(rip, ub)
	struct inode *rip;
	struct stat *ub;
{
	struct stat ds;

	ds.st_dev = rip->i_dev;
	ds.st_ino = rip->i_number;
	ds.st_mode = rip->i_mode;
	ds.st_nlink = 0;
	ds.st_uid = rip->i_uid;
	ds.st_gid = rip->i_gid;
	ds.st_rdev = (dev_t)0;
	ds.st_size = 0;
	ds.st_atime = time;
	ds.st_mtime = time;
	ds.st_ctime = time;
	if (copyout((caddr_t)&ds, (caddr_t)ub, sizeof(ds)) < 0)
		u.u_error = EFAULT;
}

/*
 *  Change an open of the root into a stopen of the communications inode.
 */
struct inode *
msopen(rip, rw)
register struct inode *rip;
{
	register struct inode *cip;
	register struct inode *nip;

	/*
	 *  Locking the root also locks out cip changes.
	 */
	plock(rip);

	/* return error if stream is dead */
	cip = rip->i_un.i_cip;
	if (cip==NULL||cip->i_sptr==NULL||cip->i_sptr->flag&HUNGUP) {
		u.u_error = ENXIO;
		iput(rip);
		return(NULL);
	}

	/*
	 *  Reopen stream, perhaps returning a fresh inode,
	 *  or NULL on error, which is what we should do too.
	 *  either this is a fresh reference to cip, or stopen will iput.
	 */
	cip->i_count++;
	nip = stopen((struct streamtab *)NULL, cip->i_un.i_rdev, rw, cip);
	iput(rip);
	return(nip);
}
