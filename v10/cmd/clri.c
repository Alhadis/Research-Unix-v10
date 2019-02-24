/*
 * clri filsys inumber ...
 */

#include <sys/param.h>
#include <sys/ino.h>
#include <sys/stat.h>

struct	ino
{
	char	junk[sizeof(struct dinode)];
} buf[INOPB(64)];

int	status;
dev_t	dev;

main(argc, argv)
char *argv[];
{
	register i, f;
	unsigned n;
	int j, k;
	long off;
	struct stat statb;

	if(argc < 3) {
		printf("usage: clri filsys inumber ...\n");
		exit(4);
	}
	f = open(argv[1], 2);
	if(f < 0) {
		printf("cannot open %s\n", argv[1]);
		exit(4);
	}
	fstat(f, &statb);
	dev = statb.st_rdev;
	for(i=2; i<argc; i++) {
		if(!isnumber(argv[i])) {
			printf("%s: is not a number\n", argv[i]);
			status = 1;
			continue;
		}
		n = atoi(argv[i]);
		if(n == 0) {
			printf("%s: is zero\n", argv[i]);
			status = 1;
			continue;
		}
		off = itod(dev, n) * BSIZE(dev);
		lseek(f, off, 0);
		if(read(f, (char *)buf, BSIZE(dev)) != BSIZE(dev)) {
			printf("%s: read error\n", argv[i]);
			status = 1;
		}
	}
	if(status)
		exit(status);
	for(i=2; i<argc; i++) {
		n = atoi(argv[i]);
		printf("clearing %u\n", n);
		off = itod(dev, n) * BSIZE(dev);
		lseek(f, off, 0);
		read(f, (char *)buf, BSIZE(dev));
		j = itoo(dev, n);
		for(k=0; k<sizeof(struct dinode); k++)
			buf[j].junk[k] = 0;
		lseek(f, off, 0);
		write(f, (char *)buf, BSIZE(dev));
	}
	exit(status);
}

isnumber(s)
char *s;
{
	register c;

	while(c = *s++)
		if(c < '0' || c > '9')
			return(0);
	return(1);
}
