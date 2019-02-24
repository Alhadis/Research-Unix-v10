/*
 * error logging
 *
 * other parts of the system may call logerr to record an error
 * reading the device recovers the error records
 */

#include "sys/param.h"
#include "sys/conf.h"
#include "sys/systm.h"		/* just for time */
#include "sys/user.h"
#include "sys/buf.h"
#include "sys/errlog.h"

/*
 * hackish use of overlays:
 * if this device isn't configured,
 * errcnt will be zero, errlog will be 1 item long (so it won't be undefined)
 * if the device is configured,
 * errcnt is the number of errlog buffers
 */

int errcnt;
struct errlog errlog[1];	/* 1 or more */

static int errlost;
static char erptr, ewptr;

int erropen(), errread(), errwrite();
struct cdevsw errcdev = cdinit(erropen, nodev, errread, errwrite, nodev);

#define	PERROR	(PUSER-1)

erropen(dev, flag)
dev_t dev;
int flag;
{

	if (errcnt == 0)
		u.u_error = ENXIO;
}

errread(dev)
dev_t dev;
{
	register int s;

	if (u.u_count < sizeof(errlog)) {
		u.u_error = EINVAL;
		return;
	}
	s = spl7();
	while (erptr == ewptr)
		sleep((caddr_t)errlog, PERROR);
	splx(s);
	s = errlog[erptr].e_hdr.e_len + sizeof(struct errhdr);
	iomove((caddr_t)&errlog[erptr], s, B_READ);
	if (++erptr >= errcnt)
		erptr = 0;
}

/*
 * mostly for debugging
 */

errwrite(dev)
dev_t dev;
{
	char b[MAXEDATA+20];
	register int len;
	register char *p;

	if ((len = u.u_count) > sizeof(b)) {
		u.u_error = EINVAL;
		return;
	}
	iomove(b, len, B_WRITE);
	if (u.u_error)
		return;
	p = b;
	while (--len >= 0)
		if (*p++ == '\n')
			break;
	p[-1] = 0;
	if (len < 0)
		len = 0;
	logerr(b, 0, p, len, 0);
}

/*
 * here to log an error:
 * dev is the name of the device or circumstance
 * unit is a unit number, e.g. which disk number broke
 * data is the stuff to be logged; there are len bytes
 * data should be something bcopy can copy (maybe not device registers)
 * `hard' is nonzero if this was a hard error
 */

logerr(dev, unit, data, len, hard)
char *dev;
int unit;
char *data;
int len;
int hard;
{
	register struct errlog *ep;
	register int s, i;
	register char *p;

	s = spl7();
	if ((i = ewptr + 1) >= errcnt)
		i = 0;
	if (i == erptr) {		/* buffer full */
		splx(s);
		errlost++;
		return;
	}
	ep = &errlog[ewptr];
	ewptr = i;
	splx(s);
	ep->e_hdr.e_1magic = E_1MAGIC;
	ep->e_hdr.e_2magic = E_2MAGIC;
	len = min(len, MAXEDATA);
	ep->e_hdr.e_len = len;
	if (hard)
		ep->e_hdr.e_len |= E_HARD;
	ep->e_hdr.e_unit = unit;
	ep->e_hdr.e_time = time;
	p = dev;
	while (*p++)		/* should be strlen */
		;
	bcopy(dev, ep->e_hdr.e_dev, min(p - dev, sizeof(ep->e_hdr.e_dev)));
	if (len)
		bcopy(data, ep->e_data, len);
	wakeup((caddr_t)errlog);
}
