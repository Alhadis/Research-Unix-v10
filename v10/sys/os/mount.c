/*
 * code related to mounting and unmounting filesystems
 */
#include "sys/param.h"
#include "sys/user.h"
#include "sys/inode.h"
#include "sys/file.h"
#include "sys/conf.h"

/*
 * sys fmount
 * call the filesystem-specific mount routine with
 *	the inode of the device to be mounted
 *	the inode of the mount point, still locked
 *	the flag argument
 */

fmount()
{
	register struct a {
		int fstype;
		int fd;
		char *name;
		int flag;
	} *uap = (struct a *)u.u_ap;
	struct file *fp;
	struct inode *ip;

	if (uap->fstype < 0 || uap->fstype >= nfstyp
	||  fstypsw[uap->fstype] == NULL) {
		u.u_error = EINVAL;
		return;
	}
	if ((fp = getf(uap->fd)) == NULL) {
		u.u_error = EBADF;
		return;
	}
	if ((ip = namei(uap->name, SEGUDATA, &nilargnamei, 1)) == NULL)
		return;
	(*fstypsw[uap->fstype]->t_mount)(fp->f_inode, ip, uap->flag, 1, uap->fstype);
	iput(ip);
}

/*
 * sys funmount
 * call the fs-particular unmount routine with
 * the inode of the mount point
 */

funmount()
{
	struct a {
		char *name;
	} *uap = (struct a *)u.u_ap;
	struct inode *ip, *mip;

	if ((ip = namei(uap->name, SEGUDATA, &nilargnamei, 1)) == NULL)
		return;
	if ((mip = ip->i_mpoint) == NULL
	||  mip->i_mroot != ip) {	/* sanity, also catches root */
		iput(ip);
		u.u_error = EINVAL;
		return;
	}
	iput(ip);	/* bag this? */
	(*fstypsw[ip->i_fstyp]->t_mount)((struct inode *)NULL, mip, 0, 0, ip->i_fstyp);
}
