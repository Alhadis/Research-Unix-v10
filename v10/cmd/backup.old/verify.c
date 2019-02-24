#include	<fio.h>
#include	"backup.h"

int verbose = 0;

main(argc, argv)
	char **argv;
{
	char *s;

	argv++;
	if(*argv && (strcmp(*argv, "-v") == 0))
		verbose = 1, argv++;
	if(*argv)
		while(*argv)
			vfy(*argv++);
	else
		while(s = Frdline(0))
			vfy(s);
	exit(0);
}

struct backfile h;

vfy(s)
	char *s;
{
	int fd, k;
	char buf[4096];
	long n, size;
	unsigned short csum;

	if((fd = open(s, 0)) < 0){
		perror(s);
		return;
	}
	if(read(fd, (char *)&h, sizeof h) != sizeof h){
		perror("header read");
		close(fd);
		return;
	}
	if((h.version <= 0) || (h.version > CURVERSION))
		Fprint(2, "%s: bad version; got #%x\n", s, h.version);
	else {
		if(h.version != CURVERSION){
			if(verbose)
				Fprint(1, "OLD[%d] ", h.version);
		}
		csum = sum(((unsigned char *)&h)+2, sizeof h - 2, 0);
		for(size = h.sbuf.st_size, n = sizeof buf; size > 0; size -= n){
			if(n > size)
				n = size;
			if((k = read(fd, buf, (int)n)) != n){
				fprint(2, "%s: read(%d) returns %d", s, n, k);
				if(k < 0)
					perror(" ");
				else
					fprint(2, "\n");
				close(fd);
				return;
			}
			csum = sum((unsigned char *)buf, (int)n, csum);
		}
		if(h.checksum == csum){
			if(verbose)
				Fprint(1, "%s: %s -> %s [%s,%s]\n", s, h.oname, h.bname, h.uname, h.gname);
		} else
			Fprint(2, "%s: chksums; file=%ud calc=%ud\n", s, (unsigned short)h.checksum, csum); 
	}
	close(fd);
}
