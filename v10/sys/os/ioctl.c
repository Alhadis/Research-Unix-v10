#include "sys/param.h"
#include "sys/user.h"
#include "sys/inode.h"
#include "sys/file.h"
#include "sys/conf.h"
#include "sys/filio.h"

/*
 * ioctl system call
 * Check legality, execute common code, and switch out to individual
 * device routine.
 */
ioctl()
{
	register struct file *fp;
	register struct inode *ip;
	register struct a {
		int	fdes;
		int	cmd;
		caddr_t	cmarg;
	} *uap;

	uap = (struct a *)u.u_ap;
	if ((fp = getf(uap->fdes)) == NULL
	||  (fp->f_flag & (FREAD|FWRITE)) == 0) {
		u.u_error = EBADF;
		return;
	}
	ip = fp->f_inode;
	switch (uap->cmd) {

	case FIOCLEX:			/* close on exec */
		u.u_pofile[uap->fdes] |= EXCLOSE;
		return;

	case FIONCLEX:			/* no close on exec */
		u.u_pofile[uap->fdes] &= ~EXCLOSE;
		return;

	}
	if (ip->i_sptr) {
		stioctl(ip, uap->cmd, uap->cmarg);
		return;
	}
	(*fstypsw[ip->i_fstyp]->t_ioctl)(ip, uap->cmd, uap->cmarg, fp->f_flag);
}
