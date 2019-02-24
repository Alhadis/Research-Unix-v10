#include <sys/types.h>
#include <sys/stat.h>
#include <sys/inet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <ipc.h>
#include "config.h"

extern char *in_ntoa();
extern char *strcpy();

char *
in_host(addr)
	in_addr addr;
{
	static char host[128];
	char *name;

	strcpy(host, in_ntoa(addr));

	/* look up the system */
	name = qvalue("dom|name", host, (char *)NULL);
	if(name==NULL) {
		/* no such system - return the ip number */
		return host;
	} else {
		return name;
	}
}
