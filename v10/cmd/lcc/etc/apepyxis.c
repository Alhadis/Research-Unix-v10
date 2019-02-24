/* big-endian MIPS running SGI's UNIX System V at Bell Labs */

#include <string.h>
#include "conf.h"

char *cpp[] = { "/v/lib/gcc-cpp", "-undef", "-DLANGUAGE_C", "-Dsgi", "-DSVR3",
	"-Dmips", "-Dhost_mips", "-DSYSTYPE_SYSV", "-DMIPSEB", "$1", "$2", "$3", 0 };
char *com[] =  { "/v/lib/rcc", "$1", "$2", "$3", 0 };
char *include[] = { APEINC, 0 };
char *as[] = { "/usr/bin/as", "-o", "$3", "$1", "-nocpp", "$2", 0 };
char *ld[] = { "/usr/bin/ld", "-o", "$3", "/usr/lib/crt1.o", "$1", "$2", "", "",
	APELIBS, "/usr/lib/crtn.o", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0) {
		ld[3] = "/usr/lib/mcrt1.o";
		ld[7] = "-lprof1";
	} else if (strcmp(arg, "-b") == 0)
		ld[6] = "/v/lib/bbexit.o";
	else
		return 0;
	return 1;
}
