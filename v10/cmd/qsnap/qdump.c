#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"defines.h"

#define Min(a,b)	((a)>(b)?(b):(a))
#define Max(a,b)	((a)<(b)?(b):(a))

int Bright=0;
int Film = 0;
int Filter = GREEN;
int Reso;
int setbright=0;
int Xmax, Ymax, Xmin, Ymin;

unsigned char nq[9];

Usage(str)
	char *str;
{	fprintf(stderr, "qdump: %s\n", str);
	fprintf(stderr, "usage: qdump -bfF [N ...] file\n");
	fprintf(stderr, "b  brightness  [0-8,9], defaults to 0 (2k) or 2 (4k)\n");
	fprintf(stderr, "f  filmtype [0-2], defaults to 0 (custom,lin,pola)\n");
	fprintf(stderr, "F  bwfilter [0-3], defaults to 2 (n,r,g,b)\n");
	exit(1);
}

main(argc, argv)
	char **argv;
{
	int i=1, base=2;	/* base of option arguments */
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
	/* filmtype */	case 'f': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &Film);
					Film = Film%8;
					base++;
					if (Film == 6)
						Usage("unknown film type");
					break;
				  } else
				  	Usage("missing argument for `f' flag");
	/* filter */	case 'F': if (argc >= base)
				  {	sscanf(argv[base-1], "%d", &Filter);
					Filter = Filter%4;
					base++;
					break;
				  } else
				  	Usage("missing argument for `F' flag");
			default : Usage("unknown option");
			}
	}
	if (base != argc)
		Usage("bad arglist");

	qreset();
	nocalibs();
	Reso = resolution();
	if (Reso == 4 && !setbright)
		Bright = 2;

	Xmax = 1024*Reso; Ymax = 768*Reso;
	Xmin = -512*Reso; Ymin = 384*Reso;

	filmtype(Film);
	if (Bright != 9)
	{	getbright(nq);
		brightness(Bright, Bright, Bright, Bright);
		printf("brightness %d (%d)\n", Bright, nq[Bright]);
	}
	handshake(1);
	snap(argv[base-1]);
	exit(0);
}

snap(name)
	char *name;
{
	register i, fd;
	register unsigned char *p;
	int h, w;

	if ((fd = open(name, 0)) == -1)
	{	perror(name);
		exit(1);
	}
	h = w = dimension(fd);
	printf("%s: %dx%d\n", name, w, h);

	window(Xmin, Ymin, Min(Xmax, w), Min(Ymax, h));
	singlepass(Filter);
	straight(fd, h, w);
}

straight(fd, h, w)
{	register int i;
	unsigned char obuf[4096];
	int realw = Min(Xmax, w);
	int realh = Min(Ymax, h);
	int j;

	for (i = 0; i < realh; i++)
	{	if (read(fd, obuf, w) != w)
		{	fprintf(stderr, "qdump: unexpected eof\n");
			exit(1);
		}
		qwrite(obuf, realw);
	}
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
