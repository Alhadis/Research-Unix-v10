/* SGI big-endian MIPSes running IRIX at University of Utah */

#include <string.h>

char *cpp[] = { "/u/drh/bin/mipseb/gcc-cpp", "-undef", "-DLANGUAGE_C",
	"-Dmips", "-Dhost_mips", "-Dsgi", "-DSYSTYPE_SYSV", "-DMIPSEB",
	"$1", "$2", "$3", 0 };
char *com[] = { "/u/drh/bin/mipseb/rcc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/u/drh/include/mips_iris", 0 };
char *as[] = { "/usr/bin/as", "-o", "$3", "$1", "-nocpp", "$2", 0 };
char *ld[] = { "/usr/bin/ld", "-o", "$3", "/usr/lib/crt1.o", "$1", "$2", "",
	"-lc", "/usr/lib/crtn.o", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0) {
		ld[3] = "/usr/lib/mcrt1.o";
		ld[6] = "-lprof1";
	} else
		return 0;
	return 1;
}
