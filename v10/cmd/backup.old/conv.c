#include	<fio.h>
#include	"backup.h"

#define		CLEN		(960-sizeof (struct stat))

struct obackfile
{
	struct stat sbuf;	/* original inode */
	char name[CLEN];	/* original file name */
	char uname[64];		/* original user name */
};

int force = 0;

main(argc, argv)
	char **argv;
{
	char *s;

	if(*++argv)
		if(strcmp(*argv, "-") == 0){
			force = 1;
			argv++;
		}
	if(*argv)
		while(*argv)
			conv(*argv++);
	else
		while(s = Frdline(0))
			conv(s);
	exit(0);
}

struct backfile h;
struct obackfile oh;

conv(s)
	char *s;
{
	int fd;
	char buf[4096];
	long n, nn, size;
	unsigned short csum;

	if((fd = open(s, 2)) < 0){
		perror(s);
		return;
	}
	if(read(fd, (char *)&h, sizeof h) != sizeof h){
		perror("header read");
		close(fd);
		return;
	}
	if((force == 0) && (h.version == CURVERSION))
		Fprint(1, "%s: not converted\n", s);
	else {
		oh = *((struct obackfile *)&h);
		memset((char *)&h, 0, sizeof h);
		h.version = 1;
		h.sbuf = oh.sbuf;
		chown(s, h.sbuf.st_uid, h.sbuf.st_gid);
		sprint(h.gname, "#%d", h.sbuf.st_gid);
		strncpy(h.uname, oh.uname, 15);
		h.uname[15] = 0;
		strncpy(h.bname, s, FNAMELEN);
		h.bname[FNAMELEN-1] = 0;
		strncpy(h.oname, oh.name, FNAMELEN);
		h.oname[FNAMELEN-1] = 0;
	again:
		csum = sum(((unsigned char *)&h)+2, sizeof h - 2, 0);
		for(size = h.sbuf.st_size, n = sizeof buf; size > 0; size -= n){
			if(n > size)
				n = size;
			if((nn = read(fd, buf, (int)n)) != n){
				if(nn < 0){
					fprint(2, "%s: ", s), perror("read");
					close(fd);
					return;
				} else {
					h.sbuf.st_size -= size-nn;
					lseek(fd, (long)sizeof h, 0);
					fprint(2, "%s: resetting size to %ld\n", s, h.sbuf.st_size);
					goto again;
				}
			}
			csum = sum((unsigned char *)buf, (int)n, csum);
		}
		h.checksum = csum;
		lseek(fd, 0L, 0);
		if(write(fd, (char *)&h, sizeof h) != sizeof h)
			fprint(2, "%s: ", s), perror("write");
	}
	close(fd);
}
