#include <stdio.h>
#include "../comm.h"

getn()
{
	register i;

	i = (getchar()<<8) & 0xFF00;
	i |= getchar()&0xFF;
	i = (i << 16) >> 16;
	return(i);
}

char *
getstr()
{
	static char buf[256];
	register char *p;

	for(p = buf; *p = getchar(); p++)
		if(*p == 0xff) break;
	return(buf);
}

main()
{
	register c;

	while((c = getchar()) != EOF)
	{
		c &= 0377;
		if(c <= 127)
			printf("char %c (0%o)\n", c, c);
		else if(c < C_PAGE)
			printf("short horiz %d\n", c-C_HOR);
		else if(c >= C_SIZE)
			printf("short size %d\n", c-C_SIZE+SIZE_ORIG);
		else if((c >= C_FONT) && (c <= C_FONT+9))
			printf("font %d\n", c-C_FONT);
		else switch(c)
		{
		case C_PAGE:
			printf("page %d\n", getn());
			break;
		case C_BSIZE:
			printf("size %d\n", getchar());
			break;
		case C_RELH:
			printf("horiz %d\n", getn());
			break;
		case C_RELV:
			printf("vertical %d\n", getn());
			break;
		case C_DEV:
			printf("devcntl '%s'\n", getstr());
			break;
		case C_DRAW:
			printf("draw '%s'\n", getstr());
			break;
		case C_TERM:
			printf("become a terminal\n");
			break;
		case C_EXIT:
			printf("exit\n");
			break;
		}
	}
}
