#include <stdio.h>

#define SZ 512

unsigned char buf[SZ*SZ];

main(argc, argv)
	char **argv;
{
	int fd, ofd;

	if (argc != 2 || (fd = open(argv[1], 0))==-1)
	{	printf("%s\n", (argc!=2)?"usage: filler file":"no such file");
		exit(1);
	}
	read(fd, buf, SZ*SZ); close(fd);
/*
P	work(60, 60);
	work(200, 128);
**
J	work(60, 256);
	work(200, 128);
*/
	work(20, 10);
	work(200, 128);

	work(90, 40);
	work(150,170);
	work(270,100);
	work(345, 56);

	if ((ofd = creat("out", 0666)) == -1)
	{	fprintf(stderr, "cannot open 'out'\n");
		exit(1);
	}
	write(ofd, buf, SZ*SZ);
}

work(x, y)
{	register int i, j=y, k;

	if (y < 0 || y >= SZ) return;
	k = y*SZ+x;
	if (buf[k] == 255) return;
	for (i = x; i < SZ; i++, k++)
	{	if (buf[k] == 255)
			break;
		buf[k] = 255;
	}
	k = y*SZ+x-1;
	for (i = x; i >= 0; i--, k--)
	{	if (buf[k] == 255)
			break;
		buf[k] = 255;
	}
	work(x, y+1);
	work(x, y-1);
}
