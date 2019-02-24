#include	<libc.h>
#include	"worm.h"
#include	"sym.h"

char *
lkopi(s, blk, doinodes)
	register Superblock *s;
	long blk;
{
	register Inode *i;
	short fd = s->fd;
	char *b;
	long nb;
	char *nameb;
	Inode *inodes;
	static char buf[64];

	if((b = malloc(s->blocksize)) == 0){
		sprint(buf, "couldn't malloc buffer (%d bytes)", s->blocksize);
		return(buf);
	}
	numinodes = 0;
	numnamechars = 0;
	for(;;){
		if(s->magic != SMAGIC){
			fprint(2, "bad Superblock at %ld\n", blk);
			exit(1);
		}
		if(s->ninodes){
			numinodes += s->ninodes;
			numnamechars += s->ninochars;
		}
		if(doinodes && s->ninodes){
			nb = (s->ninodes+IPERB-1)/IPERB;
			inodes = (Inode *)malloc((unsigned)(s->blocksize*nb));
			if(inodes == 0){
				sprint(buf, "inode malloc(%d) fail, sbrk=%d\n",
					(s->blocksize*nb), sbrk(0));
				return(buf);
			}
			Seek(s, s->binodes);
			if(Read(s, (char *)inodes, nb))
				goto skip;
			nb = (s->ninochars+s->blocksize-1)/s->blocksize;
			nameb = malloc((unsigned)(s->blocksize*nb));
			if(nameb == 0){
				sprint(buf, "name buffer malloc(%d) fail, sbrk=%d\n",
					(s->blocksize*nb), sbrk(0));
				return(buf);
			}
			if(Read(s, nameb, nb))
				goto skip;
			for(nb = 0, i = inodes; nb < s->ninodes; nb++, i++){
				i->name.n = i->name.o+nameb;
if(strcmp(i->name.n, "v14725/791")==0) print("%s at block %d\n", i->name.n, i->block);
				if(i->block < 0)
					(void)symdel(i->name.n, S_INODE);
				else {
					(void)symlook(i->name.n, S_INODE, (void *)i);
				}
			}
			if(sym_mem_fail){
				sprint(buf, "%d inode malloc fails: %d, %d sbrk=%d\n",
					sym_mem_fail, numinodes, s->ninodes, sbrk(0));
				return(buf);
			}
		}
	skip:
		blk = s->nextsb;
		Seek(s, blk);
		if(Read(s, b, 1L))
			break;
		*s = *((Superblock *)b);
		s->fd = fd;
		if(s->myblock == 0)
			s->myblock = blk;
	}
	free(b);
	return((char *)0);
}

char *
lkwri(s, i, ni, c, nc, ndata)
	Superblock *s;
	Inode *i;
	long ni, nc, ndata;
	char *c;
{
	char *b;
	long blk;
	static char buf[256];
	long ib, ic;

	s->ninodes = ni;
	s->ninochars = nc;
	ib = (ni+IPERB-1)/IPERB;
	ic = (nc+s->blocksize-1)/s->blocksize;
	if(ndata+ib+ic+1 > s->nfree)		/* one for superblock */
		return("not enough space for new files");
	s->binodes = s->nextffree+ndata;
	s->nextffree += ndata+ib+ic;
	s->nfree -= ndata+ib+ic;

	if((b = malloc(s->blocksize)) == 0){
		sprint(buf, "couldn't malloc buffer (%d bytes)", s->blocksize);
		return(buf);
	}
	blk = s->nextsb;
	s->nextsb = s->nextffree++;
	s->nfree--;
	s->myblock = blk;
	time(&s->ctime);
	memset(b, 0, s->blocksize);
	*((Superblock *)b) = *s;
	Seek(s, blk);
	if(Write(s, b, 1L)){
		sprint(buf, "couldn't write superblock at %d", blk);
		return(buf);
	}
	free(b);
	Seek(s, s->binodes);
	if(Write(s, (char *)i, ib))
		return("write1 error");
	if(Write(s, c, ic))
		return("write2 error");
	return((char *)0);
}


char *
lkwsb(s)
	Superblock *s;
{
	char *b;
	long blk;
	static char buf[64];

	if((b = malloc(s->blocksize)) == 0){
		sprint(buf, "couldn't malloc buffer (%d bytes)", s->blocksize);
		return(buf);
	}
	blk = s->nextsb;
	s->nextsb = s->nextffree++;
	s->nfree--;
	memset(b, 0, s->blocksize);
	s->myblock = blk;
	*((Superblock *)b) = *s;
	Seek(s, blk);
	if(Write(s, b, 1L))
		return("couldn't write superblock");
	free(b);
	return((char *)0);
}

Inode *
vinodefn(s)
	char *s;
{
	return((Inode *)symlook(s, S_INODE, (void *)0));
}

void
vtraverse(fn)
	void (*fn)();
{
	symtraverse(S_INODE, fn);
}
