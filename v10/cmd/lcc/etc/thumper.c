/* SGI big-endian MIPSes running IRIX at CIT, Princeton University */

#include <string.h>

char *cpp[] = { "/u/drh/lib/mipseb/gcc-cpp", "-undef",
	"-DLANGUAGE_C", "-D_LANGUAGE_C", "-D__LANGUAGE_C",
	"-Dmips", "-D__mips", "-Dhost_mips", "-D__host_mips", "-Dunix", "-D__unix",
	"-DMIPSEB", "-D_MIPSEB", "-Dsgi", "-DSYSTYPE_SYSV", "-D_SYSTYPE_SYSV", "-DSYSTEM_FIVE", "-D__SYSTEM_FIVE",
	"$1", "$2", "$3", 0 };
char *com[] = { "/u/drh/lib/mipseb/rcc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/u/drh/include/mips_iris", 0 };
char *as[] = { "/usr/bin/as", "-o", "$3", "$1", "-nocpp", "$2", 0 };
char *ld[] = { "/usr/bin/ld", "-o", "$3", "/usr/lib/crt1.o", "$1", "$2", "", "",
	"-lc", "/usr/lib/crtn.o", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0) {
		ld[3] = "/usr/lib/mcrt1.o";
		ld[7] = "-lprof1";
	} else if (strcmp(arg, "-b") == 0 && access("/u/drh/lib/mipseb/bbexit.o", 4) == 0)
		ld[6] = "/u/drh/lib/mipseb/bbexit.o";
	else
		return 0;
	return 1;
}
