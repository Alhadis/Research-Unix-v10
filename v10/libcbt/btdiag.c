#include "stdio.h"
#include "cbt.h"
#include "sys/types.h"
#include "sys/stat.h"

extern bfile *bopen();
extern char *malloc();
extern int errno;
int errcnt;
char nodes[MXHT + 1][NDSZ];
int loc[MXHT + 1];
char *seen;
dkey *curd[MXHT + 1];
char curkey[MXHT + 1][MAXKLEN];
int keynum[MXHT + 1];
mbuf tinykey = { "", 0};
mbuf giantkey = {"\177\177\177\177", 4};

main(argc, argv)
char **argv;
{	bfile *bt;
	int n;
	if(argc < 2)
		error("file name?");
	bt = bopen(argv[1], 0);
	if(bt == 0)
		error("couldn't open tree");
	n = diagnose(bt);
	printf("return value is %d\n", n);
	exit(n);
}

diagnose(b)
bfile *b;
{	int i;
	struct stat statb;
	if(fstat(b->tfd, &statb) < 0)
		error("can't stat tree");
	if(statb.st_size % NDSZ || statb.st_size <= 0) {
		printf("tree size %d mod ndsz %d != 0\n", statb.st_size, NDSZ);
		errcnt++;
	}
	if(b->height > MXHT || b->height < 0) {
		printf("tree height %d weird max %d\n", b->height, MXHT);
		return(-1);
	}
	seen = malloc(statb.st_size / NDSZ);
	for(i = 0; i < statb.st_size/NDSZ; i++)
		seen[i] = -1;
	lseek(b->tfd, 0, 0);
	if((i = read(b->tfd, nodes[b->height], NDSZ)) != NDSZ) {
		printf("read of root returned %d (%d)\n", i, errno);
		error("can't proceed");
	}
	return(dolevel(b->tfd, b->height, tinykey, giantkey) || errcnt);
}

mbuf
firstkey(n)
{	hdr *b;
	mbuf z;
	int i;
	b = (hdr *)nodes[n];
	keynum[n] = 0;
	if(keynum[n] >= b->kcnt) {
		z.mlen = -1;
		return(z);
	}
	curd[n] = (dkey *)(b+1);
	for(i = 0; i < curd[n]->dlen - DKEYSZ; i++)
		curkey[n][i] = curd[n]->dkey[i];
	z.mlen = i;
	z.mdata = curkey[n];
	return(z);
}

mbuf
nextkey(n)
{	mbuf z;
	char *p = (char *)curd[n];
	dkey *d;
	int i;
	hdr *b = (hdr *)nodes[n];
	if(++keynum[n] >= b->kcnt) {
		z.mlen = -1;
		return(z);
	}
	p += curd[n]->dlen;
	d = curd[n] = (dkey *)p;
	z.mdata = curkey[n];
	for(i = 0; i < d->dlen - DKEYSZ; i++)
		z.mdata[d->dcom + i] = d->dkey[i];
	z.mlen = i + d->dcom;
	return(z);
}

dolevel(fd, lev, lokey, hikey)
mbuf lokey, hikey;
{	int i, j;
	mbuf left, right;
	char leftx[MAXKLEN];
	hdr *b = (hdr *)nodes[lev];
	if(lseek(fd, loc[lev] * NDSZ, 0) < 0 || read(fd, nodes[lev], NDSZ) != NDSZ) {
		printf("couldn't read node %d, level %d\n", loc[lev], lev);
		return(-1);
	}
	if(seen[loc[lev]] != -1) {
		printf("node %d visited twice, at lev %d and %d\n", lev, seen[loc[lev]], lev);
		if(lev != seen[loc[lev]])
			return(-1);
	}
	else
		seen[loc[lev]] = lev;
	if(check(loc[lev], nodes[lev], lokey, hikey))
		return(-1);
	if(lev == 0)
		return(0);
	right = firstkey(lev);
	left.mlen = lokey.mlen;
	left.mdata = leftx;
	for(j = 0; j < left.mlen; j++)
		left.mdata[j] = lokey.mdata[j];
	for(i = 0; i < b->kcnt; i++) {
		loc[lev - 1] = *ndadr(b, i);
		if(dolevel(fd, lev - 1, left, right))
			return(-1);
		left.mlen = right.mlen;
		for(j = 0; j < left.mlen; j++)
			left.mdata[j] = right.mdata[j];
		right = nextkey(lev);
		if(right.mlen < 0)
			printf("unexpected end of keys, node %d\n", loc[lev]);
	}
	loc[lev - 1] = *ndadr(b, i);
	return(dolevel(fd, lev - 1, left, hikey));
}

