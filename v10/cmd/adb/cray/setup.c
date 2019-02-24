/*
 * adb - routines to read a.out+core at startup
 * this for cray; very simple, but some hooks left for later
 */
#include "defs.h"
/*
 * beware!
 */
#undef wtoa
#undef atow
#undef wtoc
#undef ctow
#include "space.h"
#include "map.h"
#include "machine.h"
#include <sys/param.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <sys/ucomm.h>
#include <sys/signal.h>
#include <sys/dir.h>
#include <sys/proc.h>
#include <sys/user.h>

char	*symfil	= "a.out";
char	*corfil	= "core";

MAP	symmap[NMAP];
MAP	cormap[NMAP];

int fsym, fcor;

static ADDR datbase;
ADDR txtsize, datsize, stksize;
static ADDR entry;
static int magic;

setsym()
{
	ADDR loc;
	struct exec hdr;
	register MAP *mp;
	char *malloc();

	fsym = getfile(symfil, 1);
	mp = symmap;
	if (read(fsym, (char *)&hdr, sizeof hdr) != sizeof hdr ||
	    badmagic((int)hdr.a_magic)) {
		mp->f = mp->b = 0;
		mp->e = MAXFILE;
		mp->sp = DATASP;
		mp->flag = MPINUSE;
		mp++;
		mp->flag = 0;
		hksyminit(symfil, 0);
		return;
	}
	magic = hdr.a_magic;
	entry = patoba(hdr.a_entry);
	loc = watoba(hdr.a_text+hdr.a_data);
	switch (magic) {
	case A_MAGIC1:		/* only 407 files on cray! */
		txtsize = 0;
		datbase = 0;
		datsize = loc;
		mp->b = 0;
		mp->e = loc;
		mp->f = sizeof(hdr);
		mp->sp = DATASP;
		mp->flag = MPINUSE;
		break;
	}
	mp++;
	mp->flag = 0;
	hksyminit(symfil, 1);
}

setcor()
{
	register MAP *mp;

	fcor = getfile(corfil,2);
	if (fcor < 0
	||  (mapimage() == 0 && mapcore() == 0)) {
		/* not a core image */

		mp = cormap;
		mp->b = 0;
		mp->e = MAXFILE;
		mp->f = 0;
		mp->sp = DATASP;
		mp->flag = MPINUSE;
		mp++;
		mp->flag = 0;
		return;
	}
}

/*
 * set up maps for a direct process image (ptrace)
 */

#define	UOFF(x)	((int)&((struct user *)0)->x)

int
mapimage()
{
	register MAP *mp;

	if (trcimage() == 0)
		return (0);
	txtsize = 0;	/* sigh */
	datsize = MAXFILE;	/* sigh again */
#if NOTDEF		/* no ID separation */
	if (magic == IMAGIC)
		datbase = 0;
	else
#endif
		datbase = txtsize;
	mp = cormap;
	if (txtsize) {
		mp->b = 0;
		mp->e = txtsize;
		mp->f = 0;
		mp->flag = MPINUSE;
#if NOTDEF		/* no ID separation */
		if (magic == IMAGIC)
			mp->sp = INSTSP;
		else
#endif
			mp->sp = DATASP;
		mp++;
	}
	mp->b = txtsize;
	mp->e = mp->b + datsize;
	mp->f = mp->b;
	mp->sp = DATASP;
	mp->flag = MPINUSE;
	mp++;
	mp->b = 0;
	mp->e = ctob(USIZE);
	mp->f = 0;
	mp->sp = UBLKSP;
	mp->flag = MPINUSE;
	mp++;
	mp->flag = 0;
	rsnarf();
	return (1);
}

/*
 * assume single-tasked image
 * hack validity check: do first few entries in ascii offsets look good?
 * better checks invited
 */

mapcore()
{
	struct ucomm uc;
	char *p;
	struct user u;
	register MAP *mp;

	lseek(fcor, (off_t)0, 0);
	if (read(fcor, (char *)&uc, sizeof(uc)) != sizeof(uc))
		return (0);
	p = (char *)&uc.uc_ascii[0];
	if (p[0] != 'U' && p[0] != 'u')
		return (0);
	p = (char *)&uc.uc_ascii[2];
	if (p[0] != 'U' && p[0] != 'u')
		return (0);
	p = (char *)&uc.uc_ascii[4];	/* i tell you three times */
	if (p[0] != 'U' && p[0] != 'u')
		return (0);
	lseek(fcor, (off_t)ctob(UCSIZE), 0);
	if (read(fcor, (char *)&u, sizeof(u)) != sizeof(u))
		return (0);
	datsize = watoba(uc.uc_dsize);
	mp = cormap;
	mp->b = 0;
	mp->e = datsize;
	mp->f = ctob(USIZE)+ctob(UCSIZE);
	mp->sp = DATASP;
	mp->flag = MPINUSE;
	mp++;
	mp->b = 0;
	mp->e = ctob(USIZE);
	mp->f = ctob(UCSIZE);
	mp->sp = UBLKSP;
	mp->flag = MPINUSE;
	mp++;
	mp->flag = 0;
	rsnarf();
	return (1);
}

badmagic(num)
int num;
{

	switch (num) {
	case A_MAGIC1:
		return (0);

	default:
		return (1);
	}
}

cmdmap(itype, star)
register int star, itype;
{
	register MAP *mp;
	extern char lastc;

	if (itype & SYMF)
		mp = symmap;
	else
		mp = cormap;
	if (star)	/* UGH */
		mp++;
	if (expr(0))
		mp->b = expv; 
	if (expr(0))
		mp->e = expv; 
	if (expr(0))
		mp->f = expv; 
	mp->flag |= MPINUSE;
	if (rdc()=='?' && (itype&SYMF) == 0) {
		if (fcor)
			close(fcor);
		fcor=fsym;
		corfil=symfil;
	} else if (lastc == '/' && itype&SYMF) {
		if (fsym)
			close(fsym);
		fsym=fcor;
		symfil=corfil;
	} else
		reread();
}

create(f)
	char *f;
{
	register int fd;

	fd = creat(f, 0666);
	if (fd < 0)
		return (-1);
	close(fd);
	return (open(f, wtflag));
}

getfile(filnam, cnt)
	char *filnam;
{
	register int fsym;

	if (strcmp(filnam, "-") == 0)
		return (-1);
	fsym = open(filnam, wtflag);
	if (fsym < 0 && xargc > cnt) {
		if (wtflag)
			fsym = create(filnam);
		if (fsym < 0)
			printf("cannot open `%s'\n", filnam);
	}
	return (fsym);
}

setvar()
{

	var[varchk('b')] = datbase;
	var[varchk('d')] = datsize;
	var[varchk('e')] = entry;
	var[varchk('m')] = magic;
	var[varchk('s')] = stksize;
	var[varchk('t')] = txtsize;
}
