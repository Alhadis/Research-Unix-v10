#include	<libc.h>
#include	"worm.h"
#include	<sys/types.h>
#include	<sys/udaioc.h>

Inode *(*inodefn)();
void (*traversefn)();
extern Inode *vinodefn(), *binodefn();
extern void vtraverse(), btraverse();
extern char *lkopi(), *cbtinit();
long numinodes;
long numnamechars;

char *
openinode(s, flags)
	register Superblock *s;
{
	short fd = s->fd;
	char *b, *z;
	long blk;
	unsigned short ibuf[3];
	static char buf[64];
	extern char *getenv();
	extern long atol();
int goo; extern errno; int ntry;

	if(z = getenv("WORMZERO"))
		blk = atol(z);
	else
		blk = 0;
	ntry = 0;
loop:
	bigseek(s->fd, blk, 1024, 0);
	if(((goo = read(s->fd, (char *)ibuf, sizeof ibuf)) != sizeof ibuf) ||
			((((long)ibuf[1])<<16|ibuf[0]) != SMAGIC)){
		if((goo < 0) && (errno == ENXIO))
			blk++;	/* blank check (unwritten), try next blk */
		else if((goo == sizeof ibuf) && (ibuf[0] == 0) && (ibuf[1] == 0))
			blk++;	/* zeroes(??), try next blk */
		else {
			fprint(2, "DEBUGGING[%d]: read=%d magic=0x%lx errno=%d\n",
				ntry, goo, (((long)ibuf[1])<<16|ibuf[0]), errno);
			if(ntry++ < 3)
				goto loop;
		}
		bigseek(s->fd, blk, 1024, 0);
		if(read(s->fd, (char *)ibuf, sizeof ibuf) < sizeof ibuf)
			return("no block size");
	}
	if(flags&SPIN_DOWN)
		ioctl(s->fd, UIOSPDW);
	s->blocksize = ibuf[2];		/* magic is 0-1 */
	if((b = malloc(s->blocksize)) == 0){
		sprint(buf, "couldn't malloc buffer (%d bytes)", s->blocksize);
		return(buf);
	}
	bigseek(s->fd, blk, 1024, 0);
	if(read(s->fd, b, s->blocksize) != s->blocksize)
		return("superblock read error");
	*s = *((Superblock *)b);
	free(b);
	s->fd = fd;
	if(s->myblock == 0)
		s->myblock = blk;
	switch(s->version)
	{
	case VLINK:
		if(flags&DO_INODE){
			Superblock tmpsb;

			tmpsb = *s;
			if(b = lkopi(s, blk, 0))
				return(b);
			if(fastlink(s, &b, &inodefn, &traversefn))
				return(b);
			*s = tmpsb;
		}
		inodefn = vinodefn;
		traversefn = vtraverse;
		return(lkopi(s, blk, flags&DO_INODE));
	case VBTREE:
		inodefn = binodefn;
		traversefn = btraverse;
		return(cbtinit(s, blk, flags&DO_INODE));
	default:
		sprint(buf, "unknown version %d@%ld", s->version, blk);
		return(buf);
	}
}
