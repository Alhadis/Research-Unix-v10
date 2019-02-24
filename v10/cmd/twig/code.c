#include "common.h"
#include "code.h"
#define BLOCKSIZE	10

Code *cdfreep = NULL;

Code *
CodeGetBlock()
{
	static int count = 0;
	static Code *blockp = NULL;
	Code *cp;

	if(cdfreep!=NULL) {
		cp = cdfreep;
		cdfreep = cdfreep->prev;
	} else {
		if(count==0) {
			blockp = (Code *) malloc (BLOCKSIZE*sizeof(Code));
			count = BLOCKSIZE;
		}
		cp = blockp++;
		count--;
	}
	cp->prev = NULL;
	cp->firstFree = cp->segment;
	return(cp);
}

void
CodeFreeBlock(cd)
	Code *cd;
{
	if (cd!=NULL) {
		cd->prev = cdfreep;
		cdfreep = cd;
	}
}

Code *
CodeStoreChar(cd, c)
	Code *cd;
	char c;
{
	if(cd->firstFree - cd->segment >= CSEGSIZE) {
		Code *ncd = CodeGetBlock();
		ncd->prev = cd;
		cd = ncd;
	}
	*cd->firstFree = c;
	cd->firstFree++;
	return(cd);
}

Code *
CodeStoreString(cd, s)
	Code *cd;
	char *s;
{
	while(*s!='\0') cd = CodeStoreChar(cd, *s++);
	return(cd);
}

Code *
CodeAppend(cd1, cd2)
	Code *cd1, *cd2;
{
	if(cd2==NULL) return(cd1);
	cd2->prev = CodeAppend(cd1, cd2->prev);
	return(cd2);
}

void
CodeWrite(f, cd)
	FILE *f;
	Code *cd;
{
	register char *cp;

	if (cd != NULL)
		{CodeWrite(outfile, cd->prev);
		 for(cp=cd->segment; cp < cd->firstFree; cp++)
			putc(*cp, f);
		}
}

Code *
CodeMarkLine(cd,lineno)
	Code *cd;
	int lineno;
{
	char buf[100];
	sprintf(buf,"\n# line %d \"%s\"\n", lineno, inFileName);
	return(CodeStoreString(cd,buf));
}
