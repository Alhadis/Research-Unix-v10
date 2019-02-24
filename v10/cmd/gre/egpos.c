#include	"re.h"
#include	"lre.h"
#include	"hdr.h"

#ifndef	POSSTEP
#define		POSSTEP		(8*1024)
#endif

void
eg_posinit(re_re *r)
{
	if(r->nposalloc <= 0)
		r->nposalloc = POSSTEP;
	r->posbase = (int *)egmalloc(r->nposalloc*sizeof(int), "posbase");
	if (!r->posbase)
		return;
	r->posnext = 0;
}

void
eg_posset(re_re *r)
{
	r->posreset = r->posnext;
}

eg_posalloc(re_re *r, int n)
{
	register j;

	if(n < 0){
		r->posnext = r->posreset;
		return(-1);
	}
	j = r->posnext;
	r->posnext += n;
	if(r->posnext >= r->nposalloc){
		while((r->nposalloc < r->posnext) && (r->nposalloc < 256*1024))
			r->nposalloc *= 2;
		if(r->nposalloc < r->posnext){
			r->nposalloc = (r->posnext+POSSTEP-1)/POSSTEP;
			r->nposalloc *= POSSTEP;
		}
		r->posbase = (int *)egrealloc((char *)r->posbase, r->nposalloc*sizeof(int), "posbase");
		if (!r->posbase)
			return(-1);
	}
	return(j);
}
