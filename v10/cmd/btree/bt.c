#include "cbt.h"
#include "pr.h"

bfile		*curbf;
extern bfile	*newtran();
extern char	*malloc(), *strcpy();
extern long	lseek();

bfile *bopen(s, typ) char *s;	/* typ is 0 or 2 */
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
	if(ndrd(0, (ndaddr)0) == EOF)
		return(NULL);
	strcpy(p->fname + n, ".F");
	if(!treeonly(p) && (p->dfd = open(p->fname, typ)) == -1) {
		(void) close(p->tfd);
		free(p->fname);
		free((char *) p->path[0]);
		free((char *)p);
		return(NULL);
	}
	else if(treeonly(p))
		p->dfd = -1;
	p->fname[n] = 0;
	if(shared(p))
		return(newtran(p));
	else if(tranid == 0)
		tranid = getlpid();
	p->height = p->path[0]->hlev;
	for(n = 1; n <= p->height; n++)
		if((p->path[n] = (hdr *)malloc(NDSZ)) == NULL)
			goto nomem;
	if(p->height > 0)
		mvgbt((char *)p->path[p->height], (char *)p->path[0], NDSZ);
	(void) bfirst(p);
	return(p);
nomem:
	errno = BNOMEM;
	return(NULL);
}

bseek(bf, key) bfile *bf; mbuf key;
{	private m;
	int i;
	if(bf == NULL)
		return(EOF);
	if(notran(bf))
		return(EOF);
	if(!readonly(bf))
		for(i = 0; i < bf->height; i++)
			if(mustwrite(bf, i))
				if(fixpath(bf) == EOF)
					return(EOF);
	bf->rdptr.rnum = desce(bf, key, &m);
	if(bf->rdptr.rnum == EOF)
		return(EOF);
	bf->advnc = 0;
	bf->rdptr.rptr = m.d;
	if(m.match == EOF) {
		if(advance() == EOF)
			return(EOF);
		if(bf->rdptr.rptr != NULL)
			m.match = NOTFOUND;
	}
	if(m.match != EOF)
		mvgbt(bf->rdptr.rpref, key.mdata, bf->rdptr.rptr->dcom);
		/* maybe use rptr instead of m.d */
	return(m.match);
}

bfirst(bf) bfile *bf;
{	mbuf key;
	key.mlen = 0;
	return(bseek(bf, key));
}

bclose(bf) bfile *bf;
{	int i;
	if(bf == NULL)
		return;
	if(shared(bf)) {
		if(intran())
			trabort();
		rmtran(bf);
	}
	else
		bflush(bf);
	free(bf->fname);
	for(i = 0; i <= MXHT; i++)
		if(bf->path[i] != NULL)
			free((char *)bf->path[i]);
	if(bf->tfd != -1)
		(void) close(bf->tfd);
	if(bf->dfd != -1)
		(void) close(bf->dfd);
	free((char *)bf);
}

bflush(bf) bfile *bf;
{	int i;
	if(bf == NULL)
		return(0);
	if(!bf->rdwrt) {
		errno = BNOWRITE;
		return(EOF);
	}
	curbf = bf;
	if(shared(bf))
		if(intran()) {
			trabort();
			return(EOF);
		}
		else return(0);
	for(i = 0; i <= bf->height; i++) {
		if(!mustwrite(bf, i))
			continue;
		if(readonly(bf) || i == bf->height)
			if(ndwrt(bf->path[i], bf->loc[i]) == EOF)
				return(EOF);
		else
			if(fixpath(bf) == EOF)
				return(EOF);
		bf->flag[i] = 0;
	}
	return(0);
}

breclen(bf) bfile *bf;
{
	if(bf == NULL)
		return(EOF);
	if(notran(bf))
		return(EOF);
	if(bf->advnc)
		if(advance() == EOF)
			return(EOF);
	if(bf->rdptr.rnum >= bf->path[0]->kcnt)
		return(EOF);
	if(treeonly(bf))
		return(0);
	return(lfadr(bf->path[0], bf->rdptr.rnum)->llen);
}

mbuf bkey(bf) bfile *bf;
{	mbuf x;
	dkey *d;
	if(bf == NULL)
		goto eof;
	if(notran(bf))
		goto eof;
	if(bf->advnc)
		if(advance() == EOF)
			goto eof;
	if(bf->rdptr.rnum >= bf->path[0]->kcnt)
		goto eof;
	d = bf->rdptr.rptr;
	x.mlen = d->dlen - DKEYSZ + d->dcom;
	mvgbt(bf->rdptr.rpref + d->dcom, d->dkey, d->dlen-DKEYSZ);
	x.mdata = bf->rdptr.rpref;
	return(x);
eof:
	x.mlen = 0;
	x.mdata = NULL;
	return(x);
}

bread(bf, key, rec) bfile *bf; mbuf *key, *rec;
{
	dkey *d;
	lfaddr *x;
	int n;
	if(bf == NULL)
		return(NULL);
	if(notran(bf))
		return(EOF);
	if(bf->advnc)
		if(advance() == EOF)
			return(EOF);
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
			if((n = read(bf->dfd, rec->mdata, (int)rec->mlen))
				!= rec->mlen) {
				if(n >= 0)
					errno = BRDERR;
				return(EOF);
			}
		}
	}
	bf->advnc = 1;
	return(0);
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n82/10/9:bt.c\n"};
/*0001011110110101*/
