#include "cbt.h"
#include "pr.h"

typedef union {
	ndaddr na;
	lfaddr la;
} addr;
static char splitting[MXHT+1];
extern bfile *curbf;
extern ndaddr oldnode(), newnode();
extern char *malloc();

extern long brecwrite();

bwrite(bf, key, rec) mbuf key, rec; bfile *bf;
{	addr u;
	int n;
	if(bf == NULL)
		return(EOF);
	if(!bf->rdwrt) {
		errno = BNOWRITE;
		return(EOF);
	}
	if(notran(bf))
		return(EOF);
	if(key.mlen > MAXKLEN)
		return(EOF);
	if(!treeonly(bf)) {
		u.la.llen = rec.mlen;
		u.la.lloc = brecwrite(rec);
		if(u.la.lloc == EOF)
			return(EOF);
	}
	if(desce(bf, key, (private *)NULL) == EOF)
		return(EOF);
	n = xinsert(0, key, u);
	(void) bseek(bf, key);
	return(n);
}

fixpath(bf)
bfile *bf;
{	addr u;
	hdr *b;
	int i, n, j;
	for(i = 0; i < bf->height; i++) {
		if(!mustwrite(bf, i))
			continue;
		n = bf->loc[i];
		if((u.na = oldnode(i)) == EOF)
			return(EOF);
		b = bf->path[i+1];
		for(j = 0; j <= b->kcnt; j++)
			if(*ndadr(b, j) == n)
				break;
		if(j > b->kcnt){ /* curtains, the parent doesn't point to us */
			errno = BFATAL;
			return(EOF);
		}
		*ndadr(b, j) = u.na;
		mustwrite(bf, i + 1) = 1;
	}
	return(0);
}

xinsert(lev, key, u) mbuf key; addr u;
{	private x;
	int n, dellen;
	hdr *b = curbf->path[lev];
	char ba[NDSZ], bb[NDSZ];
	dkey *dx, *dy;
	if(splitting[lev])
		if(desce(curbf, key, (private *)NULL) == EOF)
			return(EOF);
	n = xscan(b, key, &x);
	if(x.match == FOUND) {
		if(treeonly(curbf))
			return(FOUND);
		else if(lev)
			*ndadr(b, n) = u.na;
		else
			*lfadr(b, n) = u.la;
		mustwrite(curbf, lev) = 1;
		return(FOUND);
	}
	dx = (dkey *)ba;
	dy = (dkey *)bb;
	dellen = newx(&x, key, dx, dy);
	if(lev)
		dellen += sizeof(ndaddr);
	else if(!treeonly(curbf))
		dellen += sizeof(lfaddr);
	if(dellen > nfree(b)) {
		if(nsplit(lev) == EOF)
			return(EOF);
		splitting[lev] = 1;
		n = xinsert(lev, key, u);
		splitting[lev] = 0;
		return(n);
	}
	b = curbf->path[lev];	/* with thanks to brian meekings */
	addaddr(b, n, u);
	newkeys(b, &x, dx, dy);
	b->kcnt++;
	nfree(b) -= dellen;
	mustwrite(curbf, lev) = 1;
	return(NOTFOUND);
}

newx(x, key, c, d) private *x; mbuf key; dkey *c, *d;
{	int i, j;
	if(x->match != EOF)
		c->dcom = x->ocom;
	else	c->dcom = x->ncom;
	i = key.mlen - c->dcom;
	c->dlen = DKEYSZ + i;
	mvgbt(c->dkey, key.mdata + c->dcom, i);
	if(x->match == EOF)
		return(c->dlen);
	j = x->ncom - x->d->dcom;
	d->dcom = x->ncom;
	d->dlen = x->d->dlen - j;
	mvgbt(d->dkey, x->d->dkey + j, d->dlen - DKEYSZ);
	return(c->dlen - j);
}

addaddr(b, n, u)
hdr *b;
addr u;
{
	if(b->hlev) {
		mvgbt((char *)ndadr(b, b->kcnt + 1),
			(char *)ndadr(b, b->kcnt),
			sizeof(ndaddr) * (b->kcnt + 1 - n) );
		*ndadr(b, n) = u.na;
		return;
	}
	if(treeonly(curbf))
		return;
	mvgbt((char *)lfadr(b, b->kcnt), (char *)lfadr(b, b->kcnt - 1),
		sizeof(lfaddr) * (b->kcnt - n));
	*lfadr(b, n) = u.la;
}

newkeys(b, x, c, d)
hdr *b;
private *x;
dkey *c, *d;
{	int n;
	char *ffree;
	if(b->hlev)
		ffree = (char *)ndadr(b, b->kcnt) - nfree(b);
	else if(treeonly(curbf))
		ffree = (char *)&nfree(b) - nfree(b);
	else
		ffree = (char *)lfadr(b, b->kcnt - 1) - nfree(b);
	if(x->match != EOF) {
		n = c->dlen + d->dlen;
		n -= x->d->dlen;
		mvgbt((char *)x->d + n, (char *)x->d, ffree - (char *)x->d);
		mvgbt((char *)x->d, (char *)c, c->dlen);
		mvgbt((char *)x->d + c->dlen, (char *)d, d->dlen);
	}
	else if(b->kcnt > 0)
		mvgbt((char *)x->d + x->d->dlen, (char *)c, c->dlen);
	else
		mvgbt((char *)x->d, (char *)c, c->dlen);
}

