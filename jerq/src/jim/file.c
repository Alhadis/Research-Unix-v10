#include "jim.h"
#include "file.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

struct stat statbuf;
char	*mktemp();
long	lseek();
char	tempname[];
int	tempfile;

static	seek();
static	newblock();
static	relblock();
static	clearblocks();

#define	MAXFREE 512
static	nfree=0;	/* number of known free blocks */
static	next=0;		/* next available block in file */
static	short freelist[MAXFREE];
static	short fileid[MAXFILES];
File *
Fileid(id)
{
	register File *f;
	for(f=file; f; f=f->next)
		if(f->id==id)
			return f;
	return 0;
}
File *
Fnew(){
	register File *f;
	register id;
	/* first see if there's a spare */
	f=(File *)alloc((ulong)sizeof(File));
	for(id=0; id<MAXFILES && fileid[id]; id++)
		;
	if(id>=MAXFILES){
		free((char *)f);
		return 0;
	}
	f->id=id;
	fileid[id]=1;
	if(file==0)
		file=f;
	else
		f->next=file[0].next, file[0].next=f;
	return f;
}
/*
 * Fill in block b with information for fresh block
 */
static
newblock(b, l)
	register Block *b;
{
	if(nfree==0)	/* no empty blocks to be reclaimed */
		b->bnum=next++;
	else
		b->bnum=freelist[--nfree];
	b->nbytes=l;
}
/*
 * Return block b to the free pool
 */
static
relblock(b)
	register Block *b;
{
	if(b->bnum==0)
		panic("relblock");
	if(nfree<MAXFREE)
		freelist[nfree++]=b->bnum;
}
static
clearblocks(f)
	register File *f;
{
	register Block *b;
	/*
	 * Always leave the first block alive, and delete
	 * in reverse order so they reallocate in forward order
	 */
	for(b= &f->block[f->nblocks-1]; b>f->block; --b)
		relblock(b);
	f->nblocks=min(1L, (long)f->nblocks);
	if(f->nblocks==1)
		f->block[0].nbytes=0;
}
static
growblock(f)
	register File *f;
{
	gcrenew(f->block, f->nalloc+=NBLOCK);
}
File *
Fcreat(f, s)
	register File *f;
	char *s;
{
	strinit(&f->name);
	strdup(&f->name, s);
	f->selloc=0;
	f->nsel=0;
	f->origin=0;
	f->date=0;
	return f;
}
File *
Fload(f)
	register File *f;
{
	strinit(&f->str);
	if(f->block==0){
		f->block=(Block *)gcalloc((ulong)NBLOCK*sizeof(Block), (long **)&f->block);
		f->nalloc=NBLOCK;
		newblock(f->block, 0);
		f->nblocks=1;
		f->curblk= -1;
	}
	Fread(f, 0L, &f->name, TRUE, 0);
	f->changed=FALSE;
	Fgetblock(f, 0);
	return f;
}
Fread(f, posn, str, setdate, r)
	register File *f;
	long posn;
	String *str;
{
	register n, nbytes=0;
	char buf[BLOCKSIZE];
	register here, b;
	register char *s;

	if(r==0){
		s=charstar(str);
		if(s[0]==0)
			return;
		if((r=open(s, 0)) == -1){
			mesg("can't open", s);
			return 0;	/* this return value is never seen */
		}
		if(setdate){
			fstat(r, &statbuf);
			f->date=statbuf.st_mtime;
		}
	}
	splitblock(f, (long)Fseek(f, posn));
	for(here=f->curblk; (n=read(r, buf, BLOCKSIZE/2))>0; ){
		nbytes+=n;
		if(f->nblocks>=f->nalloc)
			growblock(f);
		for(b=f->nblocks-1; b>here; --b)
			f->block[b+1]=f->block[b];
		newblock(&f->block[++here], n);
		f->nblocks++;
		seek(f->block[here].bnum);
		Write(tempname, tempfile, buf, (long)BLOCKSIZE);
	}
	f->curblk= -1;
	close(r);
	return nbytes;
}

/*
 * Dump file f to named file; but if fd>0, it's an open file
 * and the file should be appended to it.
 */
Fwrite(f, fname, fd)
	register File *f;
	register String *fname;
	register fd;
{
	char buf[BLOCKSIZE];
	register b;
	register newfile=fd==0;
	register char *s=fname? charstar(fname) : "some file";
	int issame=FALSE;

	Fputblock(f);
	if(fd==0){
		if(stat(s, &statbuf)!=0){
			dprintf("new file; ");
			issame=TRUE;
		}else if(f->date==DUBIOUS){
			f->date=statbuf.st_mtime;
			error("file already exists", (char *)0);
		}
		if(samefile(fname, &f->name)){
			if(statbuf.st_mtime>f->date){
				f->date=statbuf.st_mtime;
				error("unix file modified since last read/written", (char *)0);
			}
			issame=TRUE;
		}
		/* must recompute s; samefile called charstar */
		if((fd=creat(s=charstar(fname), 0666)) == -1)
			error("can't create", s);
	}
	for(b=0; b<f->nblocks; b++){
		seek(f->block[b].bnum);
		Read(tempname, tempfile, buf, f->block[b].nbytes);
		Write(s, fd, buf, (long)f->block[b].nbytes);
	}
	if(buf[f->block[b-1].nbytes-1]!='\n' && length(f)>0)
		dprintf("last char not newline; ");
	if(issame){
		fstat(fd, &statbuf);
		f->date=statbuf.st_mtime;
	}
	if(newfile)
		close(fd);
}
/*
 * Fwritepart: write out nbytes from f at posn to file descriptor fd
 */
