#include	<stdio.h>
#include	<signal.h>
#include	"piclib.h"

#define	MAXPIXEL	1024
#define MAXDICT		64
#define LARGE		32768
#define Version		10	/* number between 0 - 255 */

struct DICT {
	struct {
		int freq;
		unsigned char val;
	} dict[256];
	int ndict;
} Dict[MAXDICT];

unsigned char nDict=0;
unsigned char *out;
extern char *malloc();
char tempname[L_tmpnam];
char *oname = "_movie";
int Sflag=0;
int sfnx = 512, sfny = 512;

int frameno=0, fullsize=0, tiny=0, medium=1;
int ofd, uni = 1, verbose = 0, eprop = 0;

Usage()
{	fprintf(stderr, "usage: preflicks [-efmtlvoics] files\n");
	fprintf(stderr, "	-f,m,t fullsize, medium, or tiny\n");
	fprintf(stderr, "	-l use logarithmic grey scale\n");
	fprintf(stderr, "	-e use error propagation iso dither\n");
	fprintf(stderr, "	-v verbose\n");
	fprintf(stderr, "	-o output on standard output\n");
	fprintf(stderr, "	-i info on movie files\n");
	fprintf(stderr, "	-c catenate movie files onto standard output\n");
	fprintf(stderr, "	-sX,Y set of 512x512 frames\n");
	exit(1);
}

main(argc, argv)
	char **argv;
{	char c;
	int fd, n=1;
	int Iflag = 0;
	int Cflag = 0;
	void wrapup();
	struct pfile image;

	argv++;
	while(*argv && argv[0][0] == '-')
	{	for(n = 1; argv[0][n]; n++)
		{	switch (argv[0][n]) {
			case 'f': fullsize = 1; medium = 0; break;
			case 'm': medium = 1; break;
			case 't': tiny = 1; medium = 0; break;
			case 'e': eprop = 1; break;
			case 'l': uni = 0; break;
			case 'v': verbose = 1; break;
			case 'o': oname = 0; break;
			case 'i': Iflag++; break;
			case 'c': Cflag++; break;
			case 's':
				Sflag++;
				if (sscanf(&argv[0][n+1], "%d,%d", &sfnx, &sfny)
									== 1)
					sfny = sfnx;
				if ((sfnx < 128) || (sfnx > 512)
				||  (sfny < 120) || (sfny > 512))
				{	printf("bad sframe: %dx%d\n", sfnx, sfny);
					exit(1);
				}
				argv[0][n--] = 0;   /* break cracking this arg */
				break;
			default : Usage(); break;
			}
		}
		argv++;
	}
	if (Iflag)
	{	if(*argv == 0) *--argv = oname;
		while(*argv)
			what(*argv++);
		exit(0);
	}
	if (Cflag)
	{	while(*argv)
			catenate(*argv++);
		purge();
		exit(0);
	}
	tmpnam(tempname);
	newdict();
	out = (unsigned char *) malloc(LARGE);	/* enough for -f worst case */
	if (!out)
	{	fprintf(stderr, "preflicks: not enough memory\n");
		exit(1);
	}
	if ((ofd = creat(tempname, 0666)) == -1)
	{	perror(tempname);
		exit(1);
	}
	signal(SIGINT, wrapup);
	if (Sflag)
	{	image.r.corner.x = sfnx; image.r.corner.y = sfny;
		image.r.origin.x = image.r.origin.y = 0;
		image.nchan = 1; image.type = PICO;
		image.pixred = (unsigned char *) malloc(sfnx*sfny);
		if ((fd = open(*argv, 0)) >= 0)
		{	while (read(fd, image.pixred, sfnx*sfny) == sfnx*sfny)
			{	if (!eprop)
					dsend(*argv, &image);
				else
					esend(*argv, &image);
				frameno++;
			}
			close(fd);
		} else
			fprintf(stderr, "preflicks: no file %s\n", *argv);
	} else
	{	while(*argv)
		{	if ((fd = openf(*argv,&image)) == -1)
			{	fprintf(stderr,"preflicks: can't open %s\n",*argv);
				argv++;
			} else if (!readf(fd,&image))
			{	fprintf(stderr,"preflicks: can't read %s\n",*argv);
				argv++;
				close(fd);
			} else
			{	if (!eprop)
					dsend(*argv++, &image);
				else
					esend(*argv++, &image);
				frameno++;
				close(fd); closef(&image);
	}	}	}
	wrapup();
}

