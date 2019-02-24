#include "I_lut.h"

#define	MAX	4096		/* max no of 16bit words per read */

/*
	set, on the scanner console:
	off	normalization		(N menu) (press 'N' <enter>)
	off	log lookup tables	(N menu)
	ON	log circuit		(N menu)
	off	historical calibration	(H menu)
*/

#define	IMRESET	(('i'<<8)|1)
#define	IMWAIT	(('i'<<8)|2)

int printf;		/* andrew doesn't like printf */

int SCALE = 4;		/* reduction factor of 4 */
int LENS  = 0;		/* defaults to 8.5 inch lens */
int TRANS = 0;		/* reflection scanning is default */
int ALLOC = 0;		/* pre-allocate memory */
int correct = 0;

Usage()
{
	print("usage: imscan [-sN] [-lN] [-t] [-r] file\n");
	print("\t-sN	reduction factor N:[1-9] (default is 4)\n");
	print("\t-lN	N=5 for 5 inch lens; N=8 for 8.5 inch lens\n");
	print("\t	5=754 dpi, 8=480 dpi (the default is 8)\n");
	print("\t-t	transmission (glass plate)\n");
	print("\t-r	reflection   (metal plate - default)\n");
	print("\t-n	apply image correction\n");
	print("\t-aN	pre-allocate for an Nbyte (N=wxh) image\n");
	exit(1);
}

main(argc, argv)
	char **argv;
{	int fd, ofd;

	while (argc > 1 && argv[1][0] == '-')
	{	switch (argv[1][1]) {
		case 's':	if (argv[1][2] < '1' || argv[1][2] > '9')
					Usage();
				SCALE = argv[1][2] - '0';
				break;
		case 'l':	if (argv[1][2] != '5' && argv[1][2] != '8')
					Usage();
				LENS = (argv[1][2] == '5');
				break;
		case 't':	TRANS = 1; break;
		case 'r':	TRANS = 0; break;
		case 'n':	correct = 1-correct; break;
		case 'a':	if (argv[1][2] < '1' || argv[1][2] > '9')
					Usage();
				ALLOC = atoi(&argv[1][2]);
				break;
		default :	Usage();
		}
		argc--; argv++;
	}
	if (argc < 2)
		Usage();

	print("reduction factor %d, ", SCALE);
	print("%s lens, ", (LENS)?"5 inch":"8.5 inch");
	print("%s scan, ", (TRANS)?"transmission":"reflection");
	print("image file: %s\n", argv[1]);

	if ((fd = open("/dev/im0", 2)) < 0)
	{	perror("/dev/im0");
		exit(1);
	}
	if ((ofd = creat(argv[1], 0666)) < 0)
	{	perror(argv[1]);
		exit(1);
	}
	seticonfig(fd);
	setmodes(fd);
	scan(fd, ofd);

	exit(0);
}