Fwritepart(f, posn, nbytes, fd)
	register File *f;
	long posn;
	long nbytes;
	register fd;
{
	register n, nthisblock;
	while(nbytes>0){
		if((n=Fseek(f, posn+1)-1)<0)
			break;
		nthisblock=min((long)(f->str.n-n), nbytes);
		if(write(fd, f->str.s+n, nthisblock)!=nthisblock)
			break;
		posn+=nthisblock;
		nbytes-=nthisblock;
	}
	return nbytes!=0;
}
samefile(f1, f2)
	String *f1, *f2;
{
	struct stat stat2;
	return stat(charstar(f1), &statbuf)==0 /* else file doesn't exist, no problem */
	    && stat(charstar(f2), &stat2)==0
            && statbuf.st_dev==stat2.st_dev
	    && statbuf.st_ino==stat2.st_ino ;	/* the same file, for sure */
}
/* shut down, but keep the storage allocated */
Fclose(f)
	register File *f;
{
	if(f->block)
		clearblocks(f);
	strfree(&f->name);
	strfree(&f->str);
	f->str.s=f->name.s=0;	/* shows that's it's clear */
}
Ffree(f)
	register File *f;
{
	register File *g;
	Fclose(f);
	fileid[f->id]=0;
	if(f==file)
		panic("Ffree 1");
	for(g=file; g->next!=f; g=g->next)
		if(g->next==0)
			panic("Ffree 2");
	g->next=f->next;
	free((char *)f);
}
Freset(f)
	register File *f;
{
	if(f->block)
		clearblocks(f);
	strzero(&f->str);
	f->origin=0;
	f->nsel=0;
	f->selloc=0;
}
toolarge(f)
	register File *f;
{
	error("temp file too large for", charstar(&f->name));
}
Read(s, fd, a, n)
	char *s;
	int fd;
	char *a;
	int n;
{
	if(read(fd, a, n) != n)
		ioerr("read", s);
}

Write(s, fd, a, n)
	char *s;
	int fd;
	char *a;
	ulong n;
{
	if(write(fd, a, (int)n) != (int)n)
		ioerr("write", s);
}

/*
 * Bring into memory block n of File f
 */
Fgetblock(f, n)
	register File *f;
	register n;
{
	if(n==f->curblk || n<0 || n>=f->nblocks)	/* last two are just safety */
		return;
	strinsure(&f->str, (ulong)f->block[n].nbytes);
	seek(f->block[n].bnum);
	Read(tempname, tempfile, f->str.s, f->block[n].nbytes);
	f->str.n=f->block[n].nbytes;
	f->curblk=n;
}

/*
 * do lseek to the start of the named block 
 */
static
seek(b)
	register b;
{
	if(lseek(tempfile, b*(long)BLOCKSIZE, 0) == -1L)
		ioerr("lseek", tempname);
}

/*
 * Insert a new block into f after block f->curblk
 */
makeblock(f, n)
	register File *f;
{
	register Block *b;

	if(f->nblocks>=f->nalloc)
		growblock(f);
	for(b= &f->block[f->nblocks-1]; b>&f->block[f->curblk]; --b)
		b[1]=b[0];
	b=f->block+f->curblk+1;
	newblock(b, n);		/* REQUIRES NEWBLOCK NOT CAUSE GC */
	f->nblocks++;
	seek(b->bnum);
}

/*
 * Write f->curblk to disk.  This may require generating
 * more disk blocks.  f->curblk is left at the BEGINNING
 * of the original block, but the associated string may be shorter.
 */
Fputblock(f)
	register File *f;
{
	register nbytes=f->str.n;

	if(f->curblk<0)
		return;
	while(nbytes>=BLOCKSIZE)	/* the = isn't necessary */
		splitblock(f, (long)(nbytes-=BLOCKSIZE/2));
	f->block[f->curblk].nbytes=f->str.n;
	seek(f->block[f->curblk].bnum);
	Write(tempname, tempfile, f->str.s, f->str.n);
}
/*
 * Split f->curblk by moving bytes after position n into new block
 */
splitblock(f, n)
	register File *f;
	long n;
{
	register long nchop=f->str.n-n;

	if(nchop==0)
		return 0;	/* why do anything? */
	(void)makeblock(f, (int)nchop);
	Write(tempname, tempfile, f->str.s+n, nchop);
	strdelete(&f->str, (ulong)n, (ulong)(n+nchop));
	f->block[f->curblk].nbytes-=nchop;
	return 1;
}
/*
 * Seek to absolute location m in file.  Pick up the block
 * and return character number of that location in the block.
 */
