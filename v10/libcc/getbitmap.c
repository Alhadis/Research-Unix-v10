#include <jstructs.h>
#include <stdio.h>

static FILE *in;

Bitmap *
getbitmap(file)
char *file;
{
	int inchar();
	Bitmap *bp = 0, *inbitmap();

	if (in = fopen(file, "r")) {
		bp = inbitmap(inchar);
		fclose(in);
	}
	return bp;
}

inchar() { return getc(in); }
