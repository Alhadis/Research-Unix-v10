#include <stdio.h>
#include "addrformat.h"
int debug;
main()
{
	char buf[256];
	char *cp;

	printf(">");
	fflush(stdout);
	while(fgets(buf, sizeof buf, stdin)) {
		if(cp = (char *)strchr(buf, '\n'))
			*cp = 0;
		printf("%s\n>", convertaddr(buf, ".att.com", SOURCEROUTE));
		fflush(stdout);
	}
}
