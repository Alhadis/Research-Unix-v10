#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"defines.h"
#include	"piclib.h"

#define XTIMES		Times
#define YTIMES		Times
#define Min(a,b)	((a)>(b)?(b):(a))
#define Max(a,b)	((a)<(b)?(b):(a))

int Xmax, Ymax, Xmin, Ymin;
int Dither=0, Times=3, Bright=0;
int Film = 0;
int Filter = GREEN;
int Reso = 0;
int redonly = 0;
int settimes= 0;
int setbright=0;
int xoffset = 0;
int yoffset = 0;

unsigned char nq[9];
char *malloc();
unsigned char *rgb;
int lsize;
int multiple=1;		/* number of exposures */
int fad=1, dodub=0;

Usage(str)
	char *str;
{	fprintf(stderr, "qsnap: %s\n", str);
	fprintf(stderr, "usage: qsnap -bdfmnrxFLRXY [N ...] file\n");
	fprintf(stderr, "b  brightness  [0-8], defaults to 0 (2k) or 2 (4k)\n");
	fprintf(stderr, "d  dither image +- N [2-255]\n");
	fprintf(stderr, "f  filmtype [0-2], defaults to 0 (custom,lin,pola)\n");
	fprintf(stderr, "m  enlargement [1-N], defaults to 3(2k) or 6(4k)\n");
	fprintf(stderr, "n  TMAX-100 b&w corrected exposure\n");
	fprintf(stderr, "r  expose red only (for multiple red overlays)\n");
	fprintf(stderr, "x  N exposures (not with -L)\n");
	fprintf(stderr, "F  bwfilter [0-3], defaults to 2 (n,r,g,b)\n");
	fprintf(stderr, "L  raw file, no header, bw, line by line (no m,d,x)\n");
	fprintf(stderr, "R  N set resolution to N [N=2 or N=4]\n");
	fprintf(stderr, "X  add x-offset N (eg 480/m, for 35mm camera)\n");
	fprintf(stderr, "Y  add y-offset N\n");
	exit(1);
}

main(argc, argv)
	char **argv;
{
	int i=1, base=2;	/* base of option arguments */
	int linebyline = 0;
	char c;

	if (argc > 1 && argv[1][0] == '-')
	{	base++;
		while ((c = argv[1][i++]) != '\0')
			switch (c) {
	/* TMAX-100 */	case 'n': dodub = 1; break;
	/* N exps */	case 'x': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &multiple);
					base++;
					fad = 0;
					break;
				  } else
				  	Usage("missing argument for `a' flag");
	/* bright   */	case 'b': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &Bright);
					base++;
					Bright = Bright%9;
					setbright = 1;
					break;
				  } else
				  	Usage("missing argument for `b' flag");
	/* dither   */	case 'd': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &Dither);
					base++;
					if (Dither < 0) Dither = -Dither;
					break;
				  } else
				  	Usage("missing argument for `d' flag");

	/* filmtype */	case 'f': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &Film);
					Film = Film%8;
					base++;
					if (Film == 6)
						Usage("unknown film type");
					break;
				  } else
				  	Usage("missing argument for `f' flag");
	/* multiply */	case 'm': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &Times);
					base++;
					settimes = 1;
					break;
				  } else
				  	Usage("missing argument for `m' flag");
	/* filter */	case 'F': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &Filter);
					Filter = Filter%4;
					base++;
					break;
				  } else
				  	Usage("missing argument for `F' flag");
			case 'L': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &lsize);
					linebyline = 1;
					base++;
					break;
				  } else
				  	Usage("missing argument for `L' flag");
			case 'R': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &Reso);
					base++;
					break;
				  } else
				  	Usage("missing argument for `R' flag");
			case 'X': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &xoffset);
					base++;
					break;
				  } else
				  	Usage("missing argument for `X' flag");
			case 'Y': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &yoffset);
					base++;
					break;
				  } else
				  	Usage("missing argument for `Y' flag");
			case 'r': redonly = 1; break;
			default : Usage("unknown option");
			}
	}
	if (dodub == 1 && (setbright || Film))
	{	fprintf(stderr, "warning: -b and -f flags have ");
		fprintf(stderr, "no effect when combined with -n\n");
	}
	if (base != argc) Usage("bad arglist");

	qreset();
	getbright(nq);
	if (Reso == 2 || Reso == 4)
		setreso(Reso);
	qpause();
	Reso = resolution();
	Xmax = 1024*Reso;
	Ymax =  768*Reso;
	Xmin = -512*Reso;
	Ymin =  384*Reso;
	if (linebyline && lsize > Xmax)
	{	fprintf(stderr, "image too wide: %d, max is %d\n", lsize, Xmax);
		exit(1);
	}
	if (dodub && nq[2] != 81)
	{	fprintf(stderr, "option -n requires that you first set\n");
		fprintf(stderr, "\t$ qbright 36 58 81 103 126 148 171 193\n");
		exit(1);
	}
	if (Reso == 4)
	{	if (!settimes)  Times = 6;
		if (!setbright) Bright = 2;
	}

	qpause();
	nocalibs();
	handshake(1); fflush(stdout);
	if (dodub)
	{	qpause();
		customluts(TMAX_100);
		Bright = 2;
	} else
		filmtype(Film);
	qpause();
	advance(fad);
	qpause();
	if (fad==0)
		shutter(1);
	fflush(stdout);
