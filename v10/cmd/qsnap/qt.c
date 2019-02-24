#include	<stdio.h>
#include	<sys/types.h>
#include	<sys/stat.h>
#include	"defines.h"
#include	"piclib.h"

#define Min(a,b)	((a)>(b)?(b):(a))
#define Max(a,b)	((a)<(b)?(b):(a))

int Wmax, Hmax, Xmin, Ymin;
int Dither=0, Times=1, Bright=2, Film=8, Filter=GREEN, Reso=4;
int xoffset=0, yoffset=0, fad=1, h, w, H, W;
struct pfile image;
unsigned char nq[9];

give_status()
{	int i;
	getbright(nq);
	Reso = resolution();
	printf("qbright ref   	36 58 81 103 126 148 171 193 220\n");
	printf("        actual	");
	for(i = 0; i < 9; i++)
		printf("%d ", nq[i]);
	printf("\n");
	moduleno();
	printf("shutter: %s\n", (fad)?"automatic":"manual");
	printf("dither : %d\n", Dither);
	printf("offset : %d,%d\n", xoffset, yoffset);
	printf("enlarge: %d\n", Times);
	if (W || H) printf("image  : %d,%d\n", W, H);
	printf("filmtype: ");
	switch (Film) {
	case NONE:	printf("polaroid type 52\n"); break;
	case LINEAR:	printf("linear lookup tables\n"); break;
	case EKTA100:	 printf("ektachrome 100 - for 2k mode\n"); break;
	case EKTA100_4k: printf("ektachrome 100 - for 4k mode\n"); break;
	case POLA559:	 printf("polaroid type 559 - for 2k mode\n"); break;
	case POLA559_4k: printf("polaroid type 559 - for 4k mode\n"); break;
	case POLA809_4k: printf("polaroid type 809 - for 4k mode\n"); break;
	case TMAX_100:	printf("tmax_100 film\n"); break;
	default:	printf("filmtype unknown\n"); break;
	}
	printf("filter  : ");
	switch (Filter) {
	case   RED: printf("red\n"); break;
	case GREEN: printf("green\n"); break;
	case  BLUE: printf("blue\n"); break;
	case NEUTRAL: printf("neutral\n"); break;
	   default: printf("none\n"); break;
	}
}

reminder()
{	printf("PM: reset, bright, enlarge, filmtype, load, expose\n");
}

help()
{	printf("reset              -	a full reset\n");
	printf("status             -	report all current settings\n");
	printf("error              -	expanded error status\n");
	printf("exit               -	return to Unix\n");
	printf("load	 imagefile -	read in the file to be printed\n");
	printf("dither	 on/off/N  -	dithering off, on, on mod(N)\n");
	printf("shutter	 open/closed/auto - auto is default\n");
	printf("filter	 red/green/blue/neutral - is used for b&w imaging\n");
	printf("expose             -	a black&white image\n");
	printf("expose3            -	a color image\n");
	printf("expose1	 red/green/blue - imaging of 1 specific channel\n");
	printf("offset	 X Y       -	offset from lower left hand corner\n");
	printf("bright	 N         -	set brightness level N [0-8]\n");
	printf("enlarge	 N         -	enlargement factor\n");
	printf("reso	 N         -	set resolution N [2,4]\n");
	printf("filmtype tmax_100  -	select tmax_100 correction tables\n");
	printf("	other available filmtypes: \n");
	printf("	linear	\t(no correction)\n");
	printf("	tmax_100\t(Kodak 35mm Tmax 100 ASA b&w negative)\n");
	printf("	ekta_100\t(Kodak 35mm Ektachrome 100 ASA color neg)\n");
	printf("	pola_52	\t(Polaroid Type 52  b&w 4x5 inch sheet)\n");
	printf("	pola_559\t(Polaroid Type 559 b&w pack film)\n");
	printf("	pola_809\t(Polaroid Type 809 color 8x10 inch)\n");
}

set_luts(n)
{
	switch (n) {
	case NONE:
	case LINEAR:
	case POLA559:
	case EKTA100:
	case EKTA100_4k:
	case POLA559_4k:
	case POLA809_4k:
	case TMAX_100:
		Film = n;
		filmtype(n);
		break;
	default:
		printf("bad filmtype %d\n", n);
		break;
	}
}

