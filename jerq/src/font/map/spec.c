struct
{
	short	filesize;	/* number of bytes in file, */
				/* excluding dev part */
	short	res;		/* basic resolution in goobies/inch */
	short	hor;		/* goobies horizontally */
	short	vert;
	short	unitwidth;	/* size at which widths are given, in effect */
	short	nfonts;	/* number of fonts physically available */
	short	nsizes;	/* number of sizes it has */
	short	sizescale;	/* scaling for fractional point sizes */
	short	paperwidth;	/* max line length in units */
	short	paperlength;	/* max paper length in units */
	short	nchtab;	/* number of funny names in chtab */
	short	lchname;	/* length of chname table */
	short	spare1;	/* in case of expansion */
	short	spare2;
} dev;
char *chname;
short *psizes, *chtab;

struct font {		/* characteristics of a font */
	unsigned char	nwfont;	/* number of width entries for this font */
	char	specfont;	/* 1 == special font */
	char	ligfont;	/* 1 == ligatures exist on this font */
	char	spare1;	/* unused for now */
	char	name[10];	/* name of this font (e.g., "R" */
	char	iname[10];	/* internal name (=number) on device, in ascii */
} font;
char *widths, *kern, *map, *codes;

/* ligatures, ORed into ligfont */
#define	LFF	01
#define	LFI	02
#define	LFL	04
#define	LFFI	010
#define	LFFL	020

int  fraca, fracb;

main(argc, argv)
	char  **argv;
{
	int fd, i;
	char buf[256];

	fraca = atoi(argv[1]);
	fracb  = 311;
	fd = open("/usr/lib/font/dev202/DESC.out", 0);
	read(fd, &dev, sizeof dev);
	psizes = (short *)malloc(2*(dev.nsizes+1));
	read(fd, psizes, 2*(dev.nsizes+1));
	chtab = (short *)malloc(2*dev.nchtab);
	read(fd, chtab, 2*dev.nchtab);
	chname = (char *)malloc(dev.lchname);
	read(fd, chname, dev.lchname);
	close(fd);

	sprintf(buf, "/usr/lib/font/dev202/%s",  "S.out");
	fd = open(buf, 0);
	dump(fd);
}

dump(fd)
{
	register i, k;
	int xx[512];
	char buf[3];

	for(i = 511; i >= 0; i--)
		xx[i] = 0;
	read(fd, &font, sizeof font);
	widths = (char *)malloc(font.nwfont);
	read(fd, widths, font.nwfont);
	kern = (char *)malloc(font.nwfont);
	read(fd, kern, font.nwfont);
	codes = (char *)malloc(font.nwfont);
	read(fd, codes, font.nwfont);
	map = (char *)malloc(dev.nchtab+96);
	read(fd, map, dev.nchtab+96);

	buf[1] = 0;
	for(i = 32; i < 128; i++)
		if(map[i-32])
		{
			buf[0] = i;
			xx[smap(buf)] = 0xFF&codes[0xFF&map[i-32]];
		}
	for(i = 0; i < dev.nchtab; i++)
		if(map[i+96])
		{
			strcpy(buf, &chname[chtab[i]]);
			xx[smap(buf)] = 0xFF&codes[0xFF&map[i+96]];
		}
	for(i = 511; xx[i] == 0; i--);
	k = i;
	for(i = 0; i <= k; i++)
		if(xx[i])
			printf("merg C%03d f 0 p %d %d r\n", xx[i], fraca, fracb);
		else
			printf("f 0 s\n");
	printf("f 0 w newS\n");
}
