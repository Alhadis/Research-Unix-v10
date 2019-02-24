#include	<libc.h>
#include	"worm.h"

static Inode *inodebase;
static char *namebase;
static long nnames, ninodes;
static char *expanded;
static readinodes;
static ifd;

static Inode *diskinode();

static Inode *
finode(s)
	char *s;
{
	register lo, hi, m;
	register Inode *i;

	if(readinodes == 0)
		return(diskinode(s));
#define	EXPAND(in)	(i = inodebase+(in), expanded[in]?0:(i->name.n=i->name.o+namebase, expanded[in]=1))
#define	CMP(str, in)	(EXPAND(in), strcmp(str, i->name.n))

	if(CMP(s, lo = 0) < 0)
		return(0);
	if(CMP(s, (hi = ninodes)-1) > 0)
		return(0);
	while(lo < hi-1){
		m = (lo+hi)/2;
		if(CMP(s, m) < 0)
			hi = m;
		else
			lo = m;
	}
	if(CMP(s, lo) == 0)
		return(inodebase+lo);
	else
		return(0);
}

static void
ftraverse(fn)
	void (*fn)();
{
	register Inode *i;
	register n;

	if(readinodes == 0){
		readinodes = 1;
		lseek(ifd, 8L, 0);
		read(ifd, (char *)inodebase, (int)ninodes*sizeof(Inode));
	}
	for(n = 0; n < ninodes; n++){
		EXPAND(n);
		(*fn)(i);
	}
}

fastlink(s, msg, ifn, tfn)
	Superblock *s;
	char **msg;
	Inode *(**ifn)();
	void (**tfn)();
{
	int n;
	long t;
	static char buf[256];

	*msg = 0;
	*ifn = finode;
	*tfn = ftraverse;
	sprint(buf, "/usr/worm/tmp/%s", s->vol_id);
	if((ifd = open(buf, 0)) < 0)
		return(0);
	read(ifd, (char *)&t, 4);
	if(t != s->ctime)
		return(0);
	read(ifd, (char *)&ninodes, 4);
	inodebase = (Inode *)malloc(n = ninodes*sizeof(Inode));
	expanded = malloc((int)ninodes);
	memset(expanded, 0, (int)ninodes);
	lseek(ifd, (long)n, 1);
	readinodes = 0;
	read(ifd, (char *)&nnames, 4);
	namebase = malloc((int)nnames);
	read(ifd, namebase, (int)nnames);
	numinodes = ninodes;
	numnamechars = nnames;
	return(1);
}

static
readi(ino, ip)
	Inode *ip;
{
	static myino = -1;
	static Inode myi;

	if(ino != myino){
		myino = ino;
		lseek(ifd, 8L+myino*sizeof(Inode), 0);
		read(ifd, (char *)&myi, sizeof myi);
		myi.name.n = myi.name.o+namebase;
	}
	*ip = myi;
}

static Inode *
diskinode(s)
	char *s;
{
	register lo, hi, m;
	static Inode ii;
	register Inode *i = &ii;

#define READ(in)	readi(in, i)
#undef	CMP
#define	CMP(str, in)	(READ(in), strcmp(str, i->name.n))

	if(ninodes <= 0)
		return(0);
	if(CMP(s, lo = 0) < 0)
		return(0);
	if(CMP(s, (hi = ninodes)-1) > 0)
		return(0);
	while(lo < hi-1){
		m = (lo+hi)/2;
		if(CMP(s, m) < 0)
			hi = m;
		else
			lo = m;
	}
	if(CMP(s, lo) == 0)
		return(i);
	else
		return(0);
}
