/* Copyright 1989 by AT&T Bell Laboratories */
extern chatting();
#define printf chatting
#define plural(a,b,c) (((a)==1)?(b):(c))

#ifdef PROFILE

#include <stdio.h>
#include "prof.h"

int profvec[PROFSIZE];
#define getprof(x) (profvec[x]/2)

int num_closures = 0;
int space_closures = 0;
int num_closure_accesses = 0;
int num_links_traced = 0;
int num_records = 0;
int space_records = 0;
int num_spills = 0;
int space_spills = 0;
int total = 0;
int descriptors = 0;
int num_calls = 0;
int num_arith_ops = 0;
int num_arith_ovh = 0;
extern int arenasize;

/*
#define links(i) (getprof(LINKS+i))
*/
#define closures(i) (getprof(CLOSURES+i))
#define records(i) (getprof(RECORDS+i))
#define spills(i) (getprof(SPILLS+i))
#define arith(i) (getprof(ARITHOVH+i))

find_profile_info()
{
	int i;

	num_calls = getprof(KNOWNCALLS) + getprof(STDKCALLS) + getprof(STDCALLS);

	for (i = 0; i < CLOSURESLOTS; i++)
		num_closures += closures(i);
	for (i = 1; i < CLOSURESLOTS; i++)
		space_closures += closures(i) * (i+1);
	space_closures += getprof(CLOSUREOVFL) + closures(0);

/*
	for (i = 0; i < LINKSLOTS; i++)
		num_closure_accesses += links(i);
	for (i = 1; i < LINKSLOTS; i++)
		num_links_traced += links(i) * i;
	num_links_traced += getprof(BIGLINKS);
*/

	for (i = 0; i < RECORDSLOTS; i++)
		num_records += records(i);
	for (i = 1; i < RECORDSLOTS; i++)
		space_records += records(i) * (i+1);
	space_records += getprof(RECORDOVFL) + records(0);

	for (i = 0; i < SPILLSLOTS; i++)
		num_spills += spills(i);
	for (i = 1; i < SPILLSLOTS; i++)
		space_spills += spills(i) * (i+1);
	space_spills += getprof(SPILLOVFL) + spills(0);
	total = space_closures + space_records + space_spills
		+ getprof(ARRAYSIZE) + getprof(ARRAYS)
		+ getprof(STRINGSIZE) + getprof(STRINGS)
		+ getprof(REFCELLS) * 2
		+ getprof(REFLISTS) * 2;

	descriptors = num_closures + num_records + num_spills
		 + getprof(ARRAYS) + getprof(STRINGS)+ getprof(REFCELLS);


	for (i = 0; i < ARITHSLOTS; i++)
		num_arith_ops += arith(i);
	for (i = 1; i < ARITHSLOTS; i++)
		num_arith_ovh += arith(i) * i;

}

