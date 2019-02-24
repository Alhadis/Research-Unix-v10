#include	<stdio.h>
#include	"defines.h"

main()
{	int i, j, k; int xo=-2048, yo=1536;
	unsigned char obuf[256];

	qreset();
	resolution(4);
	rotate(0);

	for (k = 0; k < 8; k++)
	{	for (i = 0; i < 256; i++)
			obuf[i] = 255 - k*32;
		printf("value: %d\n", 255-k*32);
		for (j = 0; j < 9; j++)
		{	handshake(1);
			i = j;
		/*	i = (j == 2 || j == 3) ? 5-j : j;	*/
			brightness(i, i, i, i);
			window(xo + j*256, yo - k*256, 256, 256);
			singlepass(GREEN);
			for (i = 0; i < 256; i++)
				qwrite(obuf, 256);
		}
	}
}
