/* big-endian MIPS running MIPS's UNIX System V at Bell Labs */

#include <string.h>

char *cpp[] = { "/usr/local/lib/gcc-cpp", "-undef", "-DLANGUAGE_C", "-D_LANGUAGE_C",
	"-Dmips", "-Dhost_mips", "-DSYSTYPE_SYSV", "-D_SYSTYPE_SYSV", "-DMIPSEB", "-D_MIPSEB",
	"$1", "$2", "$3", 0 };
char *com[] = { "/usr/local/lib/rcc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include", "-I/usr/local/include/lcc", 0 };
char *as[] = { "/usr/bin/as", "-o", "$3", "$1", "-nocpp", "$2", 0 };
char *ld[] = { "/usr/bin/ld", "-o", "$3", "/lib/crt1.o", "$1", "$2", "", "",
	"-lc", "/lib/crtn.o", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0) {
		ld[3] = "/lib/mcrt1.o";
		ld[7] = "/usr/lib/cmplrs/cc/libprof1.a";
	} else if (strcmp(arg, "-b") == 0)
		ld[6] = "/usr/local/lib/bbexit.o";
	else
		return 0;
	return 1;
}
