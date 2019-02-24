#include "sys/param.h"
#include "sys/inode.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/conf.h"

/*
 * default namei flagp argument
 * use as is for simple searches;
 * copy and fill in the blanks for more complicated things
 */
struct argnamei nilargnamei = {NI_SEARCH};

/*
 * Convert a pathname into a pointer to
 * a locked inode.
 *
 * str = address of pathname; might be user or kernel space address
 * seg = segment where str lives; save values as u_segflg
 * flagp = address of structure with miscellaneous parameters
 * follow = 1 if links are to be followed at the end of the name
 *
 * Most of the work is done in the different filesystem namei routines,
 * which are called with a structure full of value-result parameters
 * (in effect a bundle of global variables on the stack):
 *	dp, the current inode
 *	cp, the current character pointer
 *	nlink, the current symlink count (to catch loops)
 *	nbuf and nlen, the address and length of the buffer
 *		where the pathname is stored
 * If the fs namei routine returns 1, there is more work to do;
 * call the namei routine for the current dp->i_fstyp
 * (perhaps different from the one that returned)
 * If the routine returns 0, all is done;
 * p.dp contains the inode to return, which may be NULL
 * u.u_error may contain an error code
 */

struct inode *
namei(str, seg, flagp, follow)
char *str;
int seg;
struct argnamei	*flagp;
int follow;
{
	register char	*cp;
	struct nx	p;
	register struct buf *bp;

	bp = geteblk();
	p.nbuf = bp->b_un.b_addr;
	p.nlen = bp->b_bcount;
	p.dp = NULL;		/* in case of error */
	switch (seg) {
	case SEGSYS:
		/* assume it fits in the buffer for now */
		strcpy(p.nbuf, str);
		break;

	case SEGUDATA:
		if (fustrncpy(p.nbuf, str, p.nlen) < 0) {
			u.u_error = EFAULT;
			goto out;
		}
		break;

	default:
		u.u_error = EFAULT;
		goto out;
	}
	cp = p.nbuf;
	if(*cp == '/') {
		while(*cp == '/')
			cp++;
		if((p.dp = u.u_rdir) == NULL)
			p.dp = rootdir;
	} else
		p.dp = u.u_cdir;
	p.nlink = 0;
	p.cp = cp;
	plock(p.dp);
	p.dp->i_count++;
	while ((*fstypsw[p.dp->i_fstyp]->t_nami)(&p, flagp, follow))
		;
out:
	brelse(bp);
	return (p.dp);
}

/*
 * for filesystems without namei code
 * allow ordinary lookups;
 * pretend create worked (hence open for write)
 * reject anything else
 */

int
nullnami(p, flagp, follow)
register struct nx *p;
register struct argnamei *flagp;
int follow;
{

	switch (flagp->flag) {
	case NI_CREAT:
		flagp->un.mode = ~flagp->un.mode;	/* magic */
		/* fall through */
	case NI_SEARCH:
		return (0);

	default:
		u.u_error = EACCES;
		iput(p->dp);
		p->dp = NULL;
		return (0);
	}
}
