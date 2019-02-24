#include	<stdio.h>
#include	<signal.h>
#include	<sgtty.h>
#include	"piclib.h"
#include	"/usr/jerq/include/jioctl.h"

#define	MAXPIXEL	512

struct sgttyb sttybuf, sttysave;
int frameno=0, fullsize=0, tiny=0, medium=0, dither=1;

Usage()
{	fprintf(stderr, "usage: flicks [-fmte] files\n");
	fprintf(stderr, "	-f,m,t fullsize, medium, or tiny\n");
	fprintf(stderr, "	-e   error propagation\n");
	exit(1);
}

main(argc, argv)
	char **argv;
{	char c;
	int n=1;

	if (argc > 1 && argv[1][0] == '-')
	{	while ((c = argv[1][n++]) != '\0')
		{	switch (c) {
			case 'f': fullsize = 1; break;
			case 'm': medium = 1; break;
			case 't': tiny = 1; break;
			case 'e': dither = 0; break;
			default : Usage(); break;
			}
		}
		argv++; argc--;
	}
	if (argc < 2)
		Usage();

	if (!fullsize && !tiny && !medium)
	{	if (argc <= 2)
			fullsize = 1;
		else if (argc > 12)
			tiny = 1;
	}

	loadjerq();
	while (getchar() != 103) ;
	sendn(argc-1);
	for (n = 1, argv++; n < argc; n++)
	{	while (getchar() != 103) ;
		if (dither)
		{	if (!dsend(*argv++)) break;
		} else
		{	if (!esend(*argv++)) break;
		}
	}
	fflush(stdout);
	getchar();

	sendn(-1);
	fflush(stdout);
	(void)getchar();
	quit(0);
}

loadjerq()
{
	if (system("/usr/jerq/bin/32ld /usr/jerq/mbin/flicks.m")) exit(1);
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
	if (s) printf("flicks: %s\n", s);
	exit(0);
}

#define	DITHSIZE	16
#define	DITHMASK	(DITHSIZE-1)
#define DITHMAX		DITHSIZE*DITHSIZE-1
#define ZUNK		if ((data&DITHMAX) < dth[x&DITHMASK]) byte |= mask;

int dith[DITHSIZE][DITHSIZE]={
#include "udither.data"
};

dsend(file)
	char *file;
{
	register byte=0, mask=0x80, data, col;
	int fd, x, y, ll, ht, len=0, row;
	register unsigned char *s;
	register int *dth;
	struct pfile image;

	if((fd = openf(file, &image)) == -1 || !readf(fd, &image))
		return 1;	/* skip it */

	len = image.r.corner.x - image.r.origin.x;
	ht  = image.r.corner.y - image.r.origin.y;
	s = image.pixred;
	ll = len;
	if (!fullsize) len /= 2, ht /= 2;
	if (tiny) len /= 2, ht /= 2;
	sendn(frameno++); sendn(len); sendn(ht);
	if (getchar() != 103) return 0;	/* jerq out of mem */

	row = ((len+31)&(~31))/8;	/* assumes 32bit allignment in jerq */

	for (y = ht-1; y >= 0; y--, s += (!fullsize)?ll:0)
	{
		dth = dith[y&DITHMASK];
		for(x = len-1, col = 0; x >= 0; x--)
		{	data = (unsigned char)*s++;
			if (!fullsize)
			{	data += (unsigned char)*s++;
				data += (unsigned char)*(s+ll-1);
				data += (unsigned char)*(s+ll);
				data >>= 2;
				if (tiny) s += 2;
			}
			ZUNK;
			mask >>= 1;
			if (mask == 0)
			{	putchar(byte); col++;
				byte = 0; mask = 0x80; 
			}
		}
		if (mask != 0x80)
		{	putchar(byte); col++;
			byte = 0; mask = 0x80; 
		}
		while(col++ < row)
			putchar(byte);
		if (tiny) s += 2*ll;
	}
	return 1;
}

short cnt[MAXPIXEL+1][MAXPIXEL+1];

esend(file)
	char *file;
{
	register byte = 0, mask = 0x80, data;
	int fd, x, y, ll, ht, len=0, error;
	register unsigned char *s;
	struct pfile image;

	if((fd = openf(file, &image)) == -1 || !readf(fd, &image))
		return 1;	/* skip it */

	len = image.r.corner.x - image.r.origin.x;
	ht  = image.r.corner.y - image.r.origin.y;
	s = image.pixred;

	ll = len;
	if (!fullsize) len /= 2, ht /= 2;
	if (tiny) len /= 2, ht /= 2;
	if (len > MAXPIXEL || ht > MAXPIXEL)
		quit("too large, aborting\n");
	sendn(frameno++); sendn(len); sendn(ht);
	if (getchar() != 103) return 0;	/* jerq out of mem */

	for(y = ht-1; y >= 0; y--, s += (!fullsize)?ll:0)
	{	for(x = len-1; x >= 0; x--)
		{	data = (unsigned char)*s++;
			if (!fullsize)
			{	data += (unsigned char)*s++;
				data += (unsigned char)*(s+ll-1);
				data += (unsigned char)*(s+ll);
				data >>= 2;
				if (tiny) s += 2;
			}
			cnt[x][y] = data;
		}
		if (tiny) s += 2*ll;
	}
	for (y = len-1; y >= 0; y--)
	for (x = len-1; x >= 0; x--)
	{	if (cnt[x][y] < 128)
		{	error = cnt[x][y];
			byte |= mask;
		} else
			error = cnt[x][y]-255;
		error /= 4;
		if (x)		cnt[x-1][y] += error;
		if (y)		cnt[x][y-1] += error;
		if (x && y)	cnt[x-1][y-1] += error;
		mask >>= 1;
		if (mask == 0)
		{	putchar(byte);
			byte = 0; mask = 0x80; 
		}
	}
	return 1;
}

sendn(n)
{	putchar(n>>8);
	putchar(n);
}
