#include <signal.h>

#define	DITHSIZE	16
#define	DITHMASK	(DITHSIZE-1)

extern void	onquit();
int ofd;

static int dith[DITHSIZE][DITHSIZE]={
	  0,128, 32,160,  8,136, 40,168,  2,130, 34,162, 10,138, 42,170,
	192, 64,224, 96,200, 72,232,104,194, 66,226, 98,202, 74,234,106,
	 48,176, 16,144, 56,184, 24,152, 50,178, 18,146, 58,186, 26,154,
	240,112,208, 80,248,120,216, 88,242,114,210, 82,250,122,218, 90,
	 12,140, 44,172,  4,132, 36,164, 14,142, 46,174,  6,134, 38,166,
	204, 76,236,108,196, 68,228,100,206, 78,238,110,198, 70,230,102,
	 60,188, 28,156, 52,180, 20,148, 62,190, 30,158, 54,182, 22,150,
	252,124,220, 92,244,116,212, 84,254,126,222, 94,246,118,214, 86,
	  3,131, 35,163, 11,139, 43,171,  1,129, 33,161,  9,137, 41,169,
	195, 67,227, 99,203, 75,235,107,193, 65,225, 97,201, 73,233,105,
	 51,179, 19,147, 59,187, 27,155, 49,177, 17,145, 57,185, 25,153,
	243,115,211, 83,251,123,219, 91,241,113,209, 81,249,121,217, 89,
	 15,143, 47,175,  7,135, 39,167, 13,141, 45,173,  5,133, 37,165,
	207, 79,239,111,199, 71,231,103,205, 77,237,109,197, 69,229,101,
	 63,191, 31,159, 55,183, 23,151, 61,189, 29,157, 53,181, 21,149,
	255,127,223, 95,247,119,215, 87,253,125,221, 93,245,117,213, 85,
};

pvputitall(m)
{	int y;
	y = CUROLD; CUROLD = CURSCRATCH; CURSCRATCH = y;
	for (y = 0; y < DEF_NL; y++)
		pvlyank(0, y);
	y = CUROLD; CUROLD = CURSCRATCH; CURSCRATCH = y;
}

unsigned char out[64];

pvlyank(xx, yy)
{	unsigned char *s = scratchred + DEF_LL*(yy-1);
	int byte=0, mask=0x80, nout=0, x;
	int y = yy;

	if (xx != 0) return;

	if (DEF_NL == 1024)
	{	if (y%4) return;
		y /= 4;
		out[nout++] = y>>8;
		out[nout++] = y&255;
		for (x = 0; x < 256; x++, s+=4)
		{	if (*s <= dith[y&DITHMASK][x&DITHMASK])
				byte |= mask;
			mask >>= 1;
			if (mask == 0)
			{	out[nout++] = byte;
				byte = 0;
				mask = 0x80;
			}
		}
	} else
	{	if (y%2) return;
		y /= 2;
		out[nout++] = y>>8;
		out[nout++] = y&255;
		for (x = 0; x < 256; x++, s += 2)
		{	if (*s <= dith[y&DITHMASK][x&DITHMASK])
			byte |= mask;
			mask >>= 1;
			if (mask == 0)
			{	out[nout++] = byte;
				byte = 0;
				mask = 0x80;
			}
		}
	}
	signal(SIGINT, SIG_IGN);
	write(ofd, out, nout);
	signal(SIGINT, onquit);
}
