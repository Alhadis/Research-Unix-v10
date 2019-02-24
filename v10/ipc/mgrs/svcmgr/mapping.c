#include "mgr.h"

Mapping *maphead;		
Mapping *maptail;

/*
 *  Create a new mapping from a string.  The mapping is of the form:
 *  `service-re source-re olduser-re newuser-exp'
 */
Mapping *
newmap(cp)
	char *cp;
{
	Mapping *mp = (Mapping *)malloc(sizeof(Mapping));
#	define FIELDS 4
	char *fields[FIELDS];
	int n;

	if(mp==NULL) {
		logevent("out of memory allocating mapping\n");
		return NULL;
	}
	mp->user = mp->from = mp->serv = NULL;
	mp->luser = NULL;
	setfields(" \t");
	n = getmfields(cp, fields, FIELDS);
	switch(n) {
	case 0:
		freemap(mp);
		return NULL;
	case 1:
		fields[1] = "";
	case 2:
		fields[2] = ".*";
	case 3:
		fields[3] = "&";
		break;
	case 4:
		break;
	default:
		logevent("incorrect number of fields in mapping %s\n", fields[0]);
		freemap(mp);
		return NULL;
	}
	if((mp->serv=newre(fields[0]))==NULL
	|| (mp->from=newre(fields[1]))==NULL
	|| (mp->user=newre(fields[2]))==NULL) {
		logevent("illegal reg exp in mapping `%s %s %s %s'\n", fields[0],
			fields[1], fields[2], fields[3]);
		freemap(mp);
		return NULL;
	}
	mp->luser = strdup(fields[3]);
	logevent("newmap(%s %s %s %s)\n", mp->serv->re, mp->from->re,
		 mp->user->re, mp->luser);
	return mp;
}

freemap(mp)
	Mapping *mp;
{
	if(mp==NULL)
		return;
	if(mp->from)
		freere(mp->from);
	if(mp->serv)
		freere(mp->serv);
	if(mp->user)
		freere(mp->user);
	if(mp->luser)
		free(mp->luser);
}

/*
 *  add a map entry
 */
addmap(mp)
	Mapping *mp;
{
	mp->next = NULL;
	if(maphead==NULL)
		maphead = mp;
	else
		maptail->next = mp;
	maptail = mp;
}

/*
 *  free all map entries
 */
resetmaps()
{
	Mapping *mp, *nxt;

	logevent("resetmaps()\n");
	for(mp=maphead; mp; mp=nxt) {
		nxt = mp->next;
		freemap(mp);
	}
	maphead = maptail = NULL;
}

/*
 *  return a new user id for call
 */
char *
mapuser(service, source, user)
	char *service;
	char *source;
	char *user;
{
	Mapping *mp;
	regsubexp sub[10];
	static char luser[ARB];

	for(mp=maphead; mp; mp=mp->next) {
		if(!execre(mp->serv, service, (regsubexp *)0, 0))
			continue;
		if(!execre(mp->from, source, (regsubexp *)0, 0))
			continue;
		if(!execre(mp->user, user, sub, 10))
			continue;
		regsub(mp->luser, luser, sub, 10);
		return luser;
	}
	return NULL;
}