char stib[MAXPIXEL*(MAXPIXEL+16)]; int nstib=0;

#define	DITHSIZE	16
#define	DITHMASK	(DITHSIZE-1)
#define DITHMAX		DITHSIZE*DITHSIZE-1
#define ZUNK		if ((data&DITHMAX) < dth[x&DITHMASK]) byte |= mask;

int udith[DITHSIZE][DITHSIZE]={
#include "udither.data"
};
int ldith[DITHSIZE][DITHSIZE]={
#include "ldither.data"
};

dsend(file, image)	
	char *file;
	struct pfile *image;
{
	register byte=0, mask=0x80, data;
	int x, y, ll, len, row, zz=0, col, ht;
	register unsigned char *s, *ss;
	register int *dth;

	len = image->r.corner.x - image->r.origin.x;
	ht  = image->r.corner.y - image->r.origin.y;
	s = image->pixred;

	if (verbose) fprintf(stderr, "%s: dither, ", file);
	ll = len;
	if (!fullsize) len /= 2, ht /= 2;
	if (tiny) len /= 2, ht /= 2;
	ss = s+ll;

	row = ((len+31)&(~31))/8;	/* assumes 32bit allignment in jerq */

	for (y = ht-1; y >= 0; y--, s += (!fullsize)?ll:0, ss += (!fullsize)?ll:0)
	{	
		dth = uni? udith[y&DITHMASK] : ldith[y&DITHMASK];
		for(x = len-1, col = 0; x >= 0; x--)
		{	data = (unsigned char)*s++;
			if (!fullsize)
			{	data += (unsigned char)*s++;
				data += (unsigned char)*ss++;
				data += (unsigned char)*ss++;
				data >>= 2;
				if (tiny) s += 2, ss += 2;
			}
			ZUNK;
			mask >>= 1;
			if (mask == 0)
			{	out[zz++] = byte; col++;
				byte = 0; mask = 0x80; 
			}
		}
		if (mask != 0x80)
		{	out[zz++] = byte; col++;
			byte = 0; mask = 0x80; 
		}
		while(col++ < row)
			out[zz++] = byte;
		if (tiny) s += 2*ll, ss += 2*ll;
	}
	if (verbose) fprintf(stderr, "compact, ");
	munge(len, ht, zz);
}

short cnt[MAXPIXEL][MAXPIXEL];

esend(file, image)
	char *file;
	struct pfile *image;
{
	register byte = 0, mask = 0x80, data;
	int x, y, ll, ht, zz = 0, len, row, col, error;
	register unsigned char *s;

	len = image->r.corner.x - image->r.origin.x;
	ht  = image->r.corner.y - image->r.origin.y;
	s = image->pixred;

	ll = len;
	if (!fullsize) len /= 2, ht /= 2;
	if (tiny) len /= 2, ht /= 2;
	if (verbose) fprintf(stderr, "%s: error prop, ", file);
	row = ((len+31)&(~31))/8;	/* assumes 32bit allignment in jerq */

