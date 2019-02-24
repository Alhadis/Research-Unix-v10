#include <stdio.h>

#define	CHARSIZE	/*((7*972)/368)*/ 9

#define OX	0
#define OY	0

struct Rectangle {
	short ox, oy;
	short cx, cy;
} rect;

char rasters[2][100] = { 0 };

main()
{
	register unsigned bit=0, byte, h, v;
	int nrasters, rastwid, i, nw, k, j;
	long t = time((long *)0);

	nrasters=getw(stdin);
	rastwid =getw(stdin);

	if (rastwid & 0x8000) {
		rastwid &= ~0x8000;
		rect.cx = OX + rastwid;
		rastwid = (rastwid + 15)/16;
	} else
		rect.cx = OX + rastwid*16;

	rect.ox = OX;
	rect.oy = OY; rect.cy = OY + nrasters;
	nw = (rect.cx-rect.ox+31)/32;
	nw *= 4;

	printf("static Word bits%d[%d] = {\n", t, nrasters*nw/4);

	for(i=0, v=rect.oy; v<rect.cy; i=1-i, v++) {
		readrast(i, rastwid, stdin);
		k = (nw/2+3)/4*4;
		for(j = 0; j < k; j++)
			printf("%s%02x%s", (j%4==0)?"0x":"", rasters[i][j]&255,
				(j%4==3)?", ":"");
		printf("\n");
		for(; j < nw; j++)
			printf("%s%02x%s", (j%4==0)?"0x":"", rasters[i][j]&255,
				(j%4==3)?", ":"");
		printf("\n");
	}
	printf("};\n");
	printf("Bitmap strike = {\n	bits%d,\n", t);
	printf("	%d,\n", nw/4);
	printf("	%d, %d, %d, %d,\n", rect.ox, rect.oy, rect.cx, rect.cy);
	printf("	0,\n};\n");
	exit(0);
}

readrast(n,nw,in)
	int n, nw; FILE *in;
{
	int count, ctype;
	int nb = nw*2;
	char *p1 = rasters[n];

	while (nw>0) {
		count=getc(in); count &= 255;
		ctype=count&128; count &= 127;
		nw -= count; count *= 2;

		if (ctype) {
			fread(p1,2,1,in);
			for (count-=2; count>0; count--) {
				*(p1+2) = *p1;
				p1 += 1;
			}
			p1 += 2;
		} else {
			fread(p1,count,1,in);
			p1 += count;
		}
	}
	while(nb--)
		rasters[n][nb] ^= rasters[1-n][nb];
}

getw(stream)
FILE *stream;
{
	register l, h;
	l=getc(stream); h=getc(stream); return (h&255)<<8 | (l&255);
}
