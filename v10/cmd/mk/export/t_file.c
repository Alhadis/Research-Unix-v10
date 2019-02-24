#include	"mk.h"
#include	<sys/types.h>
#include	<sys/stat.h>
#include	<errno.h>

#define	MYSEP(s)	((*s == ' ') || (*s == ',') || (*s == '\n'))

timeinit(s)
	register char *s;
{
	register long t;
	register char *os;

	for(t = time((long *)0); *s; s = os){
		for(os = s; !MYSEP(os); os++)
			if(*os == 0){
				symlook(strdup(s), S_TIME, (char *)t)->value = (char *)t;
				return;
			}
		*os++ = 0;
		symlook(strdup(s), S_TIME, (char *)t)->value = (char *)t;
		while(MYSEP(os))
			os++;
	}
}

long
ftimeof(force, name)
	char *name;
{
	struct stat sbuf;
	Symtab *sym;

	if(!force && (sym = symlook(name, S_TIME, (char *)0))){
		return((long)sym->value);
	}
	if(stat(name, &sbuf) < 0)
		sbuf.st_mtime = 0;
	symlook(name, S_TIME, (char *)sbuf.st_mtime);
	return((long)sbuf.st_mtime);
}

void
ftouch(name)
	char *name;
{
	int fd;
	char buf[1];
	struct stat sbuf;

	if(stat(name, &sbuf) < 0)	/* not there and zero length are same case */
		sbuf.st_size = 0;
	if(sbuf.st_size == 0){
		if((fd = creat(name, 0666)) < 0){
	bad:
			perror(name);
			Exit();
		}
		close(fd);
		return;
	}
	if((fd = open(name, 2)) < 0)
		goto bad;
	if(read(fd, buf, 1) != 1)
		goto bad;
	if(lseek(fd, 0L, 0) < 0)
		goto bad;
	if(write(fd, buf, 1) != 1)
		goto bad;
	close(fd);
}

void
fdelete(s)
	char *s;
{
	if(unlink(s) < 0)
		perror(s);
}
