#include	<string.h>
#include	<stdlib.h>

typedef struct Time
{
	short f, s, m, h;
} Time;
extern Time atotime(), gettime();

#define		BYTE(msd, lsd)	(((msd<<4)&0xF0)|(lsd&0xF))

extern int logfd;
