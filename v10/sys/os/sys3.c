/*	sys3.c	4.9	81/03/08	*/

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/buf.h"
#include "sys/user.h"
#include "sys/inode.h"
#include "sys/file.h"
#include "sys/conf.h"
#include "sys/stat.h"

/*
 * the fstat system call.
 */
fstat()
{
	register struct file *fp;
	register struct a {
		int	fdes;
		struct stat *sb;
	} *uap;
	register struct inode *ip;

	uap = (struct a *)u.u_ap;
	fp = getf(uap->fdes);
	if(fp == NULL) {
		u.u_error = EBADF;
		return;
	}
	ip = fp->f_inode;
	plock(ip);
	iupdat(ip, &time, &time, 0);
	(*fstypsw[ip->i_fstyp]->t_stat)(ip, uap->sb);
	prele(ip);
}

/*
 * the stat system call.  This version follows links.
 */
stat()
{
	register struct inode *ip;
	register struct a {
		char	*fname;
		struct stat *sb;
	} *uap;

	uap = (struct a *)u.u_ap;
	ip = namei(uap->fname, SEGUDATA, &nilargnamei, 1);
	if(ip == NULL)
		return;
	iupdat(ip, &time, &time, 0);
	(*fstypsw[ip->i_fstyp]->t_stat)(ip, uap->sb);
	iput(ip);
}

/*
 * the lstat system call.  This version does not follow links.
 */
lstat()
{
	register struct inode *ip;
	register struct a {
		char	*fname;
		struct stat *sb;
	} *uap;

	uap = (struct a *)u.u_ap;
	ip = namei(uap->fname, SEGUDATA, &nilargnamei, 0);
	if(ip == NULL)
		return;
	iupdat(ip, &time, &time, 0);
	(*fstypsw[ip->i_fstyp]->t_stat)(ip, uap->sb);
	iput(ip);
}

/*
 *  readlink -- return target name of a symbolic link
 */
readlink()
{
	register struct inode *ip;
	register struct a {
		char	*name;
		char	*buf;
		int	count;
	} *uap;

	uap = (struct a *)u.u_ap;
	ip = namei(uap->name, SEGUDATA, &nilargnamei, 0);
	if (ip == NULL)
		return;
	if ((ip->i_mode&IFMT) != IFLNK) {
		u.u_error = ENXIO;
		goto out;
	}
	u.u_offset = ltoL(0);
	u.u_base = uap->buf;
	u.u_count = uap->count;
	u.u_segflg = SEGUDATA;
	readi(ip);
out:
	iput(ip);
	u.u_r.r_val1 = uap->count - u.u_count;
}

/*
 * symlink -- make a symbolic link
 */
symlink()
{
	register struct a {
		char	*target;
		char	*linkname;
	} *uap;
	register struct inode *ip;
	register nc;
	struct argnamei nmarg;

	uap = (struct a *)u.u_ap;
	if ((nc = fustrlen(uap->target)) < 0) {
		u.u_error = EFAULT;
		return;
	}
	nc--;		/* fustrlen includes NUL at end */
	nmarg = nilargnamei;
	nmarg.flag = NI_NXCREAT;
	nmarg.un.mode = IFLNK|0777;
	ip = namei(uap->linkname, SEGUDATA, &nmarg, 0);
	if (ip == NULL)
		return;
	u.u_base = uap->target;
	u.u_count = nc;
	u.u_offset = ltoL(0);
	u.u_segflg = SEGUDATA;
	writei(ip);
	iput(ip);
}

/*
 * the dup system call.
 */
dup()
{
	register struct file *fp;
	register struct a {
		int	fdes;
		int	fdes2;
	} *uap;
	register i, m;

	uap = (struct a *)u.u_ap;
	m = uap->fdes & ~077;
	uap->fdes &= 077;
	fp = getf(uap->fdes);
	if(fp == NULL) {
		u.u_error = EBADF;
		return;
	}
	if ((m&0100) == 0) {
		if ((i = ufalloc()) < 0)
			return;
	} else {
		i = uap->fdes2;
		if (i<0 || i>=NOFILE) {
			u.u_error = EBADF;
			return;
		}
		u.u_r.r_val1 = i;
	}
	if (i!=uap->fdes) {
		if (u.u_ofile[i]!=NULL)
			closef(u.u_ofile[i]);
		u.u_ofile[i] = fp;
		u.u_pofile[i] = 0;
		fp->f_count++;
	}
}

/* dirread(fd, buf, buflen) */
dirread()
{	struct a {
		int fdes;
		char *buf;
		int len;
	} *uap;
	register struct file *fp;
	register struct inode *ip;
	
	uap = (struct a *)u.u_ap;
	fp = getf(uap->fdes);
	if(fp == NULL) {
		u.u_error = EBADF;
		return;
	}
	if((fp->f_flag&FREAD) == 0) {
		u.u_error = EBADF;
		return;
	}
	u.u_offset = fp->f_offset;
	u.u_base = uap->buf;
	ip = fp->f_inode;
	plock(ip);
	ip->i_flag |= IACC;
	(*fstypsw[ip->i_fstyp]->t_dirread)(ip, uap->len);
	fp->f_offset = u.u_offset;
	prele(ip);
}
