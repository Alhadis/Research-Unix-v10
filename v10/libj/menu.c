#include "jerq.h"
#include "commands.h"
#include "io.h"

#define MAXMENU 20
Menu *menutab[MAXMENU];

static char **table;

static char *tablegen(i)
{
	return table[i];
}

int findMenu(m) Menu *m;
{	int i, j;
	char *s, *(*generator)();

	for (i=0; i<MAXMENU && menutab[i]!=m & menutab[i]!=(Menu *)NULL; i++) ;
	if (i==MAXMENU) i=nrand(MAXMENU);
	if (menutab[i]!=m) {
		menutab[i] = m;
		sendCommand(CCMENU);
		sendInt(i);
		generator = (table=m->item) ? tablegen : m->generator;
		for(j=0; s=(*generator)(j); ++j) ;
		sendInt(j);
		for(j=0; s=(*generator)(j); ++j) sendToChar(s);
	}
	return(i);
}


int menuhit(m, b) Menu *m; int b;
{	int i;
	i = findMenu(m);
	sendCommand(CCMENUHIT);
	sendInt(i);
	sendInt(b);
	flush();
	clearMouseQ();
	return recInt();
}

