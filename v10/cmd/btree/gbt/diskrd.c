#include "cbt.h"

extern bfile *curbf;
extern long lseek();

ndrd(lev, where) ndaddr where;
{
	register n;
	if(mustwrite(curbf, lev)) {
		/* do we ever get here? (yes, while splitting) */
		if(ndwrt(curbf->path[lev], curbf->loc[lev]) == EOF)
			return(EOF);
		mustwrite(curbf, lev) = 0;
	}
	if(btbufread(curbf->tfd, where, (char *)curbf->path[lev]) == EOF)
		return(EOF);
	/*if(lseek(curbf->tfd, where * (long)NDSZ, 0) == -1)
		return(EOF);
	if((n = read(curbf->tfd, (char *)curbf->path[lev], NDSZ)) != NDSZ) {
		if(n >= 0)
			errno = BRDERR;
		return(EOF);
	}*/
	curbf->loc[lev] = where;
	return(0);
}

getincore(lev, where) ndaddr where;
{
	if(ndrd(lev, where) == EOF)
		return(EOF);
	if(curbf->path[lev]->hlev != lev) {
		errno = BRDERR;
		curbf->fatal++;
		return(EOF);
	}
	return(0);
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/8/9:diskrd.c\n"};
/*1010100001000111*/
