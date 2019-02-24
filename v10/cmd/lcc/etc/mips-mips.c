/* big-endian MIPS running MIPS's UNIX System V at Princeton U. */

#include <string.h>

char *cpp[] = { "/usr/gnu/lib/gcc-cpp", "-undef", "-nostdinc", "-DLANGUAGE_C",
	"-Dmips", "-Dhost_mips", "-DSYSTYPE_BSD43", "-DMIPSEB",
	"$1", "$2", "$3", 0 };
char *com[] = { "/usr/local/lib/rcc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include/ansi", "-I/bsd43/usr/include2.0",
	"-I/bsd43/usr/include", 0 };
char *as[] = { "/usr/bin/as", "-o", "$3", "$1", "-nocpp", "$2", 0 };
char *ld[] = { "/usr/bin/ld", "-systype", "/bsd43/", "-o", "$3",
	"/bsd43/usr/lib/cmplrs/cc/crt1.o", "$1", "$2", "",
	"/sysv/usr/lib/libc.a", "-lc", "/bsd43/usr/lib/cmplrs/cc/crtn.o", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0) {
		ld[5] = "/bsd43/usr/lib/cmplrs/cc/mcrt1.o";
		ld[8] = "/bsd43/usr/lib/cmplrs/cc/libprof1.a";
	} else
		return 0;
	return 1;
}
