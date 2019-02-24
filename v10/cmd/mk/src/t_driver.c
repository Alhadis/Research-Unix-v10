#include	"mk.h"
#include	<ar.h>

#define	AR	123456L

extern long ftimeof(), atimeof();
extern void ftouch(), atouch();
extern void fdelete(), adelete();
static long split();

struct ftype
{
	long magic;
	long (*time)();
	void (*touch)();
	void (*delete)();
} ftab[] =
{
	{ 0L,	ftimeof,	ftouch,		fdelete },
	{ AR,	atimeof,	atouch,		adelete },
	{ 0L,	(long (*)())0 }
};

long
timeof(name, force)
	char *name;
{
	char buf[BIGBLOCK], *part2;
	register struct ftype *f;
	long magic;

	magic = split(name, buf, &part2);
	for(f = ftab; f->time; f++)
		if(f->magic == magic)
			return((*f->time)(force, name, buf, part2));
	Fprint(2, "mk: '%s' appears to have an unknown magic number (%ld)\n", name, magic);
	Exit();
	return(0L);	/* shut cyntax up */
}

void
touch(name)
	char *name;
{
	char buf[BIGBLOCK], *part2;
	register struct ftype *f;
	long magic;

	magic = split(name, buf, &part2);
	Fprint(1, "touch(%s)\n", name);
	if(nflag)
		return;
	for(f = ftab; f->time; f++)
		if(f->magic == magic){
			(*f->touch)(name, buf, part2);
			return;
		}
	Fprint(2, "mk: give me a break! I never heard of magic=%ld\n", magic);
	Exit();
}

void
delete(name)
	char *name;
{
	char buf[BIGBLOCK], *part2;
	register struct ftype *f;
	long magic;

	magic = split(name, buf, &part2);
	for(f = ftab; f->time; f++)
		if(f->magic == magic){
			(*f->delete)(name, buf, part2);
			return;
		}
	Fprint(2, "mk: give me a break! I never heard of magic=%ld\n", magic);
	Exit();
}

static long
type(file)
	char *file;
{
	int fd;
	char buf[SARMAG];
	short m;
	long goo;

	if(access(file, 0) < 0){
		if(symlook(file, S_BITCH, (char *)0) == 0){
			Fprint(1, "%s doesn't exist: assuming it will be an archive\n", file);
			(void)symlook(file, S_BITCH, file);
		}
		return(AR);
	}
	if((fd = open(file, 0)) < 0){
		perror(file);
		Exit();
	}
	if(read(fd, buf, SARMAG) != SARMAG){
		close(fd);
		return(-1L);
	}
	if(strncmp(ARMAG, buf, SARMAG) == 0)
		goo = AR;
	else {
		lseek(fd, 0L, 0);
		if(read(fd, (char *)&m, sizeof m) == sizeof m)
			goo = m;
		else
			goo = -1;
	}
	close(fd);
	return(goo);
}

static long
split(name, buf, p2)
	char *name, *buf, **p2;
{
	register char *s;

	strcpy(buf, name);
	if(s = strchr(buf, '(')){
		*s++ = 0;
		*p2 = s;
		while(*s && (*s != ')'))
			s++;
		*s = 0;
		return(type(buf));
	} else
		return(0L);
}
