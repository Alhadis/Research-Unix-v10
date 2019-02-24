#include "fs.h"

char xbuf[4096];	/* also used by prname */
char *
errnm(n)
{
	switch(n) {
	default:
		sprintf(xbuf, "?%d?", n);
		return(xbuf);
	case Elinksize:	return("Elinksize");
	case Ebadaddr:	return("Ebadaddr");
	case Efirst:	return("Efirst");
	case Ebadread:	return("Ebadread");
	case Efirstio:	return("Efirstio");
	case Edirio:	return("Edirio");
	case Enotdot:	return("Enotdot");
	case Enotdotdot:	return("Enotdotdot");
	case Edotino:	return("Edotino");
	case Ebadino:	return("Ebadino");
	case Ebadname:	return("Ebadname");
	case Enullable:	return("Enullable");
	case Elinkcnt:	return("Elinkcnt");
	case Emulti:	return("Emulti");
	case Ebadparent:	return("Ebadparent");
	case Esuperino:	return("Esuperino");
	case Esuperfree:	return("Esuperfree");
	case Einvalid:	return("Einvalid");
	case Efreelist:	return("Efreelist");
	case Edup:	return("Edup");
	case Eshort:	return("Eshort");
	case Ehole:	return("Ehole");
	case Etriple:	return("Etriple");
	case Eshortdir:	return("Eshortdir");
	case Eattach:	return("Eattach");
	case Efakeroot:	return("Efakeroot");
	}
}

adderr(n, a, b)
{
	if(!erlen) {
		erlen = 100;
		erlist = (er *) malloc(erlen * sizeof(er));
		if(!erlist)
			fatal("can't alloc %d erlist\n", erlen);
	}
	if(erptr >= erlen) {
		erlen *= 3;
		erlist = (er *) realloc((char *)erlist, erlen * sizeof(er));
		if(!erlist)
			fatal("can't realloc %d erlist\n");
	}
	errcnts[n]++;
	erlist[erptr].type = n;
	erlist[erptr].a = a;
	erlist[erptr].b = b;
	erlist[erptr].done = 0;
	erptr++;
}

supererr(n, a, b)
{
	adderr(n, a, b);
}

inoerr(n, ino, i)
{
	adderr(n, ino, i);
}

dupblock(bp, a, b)
bm *bp;
{	static cnt;
	if(++cnt <= 20)
		pmesg("dup block %d %s %s\n", bp-bmap, btype(a), btype(b));
	else if(cnt == 20)
		pmesg("more dups\n");
	adderr(Edup, bp-bmap, bp->ino);
}

direrror(n, ino, i)
{
	adderr(n, ino, i);
}

