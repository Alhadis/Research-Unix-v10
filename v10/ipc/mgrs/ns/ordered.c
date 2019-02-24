#include <fio.h>
#include <string.h>
#include <libc.h>
#include "dbtypes.h"

#define MAXDEPTH 32

/*
 *  free the pointer list
 */
Ordered::~Ordered()
{
	if(ptr) {
		free((char *)ptr);
	}
}

/*
 *  Create the head of an ordered list.  It has `l' pointers and all
 *  point nowhere.
 */
Ordered::Ordered(int l)
{
	int i;

	if(l>MAXDEPTH)
		l = MAXDEPTH;
	level = l-1;
	ptr = (Ordered **)malloc(l*sizeof(Ordered *));
	for(i=0; i<l; i++)
		ptr[i] = (Ordered *)0;
}

/*
 *  insert a node into a list
 */
void
Ordered::insert(Ordered *o)
{
	register Ordered *cp;
	register int i;
	Ordered *cptr[MAXDEPTH];

	/*
	 *  start all search pointers at the list head
	 */
	cptr[o->level] = o;

	/*
	 *  find the last entry <= this one
	 */
	for(i=o->level; i>=0; i--){
		while(cp=cptr[i]->ptr[i]){
			if(cp->compare(this)>0)
				break;
			cptr[i] = cp;
		}
		if(i>0)
			cptr[i-1] = cptr[i];
	}

	/*
	 *  pick a depth 
	 */
	level = o->level+1;
	i = nrand((1<<level)-1)+1;
	for(; i; i = i>>1)
		level--;

	/*
	 *  chain it after current nodes
	 */
	ptr = (Ordered **)malloc((level+1)*sizeof(Ordered *));
	for(i=0; i<=level; i++) {
		ptr[i] = cptr[i]->ptr[i];
		cptr[i]->ptr[i] = this;
	}
}

/*
 *  search an ordered list
 */
Ordered *
Ordered::search(Ordered *o)
{
	register int i, rv;
	Ordered *cptr[MAXDEPTH];

	/*
	 *  start all search pointers at the list head
	 */
	cptr[o->level] = o;

	/*
	 *  Find the first entry matching this key.  Note that
	 *  we return only if we find a match at level 0.  Otherwise
	 *  we may not find the first match.
	 */
	for(i=o->level; i>=0; i--){
		while(cptr[i]->ptr[i]){
			rv = cptr[i]->ptr[i]->compare(this);
			if(rv==0 && i==0)
				return cptr[i]->ptr[i];
			if(rv>=0)
				break;
			cptr[i] = cptr[i]->ptr[i];
		}
		if(i>0)
			cptr[i-1] = cptr[i];
	}

	return (Ordered *)0;
}

/*
 *  output an ordered list
 */
void
Ordered::printlist(int fd)
{
	Ordered *o;

	for(o=ptr[0]; o; o=o->ptr[0]){
		Fprint(fd, "%d: ", o->level);
		o->print(fd);
		Fprint(fd, "\n");
	}
}

/*
 *  output an ordered list element
 */
void
Ordered::print(int fd)
{
	Fprint(fd, "element");
}

/*
 *  compare two ordered list elements
 */
int
Ordered::compare(Ordered *o)
{
	return o-this;
}