check(noden, b, lokey, hikey)
hdr *b;
mbuf lokey, hikey;
{	int i, plen, sz, j;
	char *p, prefix[MAXKLEN];
	dkey *d;

	/* check space allocation */
	sz = sizeof(hdr) + sizeof(trailer) + nfree(b);
	if(nfree(b) < 0) {
		printf("nfree: %d < 0, node %d\n", nfree(b), noden);
		errcnt++;
	}
	if(sz > NDSZ) {
		printf("nfree: %d impossibly large, node %d\n", nfree(b), noden);
		errcnt++;
	}
	if(b->kcnt < 0) {
		printf("kcnt %d < 0, node %d\n", b->kcnt, noden);
		return(-1);
	}
	p = (char *)(b+1);
	for(i = 0; i < b->kcnt; i++) {
		d = (dkey *)p;
		if(d->dlen < DKEYSZ) {
			printf("node %d, key %d, dlen %d too small\n", noden, i, d->dlen);
			return(-1);
		}
		if(b->hlev)
			sz += sizeof(ndaddr);
		else if(!(b->htype & INDEX))
			sz += sizeof(lfaddr);
		if((sz += d->dlen) > NDSZ) {
			printf("node %d, contents too large\n", noden);
			return(-1);
		}
		p += d->dlen;
	}
	if(b->hlev)
		sz += sizeof(ndaddr);
	if(sz != NDSZ) {
		printf("node %d, size %d ndsz %d\n", noden, sz, NDSZ);
		return(-1);
	}
	/* check key ordering */
	p = (char *)(b + 1);
	d = (dkey *)p;
	if(d->dcom != 0) {
		printf("node %d first key has dcom %d > 0\n", noden, d->dcom);
		return(-1);
	}
	plen = lokey.mlen;
	for(i = 0; i < plen; i++)
		prefix[i] = lokey.mdata[i];
	for(i = 0; i < b->kcnt; i++) {
		if(d->dcom > plen) {
			printf("node %d key %d, dcom %d bigger than len %d of prev key\n",
				noden, i, d->dcom, plen);
			return(-1);
		}
		for(j = 0; j < d->dlen - DKEYSZ; j++) {
			if(j + d->dcom > plen)
				break;
			if(d->dkey[j] < prefix[d->dcom + j]) {
				printf("node %d, key %d out of order\n",
					noden, i);
				errcnt++;
				break;
			}
			else if(d->dkey[j] == prefix[d->dcom + j])
				continue;
			else
				break;
		}
		for(j = 0; j < d->dlen - DKEYSZ; j++)
			prefix[j + d->dcom] = d->dkey[j];
		plen = j + d->dcom;
	}
	for(i = 0; i < plen && i < hikey.mlen; i++) {
		if(prefix[i] < hikey.mdata[i])
			return(0);
		else if(prefix[i] == hikey.mdata[i])
			continue;
		printf("node %d last key too large pref %s, hi %s\n", noden, prefix, hikey.mdata);
		return(-1);
	}
	if(plen > hikey.mlen) {
		printf("node %d last key too large plen %d, hikey.len %d\n", noden, plen, hikey.mlen);
		return(-1);
	}
	return(0);
}

error(s)
char *s;
{
	printf("%s\n", s);
	exit(1);
}