main()
{	reminder();
	while (1)
	{	printf("qt: ");
		fflush(stdout);
		yyparse();
		fflush(stdout);
	}
}

setbright(n)
{	extern int Errors;
	Bright = Min((Max(0,n)), 8);
	brightness(Bright, Bright, Bright, Bright);
	if (!Errors)
	printf("brightness %d = %d\n", n, nq[n]);
}

full_reset()
{
	qreset();
	getbright(nq);
	Reso = resolution();
	nocalibs();
	handshake(1);
	filmtype(Film);
	advance(fad);
	setbright(Bright);
}

load_image(fname)
	char *fname;
{
	int fd;

	if((fd = openf(fname, &image)) == -1 || !readf(fd, &image))
	{	printf("bad image %s\n", fname);
		closef(&image);
	}
	close(fd);
	W = image.r.corner.x - image.r.origin.x;
	H = image.r.corner.y - image.r.origin.y;
	if (Reso == 2)
	{	xoffset = (2048-W)/2;
		yoffset = (1536-H)/2;
	} else
	{	xoffset = (4096-W)/2;
		yoffset = (2732-H)/2;
	}
	printf("%s loaded, size %dx%d, offset %d,%d\n",
		fname, W, H, xoffset, yoffset);
	fflush(stdout);
}

crop()
{	if (!image.pixred)
	{	printf("error: no image loaded\n");
		return 0;
	}
	if (Reso == 2)
	{	Wmax =  2048; Hmax =  1366;
		Xmin = -1024; Ymin =  683;
	} else
	{	Wmax =  4096; Hmax =  2732;
		Xmin = -2048; Ymin =  1366;
	}
	w = Min((Wmax/Times - xoffset), W);
	h = Min((Hmax/Times - yoffset), H);
	printf("image: %d x %d -> %d x %d\n", W, H, w, h);
	printf("offset %d %d, enlarge %d\n", xoffset, yoffset, Times);
	window(Xmin+xoffset, Ymin-yoffset, w*Times, h*Times);
	fflush(stdout);
	return 1;
}

one_pass(which)
{
	if (!crop()) return;
	singlepass(Filter);
	switch (which) {
	default:
	case   RED: onechannel(image.pixred, h, w); break;
	case GREEN: onechannel(image.pixgrn, h, w); break;
	case  BLUE: onechannel(image.pixblu, h, w); break;
	}
}

new_offset(a, b)
{	xoffset = a;
	yoffset = b;
}

three_pass(height, width)
{	extern int Errors;
	if (!crop() || Errors) return;
	if (image.nchan != 3)
	{	printf("error: no color image loaded\n");
		return;
	}
	threepass();
	printf("red.."); onechannel(image.pixred, h, w);
	printf("grn.."); onechannel(image.pixgrn, h, w);
	printf("blu.."); onechannel(image.pixblu, h, w);
}

onechannel(from, h, w)
	unsigned char *from;
{
	register i, j, k;
	register unsigned char *p, *q;
	unsigned char obuf[8192];
	int chunk = w*Times;
	extern int Errors;

	fflush(stdout);
	if (Errors)
		return;
	if (Dither)
		onedither(from, h, w);
	else
	for (i = 0; i < h; i++)
	{	p = &from[W*i];
		q = obuf;
		for (j = 0; j < w; j++, p++)
		for (k = 0; k < Times; k++)
			*q++ = *p;
		for (k = 0; k < Times; k++)
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
	register unsigned char *p, *q;
	unsigned char obuf[8192];
	int i, j, k;
	int chunk = w*Times;

	prerand();
	for (i = 0; i < h; i++)
	for (k = 0; k < Times; k++)
	{	q = obuf;
		p = &from[W*i];
		j = w;
		do {	m = Times;
			do {	c = *p + Nrand[kk];
				if (++kk >= 5000) kk = 0;
				if (c<0)
					c=0;
				else if (c>255)
					c=255;
				*q++ = c;
			} while(--m > 0);
			p++;
		} while(--j > 0);
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
