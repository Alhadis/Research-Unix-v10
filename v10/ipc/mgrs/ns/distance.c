#include <libc.h>
#include <string.h>
#include <fio.h>
#include "dbtypes.h"

dom_distance(
	char *point,
	char *origin
){
	char *p;
	char *o;
	int difference;

	difference = 0;
	p = point+strlen(point)-1;
	o = origin+strlen(origin)-1;

	/*
	 *  find first difference
	 */
	for(; o>=origin && p>=point; o--, p--){
		if(*o!=*p)
			break;
	}
	if(o<origin){
		return 0;	/* the same */
	}

	/*
	 *  count different elements
	 */
	for(; o>=origin; o--)
		if(*o=='.')
			difference++;
	return difference+1;
}

dk_distance(
	char *point,
	char *origin
){
	int difference;

	/*
	 *  find first difference
	 */
	for(difference = 0; *origin; origin++, point++){
		if(*origin!=*point)
			break;
	}
	if(*origin=='\0'){
		return 0;
	}

	/*
	 *  count different elements
	 */
	for(; *origin; origin++)
		if(*origin=='/')
			difference++;
	return difference+1;
}
