/*
 * memory controller support for nautilus
 *
 * an interpretation of the VMS code
 */

#include "sys/param.h"
#include "sys/systm.h"

extern long *mcrcsr;	/* virt addr of mem csrs; filled in when spt built */

/*
 * mem csrs have no names, just numbers 0-7
 * 0-3 have control and status bits
 * 4-7 are just magic places to read:
 *	csr4 clears memory interrupt
 *	csr5 clears memory (and nmi) fault
 */

/*
 * csr0
 */

#define	TOCODE	0x7000000	/* timeout code */
#define	ILTO	0x2000000	/* interlock timeout */

/*
 * csr1
 */

#define	WENABLE	0x100000	/* decode ram write enable -- make sure off */

/*
 * csr2
 */

#define	RDSHER	0x80000000	/* rds thick and fast */
#define	RDS	0x40000000	/* read data substitute (hard error) */
#define	CRD	0x20000000	/* corrected read data (soft error) */
#define	RDFORCE	0x10000000	/* RDS really NMI bus error */

/*
 * csr3
 */

#define	IEINTERR 0x80000000	/* internal error intr enab */
#define	IEILERR	0x40000000	/* interlock error ie */
#define	IECRD	0x20000000	/* soft error ie */
#define	IERDS	0x10000000	/* hard error ie */
#define	INTERR	0x2000000	/* internal error */

#define	IE	(IEINTERR|IEILERR|IECRD|IERDS)

/*
 * init the memory controller:
 * enable interrupts
 */

mcrinit()
{
	register long junk;
	register long *m;

	if ((m = mcrcsr) == 0)
		panic("mcrinit");
	mcrclr();
	m[3] = IE;
}

/*
 * here on memory interrupt
 * guess whether it's an error
 */

mcrint()
{
	register long *m;

	if ((m = mcrcsr) == 0)
		return;
	/* cknofault? */
	mcrerr();
}

/*
 * memory error, because controller interrupted or otherwise
 */
static int crdoff;
int mcrreenb();

mcrerr()
{
	register long *m;
	register long csr0, csr1, csr2, csr3;
	register long xcsr1;		/* eventually toss out */
	register int hard;
	static time_t lastcrd;

	if ((m = mcrcsr) == 0)
		return;		/* hopeless */
	csr0 = m[0];
	xcsr1 = m[1];
	m[1] = xcsr1 & ~WENABLE;
	csr1 = m[1];
	csr2 = m[2];
	csr3 = m[3];
	mcrclr();
	hard = 0;
	if (csr3 & INTERR)
		hard++;
	if ((csr0 & TOCODE) == ILTO)
		hard++;
	if (csr2 & (RDS|RDFORCE))
		hard++;
	printf("mem %s err: %x %x %x %x (%x)\n", hard ? "hard" : "soft",
		csr0, csr1, csr2, csr3, xcsr1);
	if (hard)
		panic("mcrerr");
	if (lastcrd && lastcrd + 5 < time) {
		if (crdoff++ == 0)
			timeout(mcrreenb, (caddr_t)0, 15*HZ);
		m[3] = (IE &~ IECRD);
	}
	else
		lastcrd = time;
}

mcrreenb(x)
caddr_t x;
{
	crdoff = 0;
	mcrcsr[3] = IE;
}

mcrclr()
{
	register long junk;
	register long *m;

	if ((m = mcrcsr) == 0)
		return;
	junk = m[4];
	junk = m[5];
	m[0] = m[0]|TOCODE;
	m[1] = m[1];
	m[2] = m[2];
}
