#include "defs.h"
#include <ctype.h>
#include <sys/inet/in.h>

/*
 *  convert a port number into a file system name
 */
char *
tcptofs(port)
	int port;
{
	static char name[PATHLEN];
	struct in_service *s;

	s = in_service((char *)0, "tcp", (unsigned long)port);
	if(s)
		sprintf(name, "/cs/%s", s->name);
	else
		sprintf(name, "/cs/tcp.%d", port);
	return name;
}

/*
 *  convert an internal name into a port number
 */
fstotcp(name)
	char *name;
{
	int port;
	struct in_service *s;

	/*
	 *  the format could be
	 *	[/cs/][tcp.]nnn
	 *  where the []'s indicate optional peices and
	 *  nnn is the port number
	 */
	if (strncmp(name, CSROOT, sizeof(CSROOT)-1)==0)
		name += sizeof(CSROOT)-1;
	if (strncmp(name, "tcp.", sizeof("tcp.")-1)==0)
		name += sizeof("tcp.")-1;
	for (port=0; *name; name++) {
		if (!isdigit(*name)){
			port=0;
			break;
		}
		port = port*10 + (*name - '0');
	}
	/*
	 *  or this may be a name in /etc/inservices
	 */
	if(port == 0) {
		s = in_service(name, "tcp", (unsigned long)0);
		if(s)
			port = s->port;
	}
	return port;
}
