/* DEC little-endian MIPSes running ULTRIX at Princeton University */

#include <string.h>

char *cpp[] = { "/usr/gnu/lib/gcc-cpp", "-undef", "-DLANGUAGE_C", "-Dmips",
	"-DMIPSEL", "$1", "$2", "$3", 0 };
char *com[] =  { "/usr/local/lib/rcc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include/ansi", 0 };
char *as[] =  { "/bin/as", "-o", "$3", "", "$1", "-nocpp", "-EL", "$2", 0 };
char *ld[] =  { "/usr/bin/ld", "-o", "$3", "/usr/lib/crt0.o", "$1", "$2", "", "-lm", "-lc", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		as[3] = "-g";
	else if (strcmp(arg, "-p") == 0) {
		ld[3] = "/usr/lib/mcrt0.o";
		ld[6] = "/usr/lib/libprof1.a";
	} else
		return 0;
	return 1;
}
