/*
 * access the files:
 * read or write data from some address in some space
 * -DPTRACE and provide the appropriate routines somewhere
 * if i/o to a process doesn't look like i/o to a file (eg v7 ptrace)
 */

#include "defs.h"
#include "regs.h"
#include "space.h"
#include "map.h"

#define	min(x, y)	((x) < (y) ? (x) : (y))

static int intrget(), intrput();
static int memclr(), memcpy();

/*
 * routines to get/put various types
 */

TLONG
lget(addr, space)
ADDR addr;
int space;
{
	TLONG x;

	if (space == NOSP)
		return (wtol((WORD)dot));
	if (fget(addr, space, (char *)&x, SZLONG) == 0)
		return (0);
	return (x);
}

TSHORT
sget(addr, space)
ADDR addr;
int space;
{
	TSHORT x;

	if (space == NOSP)
		return (wtos((WORD)dot));
	if (fget(addr, space, (char *)&x, SZSHORT) == 0)
		return (0);
	return (x);
}

TCHAR
cget(addr, space)
ADDR addr;
int space;
{
	TCHAR x;

	if (space == NOSP)
		return (wtoc((WORD)dot));
	if (fget(addr, space, (char *)&x, SZCHAR) == 0)
		return (0);
	return (x);
}

TADDR
aget(addr, space)
ADDR addr;
int space;
{
	TADDR x;

	if (space == NOSP)
		return (wtoa((WORD)dot));
	if (fget(addr, space, (char *)&x, SZADDR) == 0)
		return (0);
	return (x);
}

lput(addr, space, v)
ADDR addr;
int space;
TLONG v;
{

	return (fput(addr, space, (char *)&v, SZLONG));
}

sput(addr, space, v)
ADDR addr;
int space;
TSHORT v;
{

	return (fput(addr, space, (char *)&v, SZSHORT));
}

cput(addr, space, v)
ADDR addr;
int space;
TCHAR v;
{

	return (fput(addr, space, (char *)&v, SZCHAR));
}

aput(addr, space, v)
ADDR addr;
int space;
TADDR v;
{

	return (fput(addr, space, (char *)&v, SZADDR));
}

/*
 * the real io code
 */

int
fget(addr, space, buf, size)
ADDR addr;
register int space;
char *buf;
int size;
{
	int fd;
	long off;
	ADDR oa;
	static ADDR regaddr();
	long lseek();

	if ((space & SPTYPE) == NOSP) {
		memclr(buf, size);
		size = min(size, sizeof(dot)-addr);
		memcpy(buf, (char *)&dot, min(size, sizeof(dot)));
		return (1);
	}
	if (space & SYMF)
		fd = fsym;
	else
		fd = fcor;
	oa = addr;
	if ((space & SPTYPE) == REGSP) {
		if (space & SYMF) {
			errflg = "registers in corefile only";
			return (0);
		}
		if ((addr = regaddr(addr)) == 0)
			return (intrget(oa, buf, size));
		space &=~ SPTYPE;
		space |= DATASP;
	}
	if (reloc(addr, space, &off) == 0)
		return (0);
#ifdef PTRACE
	if ((space & SYMF) == 0 && pid)
		return (ptri(addr, space, buf, size));
#endif
	if (lseek(fd, off, 0) == -1
	||  read(fd, buf, size) != size) {
		if ((space & SPTYPE) == INSTSP)
			errflg = "can't read text";
		else
			errflg = "can't read data";
		return (0);
	}
	return (1);
}

