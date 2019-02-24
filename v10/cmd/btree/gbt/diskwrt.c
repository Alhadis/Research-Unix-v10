#include "cbt.h"

extern bfile *curbf;
extern long lseek();

ndwrt(b, where) hdr *b; ndaddr where;
{
	register n;
	if(lseek(curbf->tfd, where * (long)NDSZ, 0) == -1)
		return(EOF);
	if((n = write(curbf->tfd, (char *)b, NDSZ)) != NDSZ) { /*unacceptable*/
		if(n >= 0)
			errno = BIOWRT;
		curbf->fatal++;
		return(EOF);
	}
	btbufwrt(curbf->tfd, where, (char *)b);
	return(0);
}

long brecwrite(rec) mbuf rec;
{	long loc;
	int n;
	errno = 0;
	loc = lseek(curbf->dfd, 0L, 2);
	if((n = write(curbf->dfd, rec.mdata, rec.mlen)) == rec.mlen)
		return(loc);
	else if(n == -1 || errno)
		return(EOF);
	errno = BIOWRT;
	return(EOF);
}

ndaddr newnode(b) hdr *b;
{	long loc;
	int n;
	b->hstamp = tranid;
	loc = lseek(curbf->tfd, 0L, 2);
	if(loc == -1)
		return(EOF);
	if((n = write(curbf->tfd, (char *)b, NDSZ)) != NDSZ)
		if(n < 0)
			return(EOF);
		else {
			errno = BIOWRT;
			return(EOF);
		}
	btbufwrt(curbf->tfd, loc/NDSZ, (char *)b);
	return(loc/NDSZ);
}

ndaddr oldnode(lev)
{	int n;
	ndaddr a;
	if(curbf->path[lev]->hstamp != tranid) {
		a = newnode(curbf->path[lev]);
		if(a == EOF)
			curbf->fatal++;
		curbf->loc[lev] = a;
		return(a);
	}
	/*if(lseek(curbf->tfd, (long)curbf->loc[lev]*NDSZ, 0) == EOF)
		return(EOF);
	if((n = write(curbf->tfd, (char *)curbf->path[lev], NDSZ)) != NDSZ) {
		if(n >= 0)
			errno = BIOWRT;
		return(EOF);
	}*/
	btbufwrt(curbf->tfd, curbf->loc[lev], (char *)curbf->path[lev]);
	mustwrite(curbf, lev) = 0;
	return(curbf->loc[lev]);
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/8/9:diskwrt.c\n"};
