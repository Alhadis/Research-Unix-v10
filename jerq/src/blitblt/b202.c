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
	int nrasters, rastwid, i, iprev, j, hprev, hdelta, hnl = 0;

	if (nrasters=getw(stdin)) {
		if ((rastwid =getw(stdin)) & 0x8000) {
			rastwid &= ~0x8000;
			rect.cx = OX + rastwid;
			rastwid = (rastwid + 15)/16;
		} else
			rect.cx = OX + rastwid*16;
		rect.ox = OX;
		rect.oy = OY; rect.cy = OY + nrasters;
	} else {
		rect.ox = getw(stdin);
		rect.oy = getw(stdin);
		rect.cx = getw(stdin);
		rect.cy = getw(stdin);
		nrasters =  rect.cy - rect.oy;
		rastwid  = (rect.cx - rect.ox + 15)/16;
	}

	printf("x T 202\nx res 972 1 2\nx init\n");
	printf("x font 1 H\nx font 2 HI\nx font 3 HB\nx font 10 S\n");
	printf("p1\nf1\ns6\n");

	for (i=0,iprev=1,v=rect.oy; v<rect.cy; iprev=i,i=1-i,v++) {
		hprev = rect.ox*CHARSIZE;
		printf("H%dV%d\n", hprev, v*CHARSIZE);
		readrast(rasters[i],rastwid,stdin);
		for (bit=0,j=0,h=rect.ox; h<rect.cx; h++) {
			if ((bit >>= 1) == 0) {
				byte = (rasters[i][j] ^= rasters[iprev][j]);
				bit  = 0x80; j++;
			}
			if (byte&bit) {
				if ((hdelta = h*CHARSIZE - hprev) < 100)
					printf("%02d.", hdelta);
				else
					printf("h%dc.", hdelta);
				hprev += hdelta;
				if ((hnl += 5) >= 80) {
					printf("\n");
					hnl = 0;
				}
			}
		}
		if (hnl) {
			printf("\n");
			hnl = 0;
		}
	}
	exit(0);
}

readrast(p1,nw,in)
char *p1; int nw; FILE *in;
{
	int count, ctype;

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
}

getw(stream)
FILE *stream;
{
	register l, h;
	l=getc(stream); h=getc(stream); return (h&255)<<8 | (l&255);
}
