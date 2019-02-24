#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"defines.h"
#include	"/graphics/piclib/piclib.h"

#define XTIMES		Times
#define YTIMES		Times
#define Min(a,b)	((a)>(b)?(b):(a))
#define Max(a,b)	((a)<(b)?(b):(a))

int Dither=0, Times=3, Bright=0;
int Film = 0;
int Filter = GREEN;
int Reso;
int redonly = 0;
int mustrot = 0;
int settimes= 0;
int setbright=0;

unsigned char nq[9];
char *malloc();
unsigned char *rgb;

Usage(str)
	char *str;
{	fprintf(stderr, "qsnap: %s\n", str);
	fprintf(stderr, "usage: qsnap -bdfmrRFL [N ...] file\n");
	fprintf(stderr, "b  brightness  [0-8,9], defaults to 0 (2k) or 2 (4k)\n");
	fprintf(stderr, "d  dither image +- N [2-255]\n");
	fprintf(stderr, "f  filmtype [0-2], defaults to 0 (custom,lin,pola)\n");
	fprintf(stderr, "m  enlargement [1- ], defaults to 3(2k) or 6(4k)\n");
	fprintf(stderr, "r  expose red only (for multiple red overlays)\n");
	fprintf(stderr, "F  bwfilter [0-3], defaults to 2 (n,r,g,b)\n");
	fprintf(stderr, "R  rotate image 90o clockwise\n");
	fprintf(stderr, "L  straight (no m,d), headerless 1 chan, line by line\n");
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
	/* bright   */	case 'b': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &Bright);
					base++;
					Bright = Bright%10;
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
			case 'R': mustrot = 1; break;
			case 'L': linebyline = 1; break;
			case 'r': redonly = 1; break;
			default : Usage("unknown option");
			}
	}
	if (base != argc) Usage("bad arglist");
	qreset();
	nocalibs();
	Reso = resolution();
	if (Reso == 4)
	{	if (!settimes)  Times = 6;
		if (!setbright) Bright = 2;
	}

	rotate(mustrot);
	filmtype(Film);
	if (Bright != 9)
	{	getbright(nq);
		brightness(Bright, Bright, Bright, Bright);
		printf("brightness %d (%d)\n", Bright, nq[Bright]);
	}
	handshake(0);
	linedelay(1);

	if (linebyline)
		znap(argv[base-1]);
	else
		snap(argv[base-1]);
	exit(0);
}

znap(name)
	char *name;
{
	register i;
	int fd, h, w;
	unsigned char obuf[8192];
	int Xmax = 1024*Reso, Ymax = 768*Reso;
	int Xmin = -512*Reso, Ymin = 384*Reso;

	if ((fd = open(name, 0)) < 0)
	{	fprintf(stderr, "cannot open %s\n", name);
		return;
	}
	w = dimension(fd);
	if (w > Xmax) { fprintf(stderr, "image too wide (%d)\n", w); exit(1); }
	h = Min(Ymax, w);

	printf("%s: %dx%d\n", name, w, h);
	window(Xmin+500, Ymin, w, h);
	singlepass(Filter);
	for (i = 0; i < h; i++)
	{	read(fd, obuf, w);
		qwrite(obuf, w);
	}
	printf("done\n");
}

snap(name)
	char *name;
{
	int fd, h, w;
	int Xmax = 1024*Reso, Ymax = 768*Reso;
	int Xmin = -512*Reso, Ymin = 384*Reso;
	struct pfile image;

	if((fd = openf(name, &image)) == -1 || !readf(fd, &image))
	{	fprintf(stderr, "bad image %s\n", name);
		return;
	}
	w = image.r.corner.x - image.r.origin.x;
	if (w*XTIMES > Xmax)
	{	fprintf(stderr, "width is too large\n");
		fprintf(stderr, "you have: %dx%d=%d\n",w,XTIMES,w*XTIMES);
		exit(1);
	}
	h = image.r.corner.y - image.r.origin.y;
	h = Min(Ymax/YTIMES, h);
	printf("%s: %dx%d\n", name, w, h);
	window(Xmin, Ymin, w*XTIMES, h*YTIMES);

	if (image.nchan == 1)
	{	singlepass(Filter);
		onechannel(image.pixred, h, w);
	} else
	{	threepass();
		printf("red\n"); onechannel(image.pixred, h, w);
		if (redonly) goto done;
		printf("grn\n"); onechannel(image.pixgrn, h, w);
		printf("blu\n"); onechannel(image.pixblu, h, w);
	}
done:	printf("done\n");
}

onechannel(from, h, w)
	unsigned char *from;
{
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
