#include "sys/param.h"
#include "sys/inode.h"
#include "sys/user.h"
#include "sys/conf.h"
#include "sys/buf.h"

/*
 * Read the file corresponding to
 * the inode pointed at by the argument.
 * The actual read arguments are found
 * in the variables:
 *	u_base		core address for destination
 *	u_offset	byte offset in file
 *	u_count		number of bytes to read
 *	u_segflg	read to kernel/user
 */
readi(ip)
register struct inode *ip;
{

	if(u.u_count == 0)
		return;
	ip->i_flag |= IACC;
	if (ip->i_sptr) {
		u.u_nbadio = 0;	/* approximates a bad-count per stream */
		stread(ip);
		return;
	}
	(*fstypsw[ip->i_fstyp]->t_read)(ip);
}

/*
 * Write the file corresponding to
 * the inode pointed at by the argument.
 * The actual write arguments are found
 * in the variables:
 *	u_base		core address for source
 *	u_offset	byte offset in file
 *	u_count		number of bytes to write
 *	u_segflg	write to kernel/user/user I
 */

writei(ip)
register struct inode *ip;
{

	if (ip->i_sptr) {
		ip->i_flag |= IUPD|ICHG;
		stwrite(ip);
		return;
	}
	(*fstypsw[ip->i_fstyp]->t_write)(ip);
}

/*
 * Return the logical maximum
 * of the 2 arguments.
 */
unsigned
max(a, b)
unsigned a, b;
{

	if(a > b)
		return(a);
	return(b);
}

/*
 * Return the logical minimum
 * of the 2 arguments.
 */
unsigned
min(a, b)
unsigned a, b;
{

	if(a < b)
		return(a);
	return(b);
}

/*
 * Move n bytes at byte location
 * &bp->b_un.b_addr[o] to/from (flag) the
 * user/kernel (u.segflg) area starting at u.base.
 * Update all the arguments by the number
 * of bytes moved.
 *
 * Botchy VAX-ism: believes user I and user D are the same
 */
iomove(cp, n, flag)
	register caddr_t cp;
	register unsigned n;
{
	register int t;

	if (n==0)
		return;
	if (u.u_segflg != SEGSYS) {
		if (flag==B_WRITE)
			t = copyin(u.u_base, (caddr_t)cp, n);
		else
			t = copyout((caddr_t)cp, u.u_base, n);
		if (t) {
			u.u_error = EFAULT;
			return;
		}
	} else
		if (flag == B_WRITE)
			bcopy(u.u_base, (caddr_t)cp, n);
		else
			bcopy((caddr_t)cp, u.u_base, n);
	u.u_base += n;
	u.u_offset = Lladd(u.u_offset, n);
	u.u_count -= n;
}
