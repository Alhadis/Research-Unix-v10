/*
 * init routines -- dummy version
 */
#include "defs.h"
#include "space.h"
#include "map.h"

char	*symfil	= "a.out";
char	*corfil	= "core";

MAP	symmap[NMAP];
MAP	cormap[NMAP];

int fsym, fcor;

setsym()
{

	fsym = getfile(symfil, 1);
	dumbmap(symmap);
}

setcor()
{

	fcor = getfile(corfil, 2);
	dumbmap(cormap);
}

static
dumbmap(mp)
register MAP *mp;
{

	mp->b = mp->f = 0;
	mp->e = HUGE;
	mp->sp = DATASP;
	mp->flag = MPINUSE;
	mp++;
	mp->flag = 0;
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
}
