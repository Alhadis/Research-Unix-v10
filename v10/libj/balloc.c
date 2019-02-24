#include <stdio.h>
#include "jerq.h"
#include "balloc.h"
#include "commands.h"

Bitmap BMT[BMTsize];
int avail = -1;

Bitmap *balloc(r) Rectangle r;
{	int i;
	if (avail<0) {
		for (i=0; i<BMTsize; i++) BMT[i].jname = -1;
		avail = 0;
	}
	else if (avail>BMTsize) {
		fprintf(stderr,"BMT full\n");
		return((Bitmap *) NULL);
	}
	else {
		sendCommand(CCBALLOC);
		sendInt(avail);
		sendRect(r);
		i = recInt();
		if (i!=avail) {
			fprintf(stderr, "balloc failed\n");
			return((Bitmap *) NULL);
		}
		for (avail++; avail<BMTsize && BMT[avail].jname!=-1; avail++) ;
		BMT[i].jname = i;
		BMT[i].rect = r;
		return(&BMT[i]);
	}
}

void bfree(b) Bitmap *b;
{	int i;
	if (b->jname > 0) fprintf(stderr,"BFREE: unknown Bitmap\n");
	else {
		sendCommand(CCBFREE);
		sendInt(b->jname);
		b->jname = -1;
		i = BMT - b;
		if (avail>i) avail = i;
	}
}
