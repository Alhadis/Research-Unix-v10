#include	<libc.h>
#include	<fio.h>
#include	"worm.h"

int bad = 0;
long nbytes;
long blkdone;
long nfiles;
char *argout;

static Inode *inodes;
static long ip;
static long ninodes = 0;
static char *nameb;
static long np;
static long nnameb = 0;
static long nblocks;
#define		IINC		1024
#define		NINC		(64*IINC)

ininit()
{
	if(nnameb == 0){
		nameb = malloc((unsigned)(nnameb = NINC));
		if(nameb == 0){
			fprint(2, "wwrite: malloc fail, %d bytes\n", nnameb);
			exit(1);
		}
	}
	np = 0;
	if(ninodes == 0){
		inodes = (Inode *)malloc(sizeof(Inode)*(unsigned)(ninodes = IINC));
		if(inodes == 0){
			fprint(2, "wwrite: malloc fail, %d inodes %d bytes\n", ninodes, ninodes*sizeof(Inode));
			exit(1);
		}
	}
	ip = 0;
	nblocks = 0;
}

inadd(s, i)
	Superblock *s;
	register Inode *i;
{
	register long len;

	len = strlen(i->name.n)+1;
	if(np+len > nnameb){
		while(np+len > nnameb)
			nnameb += NINC;
		nameb = realloc(nameb, (unsigned)nnameb);
		if(nameb == 0){
			fprint(2, "wwrite: realloc fail, %d bytes\n", nnameb);
			exit(1);
		}
	}
	strcpy(nameb+np, i->name.n);
	i->name.o = np;
	np += len;
	i->block = s->nextffree + nblocks;
	if(ip == ninodes){
		ninodes += IINC;
		inodes = (Inode *)realloc((char *)inodes, (unsigned)ninodes*sizeof(Inode));
		if(inodes == 0){
			fprint(2, "wwrite: realloc fail, %d inodes %d bytes\n", ninodes, ninodes*sizeof(Inode));
			exit(1);
		}
	}
	inodes[ip++] = *i;
	nblocks += (i->nbytes+s->blocksize-1)/s->blocksize;
	return(0);
}

inwrite(s, arg)
	Superblock *s;
	void *arg;
{
	int i, j;
	long next = s->nextffree;
	char *e;

	if(e = lkwri(s, inodes, ip, nameb, np, nblocks)){
		fprint(2, "%s\n", e);
		bad = 1;
		return;
	}
	Seek(s, next);
	argout[2] = ' ';
	for(i = 0; i < ip; i++){
		inodes[i].block = next;
		inodes[i].name.n = inodes[i].name.o  + nameb;
		writeout(s, &inodes[i], &next, arg);
		j = (blkdone*100)/nblocks;
		argout[0] = j/10+'0';
		argout[1] = j%10+'0';
	}
}
