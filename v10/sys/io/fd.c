/*
 * file descriptor driver
 * open(minor n) -> dup of minor n
 */

#include "sys/param.h"
#include "sys/file.h"
#include "sys/user.h"
#include "sys/conf.h"

int fdopen();

struct cdevsw fdcdev = cdinit(fdopen, nodev, nodev, nodev, nodev);

fdopen(dev, flag)
{
	register struct file *fp;
	register int ofd;

	ofd = u.u_r.r_val1;		/* magic knowledge */
	if (minor(dev) == ofd || (fp = getf(minor(dev))) == NULL) {
		u.u_error = EBADF;
		return;
	}
	/* now for the naughty part */
	if ((unsigned)ofd >= NOFILE || u.u_ofile[ofd] == NULL)
		panic("fdopen");
	closef(u.u_ofile[ofd]);
	u.u_ofile[ofd] = fp;
	fp->f_count++;
}
