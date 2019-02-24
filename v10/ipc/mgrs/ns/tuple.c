#include <libc.h>
#include <fio.h>
#include <string.h>
#include "dbtypes.h"

/*
 *  free a tuple and all the attributes making it up
 */
Tuple::~Tuple()
{
	register Attribute *ap, *nap;

	for(ap=first; ap; ap=nap){
		nap = ap->next;
		delete ap;
	}
}

/*
 *  Parse a list of attributes and add the attributes to this tuple.
 *  If o is non-zero add each attribute to that ordered list.
 */
Tuple::Tuple(char *str, Ordered *o, Tuple **alist, char wildcard)
{
	char *attributes[MAXPAIRS];
	int i, n;

	first = last = (Attribute *)0;
	tail = (Tuple *)0;

	/*
	 *  parse and create the tuple
	 */
	setfields(" \t\n");
	n = getmfields(str, attributes, MAXPAIRS);
	for(i=0; i<n; i++){
		if(*attributes[i]=='\0')
			break;
		if(first==(Attribute *)0)
			first = last =new Attribute(attributes[i], this, wildcard);
		else {
			last->next = new Attribute(attributes[i], this, wildcard);
			last = last->next;
		}
		/*
		 *  insert each attribute into an ordered list
		 */
		if(o)
			last->insert(o);
	}
	/*
	 *  chain into an allocation list for later deallocation
	 */
	if(alist){
		anext = *alist;
		*alist = this;
	}
}

/*
 *  concatenate two tuples
 */
void
Tuple::cat(Tuple *ntail)
{
	tail = ntail;
}

/*
 *  Calculate the distance of this tuple from some other tuple.  The
 *  distance measure is the minimum of the distances between all of this
 *  tuple's attributes and all of the origin's.
 */
void
Tuple::distance(Tuple *origin)
{
	Attribute *o;
	Attribute *p;
	Tuple *t;
	int ndist;

	dist = MAXDISTANCE;
	for(t=this; t; t=t->tail){
		for(p=t->first; p; p=p->next){
			for(o=origin->first; o; o=o->next){
				ndist = p->distance(o);
				if(ndist<dist) {
					dist = ndist;
				}
			}
		}
	}
}

/*
 *  Return 0 if the tuple doesn't match the pattern, non-zero otherwise.
 */
Tuple::match(Tuple *pattern)
{
	Attribute *p;
	Attribute *me;
	Tuple *t;

	for(p=pattern->first; p; p=p->next){
		/*
		 *  each tuple can be a list of tuples
		 */
		for(t=this; t; t=t->tail){
			for(me=t->first; me; me=me->next){
				if(me->compare(p)==0)
					break;
			}
			if(me)
				break;
		}
		if(me==(Attribute *)0)
			return 0;
	}
	return 1;
}

/*
 *  output a linear version of the tuple on fd
 */
int
Tuple::print(int fd)
{
	Attribute *p;
	Tuple *t;

	for(t=this; t; t=t->tail){
		for(p=t->first; p; p=p->next){
			if (p->simpleprint(fd) < 0)
				return -1;
		}
	}
	return 0;
}

/*
 *  Free a list of tuples
 */
void
freetuplelist(Tuple **list)
{
	Tuple *t, *nt;

	if(*list==(Tuple *)0)
		return;

	for(t=*list; t; t=nt){
		nt = t->anext;
		delete t;
	}

	*list = (Tuple *)0;
}

/*
 *  Lookup a tuple in an ordered list.  Return the set of matches.
 */
Set *
lookup(char *ks, Ordered *o)
{
	Tuple *kt;
	Attribute *a;
	Set *s=0;
	
	/*
	 *  parse the key
	 */
	kt = new Tuple(ks, '*');
	/*
	 *  Using first attribute as the primary key, find the first
	 *  attribute in the ordered list that matches.
	 */
	a = (Attribute *)kt->first->search(o);
	/*
	 *  while the next attribute in the ordered list
	 *  matches the search key, check the rest of the
	 *  attributes for a match
	 */
	while(a && a->compatible(kt->first)) {
		if(a->tuple->match(kt)){
			/*
			 *  add each matching tuple to the
			 *  set to be returned
			 */
			if(s==(Set*)0)
				s = new Set;
			s->add(a->tuple);
		}
		a = (Attribute *)a->succ();
	}
	delete kt;
	return s;
}

/*
 *  Print the value of the first attribute matching type.  Return 0 if a value
 *  is printed.
 */
int
Tuple::printvalue(int fd, char *type)
{
	Tuple *t;
	Attribute *a;

	for(t=this; t; t=t->tail)
		for(a=t->first; a; a=a->next)
			if(a->printvalue(fd, type)==0)
				return 0;
	return -1;
}
