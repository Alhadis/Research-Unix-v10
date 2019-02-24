#include <sys/inet/in.h>
#include <stdio.h>
#include <ctype.h>
#include <ipc.h>
#include "config.h"

extern in_addr in_aton();

/* translate a host name into its first address */
in_addr
in_address(host)
char *host;
{
	char *inet;
	in_addr addr;

	/* is it an address */
	addr = in_aton(host);
	if(addr!=INADDR_ANY)
		return addr;

	/* look up the system */
	inet = qvalue("in", host, (char *)NULL);
	if(inet==NULL)
		return INADDR_ANY;
	else
		return in_aton(inet);
}
