#include "ps.h"
#include <pwd.h>

static List namelist;
static Entry *prev;

char *
getuname(uid)
register uid;
{
	register Entry *ep;

	if((ep = prev) && ep->e_val == uid)
		return ep->e_name;
	for(ep = namelist.begin; ep; ep = ep->next) {
		if(ep->e_val == uid) {
			prev = ep;
			return ep->e_name;
		}
	}
	return "?";
}

getlogins()
{
	register Entry *ep;
	register Entry *last;
	register struct passwd *pp;

	last = NULL;
	while(pp = getpwent()) {
		if((ep = Malloc(Entry, 1)) == 0)
			break;
		if (last)
			last->next = ep;
		else
			namelist.begin = ep;
		ep->next = NULL;
		last = ep;
		namelist.nentry++;
		ep->e_val = pp->pw_uid;
		strncpy(ep->e_name, pp->pw_name, sizeof(ep->e_name));
	}
	endpwent();
}
