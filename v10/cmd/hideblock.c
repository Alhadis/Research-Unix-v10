/*
 * hideblock filesystem(not raw) file b1 b2 ...
 *  -- puts blocks in named file in file system.
 */

#include <stdio.h>
#include <sys/param.h>
#include <sys/ino.h>
#include <sys/stat.h>

#ifdef	BITFS
#define	Bsize	BSIZE(dev)
#define	Inopb	INOPB(dev)
#else
#define	Bsize	BSIZE
#define	Inopb	INOPB
#endif

int	fd;
struct	dinode di;
ino_t	inode;
char	cntflag;
long	count;
long	size;
dev_t	dev;
int	writing;

main(argc,argv)
char **argv;
{
	long pos;
	int i,j;
	long addr[13];
	char *file;
	struct stat s;

	if(argc<3) {
		printf("usage: %s non-raw-fs file b1 b2 ... (512)\n", argv[0]);
		exit(1);
	}
	writing = argc>3;
	if ((fd=open(argv[1], writing?2:0)) < 0) {
		perror(argv[1]);
		printf("%s couldn't open %s\n", argv[0], argv[1]);
		exit(1);
	}
	if (stat(argv[2], &s) < 0) {
		printf("%s: %s must exist\n", argv[0], argv[2]);
		exit(1);
	}
	if (s.st_size != 0 && argc>3) {
		printf("%s: %s must be 0 length\n", argv[0], argv[2]);
		exit(1);
	}
	inode = s.st_ino;
	dev = s.st_dev;
	pos = ((long)inode-1)/Inopb*Bsize+((inode-1)%Inopb)*sizeof(struct dinode);
	pos += 2*Bsize;
	lseek(fd, pos, 0);
	bread(fd, &di, sizeof(di));
	l3tol(addr, di.di_addr, 13);
	for (i = 0; i < 10; i++) {
		if (addr[i])
			printf(" %ld", addr[i]*(Bsize/512));
		else
			break;
	}
	argc -= 2;
	argv += 2;
	while (argc > 1) {
		if (i >= 10) {
			printf("\nhideblock: at most 10 blocks\n");
			break;
		}
		addr[i] = atol(argv[1])/(Bsize/512);
		printf(" %ld", addr[i]*(Bsize/512));
		i++;
		argc--;
		argv++;
	}
	printf("\n");
	if (writing) {
		ltol3(di.di_addr, addr, 13);
		lseek(fd, pos, 0);
		bwrite(fd, &di, sizeof(di));
	}
	return(0);
}

bread(f, b, c)
char *b;
{
	register n;

	n = read(f, b, c);
	if (n!=c)
		printf("bad read on %d: %d\n", f, n);
}

bwrite(f, b, c)
char *b;
{
	register n;

	n = write(f, b, c);
	if (n!=c)
		printf("bad write on %d: %d\n", f, n);
}
