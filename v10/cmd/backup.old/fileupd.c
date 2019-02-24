#include	<fio.h>
#include	"backup.h"
#include	<utsname.h>
#include	<libc.h>

static char *base;
static nbase;

main()
{
	char *s;

	nbase = 1024;
	base = malloc(nbase);
	while(s = Frdline(0))
		process(s);
	exit(0);
}

usage()
{
	fprint(2, "usage: fileupd\n");
	exit(1);
}

process(s)
	char *s;
{
	int fd;
	struct backfile h;
	struct stat sb;
	long t, time();

	if((fd = open(s, 0)) < 0){
		perror(s);
		exit(1);
	}
	if(fstat(fd, &sb) >= 0)
		t = sb.st_mtime;
	else
		t = time((long *)0);
	if(read(fd, (char *)&h, sizeof h) != sizeof h){
		perror(s);
		close(fd);
		return;
	}
	Fprint(1, "%s%c%s%c%ld%c%d%c%ld%c%s%c%s%c%ld\n", h.oname, 0, h.bname,
		0, h.sbuf.st_ctime, 0, h.sbuf.st_mode, 0, h.sbuf.st_size,
		0, h.uname, 0, h.gname, 0, t);
	if((h.sbuf.st_mode&S_IFMT) == S_IFDIR){
		register char *s;
		int n;

		if(nbase < h.sbuf.st_size){
			nbase = h.sbuf.st_size;
			base = realloc(base, nbase);
		}
		read(fd, base, (int)h.sbuf.st_size);
		if(base[0] || base[1] || base[2] || base[3]){
			n = 0;
			Fwrite(1, (char *)&n, 4L);
		} else {
			for(s = base+4; *s;)
				while(*s++)
					;
			n = 1+s-base;
			Fwrite(1, (char *)&n, 4L);
			Fwrite(1, base, (long)n);
		}
	}
	close(fd);
}
