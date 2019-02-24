#include	<stdio.h>
#include	<signal.h>
#include	<sgtty.h>
#include	"/usr/jerq/include/jioctl.h"

#define Version		10	/* should match the one in preflicks */

unsigned char *bits;

struct sgttyb sttybuf, sttysave;
int frameno=0;

main(argc, argv)
	char **argv;
{	int fd; char c, file[128];

	bits = (unsigned char *) malloc(512*512);

	strcpy(file, (argc > 1) ? *(argv+1) : "_movie");
	if ((fd = open(file, 0)) == -1)
	{	fprintf(stderr, "fflicks: cannot open %s\n", file);
		exit(0);
	}
	read(fd, &c, 1);
	if (c != Version)
	{	fprintf(stderr, "fflicks: %s is outdated (version %d)\n", file, c);
		exit(0);
	}
	loadjerq("/usr/jerq/bin/32ld /usr/jerq/mbin/fflicks.m");
	putdicts(fd);
	dsend(fd);

	sendn(-1);
	fflush(stdout);
	getchar();
	quit(0);
}

loadjerq(buf)
	char *buf;
{
	if (system(buf)) exit(1);
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
	if (s) printf("fflicks: cannot open %s\n", s);
	exit(0);
}

putdicts(ifd)
{	register int len;
	register unsigned char *s = bits;

	if (read(ifd, bits,   2) !=   2) quit("bad infile 1");
	len = bits[0]; len = (len<<8) + bits[1];
	if (read(ifd, bits, len) != len) quit("bad infile 2");
	for (s = bits; s < bits+len; )
		putchar(*s++);
}

dsend(ifd)
{	int len, width, ht, n, m;
	unsigned char *s,*t;

	while ((n=read(ifd, bits, 6)) == 6)
	{	getchar();	/* synchronize: jerq ready */
		width = bits[0]; width = (width << 8) + bits[1];
		ht    = bits[2]; ht    = (ht    << 8) + bits[3];
		len   = bits[4]; len   = (len   << 8) + bits[5];
		if ((n=read(ifd, bits, len+1)) != len+1) break;
		sendn(frameno++);
		sendn(width);
		sendn(ht);
		sendn(len);
		if (getchar() == '0') break;	/* jerq out of mem  */
		for (s = bits, t = bits+len+1; s < t; )
			putchar(*s++);
	}
	getchar();
}

sendn(n)
{	putchar(n>>8);
	putchar(n&255);
}
