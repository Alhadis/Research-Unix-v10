/*% cyntax -c -DKERNEL % && cc -c -DKERNEL %
 * I/O page driver.
 * Maps device registers into user address space on open.
 * Ioctl gets virtual address of registers.
 * Used for ITI framebuffer and miscellaneous hackage.
 */
#include "sys/param.h"
#include "sys/conf.h"
#include "sys/user.h"
#include "sys/ubaddr.h"
#include "sys/pte.h"
#include "sys/pgio.h"
extern struct ubaddr pgaddr[];	/* defined in conf */
extern int pgcnt;		/* defined in conf */
extern caddr_t pgcaddr[];	/* defined in conf.  0 when closed */
int pgopen(), pgclose(), pgioctl();
struct cdevsw pgcdev=cdinit(pgopen, pgclose, nodev, nodev, pgioctl);
static maptouser(vaddress)
caddr_t vaddress;
{
	Sysmap[((unsigned)vaddress-0x80000000)>>9].pg_prot=PG_UW>>27;
}
pgopen(dev, flag)
dev_t dev;
{
	register d=minor(dev);
	register caddr_t *p=&pgcaddr[d];
	if(d>=pgcnt
	|| *p
	|| (*p=ubaddr(&pgaddr[d]))==0
	|| ubbadaddr(pgaddr[d].ubno, *p, sizeof(short)))
		u.u_error=ENXIO;
	else
		maptouser(*p);
}
pgclose(dev)
dev_t dev;
{
	register caddr_t *p=&pgcaddr[minor(dev)];
	*p=0;
}
pgioctl(dev, cmd, arg, flag)
dev_t dev;
caddr_t arg;
{
	switch(cmd){
	case PGADDR:
		copyout((caddr_t)&pgcaddr[minor(dev)], arg, sizeof(caddr_t));
		break;
	default:
		return (ENOTTY);
	}
	return (0);
}
