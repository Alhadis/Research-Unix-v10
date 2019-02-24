#include <jerq.h>

texture(b, r, t, c)
	Bitmap *b;
	Rectangle r;
	Texture *t;
	Code c;
{
	Texture32 local;
	register short *dest=(short *)&local;
	register short *ssrc=(short *)t;
	register int i;

	i=16;
	do{
		*dest= *ssrc;
		*(dest+1)= *ssrc;
		*((int *)(dest+32))= *((int *)dest);
		ssrc++;
		dest+=2;
	}while(--i>0);
	texture32(b, r, &local, c);
}
