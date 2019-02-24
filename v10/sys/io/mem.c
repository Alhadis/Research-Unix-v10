/*
 * Memory special file
 *	minor device 0 is physical memory
 *	minor device 1 is kernel memory 
 *	minor device 2 is EOF/RATHOLE
 *	minor device 3 is unibus memory (addressed by shorts)
 *	minor device 4 is obsolete
 *	minor device 5 is processor registers
 */

#include "sys/param.h"
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/conf.h"
#include "sys/pte.h"
#include "sys/mtpr.h"
#include "sys/vm.h"

int mmread(), mmwrite();
struct cdevsw mmcdev = cdinit(nulldev, nulldev, mmread, mmwrite, nodev);

char *vmmap;
struct pte *mmap;

mmread(dev)
{
	register int o; long lbuf;
	register unsigned c, v;

	switch (minor(dev)) {

	case 0:
		while (u.u_count != 0 && u.u_error == 0) {
			if (fubyte(u.u_base) == -1)
				goto fault;
			v = btop(Ltol(u.u_offset));
			*(int *)mmap = v | (PG_V | PG_KR);
			mtpr(TBIS, vmmap);
			o = Ltol(u.u_offset) & PGOFSET;
			c = min((unsigned)(NBPG - o), u.u_count);
			c = min(c, (unsigned)(NBPG - ((int)u.u_base&PGOFSET)));
			if (copyout((caddr_t)&vmmap[o], u.u_base, c))
				goto fault;
			u.u_count -= c;
			u.u_base += c;
			u.u_offset = Lladd(u.u_offset, c);
		}
		return;

	case 1:
		iomove((caddr_t)Ltol(u.u_offset), u.u_count, B_READ);
		return;

	case 2:
		return;

	case 3:
		if (!useracc(u.u_base, u.u_count, B_WRITE))
			goto fault;
		if (UNIcpy((caddr_t)Ltol(u.u_offset), u.u_base, u.u_count, B_READ))
			goto fault;
		c = u.u_count;
		u.u_count = 0;
		u.u_base += c;
		u.u_offset = Lladd(u.u_offset, c);
		return;

	case 5:
		c = min(u.u_count, sizeof(long));
		lbuf = umfpr(Ltol(u.u_offset)/sizeof(long));
		if (copyout((caddr_t)&lbuf, u.u_base, c))
			goto fault;
		u.u_count -= c;
		u.u_base += c;
		u.u_offset = Lladd(u.u_offset, c);
		return;
	}
fault:
	u.u_error = EFAULT;
	return;
}

mmwrite(dev)
{
	register int o; long lbuf;
	register unsigned c, v;

	switch (minor(dev)) {

	case 0:
		while (u.u_count != 0 && u.u_error == 0) {
			if (fubyte(u.u_base) == -1)
				goto fault;
			v = btop(Ltol(u.u_offset));
			*(int *)mmap = v | (PG_V | PG_KW);
			mtpr(TBIS, vmmap);
			o = Ltol(u.u_offset) & PGOFSET;
			c = min((unsigned)(NBPG - o), u.u_count);
			c = min(c, (unsigned)(NBPG - ((int)u.u_base&PGOFSET)));
			if (copyin(u.u_base, (caddr_t)&vmmap[o], c))
				goto fault;
			u.u_count -= c;
			u.u_base += c;
			u.u_offset = Lladd(u.u_offset, c);
		}
		return;

	case 1:
		iomove((caddr_t)Ltol(u.u_offset), u.u_count, B_WRITE);
		return;

	case 2:
		u.u_offset = Lladd(u.u_offset, u.u_count);
		u.u_count = 0;
		return;

	case 3:
		if (!useracc(u.u_base, u.u_count, B_READ))
			goto fault;
		if (UNIcpy((caddr_t)Ltol(u.u_offset), u.u_base, u.u_count, B_WRITE))
			goto fault;
		u.u_base += u.u_count;
		u.u_offset = Lladd(u.u_offset, u.u_count);
		u.u_count = 0;
		return;

	case 5:
		if (u.u_count < sizeof(long))
			goto fault;
		if (copyin(u.u_base, (caddr_t)&lbuf, sizeof(long)))
			goto fault;
		if (umtpr(Ltol(u.u_offset) / sizeof(long), lbuf) == 0)
			goto fault;
		u.u_count -= sizeof(long);
		u.u_base += sizeof(long);
		u.u_offset = Lladd(u.u_offset, sizeof(long));
		return;
	}
fault:
	u.u_error = EFAULT;
	return;
}