	for (y = ht-1; y >= 0; y--, s += (!fullsize)?ll:0)
	{	for (x = len-1; x >= 0; x--)
		{	data = (unsigned char)*s++;
			if (!fullsize)
			{	data += (unsigned char)*s++;
				data += (unsigned char)*(s+ll-1);
				data += (unsigned char)*(s+ll);
				data >>= 2;
				if (tiny) s += 2;
			}
			cnt[x][y] = (short) data;
		}
		if (tiny) s += 2*ll;
	}
	for (y = ht-1; y >= 0; y--)
	{	for (x = len-1, col = 0; x >= 0; x--)
		{
			if (cnt[x][y] < 128)
			{	error = cnt[x][y];
				byte |= mask;
			} else
				error = cnt[x][y]-255;
			error /= 4;
			if (x)		cnt[x-1][y] += error;
			if (y)		cnt[x][y-1] += error;
			if (x && y)	cnt[x-1][y-1] += error;
/*
	Stucki:
			if (x)   cnt[x-1][y] += error*8/42;
			if (x>1) cnt[x-2][y] += error*4/42;

			if (y)
			{	cnt[x+2][y-1] += error*2/42;
				cnt[x+1][y-1] += error*4/42;
				cnt[x  ][y-1] += error*8/42;
				if (x)   cnt[x-1][y-1] += error*4/42;
				if (x>1) cnt[x-2][y-1] += error*2/42;
				if (y>1)
				{
					cnt[x+2][y-2] += error*1/42;
					cnt[x+1][y-2] += error*2/42;
					cnt[x  ][y-2] += error*4/42;
					if (x)   cnt[x-1][y-2] += error*2/42;
					if (x>1) cnt[x-2][y-2] += error*1/42;
			}	}
*/
			mask >>= 1;
			if (mask == 0)
			{	out[zz++] = byte; col++;
				byte = 0; mask = 0x80; 
			}
		}
		while(col++ < row)
			out[zz++] = byte;
	}
	if (verbose) fprintf(stderr, "compact, ");
	munge(len, ht, zz);
}

cleanout(len, ht)
{
	if (verbose) fprintf(stderr, "write (%dx%d) %d\n", len, ht, nstib);
	twobyte(ofd, len);		/* width          */
	twobyte(ofd, ht);		/* height         */
	twobyte(ofd, nstib);		/* compacted size */
	onebyte(ofd, nDict-1);		/* current dict # */
	write(ofd, stib, nstib);	/* encoded frame  */
}

#define RUN(a)		{	z = a;					\
				while (z >= 255)			\
				{	stib[nstib++] = 255;		\
					z -= 255;			\
				}					\
				stib[nstib++] = (unsigned char) z;	\
				for (j = 0; j < a; j++)			\
					stib[nstib++] = tmp[j];		\
				a = 0;					\
			}
#define NONRUN(b)	{	if ((z = insert(b, *s)) == -1)		\
				{	newdict();			\
					munge(len, ht, olen);		\
					return;				\
				}					\
				stib[nstib++] = (unsigned char) z;	\
				b = 0;					\
			}	

munge(len, ht, olen)
{	register unsigned char *s = out;
	register int i, j, m = 0, n = 0, z;
	unsigned char tmp[LARGE];

	nstib = 0;
	if (*s == *(s+1))	/* always start with non-runlength */
		stib[nstib++] = 0;

	for (i = 0; i < olen; i++, s++)
	{	if (*s == *(s+1))
		{	if (m > 0) RUN(m)
			n++;
		} else
		{	if (n > 0) NONRUN(n)
			tmp[m++] = *s;
		}
	}

	if (m > 0) RUN(m)
	if (n > 0) NONRUN(n)

	cleanout(len, ht);
}

newdict()
{
	if (nDict >= MAXDICT)
	{	fprintf(stderr, "preflicks: too many dictionaries\n");
		exit();
	}
	Dict[nDict++].ndict = 0;
}

insert(f, v)
{	register int i, j, k;
	k = nDict-1;

	for (i = 0; i < Dict[k].ndict; i++)
		if (Dict[k].dict[i].freq == f && Dict[k].dict[i].val == v)
			return i;
	if (i >= 256)
		return -1;

	Dict[k].dict[i].freq = f;
	Dict[k].dict[Dict[k].ndict++].val = v;

	return i;
}

twobyte(fd, n)
{	unsigned char y;
	y = (unsigned char)(n>>8);
	write(fd, &y, 1);
	y = (unsigned char)n;
	write(fd, &y, 1);
}

onebyte(fd, a)
{	unsigned char c = a;
	write(fd, &c, 1);
}

