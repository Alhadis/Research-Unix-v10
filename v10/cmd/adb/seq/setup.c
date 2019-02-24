/*
 * adb - routines to read a.out+core at startup
 * stuff in here is pretty machine dependent
 * this for sequent
 */
#include "defs.h"
#include "map.h"
#include "space.h"
#include "machine.h"
#include <a.out.h>
#include <sys/param.h>
#include <sys/dir.h>
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
	    badmagic(hdr.a_magic)) {
		mp->f = mp->b = 0;
		mp->e = MAXFILE;
		mp->sp = DATASP;
		mp->flag = MPINUSE;
		mp++;
		mp->flag = 0;
		return;
	}
	magic = hdr.a_magic;
	entry = hdr.a_entry;
	loc = hdr.a_text+hdr.a_data;
	switch (magic) {

	case OMAGIC:
		txtsize = 0;
		datbase = 0;
		datsize = loc;
		mp->b = 0;
		mp->e = loc;
		mp->f = N_TXTOFF(hdr);
		mp->sp = DATASP;
		mp->flag = MPINUSE;
		break;

	case ZMAGIC:
	case XMAGIC:
	case SMAGIC:
		mp->b = N_ADDRADJ(hdr);
		mp->e = txtsize = hdr.a_text + mp->b;
		mp->f = N_TXTOFF(hdr);
		mp->sp = INSTSP;
		mp->flag = MPINUSE;
		mp++;
		mp->b = datbase = round((WORD)hdr.a_text, (WORD)PAGSIZ);
		mp->e = datsize = datbase + hdr.a_data;
		mp->f = N_DATAOFF(hdr);
		mp->sp = DATASP;
		mp->flag = MPINUSE;
		break;
	}
	mp++;
	mp->flag = 0;
	syminit(&hdr);
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

#define	UOFF(x)	(ADDR)(&((struct user *)0)->x)

int
mapimage()
{
	register MAP *mp;

	if (trcimage() == 0)
		return (0);
	sigcode = trcunab(UOFF(u_code));
	txtsize = ctob(trcunab(UOFF(u_tsize)));
	datsize = ctob(trcunab(UOFF(u_dsize)));
	stksize = ctob(trcunab(UOFF(u_ssize)));
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
	mp->b = MAXSTOR - stksize;
	mp->e = MAXSTOR;
	mp->f = mp->b;
	mp->sp = DATASP;
	mp->flag = MPINUSE;
	mp++;
	mp->b = 0;
	mp->e = ctob(UPAGES);
	mp->f = 0;
	mp->sp = UBLKSP;
	mp->flag = MPINUSE;
	mp++;
	mp->flag = 0;
	rsnarf();
	return (1);
}

mapcore()
{
	struct user u;
	register MAP *mp;

	lseek(fcor, (off_t)0, 0);
	if (read(fcor, (char *)&u, sizeof(u)) != sizeof(u)
	||  badmagic(u.u_objmag))
		return (0);
	if (magic && magic != u.u_objmag)
		printf("%s: not from %s\n", corfil, symfil);
	magic = u.u_objmag;
	signo = u.u_arg[0];
	sigcode = 0;	/* hack */
	txtsize = ctob(u.u_tsize);
	datsize = ctob(u.u_dsize);
	stksize = ctob(u.u_ssize);
	mp = cormap;
	switch (magic) {

	case ZMAGIC:
	case XMAGIC:
	case SMAGIC:
		mp->b = txtsize;
		mp->e = mp->b + datsize;
		mp->f = ctob(UPAGES);
		mp->sp = DATASP;
		mp->flag = MPINUSE;
		mp++;
		mp->b = MAXSTOR - stksize;
		mp->e = MAXSTOR;
		mp->f = datsize + ctob(UPAGES);
		mp->sp = DATASP;
		mp->flag = MPINUSE;
		break;
	}
	mp++;
	mp->b = 0;
	mp->e = ctob(UPAGES);
	mp->f = 0;
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
	case OMAGIC:
	case XMAGIC:
	case ZMAGIC:
	case SMAGIC:
		return (0);

	default:
		return (1);
	}
}

extern char lastc;

cmdmap(itype, star)
register int star, itype;
{
	register MAP *mp;

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
