#include <libc.h>
#include <fio.h>
#include <string.h>
#include "dbtypes.h"

/*
 *  free the value and type of an attribute
 */
Attribute::~Attribute()
{
	if(value){
		free(value);
	}
	if(type){
		free(type);
	}
}

/*
 *  Parse a linear version of an attribute.  Anything ending in an 
 *  the wildcard character specifies that this attribute is a key matching
 *  anything following the wildcard position.
 *
 *  Any unrooted domain name is treated has a '.' attached and is treated
 *  as if it ended in a wild card.
 */
Attribute::Attribute(char *str, Tuple *t, char wildcard)
{
	char *ttype;
	char *wildp;
	char *p;

	ttype = strchr(str, ',');
	if(ttype){
		*ttype++ = '\0';
		type = strdup(ttype);
	} else
		type = (char *)0;

	wildp = str+strlen(str)-1;
	if(*wildp==wildcard){
		*wildp = '\0';
		wild = 1;
	} else
		wild = 0;

	if(fstrcmp(type, "dom")==0){
		/*
		 *  domains are case insensitive, convert to lower
		 */
		for(p = str; *p; p++)
			*p = tolower(*p);

		/*
		 *  root the wild card
		 */
		if(wildcard && type && !wild && *wildp!='.'){
			*(wildp+1) = '.';
			*(wildp+2) = '\0';
			wild = 1;
		}
	}
	value = strdup(str);
	tuple = t;
	next = 0;
}

/*
 *  Compute the distance between two attributes.  The distance measure is at
 *  most MAXDISTANCE and depends on the type of the attributes.  Only attributes
 *  of the same type match.
 */
Attribute::distance(Attribute *origin)
{
	if(type==0 || fstrcmp(origin->type, type)!=0)
		return MAXDISTANCE;
	else if(fstrcmp(origin->type, "dom")==0)
		return dom_distance(value, origin->value);
	else if(fstrcmp(origin->type, "dk")==0)
		return dk_distance(value, origin->value);
	return MAXDISTANCE;
}

/*
 *  output a linear version of the attribute on fd
 */
int
Attribute::simpleprint(int fd)
{
	if(type)
		return Fprint(fd, "%s,%s ", value, type);
	else
		return Fprint(fd, "%s ", value);
}
void
Attribute::print(int fd)
{
	simpleprint(fd);
	if(tuple){
		Fprint(fd, " -> ");
		tuple->print(fd);
	}
}

/*
 *  return true if this is a key attribute
 */
int
Attribute::iskey()
{
	return fstrcmp(type, "org")!=0;
}

/*
 *  compare 2 attributes.  the key may have the type "name" which matches
 *  only an empty type.
 */
Attribute::compare(Ordered *o)
{
	register rv;
	Attribute *pattern=(Attribute *)o;

	if(pattern->wild)
		rv = strncmp(value, pattern->value, strlen(pattern->value));
	else
		rv = fstrcmp(value, pattern->value);
	if(rv==0){
		if(pattern->type && strcmp(pattern->type, "name")!=0){
			if(type)
				rv = fstrcmp(type, pattern->type);
			else
				rv = -1;
		} else {
			if(type && pattern->type)
				rv = -1;
		}
	}
	return rv;
}

/*
 *  Compare 2 attributes.  If they have matching values, return non-zero.
 */
Attribute::compatible(Ordered *o)
{
	Attribute *pattern=(Attribute *)o;

	if(pattern->wild)
		return !strncmp(value, pattern->value, strlen(pattern->value));
	else
		return !fstrcmp(value, pattern->value);
}

/*
 *  Print the value if attribute type == t.  Return 0 if a value
 *  is printed.
 */
int
Attribute::printvalue(int fd, char *t)
{
	if(type){
		if (strcmp(t, type)==0){
			Fprint(fd, "\t%s\n", value);
			return 0;
		}
	} else if(strcmp(t, "name")==0){
		Fprint(fd, "\t%s\n", value);
		return 0;
	}
	return -1;
}
