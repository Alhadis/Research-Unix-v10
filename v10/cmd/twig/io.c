#include "common.h"
#define MAXINTONLINE 10
static int intcnt, intonline, firstint;

oreset()
{
	intcnt = 0;
	intonline = 0;
	firstint = 1;
}

ointcnt() { return(intcnt); }

oputint(i)
{
	if(!firstint) putc(',', outfile);
	else firstint = 0;
	fprintf(outfile, "%d", i);
	intonline++;
	intcnt++;
	if(intonline >= MAXINTONLINE) { putc('\n', outfile); intonline = 0; }
}

oputoct(i)
{
	if(!firstint) putc(',', outfile);
	else firstint = 0;
	fprintf(outfile, "0%o", i);
	intonline++;
	intcnt++;
	if(intonline >= MAXINTONLINE) { putc('\n', outfile); intonline = 0; }
}
