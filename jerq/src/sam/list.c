#include "sam.h"

/*
 * Check that list has room for one more element.
 */
growlist(l)
	register List *l;
{
	if(l->ptr==0 || l->nalloc==0){
		gcnew(l->ptr, l->nalloc=INCR);
		l->nused=0;
	}else if(l->nused==l->nalloc)
		gcrenew(l->ptr, l->nalloc+=INCR);
}
/*
 * Remove the ith element from the list
 */
dellist(l, i)
	register List *l;
{
	bcopy((uchar *)&l->ptr[i+1], (uchar *)&l->ptr[l->nused],
		(uchar *)&l->ptr[i], 1);
	l->nused--;
}
/*
 * Add a new element, whose position is i, to the list
 */
inslist(l, i, val)
	register List *l;
	register i;
	long val;
{
	growlist(l);
	bcopy((uchar *)&l->ptr[i], (uchar *)&l->ptr[l->nused],
		(uchar *)&l->ptr[i+1], -1);
	l->ptr[i]=val;
	l->nused++;
}
listfree(l)
	register List *l;
{
	gcfree((uchar *)l->ptr);
	free((uchar *)l);
}