scan(fd, ofd)
{	unsigned short buf1[32], buf2[32];
	int tpix, mode, npix, nlin, spl, NLINES;
	char *im, *sim, *malloc();
	register char *pim;
	register int n, tlin;

	if (ALLOC)
		pim = im = (char *) malloc(ALLOC);

	print("scan preliminaries\n");
	buf1[0] = 0x100;		/* manual swab */
	buf1[1] = 0x400;		/* SCAN1START swabbed */

	if (write(fd, buf1, 4) != 4)	{ perror("write"); return; }
	if (ioctl(fd, IMWAIT) < 0)	{ perror("imwait1"); return; }
	if (read(fd,  buf1, 12) != 12)	{ perror("read"); return; }

	swab(&buf1[1], buf2, 10);

	npix = (int) buf2[2];
	nlin = (int) buf2[3];
	mode = (int) buf2[4];

	buf1[0] = 5;
	buf1[1] = 5;			/* SCAN2START */
	buf1[2] = SCALE*256;		/* XSCALE */
	buf1[3] = SCALE*256;		/* YSCALE */
	buf1[4] = 0;			/* no run length encoding */
	buf1[5] = 0;			/* no pixel replication   */
	swab(buf1, buf2, 12);

	if (write(fd, buf2, 12) != 12)	{ perror("scan2 write"); return; }
	if (read(fd,  buf1, 12) != 12)	{ perror("scan2 read");  return; }

	swab(&buf1[1], buf2, 10);

	tpix = (int) buf2[2];
	tlin = (int) buf2[3];
	mode = (int) buf2[4];

	spl  = (tpix+15)/16;		/* line size, in 16-bit words */
	spl *= 8;			/* one byte per pixel  */
	NLINES = MAX/spl;
	n    = (NLINES*spl+3)*2;	/* no of bytes to read per chunk */

	print("%dx%d -> %dx%d pixel\n", npix, nlin, 2*spl, tlin);
	if (mode != 2) { print("error: not in b&w mode\n"); return; }
	if (NLINES<=0) { print("error: line too wide\n"); return; }

	if (ALLOC == 0)
		pim = im = (char *) malloc(npix*tlin);	/* was n*tlin */
	else
	{	if (npix*tlin > ALLOC)
		{	print("error: not enough memory allocated %d != %d*%d\n",
				ALLOC, n, tlin);
			return;
		}
	}

	buf1[0] = 2;
	buf1[1] = 7;
	buf1[2] = NLINES*spl;
	swab(buf1, buf2, 6);

	for (tlin = 0; ; tlin++)
	{	if (write(fd, buf2, 6) != 6) { perror("!reset scanner"); break; }
		if (read(fd, pim, n) != n) break;
		pim += n;
	}

	header(ofd, 2*spl, tlin*NLINES);
	if (correct)
	{	register int j;
		print("image correction...\n");
		pim = im+6;
		if (TRANS)
			for (j = tlin*n; j > 0; j--, pim++)
				*pim = 255 - *pim;
		else
			for (j = tlin*n; j > 0; j--, pim++)
				*pim = cor_refl[*pim];
	}
	sim = im+6;
	while (tlin-- > 0)
	{	write(ofd, sim, n-6);
		sim += n;
	}
	return;
}

header(ofd, a, b)
{
	Fprint(ofd, "TYPE=dump\nWINDOW=0 0 %d %d\nNCHAN=1\n", a, b);
	Fprint(ofd, "COMMAND=imscan -s%d ", SCALE);
	if (!correct) Fprint(ofd, "-n ");
	Fprint(ofd, "-l%d -%c\n\n", (LENS)?5:8, (TRANS)?'t':'r');
	Fflush(ofd);
}

seticonfig(fd)
{	unsigned short buf1[7], buf2[7];

	buf1[0] = 6;
	buf1[1] = 1;	/* SETIMAGECONFIG */
	buf1[2] = 4;	/* nparams */
	buf1[3] = 0;	/* line art*/
	buf1[4] = 0;	/* bw */
	buf1[5] = 24;	/* colour */
	buf1[6] = 40;	/* speed (40-500) 80 is about as fast as you can go */
	swab(buf1, buf2, 14);
	if (write(fd, buf2, 14) != 14)
	{	perror("seticonfig write");
		return;
	}
	if (read(fd, buf1, 6) != 6)
	{	perror("seticonfig read");
		return;
	}
}

setmodes(fd)
{	unsigned short buf1[7], buf2[7];

	buf1[0] = 6;
	buf1[1] = 21;	/* SETMODES */
	buf1[2] = LENS;	/* 1 = 5 inch, 0 = 8.5 inch lens */
	buf1[3] = 0;	/* line art (0=cont tone, 1=line art) */
	buf1[4] = TRANS;	/* 0=reflection, 1=transmission */
	buf1[5] = 1;	/* b/w */
	buf1[6] = 1;	/* autoscan */
	swab(buf1, buf2, 14);
	if (write(fd, buf2, 14) != 14)
	{	perror("setmodes write");
		return;
	}
	if (read(fd, buf1, 6) != 6)
	{	perror("setmodes read");
		return;
	}
}
