#include "mgr.h"

Config *confhead;		
Config *conftail;

/*
 *  Create a new configuration entry.  The entries are of the form
 *	address-re	path-se		my-address
 *  where:
 *	`address-re' is a regular expression matching a network address
 *	`path-se' is a substitution expression to convert the address into
 *		an ipc path
 *	`my-address' is this machine's address on this network.  This
 *		field is optional. 
 */
Config *
newconfig(cp)
	char *cp;
{
	Config *cnp = (Config *)malloc(sizeof(Config));
#	define FIELDS 3
	char *fields[FIELDS];
	int n;

	if(cnp==NULL) {
		logevent("out of memory allocating configuration\n");
		return NULL;
	}
	cnp->myaddr = cnp->pathse = NULL;
	cnp->addrre = NULL;
	setfields(" \t");
	n = getmfields(cp, fields, FIELDS);
	switch(n) {
	case 0:
		freemap(mp);
		return NULL;
	case 1:
		fields[1] = ".*";
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
	if((mp->serv=nregcomp(fields[0]))==NULL
	|| (mp->from=nregcomp(fields[1]))==NULL
	|| (mp->user=nregcomp(fields[2]))==NULL) {
		logevent("illegal reg exp in mapping `%s %s %s %s'\n", fields[0],
			fields[1], fields[2], fields[3]);
		freemap(mp);
		return NULL;
	}
	mp->luser = strdup(fields[3]);
	logevent("newmap(%s %s %s %s)\n", fields[0], fields[1], fields[2], fields[3]);
	return mp;
}

freeconfig(cp)
	Config *cp;
{
	if(mp==NULL)
		return;
	if(mp->from)
		free((char *)mp->from);
	if(mp->serv)
		free((char *)mp->serv);
	if(mp->user)
		free((char *)mp->user);
	if(mp->luser)
		free(mp->luser);
}

/*
 *  add a map entry
 */
addconfig(mp)
	Config *cp;
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
resetconfigs()
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
		if(!regexec(mp->serv, service, 0, 0))
			continue;
		if(!regexec(mp->from, source, 0, 0))
			continue;
		if(!regexec(mp->user, user, sub, 10))
			continue;
		regsub(mp->luser, luser, sub, 10);
		return luser;
	}
	return NULL;
}
