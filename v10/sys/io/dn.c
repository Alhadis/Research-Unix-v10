/*
 * DN-11 ACU interface
 *  Minor devices < <128 are standard 801's
 *  Minor devices >= 128 are Parker shared
 *  units in which the 1st digit selects a line.
 *  The only interface difference is that DLO
 *  can appear some time after the line is selected
 *  to indicate that the line is busy or out of service.
 *
 *  If the system has an accessible DR-11C, allow some of
 *  its bits to be fiddled.
 */

#include "sys/param.h"
#include "sys/user.h"
#include "sys/conf.h"
#include "sys/ubaddr.h"

struct device {
	u_short	dn_reg[4];
};

#define	PWI	0100000
#define	ACR	040000
#define	DLO	010000
#define	DONE	0200
#define	IENABLE	0100
#define	DSS	040
#define	PND	020
#define	MAINT	010
#define	MENABLE	04
#define	DPR	02
#define	CRQ	01

#define	DNPRI	(PZERO+5)

extern int dncnt;
extern struct ubaddr dnaddr[];
extern caddr_t dnreg[];

static int dnindex();

int	dnopen(), dnclose(), dnwrite();
struct cdevsw dncdev = cdinit(dnopen, dnclose, nodev, dnwrite, nodev);

dnopen(dev)
register dev;
{
	register struct device *dp;
	register unit;

	dev = minor(dev) & 0177;
	unit = dev>>2;
	if (unit > dncnt) {
		u.u_error = ENXIO;
		return;
	}
	if ((dp = (struct device *)dnreg[unit]) == NULL) {
		if ((dp = (struct device *)ubaddr(&dnaddr[unit])) == NULL
		||  ubbadaddr(dnaddr[unit].ubno, &dp->dn_reg[0], sizeof(short))) {
			printf("dn%d absent\n", unit);
			u.u_error = ENXIO;
			return;
		}
		dnreg[unit] = (caddr_t)dp;
	}
	if (dp->dn_reg[dev&03]&(PWI|DLO|CRQ)) {
		u.u_error = ENXIO;
		return;
	}
	dp->dn_reg[0] |= MENABLE;
	dp->dn_reg[dev&03] = IENABLE|MENABLE|CRQ;
}

dnclose(dev)
{
	dev = minor(dev) & 0177;
	((struct device *)dnreg[dev>>2])->dn_reg[dev&03] = MENABLE;
}

dnwrite(dev)
{
	register c;
	register u_short *dp;
	register first;
	extern lbolt;
	int ldlo;
	register int setspeed = -1;
	register k;

	ldlo = 0;
	if (dev & 0200)
		ldlo = DLO;
	dev = minor(dev) & 0177;
	dp = &((struct device *)dnreg[dev>>2])->dn_reg[dev&03];
	if (*dp & DLO) {
		u.u_error = EIO;
		return;
	}
	*dp |= CRQ;
	first = 0;
	while ((*dp & (PWI|ACR|DSS|ldlo)) == 0) {
		spl4();
		if ((*dp&PND) == 0 || u.u_count == 0 || (c=cpass()) < 0)
			sleep((caddr_t)dp, DNPRI);
		else if (c == '-') {
			sleep((caddr_t)&lbolt, DNPRI);
			sleep((caddr_t)&lbolt, DNPRI);
		} else if ((k = dnindex (c, "abcdefghABCDEFGH")) >= 0) {
			setspeed = k;
		} else {
			*dp = (c<<8)|IENABLE|MENABLE|DPR|CRQ;
			if (first == 0) {
				while (first <  800)
					first++;
				if (*dp&ldlo) {
					*dp = MENABLE;
					break;
				}
				ldlo = 0;
			}
			sleep((caddr_t)dp, DNPRI);
		}
		spl0();
	}
	if (*dp&(PWI|ACR|ldlo))
		u.u_error = EIO;
	else if (setspeed >= 0)
		drsetbit (setspeed % 8, setspeed / 8);
}

/*
 * interrupt-- "dev" is
 * system unit number, not minor device
 */
dn0int(dev)
{
	register u_short *ep;
	register struct device *dp;

	dp = (struct device *)dnreg[dev];
	dp->dn_reg[0] &= ~MENABLE;
	for (ep = &dp->dn_reg[0]; ep < &dp->dn_reg[4]; ep++)
		if (*ep&DONE) {
			*ep &= ~DONE;
			wakeup((caddr_t)ep);
		}
	dp->dn_reg[0] |= MENABLE;
}

static int
dnindex (c, s)
	register char c, *s;
{
	register char *p;

	p = s;
	while (*p) {
		if (c == *p)
			return p - s;
		p++;
	}
	return -1;
}
