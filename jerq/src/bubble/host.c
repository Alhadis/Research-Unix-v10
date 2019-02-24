#include	<stdio.h>
#include	<signal.h>
#include	<sgtty.h>
#include	"/usr/jerq/include/jioctl.h"

struct sgttyb sttybuf, sttysave;

main(argc, argv)
	char **argv;
{
	char c; FILE *fd;
	float cx, cy, cz, cr;

	if (argc != 2)
	{	fprintf(stderr, "usage: bubble datafile\n");
		exit(0);
	}
	if ((fd = fopen(argv[1], "r")) == NULL)
	{	fprintf(stderr, "cannot open %s\n", argv[1]);
		exit(0);
	}
	loadjerq();
	while (getchar() != '1');

	while (fscanf(fd, "%c %f %f %f %f\n", &c, &cx, &cy, &cz, &cr) == 5)
	{	cx = (cx*25.) + 0.5;
		cy = (cy*25.) + 0.5;
		cz = (cz*25.) + 0.5;
		cr = (cr*25.) + 0.5;
		printf("%dx %dy %dz %c %dr\n",
			(int)cx, (int)cy, (int)cz, c, (int)cr);
	}

	putchar('X');
	fflush(stdout);
	getchar();
	quit(0);
}

loadjerq()
{	if (system("32ld /usr/jerq/mbin/bubble.m")) exit(1);
	ioctl(0, TIOCGETP, &sttysave);
	sttybuf = sttysave;
	sttybuf.sg_flags |= RAW;
	sttybuf.sg_flags &= ~ECHO;
	ioctl(0, TIOCSETP, &sttybuf);
}

quit(s)
	char *s;
{
	ioctl(0, JTERM, (struct sgttyb *)0);
	ioctl(0, TIOCSETP, &sttysave);
	if (s) printf("bubble: %s\n", s);
	exit(0);
}
