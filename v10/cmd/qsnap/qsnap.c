#include	<fb.h>
#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"defines.h"

#define XTIMES		Times
#define YTIMES		Times
#define Min(a,b)	((a)>(b)?(b):(a))
#define Max(a,b)	((a)<(b)?(b):(a))

int Dither=0, BW=1, Times=3, Bright=0;
int Film = 0;
int Filter = GREEN;
int Reso;
int redonly = 0;
int mustrot = 0;
int settimes= 0;
int setbright=0;
int skip = 0;

unsigned char nq[9];
char *malloc();
unsigned char *rgb;

Usage(str)
	char *str;
{	fprintf(stderr, "qsnap: %s\n", str);
	fprintf(stderr, "usage: qsnap -bcdfmnrRF [N ...] file\n");
	fprintf(stderr, "b  brightness  [0-8,9], defaults to 0 (2k) or 2 (4k)\n");
	fprintf(stderr, "c  td format n-channel color image\n");
	fprintf(stderr, "d  dither image +- N [2-255]\n");
	fprintf(stderr, "f  filmtype [0-2], defaults to 0 (custom,lin,pola)\n");
	fprintf(stderr, "m  enlargement [1- ], defaults to 3(2k) or 6(4k)\n");
	fprintf(stderr, "n  headerless square b&w image (default)\n");
	fprintf(stderr, "r  expose red only, when combined with 'c'\n");
	fprintf(stderr, "F  bwfilter [0-3], defaults to 2 (n,r,g,b)\n");
	fprintf(stderr, "R  rotate image 90o clockwise\n");
	fprintf(stderr, "s  skip the first N pictures\n");
	exit(1);
}

main(argc, argv)
	char **argv;
{
	int i=1, base=2;	/* base of option arguments */
	int x, y;
	char c;

	if (argc > 1 && argv[1][0] == '-')
	{	base++;
		while ((c = argv[1][i++]) != '\0')
			switch (c) {
	/* neutral  */	case 'n': BW = 1; break;
	/* color    */	case 'c': BW = 0; break;
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
			case 'r': redonly = 1; break;
			case 's': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &skip);
					base++;
					break;
				  } else
				  	Usage("missing argument for `s' flag");
			default : Usage("unknown option");
			}
	}
	if (base > argc)
		Usage("bad arglist");
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

	handshake(1);

	for (x = -512*Reso, y = 384*Reso; base <= argc; base++)
	{	if (skip-- > 0)
			continue;
		i = snap(argv[base-1], x, y);
		x += i;
		if (x+i >= 512*Reso)
		{	y -= i;
			x = -512*Reso;
		}
		if (y-i < -384*Reso)
		{	fprintf(stderr, "picture %s doesn't fit\n", argv[base-1]);
			break;
		}
		qreset();
		brightness(Bright, Bright, Bright, Bright);
		handshake(1);
	}
	exit(0);
}

snap(name, Xmin, Ymin)
	char *name;
{
	PICFILE *f;
	register i, fd;
	register unsigned char *p;
	int h, w, nchan=1;
	int Xmax = 1024*Reso, Ymax = 768*Reso;

	if (BW)
	{	if ((fd = open(name, 0)) == -1)
		{	perror(name);
			exit(1);
		}
		w = dimension(fd);
		if (w*XTIMES > Xmax)
		{	fprintf(stderr, "width is too large\n");
			fprintf(stderr, "you have: %dx%d=%d\n",w,XTIMES,w*XTIMES);
			exit(1);
		}
		h = Min(Ymax/YTIMES, w);
		printf("%s: %dx%d\n", name, w, h);
		if (!(rgb = (unsigned char *) malloc(w*h* sizeof(unsigned char))))
		{	fprintf(stderr, "sorry, not enough memory\n");
			exit(1);
		}
		read(fd, (char *)rgb, w*h);
		close(fd);
	} else
	{	if ((f = openpicr(name)) == NULL)
		{	perror(name);
			exit(1);
		}
		w = f->r.co.x-f->r.or.x;
		h = f->r.co.y-f->r.or.y;
		nchan = f->nchan;
		if (w*XTIMES > Xmax)
		{	fprintf(stderr, "width too large\n");
			fprintf(stderr, "you have: %dx%d=%d\n",w,XTIMES,w*XTIMES);
			exit(1);
		}
		h = Min(Ymax/YTIMES, h);
		printf("%s: %dx%d\n", name, w, h);
		if (!(rgb = (unsigned char *)
				malloc(w*h*nchan* sizeof(unsigned char))))
		{	fprintf(stderr, "sorry, not enough memory\n");
			exit(1);
		}
		for (p = rgb, i = 0; i < h; i++)
		{	readpic(f, p);
			p += w*nchan;
		}
		closepic(f);
	}
	window(Xmin, Ymin, w*XTIMES, h*YTIMES);

	if (BW)
	{	singlepass(Filter);
		onechannel(rgb, h, w, 1);
	} else
	{	threepass();
		printf("red\n"); onechannel(rgb  , h, w, nchan);
if (!redonly) {
		printf("grn\n"); onechannel(rgb+1, h, w, nchan);
		printf("blu\n"); onechannel(rgb+2, h, w, nchan);
}
	}
	printf("done\n");
	return w*XTIMES;
}

onechannel(from, h, w, n)
	unsigned char *from;
{
	if (Dither)
	{	prerand();
		onedither(from, h, w, n);
	} else
		 straight(from, h, w, n);
}

straight(from, h, w, n)
	unsigned char *from;
{
	register i, j, k;
	register unsigned char *p, *q;
	unsigned char obuf[8192];
	int chunk = w*XTIMES;

	for (i = 0, p = from; i < h; i++)
	{	q = obuf;
		for (j = 0; j < w; j++, p += n)
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

/*onedither(from, h, w, n)
	unsigned char *from;
{
	register short c, m, kk;
	register unsigned char *op, *q;
	register int D1 = Dither;
	unsigned char *p, obuf[8192];
	int i, j, k;
	int chunk = w*XTIMES;

	for (i = 0, p = from; i < h; i++, p = op)
	for (k = 0; k < YTIMES; k++)
	{	q = obuf;
		op = p;
		for (j = 0; j < w; j++, op += n)
		for (m = 0; m < XTIMES; m++)
		{	c = (short) *op + Nrand[kk];
			if (++kk >= 5000) kk = 0;
			if(c<0)
				c=0;
			if(c>255)
				c=255;
			*q++ = (unsigned char) c;
		}
		qwrite(obuf, chunk);
	}
}*/
onedither(from, h, w, n)
	unsigned char *from;
{
	register int c, m, kk=0;
	register unsigned char *op, *q;
	register int D1 = Dither;
	unsigned char *p, obuf[8192];
	int i, j, k;
	int chunk = w*XTIMES;

	if (w <= 0 || XTIMES <= 0) abort();
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
			op += n;
		}while(--j);
		qwrite(obuf, chunk);
	}
}

qwait(n)
{	int i;
	for (i=0; i < n; i++) ;
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
