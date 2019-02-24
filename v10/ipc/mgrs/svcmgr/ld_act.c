#include "mgr.h"
#include <pwd.h>
#include <sys/filio.h>
#include <sys/inio.h>
#include "defs.h"

/*
 *  action routines to pushline disciplines
 */
mesgld(rp, ap)
	Request *rp;
	Action *ap;
{
	extern int rmesg_ld;
	int rv;

	USE(ap);
	rv = ioctl(rp->i->cfd, FIOPUSHLD, &rmesg_ld);
	if(rv<0)
		errstr = "can't push rmesg line disc";
	return rv;
}

ttyld(rp, ap)
	Request *rp;
	Action *ap;
{
	extern int tty_ld;
	int rv;

	USE(ap);
	rv = ioctl(rp->i->cfd, FIOPUSHLD, &tty_ld);
	if(rv<0)
		errstr = "can't push tty line disc";
	return rv;
}

#ifdef PEX
/*
 *  what did this come from? -- presotto
 */
pex(rp, ap)
	Request *rp;
	Action *ap;
{
	USE(ap);
	ioctl(rp->i->cfd, FIOAPX, 0);	/* make channel lockable */
	return 0;
}
#endif PEX

/*
 *  do the ioctl that disables one way hangups on tcp channels
 */
tcphup(rp, ap)
	Request *rp;
	Action *ap;
{
	USE(ap);
	ioctl(rp->i->cfd, TCPIOHUP, 0);
	return 0;
}
