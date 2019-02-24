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

main(argc, argv)
	char **argv;
{
	int fd, i;
	char buf[256];

	fd = open("/usr/lib/font/dev202/DESC.out", 0);
	read(fd, &dev, sizeof dev);
	psizes = (short *)malloc(2*(dev.nsizes+1));
	read(fd, psizes, 2*(dev.nsizes+1));
	chtab = (short *)malloc(2*dev.nchtab);
	read(fd, chtab, 2*dev.nchtab);
	chname = (char *)malloc(dev.lchname);
	read(fd, chname, dev.lchname);
	close(fd);

	sprintf(buf, "/usr/lib/font/dev202/%s",  argv[1]);
	fd = open(buf, 0);
	dump(fd);
}

dump(fd)
{
	register i;

	read(fd, &font, sizeof font);
	widths = (char *)malloc(font.nwfont);
	read(fd, widths, font.nwfont);
/*
	printf("%d widths:\n", font.nwfont);
	for(i = 0; i < font.nwfont; i++)
		printf("%3d%c", 0xFF&widths[i], (i%20)==19?'\n':' ');
	printf("\n");
*/
	kern = (char *)malloc(font.nwfont);
	read(fd, kern, font.nwfont);
	codes = (char *)malloc(font.nwfont);
	read(fd, codes, font.nwfont);
/*
	printf("%d codes:\n", font.nwfont);
	for(i = 0; i < font.nwfont; i++)
		printf("%3d%c", 0xFF&codes[i], (i%20)==19?'\n':' ');
	printf("\n");
*/
	map = (char *)malloc(dev.nchtab+96);
	read(fd, map, dev.nchtab+96);
/*
	printf("%d map:\n", dev.nchtab+96);
	for(i = 0; i < dev.nchtab+96; i++)
		printf("%3d%c", 0xFF&map[i], (i%20)==19?'\n':' ');
	printf("\n");
*/
	for(i = 32; i < 128; i++)
		if(map[i-32]) printf("char %d (%c): width=%d code=%d\n", i, i, widths[map[i-32]], 0xFF&codes[0xFF&map[i-32]]);
/*
	for(i = 0; i < 96+dev.nchtab; i++)
		printf("map[%d]=%d code[map[i]]=%d\n", i, map[i], codes[map[i]]);

	for(i = 0; i < dev.nchtab; i++)
		printf("%d: map[i+96]=%d '%s'\n", i, map[i+96], &chname[chtab[i]]);
*/
	for(i = 0; i < dev.nchtab; i++)
		if(map[i+96]) printf("%d: ('%s'): width=%d, code=%d)\n", i, &chname[chtab[i]], widths[0xFF&map[i+96]], 0xFF&codes[0xFF&map[i+96]]);
}
