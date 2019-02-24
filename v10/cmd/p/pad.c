#include <stdio.h>
#include "pad.h"

char *malloc();

#define	NPCBL	8	/* Number of entries in the circular list */

PAD *
Pfopen(f, m)
char *f, *m;
{
	int fd;
	PAD *p;

	if (*m != 'r')	/* hack */
		return (NULL);
	if ((fd = open(f, 0)) < 0)
		return (NULL);
	if ((p = Pfdopen(fd)) == NULL)
		close(fd);
	return (p);
}

PAD *
Pfdopen(fd)
int fd;
{
	register PAD *p;
	register i;

	if ((p = (PAD *)malloc(sizeof(PAD))) == NULL)
		return (NULL);
	p->Pfile = fd;
	p->Pcbl=(_PCBL *)malloc(NPCBL*sizeof(_PCBL));
	if(p->Pcbl==NULL){
		free((char *)p);
		return (NULL);
	}
	p->Pcbb = p->Pcbl;
	for(i=0; i<NPCBL; i++){
		p->Pcbl[i].Pnextp = &p->Pcbl[(i+1)%NPCBL];
		p->Pcbl[(i+1)%NPCBL].Pprevp = &p->Pcbl[i];
		p->Pcbl[i].Pbase = NULL;
		p->Pcbl[i].Phiwat = NULL;
		p->Pcbl[i].Pptr = NULL;
		p->Pcbl[i].Pcnt = 0;
	}
	return (p);
}

int
_Pfilbuf(p)
register PAD *p;
{
	register _PCBL *pcbl;

	pcbl = p->Pcbl;
	if (pcbl->Phiwat >= pcbl->Pbase + BUFSIZ) {
		p->Pcbl = pcbl = pcbl->Pnextp;
		if (pcbl->Pcnt <= 0)		/* stale data */
			pcbl->Phiwat = pcbl->Pbase;
	}
	if (pcbl->Pbase == NULL) {
		if ((pcbl->Pbase = (char *) malloc(BUFSIZ)) == NULL) {
			fprintf(stderr, "pad: can't malloc\n");	/* ? */
			return (EOF);
		}
		pcbl->Phiwat = pcbl->Pbase;
	}
	if (pcbl->Pcnt <= 0) {
		pcbl->Pptr = pcbl->Phiwat;
		pcbl->Pcnt = read(p->Pfile, pcbl->Pptr, (pcbl->Pbase + BUFSIZ) - pcbl->Phiwat);
		if (pcbl->Pcnt > 0)
			pcbl->Phiwat += pcbl->Pcnt;
	}
	if (--pcbl->Pcnt < 0) {
		pcbl->Pcnt = 0;
		return (EOF);
	}
	return (*pcbl->Pptr++);
}

Pclose(p)
	register PAD *p;
{
	register i;

	for(i=0; i<NPCBL; i++)
		if (p->Pcbb[i].Pbase)
			free(p->Pcbb[i].Pbase);
	free((char *)p->Pcbb);
	close(p->Pfile);
	free((char *)p);
}

int
Pbackc(p)
	register PAD *p;
{
	register _PCBL *pcbl;

	pcbl=p->Pcbl;
	if(pcbl->Pptr <= pcbl->Pbase){
		pcbl=pcbl->Pprevp;		/* Only local... */
		if(pcbl->Pptr <= pcbl->Pbase)
			return(EOF);
		p->Pcbl=pcbl;			/* ...until now */
		pcbl->Pcnt = 0;
	}
	++pcbl->Pcnt;
	return(*--pcbl->Pptr);
}
