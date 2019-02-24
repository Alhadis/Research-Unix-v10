#include "sam.h"

/*
 * Guarantee: No compaction from Dread, Dinsert, Ddelete, Dreplace
 */
static Discdesc	desc[NBUFFILES];
static		bkalloc();
static		bkfree();
static void	bkread();
static int	bkwrite();
static char	tempnam[30];
static int	inited=0;

Dremove()
{
	register i;
	register Discdesc *dd;
	for(i=0, dd=desc; dd->fd; i++, dd++){
		sprint(tempnam, "/tmp/sam%d.%d", getpid(), i);
		unlink(tempnam);
	}
}
Discdesc *
Dstart()
{
	register i;
	register Discdesc *dd;
	register fd;
	for(i=0, dd=desc; dd->fd; i++, dd++)
		if(i==NBUFFILES-1)
			panic("too many buffer files");
	sprint(tempnam, "/tmp/sam%d.%d", getpid(), i);
	fd=creat(tempnam, 0600);
	if(fd<0){
		unlink(tempnam);
		fd=creat(tempnam, 0600);
	}
	if(fd<0 || (close(fd), fd=open(tempnam, 2))<0)
		panic("can't create buffer file");
	closeonexec(fd);
	dd->fd=fd;
	if(!inited++)
		atexit(Dremove);
	return dd;
}
Dclosefd()
{
	register i;
	for(i=0; i<NBUFFILES; i++)
		if(desc[i].fd)
			close(desc[i].fd);
}
Disc *
Dopen(dd)
	Discdesc *dd;
{
	register Disc *d;
	d=new(Disc, 1);
	d->desc=dd;
	return d;
}
Dclose(d)
	register Disc *d;
{
	register i;
	for(i=d->block.nused; --i>=0; )	/* backwards because bkfree() stacks */
		bkfree(d, i);
	gcfree((uchar *)d->block.ptr);
	free((uchar *)d);
}
int
Dread(d, addr, n, p1)
	register Disc *d;
	uchar *addr;
	int n;
	Posn p1;
{
	register i, nb, nr;
	register Posn p=0, p2=p1+n;
	for(i=0; i<d->block.nused; i++){
		if((p+=d->block.ptr[i].nbytes)>p1){
			p-=d->block.ptr[i].nbytes;
			goto out;
		}
	}
	if(p==p1)
		return 0;	/* eof */
	return -1;		/* past eof */
    out:
	n=0;
	if(p!=p1){	/* trailing partial block */
		nb=d->block.ptr[i].nbytes;
		if(p2>p+nb)
			nr=nb-(p1-p);
		else
			nr=p2-p1;
		bkread(d, addr, nr, i, (int)(p1-p));
		/* advance to next block */
		p+=nb;
		addr+=nr;
		n+=nr;
		i++;
	}
	/* whole blocks */
	while(p<p2 && (nb=d->block.ptr[i].nbytes)<=p2-p){
		if(i>=d->block.nused)
			return n;	/* eof */
		bkread(d, addr, nb, i, 0);
		p+=nb;
		addr+=nb;
		n+=nb;
		i++;
	}
	if(p<p2){	/* any initial partial block left? */
		nr=p2-p;
		nb=d->block.ptr[i].nbytes;
		if(nr>nb)
			nr=nb;		/* eof */
		/* just read in the part that survives */
		bkread(d, addr, nr, i, 0);
		n+=nr;
	}
	return n;
}
Dinsert(d, addr, n, p0)
	register Disc *d;
	uchar *addr;	/* if null, just make space */
	int n;
	Posn p0;
{
	register i, nb, ni;
	register Posn p=0;
	uchar hold[BLOCKSIZE];
	int nhold;
	for(i=0; i<d->block.nused; i++){
		if((p+=d->block.ptr[i].nbytes)>=p0){
			p-=d->block.ptr[i].nbytes;
			goto out;
		}
	}
	if(p!=p0)
		panic("Dinsert");	/* beyond eof */
    out:
	d->nbytes+=n;
	nhold=0;
	if(i<d->block.nused && (nb=d->block.ptr[i].nbytes)>p0-p){
		nhold=nb-(p0-p);
		bkread(d, hold, nhold, i, (int)(p0-p));
		d->block.ptr[i].nbytes-=nhold;	/* no write necessary */
	}
	/* insertion point is now at end of block i (which may not exist) */
	while(n>0){
		if(i<d->block.nused && (nb=d->block.ptr[i].nbytes)<BLOCKSIZE/2){
			/* fill this block */
			if(nb+n>BLOCKSIZE)
				ni=BLOCKSIZE/2-nb;
			else
				ni=n;
			if(addr)
				bkwrite(d, addr, ni, i, nb);
			nb+=ni;
		}else{	/* make new block */
			if(i<d->block.nused)
				i++;	/* put after this block, if it exists */
			bkalloc(d, i);
			if(n>BLOCKSIZE)
				ni=BLOCKSIZE/2;
			else
				ni=n;
			if(addr)
				bkwrite(d, addr, ni, i, 0);
			nb=ni;
		}
		d->block.ptr[i].nbytes=nb;
		if(addr)
			addr+=ni;
		n-=ni;
	}
	if(nhold){
		if(i<d->block.nused && (nb=d->block.ptr[i].nbytes)+nhold<BLOCKSIZE){
			/* fill this block */
			bkwrite(d, hold, nhold, i, nb);
			nb+=nhold;
		}else{	/* make new block */
			if(i<d->block.nused)
				i++;	/* put after this block, if it exists */
			bkalloc(d, i);
			bkwrite(d, hold, nhold, i, 0);
			nb=nhold;
		}
		d->block.ptr[i].nbytes=nb;
	}
}
Ddelete(d, p1, p2)
	register Disc *d;
	Posn p1, p2;
{
	register i, nb, nd;
	register Posn p=0;
	uchar buf[BLOCKSIZE];
	for(i=0; i<d->block.nused; i++){
		if((p+=d->block.ptr[i].nbytes)>p1){
			p-=d->block.ptr[i].nbytes;
			goto out;
		}
	}
	if(p1!=d->nbytes || p2!=p1)
		panic("Ddelete");
	return;	/* beyond eof */
    out:
	d->nbytes-=p2-p1;
	if(p!=p1){	/* throw away partial block */
		nb=d->block.ptr[i].nbytes;
		bkread(d, buf, nb, i, 0);
		if(p2>=p+nb)
			nd=nb-(p1-p);
		else{
			nd=p2-p1;
			bcopy(buf+(p1-p)+nd, buf+nb, buf+(p1-p), 1);
		}
		nb-=nd;
		bkwrite(d, buf, nb, i, 0);
		d->block.ptr[i].nbytes=nb;
		p2-=nd;
		/* advance to next block */
		p+=nb;
		i++;
	}
	/* throw away whole blocks */
	while(p<p2 && (nb=d->block.ptr[i].nbytes)<=p2-p){
		if(i>=d->block.nused)
			panic("Ddelete 2");
		bkfree(d, i);
		p2-=nb;
	}
	if(p>=p2)	/* any initial partial block left to delete? */
		return;	/* no */
	nd=p2-p;
	nb=d->block.ptr[i].nbytes;
	/* just read in the part that survives */
	bkread(d, buf, nb-=nd, i, nd);
	/* a little block merging */
	if(nb<BLOCKSIZE/2 && i>0 && (nd=d->block.ptr[i-1].nbytes)<BLOCKSIZE/2){
		bcopy(buf, buf+nb, buf+nd, -1);
		bkread(d, buf, nd, --i, 0);
		bkfree(d, i);
		nb+=nd;
	}
	bkwrite(d, buf, nb, i, 0);
	d->block.ptr[i].nbytes=nb;
}
Dreplace(d, p1, p2, addr, n)
	register Disc *d;
	Posn p1, p2;
	uchar *addr;
	int n;
{
	register i, nb, nr;
	register Posn p=0;
	uchar buf[BLOCKSIZE];
	if(p2-p1>n)
		Ddelete(d, p1+n, p2);
	else if(p2-p1<n)
		Dinsert(d, (uchar *)0, (int)(n-(p2-p1)), p2);
	if(n==0)
		return;
	p2=p1+n;
	/* they're now conformal; replace in place */
	for(i=0; i<d->block.nused; i++){
		if((p+=d->block.ptr[i].nbytes)>p1){
			p-=d->block.ptr[i].nbytes;
			goto out;
		}
	}
	panic("Dreplace");
    out:
	if(p!=p1){	/* trailing partial block */
		nb=d->block.ptr[i].nbytes;
		bkread(d, buf, nb, i, 0);
		if(p2>p+nb)
			nr=nb-(p1-p);
		else
			nr=p2-p1;
		bcopy(addr, addr+nr, buf+p1-p, 1);
		bkwrite(d, buf, nb, i, 0);
		/* advance to next block */
		p+=nb;
		addr+=nr;
		i++;
	}
	/* whole blocks */
	while(p<p2 && (nb=d->block.ptr[i].nbytes)<=p2-p){
		if(i>=d->block.nused)
			panic("Dreplace 2");
		bkwrite(d, addr, nb, i, 0);
		p+=nb;
		addr+=nb;
		i++;
	}
	if(p<p2){	/* any initial partial block left? */
		nr=p2-p;
		nb=d->block.ptr[i].nbytes;
		/* just read in the part that survives */
		bkread(d, buf+nr, nb-nr, i, nr);
		bcopy(addr, addr+nr, buf, 1);
		bkwrite(d, buf, nb, i, 0);
	}
}
static void
bkread(d, loc, n, bk, off)
	Disc *d;
	uchar *loc;
	int n;
	int bk;
	int off;
{
	Lseek(d->desc->fd, BLOCKSIZE*(long)d->block.ptr[bk].bnum+off, 0);
	Read(d->desc->fd, loc, n);
}
static void
bkwrite(d, loc, n, bk, off)
	Disc *d;
	uchar *loc;
	int n;
	int bk;
	int off;
{
	Lseek(d->desc->fd, BLOCKSIZE*(long)d->block.ptr[bk].bnum+off, 0);
	Write(d->desc->fd, loc, n);
}
static
bkalloc(d, n)
	register Disc *d;
	int n;
{
	register Discdesc *dd=d->desc;
	register bnum;
	if(dd->free.nused)
		bnum=dd->free.ptr[--dd->free.nused];
	else
		bnum=dd->nbk++;
	nocompact();
	inslist((List *)&d->block, n, 0L);
	compactok();
	d->block.ptr[n].bnum=bnum;
}
static
bkfree(d, n)
	Disc *d;
	int n;
{
	Discdesc *dd=d->desc;
	nocompact();
	inslist(&dd->free, dd->free.nused, (long)d->block.ptr[n].bnum);
	compactok();
	dellist((List *)&d->block, n);
}
