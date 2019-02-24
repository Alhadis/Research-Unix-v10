#include "stdio.h"
extern int errno;

char buf[512];

main(argc, argv)
char **argv;
{	int fd, n, i, j;
	short *p = (short *) buf;
	if(argc != 3) {
		fprintf(stderr, "%s raw-disk sn\n", argv[0]);
		exit(0);
	}
	fd = open(argv[1], 2);
	n = atoi(argv[2]);
	if(fd < 0 || lseek(fd, n*512, 0) < 0) {
		perror("begin");
		exit(1);
	}
foo:
	lseek(fd, n*512, 0);
	if (read(fd, buf, sizeof(buf)) != sizeof(buf))
		perror("read");
	for(i = 0; i < 16; i++) {
		for(j = 0; j < 16; j++)
			printf("%o ", p[16 * i + j] & 0xffff);
		putchar('\n');
	}
	printf("write?\n");
	if((i = getchar()) == 'y') {
wr:
		lseek(fd, n*512, 0);
		if (write(fd, buf, sizeof(buf)) != sizeof(buf))
			perror("write");
		(void) getchar();
		goto foo;
	}
	else if(i == 'c') {
		for(j = 0; j < 512; j++)
			buf[j] = 0;
		goto wr;
	}
}