print_alloc_info()
{
	int k; float f;
	char *s,*s1;

	printf("\nProfiling information:\n\n");

	printf("%d function calls:\n",num_calls);
	printf("%d known (%.1f%%),\n%d stdk (%.1f%%),\n",
		getprof(KNOWNCALLS),100.0*getprof(KNOWNCALLS)/num_calls,
		getprof(STDKCALLS),100.0*getprof(STDKCALLS)/num_calls);
	printf("%d continuation (%.1f%%),\n%d std (%.1f%%).\n\n",
		getprof(CNTCALLS),100.0*getprof(CNTCALLS)/num_calls,
		getprof(STDCALLS),100.0*getprof(STDCALLS)/num_calls);

	printf("Maximum heap size %dk\n",arenasize/1024);
	printf("Heap allocations: (only total sizes include descriptors)\n");
	printf("Total size %d; %d descriptors accounted for %.1f%%.\n\n",
		total, descriptors, 100.0*descriptors/total);

	printf("  Size   Number   %% total   Total size    %% total\n\n");

	if (num_closures > 0) {
	printf("Closures:\n");
	for (k = 1; k < CLOSURESLOTS; k++)
		if (closures(k) > 0)
		printf("%6d%9d%9.1f%%%13d%10.1f%%\n",
			k,
			closures(k),
			100.0*closures(k)/num_closures,
			closures(k) * (k+1),
			100.0*(closures(k)*(k+1))/total);
	if (closures(0) > 0)
		printf(">%5d%9d%9.1f%%%13d%10.1f%%\n",
			CLOSURESLOTS - 1,
			closures(0),
			100.0*closures(0)/num_closures,
			getprof(CLOSUREOVFL)+closures(0),
			100.0*(getprof(CLOSUREOVFL)+closures(0))/total);
	printf("Total:%9d%23d%10.1f%%  Average size %.2f\n\n",
	    num_closures, space_closures, 100.0*space_closures/total,
	    (float)(space_closures-num_closures)/num_closures);
	}

	if (num_records > 0) {
	printf("Records:\n");
	for (k = 1; k < RECORDSLOTS; k++)
		if (records(k) > 0)
		printf("%6d%9d%9.1f%%%13d%10.1f%%\n",
			k,
			records(k),
			100.0*records(k)/num_records,
			records(k) * (k+1),
			100.0*(records(k) * (k+1)) / total);
	if (records(0) > 0)
		printf(">%5d%9d%9.1f%%%13d%10.1f%%\n",
			RECORDSLOTS - 1,
			records(0),
			100.0*records(0)/num_records,
			getprof(RECORDOVFL) + records(0),
			100.0*(getprof(RECORDOVFL) + records(0)) / total);
	printf("Total:%9d%23d%10.1f%%  Average size %.2f\n\n",
	    num_records,space_records,100.0*space_records/total,
	    (float)(space_records-num_records)/num_records);
	}

	if (num_spills > 0) {
	printf("Spills:\n");
	for (k = 1; k < SPILLSLOTS; k++)
		if (spills(k) > 0)
		printf("%6d%9d%9.1f%%%13d%10.1f%%\n",
			k,
			spills(k),
			100.0*spills(k)/num_spills,
			spills(k) * (k+1),
			100.0*(spills(k) * (k+1)) / total);
	if (spills(0) > 0)
		printf(">%5d%9d%9.1f%%%13d%10.1f%%\n",
			SPILLSLOTS - 1,
			spills(0),
			100.0*spills(0)/num_spills,
			getprof(SPILLOVFL) + spills(0),
			100.0*(getprof(SPILLOVFL) + spills(0)) / total);
	printf("Total:%9d%23d%10.1f%%  Average size %.2f\n\n",
	    num_spills,space_spills,100.0*space_spills/total,
	    (float)(space_spills-num_spills)/num_spills);
	}

	if (getprof(ARRAYS))
	printf("Arrays:%8d%23d%10.1f%%  Average size %.2f\n",
		getprof(ARRAYS),
		getprof(ARRAYSIZE) + getprof(ARRAYS),
		100.0*(getprof(ARRAYSIZE) + getprof(ARRAYS)) / total,
		(float)getprof(ARRAYSIZE)/getprof(ARRAYS));

	if (getprof(STRINGS))
	printf("Strings:%7d%23d%10.1f%%  Average size %.2f\n",
		getprof(STRINGS),
		getprof(STRINGSIZE) + getprof(STRINGS),
		100.0*(getprof(STRINGSIZE) + getprof(STRINGS)) / total,
		(float)getprof(STRINGSIZE)/getprof(STRINGS));

	if (getprof(REFCELLS))
	printf("Refs:%10d%23d%10.1f%%\n",
		getprof(REFCELLS),
		getprof(REFCELLS)*2,
		100.0*(getprof(REFCELLS)*2) / total);

	if (getprof(REFLISTS))
	printf("Reflist\ncells:%9d%23d%10.1f%%\n",
		getprof(REFLISTS),
		getprof(REFLISTS)*2,
		100.0*(getprof(REFLISTS)*2) / total);

}


print_arith_info()
{int i;
 printf("\n");
 printf("Arithmetic Overhead Information:\n");
 for(i=0;i<ARITHSLOTS;i++)
   printf("Overhead %d:    %9d\n",i,arith(i));
 printf(  "Total Overhead: %9d\n",num_arith_ovh);
 printf(  "Total Operations:%8d\n",num_arith_ops);
 printf("\n");
}

print_profile_info()
{
	find_profile_info();
	if (num_closures > 0 || num_records > 0 || num_spills > 0 || num_calls > 0)
		print_alloc_info();
        if (num_arith_ops > 0)
		print_arith_info();
}

#endif
