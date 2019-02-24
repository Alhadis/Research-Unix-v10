#include "cbt.h"
#include "pr.h"

extern bfile	*curbf;

xscan(b, key, x) hdr *b; mbuf key; private *x;
{	int ncom, i, n, val;
	char a, *p;
	dkey *d;

	n = i = ncom = 0;
	val = NOTFOUND;
	p = (char *)(b+1);
	d = (dkey *)p;
	for(; i < b->kcnt; i++, p += d->dlen) {
		d = (dkey *)p;
		if(ncom < d->dcom)
			goto onward;
		if(ncom > d->dcom) {
			if(x != NULL) {
				x->match = val;
				x->ncom = d->dcom;
				x->ocom = ncom;
				x->d = d;
			}
			return(i);
		}
		for(n = ncom; ncom < key.mlen && ncom - n < d->dlen - DKEYSZ; ncom++) {
			if((a = d->dkey[ncom - n]) == key.mdata[ncom])
				continue;
			if(a < key.mdata[ncom])
				goto onward;
			goto done;
		}
		if(ncom == key.mlen) {
			if(ncom == d->dlen - DKEYSZ + d->dcom)
				val = FOUND;
			goto done;
		}
	onward:	;
	}
/* infinity: */
	if(x != NULL) {
		x->match = EOF;
		x->ncom = ncom;
		x->ocom = n;
		x->d = d;
	}
	return(i);
done:
	if(x != NULL) {
		x->match = val;
		x->ncom = ncom;
		x->ocom = n;
		x->d = d;
	}
	return(i);
}

desce(bf, key, x) bfile *bf; mbuf key; private *x;
{	int i, j;
	ndaddr u;
	for(i = bf->height; i > 0; i--) {
		j = xscan(bf->path[i], key, (private *)NULL);
		u = *ndadr(bf->path[i], j);
		if(bf->loc[i-1] != u)
			if(getincore(i - 1, u) == EOF)
				return(EOF);
	}
	i = xscan(bf->path[0], key, x);
	return(i);
}

step(level)
/* ran off the end of node at lev-1 */
{	hdr *b;
	int n, i;
	ndaddr u;
	if(level > curbf->height)
		return(EOF);
	n = curbf->loc[level - 1];
	b = curbf->path[level];
	for(i = 0; i <= b->kcnt; i++)
		if(*ndadr(b, i) == n)
			break;
	if(i >= b->kcnt)
		return(step(level + 1));
	n = level;
	i++;
	do {
		u = *ndadr(curbf->path[n], i);
		if(curbf->loc[n-1] != u)
			if(getincore(n - 1, u) == EOF)
				return(EOF);
		i = 0;
	} while(--n > 0);
	return(0);
}

advance()
{	dkey *dold, *dnew;
	struct rdptr *y = &curbf->rdptr;
	curbf->advnc = 0;
	dold = y->rptr;
	if(++y->rnum < curbf->path[0]->kcnt) {
		dnew = (dkey *)((char *)dold + dold->dlen);
		if(dold->dcom < dnew->dcom)
			mvgbt(y->rpref + dold->dcom, dold->dkey, dnew->dcom - dold->dcom);
		y->rptr = dnew;
		return(0);
	}
	errno = 0;
	if(step(1) == EOF) {
		if(errno)
			return(EOF);
		y->rnum = curbf->path[0]->kcnt + 1;
		y->rptr  = NULL;
	}
	else {
		y->rnum = 0;
		y->rptr = (dkey *)(curbf->path[0] + 1);
	}
	return(0);
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n82/10/9:seek.c\n"};
/*1100011110101111*/
/*1101111110111111*/
