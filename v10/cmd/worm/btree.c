#include	<libc.h>
#include	<cbt.h>
#undef nfree
#include	"worm.h"
#include	"sym.h"
#include	<sys/types.h>
#include	<sys/stat.h>

static char *inonames;
static bfile *bf;
static dirlk(), wormdir();

char *
cbtinit(s, blk, doinodes)
	register Superblock *s;
	long blk;
{
	static char buf[64];
	char name[256], buf1[256];

	if(s->magic != SMAGIC){
		fprint(2, "bad Superblock at %ld\n", blk);
		exit(1);
	}
	numinodes = s->ninodes;
	if(doinodes){
		inonames = malloc(s->blocksize*(int)NBLKS(s, s->ninochars));
		if(inonames == 0){
			sprint(buf, "cbtinit: can't malloc %d\n", s->blocksize*(int)NBLKS(s, s->ninochars));
			return(buf);
		}
		if(dirlk(s) == 0)
			wormdir(s);
	} else
		inonames = 0;
	return((char *)0);
}

static
dirlk(s)
	register Superblock *s;
{
	char name[256], buf1[256];
	struct stat sbuf;
	int fd;

	sprint(name, "/usr/worm/dirs/%s", s->vol_id);
	sprint(buf1, "%s.I", name);
	if(stat(buf1, &sbuf) < 0)
		return(0);
	if(sbuf.st_mtime < s->ctime)
		return(0);	/* worm is more recent than disk */
	if((bf = bopen(name, 0)) == 0)
		return(0);
	sprint(buf1, "%s.I", name);
	if((fd = open(buf1, 0)) < 0){
		fprint(2, "%s: btree but no inodes\n", name);
		return(0);
	}
	if(read(fd, inonames, (int)s->ninochars) != s->ninochars){
		fprint(2, "%s: expected %d chars\n", buf1, s->ninochars);
		close(fd);
		return(0);
	}
	close(fd);
	return(1);
}

static
wormdir(s)
	register Superblock *s;
{
	char name[256], buf1[256];

	sprint(name, "/tmp/worm%d", getpid());
	Seek(s, s->binodes);
	sprint(buf1, "%s.F", name);
	copyout(s, buf1, s->nF, 0, 0);
	sprint(buf1, "%s.T", name);
	copyout(s, buf1, s->nT, 0, 0);
	if(Read(s, inonames, NBLKS(s, s->ninochars)))
		return(0);
	if((bf = bopen(name, 0)) == 0){
		fprint(2, "can't bopen %s", name);
		return(0);
	}
	sprint(buf1, "%s.F", name); unlink(buf1);
	sprint(buf1, "%s.T", name); unlink(buf1);
	return(1);
}

copyout(s, name, len, overwrite, verbose)
	register Superblock *s;
	char *name;
	long len;
{
	int fd, l;
	char *buf;

	if(access(name, 0) == 0){
		if(!overwrite){
			fprint(2, "%s already exists!\n", name);
			exit(1);
		}
		if(verbose)
			fprint(2, "overwriting %s\n", name);
	}
	if((fd = creat(name, 0666)) < 0){
		perror(name);
		exit(1);
	}
	if((buf = malloc(l = (BIGBLOCK/1024)*s->blocksize)) == 0){
		fprint(2, "can't malloc %d\n", l);
		exit(1);
	}
	if(verbose)
		print("%s: %d bytes\n", name, len);
	while(len >= l){
		if(Read(s, buf, NBLKS(s, l)))
			exit(1);
		if(write(fd, buf, l) != l){
			perror(name);
			exit(2);
		}
		len -= l;
	}
	if(Read(s, buf, NBLKS(s, len)))
		exit(2);
	if(write(fd, buf, (int)len) != len){
		perror(name);
		exit(2);
	}
	free(buf);
}

Inode *
binodefn(s)
	char *s;
{
	static Inode i;
	mbuf key;

	if(inonames == 0)
		return((Inode *)0);
	key.mdata = s;
	key.mlen = strlen(s);
	if(bseek(bf, key) != 1)
		return((Inode *)0);
	key.mdata = (char *)&i;
	if(bread(bf, (mbuf *)0, &key)){
		perror("inode read");
		return((Inode *)0);
	}
	i.name.n = i.name.o+inonames;
	return(&i);
}

void
btraverse(fn)
	void (*fn)();
{
	static Inode i;
	mbuf key;

	if(inonames == 0)
		return;
	bfirst(bf);
	key.mdata = (char *)&i;
	while(bread(bf, (mbuf *)0, &key) == 0){
		i.name.n = i.name.o+inonames;
		(*fn)(&i);
	}
}
