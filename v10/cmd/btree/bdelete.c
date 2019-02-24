#include "cbt.h"
#include "pr.h"

extern bfile *curbf;
bdelete(bf, key) bfile *bf; mbuf key;
{	dkey *dold, *dnew;
	hdr *b;
	int svlen, dellen;
	char *ffree;

	if(bf == NULL)
		return(EOF);
	if(notran(bf))
		return(EOF);
	if(!bf->rdwrt) {
		errno = BNOWRITE;
		return(EOF);
	}
	if(bseek(bf, key) != FOUND)
		return(NOTFOUND);
	b = bf->path[0];
	dold = bf->rdptr.rptr;
	if(bf->rdptr.rnum + 1 >= b->kcnt) {
		nfree(b) += dold->dlen + (treeonly(bf)? 0: sizeof(lfaddr));
		--b->kcnt;
		mustwrite(bf, 0) = 1;
		if(b->hlev == 0 && b->kcnt <= 0 || b->hlev > 0 && b->kcnt < 0) {
			mustwrite(bf, 0) = 0;
			ndelete(1, key);
		}
		if(b->kcnt < 0)
			b->kcnt = 0;
		(void) bseek(bf, key);
		return(FOUND);
	}

	if(treeonly(bf))
		ffree = (char *)&nfree(b) - nfree(b);
	else
		ffree = (char *)lfadr(b, b->kcnt - 1) - nfree(b);
	svlen = dold->dlen;
	dnew = (dkey *)((char *)dold + dold->dlen);
	if(dnew->dcom <= dold->dcom) {
		mvgbt((char *)dold, (char *)dnew, ffree - (char *)dnew);
		if(!treeonly(bf))
			rmlfadr(b, bf->rdptr.rnum);
		b->kcnt--;
		mustwrite(bf, 0) = 1;
		nfree(b) += svlen + (treeonly(bf)? 0: sizeof(lfaddr));
		(void) bseek(bf, key);
		return(FOUND);
	}
	dellen = dnew->dcom - dold->dcom;
	/* writing over dold */
	dold->dlen = dnew->dlen + dellen;
	mvgbt(dold->dkey + dellen, dnew->dkey, ffree - dnew->dkey);
	if(!treeonly(bf))
		rmlfadr(b, bf->rdptr.rnum);
	b->kcnt--;
	mustwrite(bf, 0) = 1;
	nfree(b) += svlen - dellen + (treeonly(bf)? 0: sizeof(lfaddr));
	(void) bseek(bf, key);
	return(FOUND);
}
rmlfadr(b, n) hdr *b; unsigned int n;
{
	mvgbt((char *)lfadr(b, b->kcnt - 2), (char *)lfadr(b, b->kcnt - 1),
		(int)sizeof(lfaddr) * (b->kcnt - 1 - n));
}
rmndadr(b, n) hdr *b; unsigned short n;
{
	mvgbt((char *)ndadr(b, b->kcnt - 1), (char *)ndadr(b, b->kcnt),
		(int)sizeof(ndaddr) * (b->kcnt - n));
}
ndelete(lev, key) mbuf key;
{	hdr *b;
	int svlen, dellen;
	unsigned short n;
	private x;
	char *ffree;
	dkey *dold, *dnew;
	if(lev > curbf->height) {
		b = curbf->path[curbf->height];
		b->hlev = 0;
		curbf->height = 0;
		mustwrite(curbf, 0) = 1;
		curbf->loc[0] = 0;
		nfree(b) = NDSZ - sizeof(hdr) - sizeof(trailer);
		b->kcnt = 0;
		mvgbt((char *)curbf->path[0], (char *)b, NDSZ);
		return;
	}
	b = curbf->path[lev];
	n = xscan(b, key, &x);
	if(x.match == EOF) {
		if(b->kcnt <= 0) {
			mustwrite(curbf, lev) = 0;
			ndelete(lev+1, key);
			return;
		}
		b->kcnt--;
		nfree(b) += sizeof(ndaddr) + x.d->dlen;
		mustwrite(curbf, lev) = 1;
		return;
	}
	dold = x.d;
	dnew = (dkey *)((char *)dold + dold->dlen);
	ffree = (char *)ndadr(b, b->kcnt) - nfree(b);
	svlen = dold->dlen;
	if(n + 1 >= b->kcnt || dnew->dcom <= dold->dcom) {
		mvgbt((char *)dold, (char *)dnew, ffree - (char *)dnew);
		rmndadr(b, n);
		b->kcnt--;
		mustwrite(curbf, lev) = 1;
		nfree(b) += sizeof(ndaddr) + svlen;
		return;
	}
	dellen = dnew->dcom - dold->dcom;
	/* writing over dold */
	dold->dlen = dnew->dlen + dellen;
	mvgbt(dold->dkey + dellen, dnew->dkey, ffree - dnew->dkey);
	rmndadr(b, n);
	b->kcnt--;
	mustwrite(curbf, lev) = 1;
	nfree(b) += sizeof(ndaddr) + svlen - dellen;
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n82/10/9:bdelete.c\n"};
/*1101110100011001*/