int
fput(addr, space, buf, size)
ADDR addr;
register int space;
char *buf;
int size;
{
	int fd;
	long off;
	ADDR oa;
	static ADDR regaddr();
	long lseek();

	if ((space & SPTYPE) == NOSP)
		return (0);
	if (wtflag == 0
	&&  !((space & SYMF) == 0 && pid))
		error("not in write mode");
	if (space & SYMF)
		fd = fsym;
	else
		fd = fcor;
	oa = addr;
	if ((space & SPTYPE) == REGSP) {
		if (space & SYMF) {
			errflg = "registers in corefile only";
			return (0);
		}
		if ((addr = regaddr(addr)) == 0)
			return (intrput(oa, buf, size));
		space &=~ SPTYPE;
		space |= DATASP;
	}
	if (reloc(addr, space, &off) == 0)
		return (0);
#ifdef PTRACE
	if ((space & SYMF) == 0 && pid)
		return (ptro(addr, space, buf, size));
#endif
	if (lseek(fd, off, 0) == -1
	||  write(fd, buf, size) != size) {
		if ((space & SPTYPE) == INSTSP)
			errflg = "can't write text";
		else
			errflg = "can't write data";
		return (0);
	}
	return (1);
}

/*
 * register address hacks
 * if the register allegedly has an address (e.g. we're knee deep in
 * stack frames), return that
 * otherwise do fake io to our internal copies of registers
 * awful
 */

extern ADDR raddr[];
extern int roffs[];

static ADDR
regaddr(reg)
register ADDR reg;
{

	reg /= SZREG;
	if (MINREG <= reg && reg <= MAXREG)
		return (raddr[reg - MINREG]);
	return (0);
}

static int
intrget(reg, buf, size)
ADDR reg;
char *buf;
{
	register char *p, *q;
	register int n;
	register int rnum;
	TREG r;

	for (p = buf, n = size; n > 0; n--)
		*p++ = 0;
	rnum = reg / SZREG;
	if (rnum < MINREG)
		return (0);
	p = buf;
	while (rnum <= MAXREG && size > 0) {
		r = rget(roffs[rnum - MINREG]);
		for (q = (char *)&r, n = SZREG; n > 0; n--)
			*p++ = *q++;
		size -= SZREG;
		rnum++;
	}
	return (size <= 0);
}

static int
intrput(reg, buf, size)
ADDR reg;
char *buf;
{
	register char *p, *q;
	register int n;
	register int rnum;
	TREG r;

	rnum = reg / SZREG;
	if (rnum < MINREG)
		return (0);
	p = buf;
	while (rnum <= MAXREG && size > 0) {
		r = rget(roffs[rnum - MINREG]);
		for (q = (char *)&r, n = SZREG; n > 0 && size > 0; n--, size--)
			*q++ = *p++;
		rput(roffs[rnum - MINREG], r);
		rnum++;
	}
	return (size <= 0);
}

/*
 * turn address to file offset
 * returns nonzero if ok
 */


int
reloc(addr, space, offp)
ADDR addr;
register int space;
long *offp;
{
	register struct map *mp;

	for (mp = (space & SYMF) ? symmap : cormap; mp->flag & MPINUSE; mp++)
		if ((space & SPTYPE) == (mp->sp & SPTYPE)
		&&  mp->b <= addr && addr < mp->e) {
			addr += mp->f - mp->b;
			if ((space & (SYMF|RAWADDR)) == 0)
				if (kmap(&addr, space) == 0)
					return (0);
			*offp = addr;
			return (1);
		}
	/*
	 * if we wanted instruction space and didn't find it,
	 * try data space now
	 */
	if ((space & SPTYPE) == INSTSP) {
		space &=~ SPTYPE;
		space |= DATASP;
		for (mp = (space & SYMF) ? symmap : cormap; mp->flag & MPINUSE; mp++)
			if ((space & SPTYPE) == (mp->sp & SPTYPE)
			&&  mp->b <= addr && addr < mp->e) {
				addr += mp->f - mp->b;
				if ((space & (SYMF|RAWADDR)) == 0)
					if (kmap(&addr, space) == 0)
						return (0);
				*offp = addr;
				return (1);
			}
	}
	if (space & SYMF)
		errflg = "text address not found";
	else
		errflg = "data address not found";
	return (0);
}

/*
 * small hackery
 */

static
memclr(buf, size)
register char *buf;
register int size;
{

	while (--size >= 0)
		*buf++ = 0;
}

static
memcpy(to, fr, size)
register char *to, *fr;
register int size;
{

	while (--size >= 0)
		*to++ = *fr++;
}
