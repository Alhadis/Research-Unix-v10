/* DEC little-endian MIPSes running ULTRIX at CIT, Princeton University */

#include <string.h>

char *cpp[] = { "/usr/princeton/lib/gnu/gcc-cpp", "-undef",
	"-DLANGUAGE_C", "-D_LANGUAGE_C", "-D__LANGUAGE_C",
	"-Dmips", "-D__mips", "-Dhost_mips", "-D__host_mips", "-Dunix", "-D__unix",
	"-DMIPSEL", "-D_MIPSEL", "-Dultrix", "-D__ultrix",
	"$1", "$2", "$3", 0 };
char *com[] =  { "/usr/princeton/lib/rcc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/princeton/include/ansi", "-I/usr/include", 0 };
char *as[] =  { "/bin/as", "-o", "$3", "", "$1", "-nocpp", "-EL", "$2", 0 };
char *ld[] =  { "/usr/bin/ld", "-o", "$3", "/usr/lib/crt0.o", "$1", "$2", "", "", "-lm", "-lc", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		as[3] = "-g";
	else if (strcmp(arg, "-p") == 0) {
		ld[3] = "/usr/lib/mcrt0.o";
		ld[7] = "/usr/lib/libprof1.a";
	} else if (strcmp(arg, "-b") == 0 && access("/usr/princeton/lib/bbexit.o", 4) == 0)
		ld[6] = "/usr/princeton//lib/bbexit.o";
	else
		return 0;
	return 1;
}
