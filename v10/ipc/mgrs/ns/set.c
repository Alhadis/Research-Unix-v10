#include <libc.h>
#include <fio.h>
#include <string.h>
#include "dbtypes.h"
extern "C" qsort(Tuple **, int, int, int (*)(Tuple **, Tuple **));

/*
 *  add a tuple to a set
 */
void
Set::add(Tuple *t)
{
	register Tuple *tp;

	/*
	 *  make sure it isn't already in the set
	 */
	for(tp=first; tp; tp=tp->next)
		if(tp==t)
			return;

	/*
	 *  add it
	 */
	if(first==(Tuple *)0)
		first = last = t;
	else {
		last->next = t;
		last = t;
	}
	last->next = (Tuple *)0;
	tuples++;
}

/*
 *  output a set in it's linear form
 */
int
Set::print(int fd)
{
	Tuple *t;

	for(t=first; t; t=t->next){
		if (Fprint(fd, "\t") < 0)
			return -1;
		if (t->print(fd) < 0)
			return -1;
		if (Fprint(fd, "\n") < 0)
			return -1;;
	}
	return 0;
}

/*
 *  sort a set of tuples on distance from the origin
 */
int
comparetuples(Tuple **a, Tuple **b)
{
	return (*a)->dist - (*b)->dist;
}
void
Set::sort(Tuple *origin)
{
	static Tuple **array;
	static int arraylen;
	Tuple *t;
	int i;

	/*
	 *  Find distance from origin of all tuples in the set, and
	 *  put them into an array for sorting.
	 */
	if(tuples > arraylen){
		if(array)
			delete array;
		array = new Tuple *[tuples];
		arraylen = tuples;
	}

	for(t=first, i=0; t; i++, t=t->next){
		t->distance(origin);
		array[i] = t;
	}

	/*
	 *  sort the easy way
	 */
	qsort(array, tuples, sizeof(Tuple *), comparetuples);

	/*
	 *  rethread
	 */
	for(i=0; i<tuples-1; i++)
		array[i]->next = array[i+1];
	array[tuples-1]->next = (Tuple *)0;
	first = array[0];
	last = array[tuples-1];
}

/*
 *  input a set from a file
 */
Set::Set(int fd)
{
	char *cp;
	Tuple *t;

	tuples = 0;
	first = last = (Tuple *)0;
	while(cp=Frdline(fd)){
		while(*cp==' ' || *cp=='\t')
			cp++;
		if(*cp==0 || *cp=='#')
			continue;
		t = new Tuple(cp, (Ordered *)0, (Tuple **)0);
		add(t);
	}
}

/*
 *  Print the value of the first attribute matching one of the types.
 */
void
Set::printvalue(int fd, char **types)
{
	Tuple *t;

	for(; *types; types++)
		for(t=first; t; t=t->next)
			if(t->printvalue(fd, *types)==0)
				return;
}