Again:
	qpause();
	brightness(Bright, Bright, Bright, Bright);
	printf("brightness %d (%d)\n", Bright, nq[Bright]); fflush(stdout);
	if (linebyline)
		znap(argv[base-1]);
	else
		snap(argv[base-1]);
	if (fad==0)
		shutter(0);
	exit(0);
}

znap(name)
	char *name;
{
	register i;
	int fd, h, w;
	unsigned char obuf[8192];

	w = lsize;
	h = (lsize>683*Reso)?(683*Reso):lsize;
	/* the maximum for 35mm; for 4x5 it is 768*Reso */

	if ((fd = open(name, 0)) < 0)
	{	fprintf(stderr, "cannot open %s\n", name);
		exit(1);
	}
	printf("file: %s\n", name);
	window(Xmin+xoffset, Ymin-yoffset, w, h);
	singlepass(Filter);
	fflush(stdout);
	for (i = 0; i < h; i++)
	{	read(fd, obuf, w);
		qwrite(obuf, w);
	}
	close(fd);
	fprintf(stderr, "done\n");
}

snap(name)
	char *name;
{
	int fd, h, w;
	struct pfile image;

	if((fd = openf(name, &image)) == -1 || !readf(fd, &image))
	{	fprintf(stderr, "bad image %s\n", name);
		exit(1);
	}
	w = image.r.corner.x - image.r.origin.x;
	if (w*XTIMES > Xmax)
	{	fprintf(stderr, "image too wide: %d, max %d\n", w*XTIMES, Xmax);
		exit(1);
	}
	h = image.r.corner.y - image.r.origin.y;
	h = Min(Ymax/YTIMES, h);
	while (multiple-- > 0)
	{	printf("file: %s\n", name);
		window(Xmin+xoffset, Ymin-yoffset, w*XTIMES, h*YTIMES);
		fflush(stdout);
		if (image.nchan == 1)
		{	singlepass(Filter);
			onechannel(image.pixred, h, w);
		} else
		{	threepass();
			fprintf(stderr, "red\n"); onechannel(image.pixred, h, w);
			if (redonly) goto done;
			fprintf(stderr, "grn\n"); onechannel(image.pixgrn, h, w);
			fprintf(stderr, "blu\n"); onechannel(image.pixblu, h, w);
		}
		sleep(10);
		if (multiple > 0)
			printf("x pass %d\n", multiple);
	}
	close(fd);
done:	fprintf(stderr, "done\n");
}

onechannel(from, h, w)
	unsigned char *from;
{
	fflush(stdout);
	if (Dither)
	{	prerand();
		onedither(from, h, w);
	} else
		 straight(from, h, w);
}

straight(from, h, w)
	unsigned char *from;
{
	register i, j, k;
	register unsigned char *p, *q;
	unsigned char obuf[8192];
	int chunk = w*XTIMES;

	for (i = 0, p = from; i < h; i++)
	{	q = obuf;
		for (j = 0; j < w; j++, p++)
		for (k = 0; k < XTIMES; k++)
			*q++ = *p;
		for (k = 0; k < YTIMES; k++)
		{	qwrite(obuf, chunk);
			if (chunk > 2048) qwait(200);
	}	}
}

short Nrand[5000];

prerand()
{	register int i, D1=Dither, D2=Dither/2;

	for (i = 0; i < 5000; i++)
		Nrand[i] = (short) (nrand(D1) - D2);
}

onedither(from, h, w)
	unsigned char *from;
{
	register int c, m, kk=0;
	register unsigned char *op, *q;
	unsigned char *p, obuf[8192];
	int i, j, k;
	int chunk = w*XTIMES;

	for (i = 0, p = from; i < h; i++, p = op)
	for (k = 0; k < YTIMES; k++)
	{	q = obuf;
		op = p;
		j = w;
		do{
			m = XTIMES;
			do{
				c = *op + Nrand[kk];
				if (++kk >= 5000) kk = 0;
				if(c<0)
					c=0;
				if(c>255)
					c=255;
				*q++ = c;
			}while(--m);
			op++;
		}while(--j);
		qwrite(obuf, chunk);
	}
}

qwait(n)
{	int i;
	for (i = 0; i < n; i++) ;
}

dimension(fd)
{ 	struct stat bam;
	int N;
	extern float fsqrt();

	if (fstat(fd, &bam)==0)
	{	N = bam.st_size;
		N = (int) fsqrt((double)N+1.0);
		return N;
	}
	return 0;
}
