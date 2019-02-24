/*
 * Open Systems file; if MANYSYS pipe from uugrep
 * currently referenced in conn.c uuname.c versys.c
 * the names are also known by uucheck.c
 */

#include "uucp.h"
VERSION(%W%);

#ifdef MANYSYS

FILE *
sysopen(sys)
char *sys;
{
	FILE *pf;
	extern FILE *vepopen();
	static char *args[] = { "uugrep", NULL, NULL };

	args[1] = sys;
	pf = vepopen("/usr/lib/uucp/uugrep", "r", args, (char **)NULL);
	return(pf);
}

#endif
