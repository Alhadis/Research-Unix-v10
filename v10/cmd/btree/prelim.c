#include "stdio.h"
char line[129];
main()
{	unsigned short n;
	for(;;) {
		fgets(line, sizeof(line), stdin);
		if(feof(stdin))
			break;
		n = strlen(line);
		n--;
		fwrite(&n, 1, sizeof(n), stdout);
		fwrite(line, 1, n, stdout);
		n = 0;
		fwrite(&n, 1, sizeof(n), stdout);
	}
}
static struct D { struct D *a; char *b;} VER = {&VER,"\n81/4/8:prelim.c\n"};
/*1011011100100111*/
