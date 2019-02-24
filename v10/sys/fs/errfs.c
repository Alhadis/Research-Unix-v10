/*
 * error file system:
 * substituted for the original file system in inodes which
 * someone still has open but which are unwanted;
 * e.g. forcing an unmount of a file system with open files outstanding
 * the various operations are all either no-ops
 * or return errors
 * t_put is explicitly a no-op.
 * the idea is to prevent any real I/O,
 * but allow the offending inodes to hang around
 * until finally put
 */
#include "sys/param.h"
#include "sys/user.h"
#include "sys/conf.h"
#include "sys/inode.h"

int errnami();
struct fstypsw erfs = fsinit(nulldev, nulldev, nodev, nodev,
	nodev, nodev, errnami, nodev, nodev, nullopen, nodev);

errnami(p, fp, follow)
register struct nx *p;
struct flagp *fp;
int follow;
{
	u.u_error = ENODEV;
	iput(p->dp);
	p->dp = NULL;
	return (0);
}
