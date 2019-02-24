#include "sys/param.h"
#include "sys/user.h"
#include "sys/filsys.h"
#include "sys/file.h"
#include "sys/conf.h"
#include "sys/inode.h"
#include "sys/stream.h"
#include "sys/buf.h"
#include "sys/acct.h"

/*
 * Convert a user supplied
 * file descriptor into a pointer
 * to a file structure.
 * Only task is to check range
 * of the descriptor.
 */

#ifndef getf	/* done inline */

struct file *
getf(f)
register int f;
{
	register struct file *fp;

	if ((unsigned)f >= NOFILE || (fp = u.u_ofile[f]) == NULL)
		return (NULL);
	return (fp);
}

#endif

/*
 * Internal form of close.
 * Decrement reference count on
 * file structure.
 * Decrement reference count on the inode following
 * removal to the referencing file structure.
 */
closef(fp)
register struct file *fp;
{
	struct inode *ip;

	if(fp == NULL)
		return;
	if (fp->f_count > 1) {
		fp->f_count--;
		return;
	}
	ip = fp->f_inode;
	plock(ip);
	iput(ip);
	fp->f_count = 0;
}

/*
 * close the file opened on ip
 * called from iput when last reference is removed;
 * ip is locked, and i_count == 1
 *
 * free files with no links;
 * call device driver for special files
 */

extern int rootfstyp;
extern dev_t rootdev;

closei(ip)
register struct inode *ip;
{
	register dev_t dev;
	register int (*cfunc)();

	if (ip->i_sptr)
		stclose(ip, 1);
	if ((ip->i_flag & IOPEN) == 0)
		return;
	ip->i_flag &=~ IOPEN;
	if(ip->i_fstyp)		/* temporary hack */
		return;
	dev = (dev_t)ip->i_un.i_rdev;
	switch(ip->i_mode & IFMT) {

	case IFCHR:
		cfunc = cdevsw[major(dev)]->d_close;
		break;

	case IFBLK:
		if (rootfstyp == 0 && rootdev == dev)	/* awful hack: root is not open */
			return;
		bflush(dev);
		binval(dev);
		cfunc = bdevsw[major(dev)]->d_close;
		break;

	default:
		return;
	}
	(*cfunc)(dev);
}

/*
 * the default open routine for the file system switch entry `t_open'
 */
struct inode *
nullopen(ip, rw)
	struct inode *ip;
{
	return(ip);
}

/*
 * openi called to allow handler
 * of special files to initialize and
 * validate before actual IO.
 * returns the inode,
 * NULL if error.
 * may return an alternate inode,
 * in which event the original has been put.
 *
 * responsibilities of open routines (fs and stream):
 * if an inode is returned, it is unlocked,
 * and any other inode has been put.
 * if an error happens, NULL is returned and ip is put.
 */
struct inode *
openi(ip, rw)
register struct inode *ip;
{
	register dev_t dev;

	dev = (dev_t)ip->i_un.i_rdev;
	if (ip->i_sptr) 		/* stream is attached */
		ip = stopen(cdevsw[major(dev)]->qinfo, dev, rw, ip);
	else
		ip = (*fstypsw[ip->i_fstyp]->t_open)(ip, rw);
	if (ip)
		ip->i_flag |= IOPEN;
	else if (u.u_error == 0)		/* temporary firewall */
		panic("openi");
	return (ip);
}

/*
 * Check mode permission on inode pointer.
 * Mode is READ, WRITE or EXEC.
 * In the case of WRITE, the
 * read-only status of the file
 * system is checked.
 * Also in WRITE, prototype text
 * segments cannot be written.
 * The mode is shifted to select
 * the owner/group/other fields.
 * The super user is granted all
 * permissions.
 */
access(ip, mode)
register struct inode *ip;
{
	register m;
	register short *gp;

	m = mode;
	if(m == IWRITE) {
		if(ip->i_fstyp==0 && ip->i_un.i_bufp->b_un.b_filsys->s_ronly != 0) {
			u.u_error = EROFS;
			return(1);
		}
		if (ip->i_flag&ITEXT)		/* try to free text */
			xrele(ip);
		if(ip->i_flag & ITEXT) {
			u.u_error = ETXTBSY;
			return(1);
		}
	}
	if(u.u_uid == 0)
		return(0);
	if(u.u_uid != ip->i_uid) {
		m >>= 3;
		if(u.u_gid == ip->i_gid)
			goto found;
		gp = u.u_groups;
		for (; gp < &u.u_groups[NGROUPS] && *gp != NOGROUP; gp++)
			if (ip->i_gid == *gp)
				goto found;
		m >>= 3;
	}
found:
	if((ip->i_mode&m) != 0)
		return(0);

	u.u_error = EACCES;
	return(1);
}

/*
 * check inode ownership.
 * succeeds if the current user owns the file,
 * or is the super-user.
 */
accowner(ip)
register struct inode *ip;
{

	if(u.u_uid == ip->i_uid)
		return(1);
	if(suser())
		return(1);
	return(0);
}

/*
 *  Return 0 if the file isn't open for writing, 1 if it is
 */
writers(ip)
	register struct inode *ip;
{
	register struct file *fp;

	for (fp = file; fp < fileNFILE; fp++)
		if (fp->f_count!=0 && fp->f_inode==ip && (fp->f_flag&FWRITE))
			return(1);
	return(0);
}

/*
 *  Control concurrent access to a file.  The possibile types are:
 *  - 1 writer and n readers	(I1WNR)
 *  - 1 writer or n readers	(ILCKD)
 *  The file pointers are searched.  If an open with the given mode
 *  would result in a violation, errno is set to ECONC.
 */
concurrency(ip, mode)
	register struct inode *ip;
	int mode;
{
	switch(ip->i_mode&ICCTYP) {
	case ISYNC:
		if ((mode&FWRITE) && writers(ip))
			u.u_error = ECONC;
		break;
	case IEXCL:
		if ((mode&FWRITE) || writers(ip))
			u.u_error = ECONC;
		break;
	}
}

/*
 * Test if the current user is the
 * super user.
 */
suser()
{

	if(u.u_uid == 0) {
		u.u_acflag |= ASU;
		return(1);
	}
	u.u_error = EPERM;
	return(0);
}

/*
 * Allocate a user file descriptor.
 */
ufalloc()
{
	register i;

	for(i=0; i<NOFILE; i++)
		if(u.u_ofile[i] == NULL) {
			u.u_r.r_val1 = i;
			u.u_pofile[i] = 0;
			return(i);
		}
	u.u_error = EMFILE;
	return(-1);
}

struct	file *lastf;
/*
 * Allocate a user file descriptor
 * and a file structure.
 * Initialize the descriptor
 * to point at the file structure.
 */
struct file *
falloc()
{
	register struct file *fp;
	register i;

	i = ufalloc();
	if (i < 0)
		return(NULL);
	if ((fp = allocfile()) == NULL) {
		u.u_error = ENFILE;
		return(NULL);
	}
	u.u_ofile[i] = fp;
	return (fp);
}

/*
 * allocate file structure
 */
struct file *
allocfile()
{
	register struct file *fp;

	if (lastf == 0)
		lastf = file;
	for (fp = lastf; fp < fileNFILE; fp++)
		if (fp->f_count == 0)
			goto gotit;
	for (fp = file; fp < lastf; fp++)
		if (fp->f_count == 0)
			goto gotit;
	tablefull("file");
	return (NULL);
gotit:
	lastf = fp + 1;
	fp->f_count++;
	fp->f_offset = ltoL(0);
	return(fp);
}
