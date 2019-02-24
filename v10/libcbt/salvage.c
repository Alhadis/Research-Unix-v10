/* write out all records pointed to by level 0 blocks,
 * whether or not the level 0 blocks are accessible.
 * the output is like that of bcat -R, except that many keys
 * will be duplicated
 * In general, the later keys are more up to date
 * btsalvage xxx | slvg | sort | slvg2 | cbt build -R newxxx
 * is the optimistic way of slavaging a b-tree in which the non-leaves
 * have been clobbered
 */
#include "cbt.h"
#include "pr.h"
#include "stdio.h"

bfile		*curbf;
extern bfile	*newtran(), *xopen();
extern char	*malloc(), *strcpy(), *realloc();
extern long	lseek();
char keybuf[NDSZ];
char *recbuf;
int buflen;

main(argc, argv)
char **argv;
{	bfile *bf;
	mbuf key, rec;
	int n;
	if(argc != 2) {
		fprintf(stderr, "usage: salvage file\n");
		exit(1);
	}
	bf = xopen(argv[1], 0);
	key.mdata = keybuf;
	if(bf == NULL) {
		fprintf(stderr, "couldn't open %s\n", argv[1]);
		exit(1);
	}
	while((n = breclen(bf)) > buflen) {
		if(buflen == 0)
			recbuf = malloc(buflen = 1024);
		else
			recbuf = realloc(recbuf, buflen += 1024);
		if(recbuf == NULL) {
			fprintf(stderr, "recbuf[%d] failed\n", buflen);
			exit(1);
		}
		rec.mdata = recbuf;
	}
	for(;;) {
		xread(bf, &key, &rec);
		write(1, (char *)&key.mlen, 2);
		write(1, key.mdata, key.mlen);
		write(1, (char *)&rec.mlen, 2);
		write(1, rec.mdata, rec.mlen);
	}
}
bfile *xopen(s, typ) char *s;	/* typ is 0 or 2 */
{	bfile *p;
	int n, i;

	p = alloc(bfile);
	if(p == NULL)
		goto nomem;
	n = strlen(s);
	p->fname = malloc((unsigned)n + 3);
	if(p->fname == NULL)
		goto nomem;
	(void) strcpy(p->fname, s);
	strcpy(p->fname + n, ".T");
	if((p->tfd = open(p->fname, typ)) == -1) {
		free(p->fname);
		free((char *)p);
		return(NULL);
	}
	p->rdwrt = typ;
	p->fatal = p->advnc = 0;
	p->altname = NULL;
	for(i = 0; i <= MXHT; i++) {
		p->path[i] = NULL;
		p->flag[i] = 0;
		p->loc[i] = 0;
	}
	p->path[0] = (hdr *)malloc(NDSZ);
	if(p->path[0] == NULL)
		goto nomem;
	curbf = p;
	if(read(p->tfd, (char *)p->path[0], NDSZ) != NDSZ) {
		perror(" first read");
		exit(1);
	}
	strcpy(p->fname + n, ".F");
	if(!treeonly(p) && (p->dfd = open(p->fname, typ)) == -1) {
		(void) close(p->tfd);
		free(p->fname);
		free(p->path[0]);
		free((char *)p);
		return(NULL);
	}
	else if(treeonly(p))
		p->dfd = -1;
	p->fname[n] = 0;
	p->height = p->path[0]->hlev;
	xfirst(p);
	return(p);
nomem:
	errno = BNOMEM;
	return(NULL);
}

breclen(bf) bfile *bf;
{
	if(bf == NULL)
		return(EOF);
	if(notran(bf))
		return(EOF);
	if(bf->advnc)
		xadvance();
	if(bf->rdptr.rnum >= bf->path[0]->kcnt)
		return(EOF);
	if(treeonly(bf))
		return(0);
	return(lfadr(bf->path[0], bf->rdptr.rnum)->llen);
}

xread(bf, key, rec) bfile *bf; mbuf *key, *rec;
{
	dkey *d;
	lfaddr *x;
	if(bf == NULL)
		return(NULL);
	if(notran(bf))
		return(EOF);
	if(bf->advnc)
		xadvance();
	if(bf->rdptr.rnum >= bf->path[0]->kcnt)
		return(EOF);
	if(key != NULL) {
		d = bf->rdptr.rptr;
		key->mlen = d->dlen - DKEYSZ + d->dcom;
		mvgbt(key->mdata, bf->rdptr.rpref, d->dcom);
		mvgbt(key->mdata + d->dcom, d->dkey, d->dlen - DKEYSZ);
	}
	if(rec != NULL && !treeonly(bf)) {
		x = lfadr(bf->path[0], bf->rdptr.rnum);
		rec->mlen = x->llen;
		if(rec->mlen != 0) {
			(void) lseek(bf->dfd, x->lloc, 0);
			(void) read(bf->dfd, rec->mdata, (int)rec->mlen);
		}
		/* errors on read ? */
	}
	bf->advnc = 1;
	return(0);
}

xstep(level)
/* ran off the end of node at lev-1 */
{	hdr *b;
	int n, i;
	ndaddr u;
	do {
		n = read(curbf->tfd, (char *)curbf->path[0], NDSZ);
		if(n == 0)
			exit(0);
		if(n != NDSZ) {
			perror("xstep");
			exit(1);
		}
	} while(curbf->path[0]->hlev != 0);
}

xadvance()
{	mbuf x;
	dkey *dold, *dnew;
	struct rdptr *y = &curbf->rdptr;
	curbf->advnc = 0;
	dold = y->rptr;
	if(++y->rnum < curbf->path[0]->kcnt) {
		dnew = (dkey *)((char *)dold + dold->dlen);
		if(dold->dcom < dnew->dcom)
			mvgbt(y->rpref + dold->dcom, dold->dkey, dnew->dcom - dold->dcom);
		y->rptr = dnew;
		return;
	}
	if(xstep(1) == EOF) {
		y->rnum = curbf->path[0]->kcnt + 1;
		y->rptr  = NULL;
	}
	else {
		y->rnum = 0;
		y->rptr = (dkey *)(curbf->path[0] + 1);
	}
}

xfirst(p)
bfile *p;
{	int n;
	while(p->path[0]->hlev != 0) {
		n = read(p->tfd, (char *)p->path[0], NDSZ);
		if(n == 0) {
			fprintf(stderr, "empty?\n");
			exit(0);
		}
		if(n != NDSZ) {
			perror("xfirst");
			exit(1);
		}
	}
	curbf->rdptr.rnum = 0;
	curbf->rdptr.rptr = (dkey *)(curbf->path[0] + 1);
}