Fseek(f, m)
	register File *f;
	register long m;
{
	register long i;
	register bn;
	register Block *b;

	for(i=0,bn=0,b=f->block; b<&f->block[f->nblocks]; b++,bn++){
		if(i+b->nbytes >= m)
			goto out;
		i+=b->nbytes;
	}
	/* If here, m is after EOF */
	return -1;
    out:
	if(f->curblk>=0 && f->curblk!=bn)
		Fputblock(f);
	Fgetblock(f, bn);
	return (int)(m-i);
}

/*
 * Insert the n-character string s at location m in file f
 */
Finstext(f, m, s)
	register File *f;
	register long m;
	register String *s;
{
	strinsert(&f->str, s, (ulong)Fseek(f, m));
	if((f->block[f->curblk].nbytes=f->str.n)>BLOCKSIZE)	/* write the block */
		Fputblock(f);
	if(s->n>0)
		modified(f);
}

/*
 * Delete n characters at absolute location m in file f.
 */
Fdeltext(f, m, n)
	register File *f;
	register long m;
	register long n;
{
	register long nbytes, loc;
	register Block *b;
	if(n>0)
		modified(f);
	while(n > 0){
		/*
		 * If string is at start of block, want to be in that block
		 * not the previous (as we want for insert).
		 */
		loc=Fseek(f, m+1)-1;
		nbytes=min((long)n, (long)f->str.n-loc);
		if(nbytes<=0)	/* at EOF */
			break;
		strdelete(&f->str, (ulong)loc, (ulong)loc+nbytes);
		if((f->block[f->curblk].nbytes-=nbytes)<=0 && f->nblocks>1){
			relblock(&f->block[f->curblk]);
			for(b= &f->block[f->curblk]; b<&f->block[f->nblocks]; b++)
				b[0]=b[1];
			f->nblocks--;
			f->curblk= -1;
		}
		n-=nbytes;
	}
}

/*
 * Set String b to string in file f at m, n chars long
 */
Fsave(f, b, m, n)
	register File *f;
	register String *b;
	register long m;
	register long n;
{
	register loc, nbytes;
	register char *p;

	strinsure(b, (ulong)n);
	b->n=0;
	while(n>0){
		loc=Fseek(f, m+1)-1;
		if(loc<0)
			break;
		nbytes=min((long)n, (long)f->str.n-loc);
		if(nbytes<=0)
			break;
		for(p=f->str.s+loc; p<f->str.s+loc+nbytes; p++)
			b->s[b->n++] = *p;
		n-=nbytes;
		m+=nbytes;
	}
}

/*
 * Return posn of char after first newline after posn, where
 * posn is expressed as a fraction (range 0-YMAX) of the total file length
 */
Forigin(f, posn)
	register File *f;
	register long posn;
{
	register n;
	register long l;
	register char *p;
	posn=((l=length(f))*posn)/YMAX;
	if(posn==0)
		return 0;
	if(posn>=l)
		return l;
	for(;;){
		n=Fseek(f, posn+1)-1;
		if(n<0)
			return(l);
		for(p=f->str.s+n; n<f->str.n; n++, p++, posn++)
			if(*p=='\n')
				return posn+1;
	}
}

/*
 * Count newlines before character position
 */
long
Fcountnl(f, charno)
	register File *f;
	register long charno;
{
	register long posn=0;
	register char *p;
	register nl=1;
	register n;
	charno=min(length(f), charno);
	for(;;){
		if((n=Fseek(f, posn+1)-1)<0)
			return nl;
		for(p=f->str.s+n; n<f->str.n; n++, p++, posn++){
			if(posn >= charno)
				return nl;
			if(*p=='\n')
				nl++;
		}
	}
}
/*
 * Number of characters forward to after nlines'th \n after posn
 */
long
Fforwnl(f, posn, nlines)
	register File *f;
	register long posn;
	register nlines;
{
	register long nchars=0;
	register char *p;
	register long l=length(f);
	register n;

	if(nlines<=0)
		return 0;
	for(;;){
		if(posn>=l || (n=Fseek(f, posn+1)-1)<0)
			return nchars;
		for(p=f->str.s+n; n<f->str.n; n++, p++, posn++, nchars++)
			if(*p=='\n' && --nlines<=0)
				return nchars+1;
	}
}

/*
 * Number of characters backward to after nlines'th \n before posn
 */
long
Fbacknl(f, posn, nlines)
	register File *f;
	register long posn;
	register nlines;
{
	register long nchars=0;
	register char *p;
	register n;

	if(nlines<=0)
		return 0;
	for(;;){
		if(posn <= 0)
			return nchars;
		n=Fseek(f, posn);
		if(n == 0){	/* should never happen */
			--posn;
			continue;
		}
		for(p=f->str.s+n-1; n>0; --n, --p, --posn, nchars++)
			if(*p=='\n' && --nlines<=0)
				return nchars;
	}
}
long
min(a, b)
	register long a, b;
{
	if(a<b)
		return a;
	return b;
}
long
length(f)
	register File *f;
{
	register Block *b=f->block;
	register long n=0;
	while(b<&f->block[f->nblocks])
		n+=b++->nbytes;
	return n;
}