void
wrapup()
{	int i, j, n, fd;

	signal(SIGINT, SIG_IGN);
	if (frameno == 0) goto shortcut;

	close(ofd);
	if ((ofd = open(tempname, 0)) == -1)
	{	fprintf(stderr, "preflicks: cannot reopen ");
		perror(tempname);
		exit(1);
	}
	if(oname){
		if ((fd = creat(oname, 0666)) == -1)
		{	perror(oname);
			exit(1);
		}
	}
	else
		fd = 1;	/* standard output */
	for (i = 0, n = 3; i < nDict; i++)
		n += 2+3*Dict[i].ndict;

	onebyte(fd, Version);
	twobyte(fd, n);
	twobyte(fd, frameno);
	onebyte(fd, nDict);

	for (i = 0; i < nDict; i++)
	{	twobyte(fd, Dict[i].ndict);
		for (j = 0; j < Dict[i].ndict; j++)
		{	twobyte(fd, Dict[i].dict[j].freq);
			onebyte(fd, Dict[i].dict[j].val);
	}	}

	while ((n = read(ofd, out, LARGE)) > 0)
		write(fd, out, n);
	close(fd);
shortcut:
	close(ofd);
	unlink(tempname);
	exit(0);
}

what(file)
	char *file;
{
	unsigned char bits[8], c;
	int i, j, k, n, fd;

	if ((fd = open(file, 0)) == -1)
	{	perror(file);
		exit(0);
	}
	read(fd, &c, 1);
	if (c != Version)
	{	fprintf(stderr, "%s is outdated (version %d)\n", file, c);
		exit(0);
	}
	if (read(fd, bits, 5) != 5)
	{	fprintf(stderr, "%s: unexpected eof\n", file);
		exit(0);
	}
	j = (bits[0]<<8) + bits[1];
	i = (bits[2]<<8) + bits[3];
	n = bits[4];
	lseek(fd, j-3, 1);
	if (read(fd, bits, 4) != 4)
	{	fprintf(stderr, "%s: unexpected eof\n", file);
		exit(0);
	}
	j = (bits[0]<<8) + bits[1];
	k = (bits[2]<<8) + bits[3];
	printf("%s: %d frame%s (%dx%d), %d lookup table%s\n",
		file, i, (i!=1)?"s":"", j, k, n, (n!=1)?"s":"");
	
}

catenate(file)
	char *file;
{	int n, fd; char c;

	if ((fd = open(file, 0)) == -1)
	{	perror(file);
		exit(1);
	}
	read(fd, &c, 1);
	if (c != Version)
	{	fprintf(stderr, "%s is outdated (version %d)\n", file, c);
		exit(1);
	}
	if (!getinfo(fd))
	{	fprintf(stderr, "%s: cannot read lookup tables\n", file);
		exit(1);
	}
	frameno++;
}

int dsize[512];
int nrtbs[512];
int nrfr=0, nrtb=0, nsize=0;
int dfd[512];

getinfo(ifd)
{	unsigned char bits[8];

	if (read(ifd, bits, 5) != 5)
		return 0;

	dfd[frameno] = ifd;
	dsize[frameno] = (bits[0]<<8) + bits[1] - 3;
	nrtbs[frameno] = nrtb;

	nsize += dsize[frameno];
	nrfr += (bits[2]<<8) + bits[3];
	nrtb += bits[4];

	return 1;
}

purge()
{	int i, j, len, dct, cumdcts=0;
	unsigned char *bits;

	bits = (unsigned char *) malloc(512*512);
	fprintf(stderr, "total: %d files, %d frames, %d tables\n",
							frameno, nrfr, nrtb);
	onebyte(1, Version);
	twobyte(1, nsize+3);
	twobyte(1, nrfr);
	onebyte(1, nrtb);
	for (i = 0; i < frameno; i++)		/* concatenate dictionaries */
	{	j = read(dfd[i], bits, dsize[i]);
		write(1, bits, j);
	}
	for (i = 0; i < frameno; i++)		/* concatenate frames */
	while (read(dfd[i], bits, 7) == 7)
	{	len   = (bits[4]<< 8) + bits[5];
		bits[6] += nrtbs[i];
		write(1, bits, 7);
		if (read(dfd[i], bits, len) != len)
		{	fprintf(stderr, "unexpected eof in file %d\n", i);
			exit(0);
		}
		write(1, bits, len);
	}
}
