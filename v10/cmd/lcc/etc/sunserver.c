/* Sun-3s & Sun-4s running Sun UNIX 4.0 at CIT, Princeton University */

#include <string.h>

char *cpp[] = { "/usr/princeton/lib/gnu/gcc-cpp", "-undef", "-Dsun",
#ifdef sparc
	"-Dsparc",
#else
	"-Dmc68000", "-Dmc68020",
#endif
	"$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/princeton/include/ansi", "-I/usr/include", 0 };
char *com[] = { "/usr/princeton/lib/rcc", "$1", "$2", "$3", 0 };
char *as[] = { "/bin/as", "-o", "$3",
#ifndef sparc
	"-mc68020",
#endif
	"$1", "$2", 0 };
char *ld[] = { "/bin/ld", "-o", "$3", "-dc", "-dp", "-e", "start", "-X",
	"$1", "/usr/lib/crt0.o",
#ifdef sparc
	"", "",
#else
	"/usr/lib/Mcrt1.o", "-L/usr/lib/f68881",
#endif
	"$2", "", "-lm", "", "-lc", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		ld[15] = "-lg";
	else if (strcmp(arg, "-p") == 0) {
		ld[9]  = "/usr/lib/mcrt0.o";
		ld[16] = "-lc_p";
	} else if (strcmp(arg, "-pg") == 0) {
		ld[9]  = "/usr/lib/gcrt0.o";
		ld[16] = "-lc_p";
	} else if (strcmp(arg, "-b") == 0 && access("/usr/princeton/lib/bbexit.o", 4) == 0)
		ld[13] = "/usr/princeton/lib/bbexit.o";
	else if (strcmp(arg, "-f68881") == 0)
		;
	else
		return 0;
	return 1;
}