nsplit(lev)
{	dkey *tod, *fromd;
	char prefix[MAXKLEN + 10];
	hdr *b = curbf->path[lev];
	mbuf key;
	addr u;
	union {
		lfaddr *la;
		ndaddr *na;
	} from, to;
	int mvd, x, i, count, n;
	hdr *ha;
	char a[NDSZ];

	x = (NDSZ - sizeof(hdr) - sizeof(trailer)) / 2;
	ha = (hdr *) a;
	mvd = count = 0;
	tod = (dkey *)(ha + 1);
	fromd = (dkey *)(b + 1);
	if(lev == 0) {
		to.la = lfadr(ha, 0);
		from.la = lfadr(b, 0);
	}
	else {
		to.na = ndadr(ha, 0);
		from.na = ndadr(b, 0);
	}
	*ha = *b;
	n = (b->kcnt + 1)/2;
	if(lev && b->kcnt - n <= 1)
		n--;
	for(; count < n && mvd <= x; count++) {
		mvgbt((char *)tod, (char *)fromd, fromd->dlen);
		mvd += fromd->dlen;
		tod = (dkey *)((char *)tod + fromd->dlen);
		fromd = (dkey *)((char *)fromd + fromd->dlen);
		if(lev) {
			*to.na-- = *from.na--;
			mvd += sizeof(ndaddr);
		}
		else if(!treeonly(curbf)) {
			*to.la-- = *from.la--;
			mvd += sizeof(lfaddr);
		}
	}
	if(lev) {	/* another pointer for non-leaves */
		*to.na-- = *from.na--;
		mvd += sizeof(ndaddr);
	}
	ha->kcnt = count;
	nfree(ha) = NDSZ - sizeof(hdr) - sizeof(trailer) - mvd;
	/* if lev == 0, we promote the last key, else the next key */
	key.mlen = lastkey(ha, prefix);
	if(lev) {
		mvgbt(prefix + fromd->dcom, fromd->dkey, fromd->dlen - DKEYSZ);
		key.mlen = fromd->dcom + fromd->dlen - DKEYSZ;
		count++;
		fromd = (dkey *)((char *)fromd + fromd->dlen);
	}
	u.na = newnode(ha);
	if(u.na == EOF) {	/* error while splitting */
		curbf->fatal++;
		return(EOF);
	}
	key.mdata = prefix;
	/* other half */
	if(lev == 0)
		to.la = lfadr(ha, 0);
	else
		to.na = ndadr(ha, 0);
	tod = (dkey *)(ha + 1);
	tod->dcom = 0;
	tod->dlen = fromd->dlen + fromd->dcom;
	mvgbt(tod->dkey, prefix, fromd->dcom);
	mvgbt(tod->dkey + fromd->dcom, fromd->dkey, fromd->dlen - DKEYSZ);
	mvd = tod->dlen;
	fromd = (dkey *)((char *)fromd + fromd->dlen);
	tod = (dkey *)((char *)tod + tod->dlen);
	if(lev) {
		*to.na-- = *from.na--;
		mvd += sizeof(ndaddr);
	}
	else if(!treeonly(curbf)) {
		*to.la-- = *from.la--;
		mvd += sizeof(lfaddr);
	}
	count++;
	for(i = 1; count < b->kcnt; i++, count++) {
		mvgbt((char *)tod, (char *)fromd, fromd->dlen);
		mvd += fromd->dlen;
		tod = (dkey *)((char *)tod + tod->dlen);
		fromd = (dkey *)((char *)fromd + fromd->dlen);
		if(lev) {
			*to.na-- = *from.na--;
			mvd += sizeof(ndaddr);
		}
		else if(!treeonly(curbf)) {
			*to.la-- = *from.la--;
			mvd += sizeof(lfaddr);
		}

	}
	if(lev) {
		*to.na-- = *from.na--;
		mvd += sizeof(ndaddr);
	}
	ha->kcnt = i;
	nfree(ha) = NDSZ - sizeof(hdr) - sizeof(trailer) - mvd;
	mvgbt((char *)b, (char *)ha, NDSZ);
	mustwrite(curbf, lev) = 1;
	if(lev < curbf->height) {
		if(xinsert(lev + 1, key, u) == EOF) {
			curbf->fatal++;
			return(EOF);
		}
	}
	else
		return(newroot(u, key, b));
	return(0);
}

newroot(u, key, b)
addr u;
mbuf key;
hdr *b;
{	hdr *x;
	dkey *d;
	if(curbf->height >= MXHT) {
		errno = BTALL;
		curbf->fatal++;
		return(EOF);
	}
	if((x = curbf->path[b->hlev + 1] = (hdr *)malloc(NDSZ)) == NULL) {
		errno = BNOMEM;
		curbf->fatal++;
		return(EOF);
	}
	*x = *b;
	x->hlev++;
	d = (dkey *)(x + 1);
	d->dlen = DKEYSZ + key.mlen;
	d->dcom = 0;
	mvgbt(d->dkey, key.mdata, key.mlen);
	*ndadr(x, 0) = u.na;
	*ndadr(x, 1) = curbf->loc[b->hlev] = newnode(b);
	x->kcnt = 1;
	nfree(x) = NDSZ - sizeof(hdr) - sizeof(trailer) - DKEYSZ
		- key.mlen - 2 * sizeof(ndaddr);
	mustwrite(curbf, ++curbf->height) = 1;
	return(0);
}

lastkey(b, s)
hdr *b;
char *s;
{	int i, n;
	dkey *p;
	p = (dkey *)(b + 1);
	for(n = i = 0; i < b->kcnt; i++) {
		mvgbt(s + p->dcom, p->dkey, p->dlen - DKEYSZ);
		n = p->dlen + p->dcom - DKEYSZ;
		p = (dkey *)((char *) p + p->dlen);
	}
	return(n);
}

static struct D { struct D *a; char *b;} VER = {&VER,"\n82/10/9:bwrite.c\n"};
/*0010010000010101*/
