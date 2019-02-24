/* Sun3s & Sun4s running Sun UNIX 4.0 at Princeton University */

#include <string.h>

char *cpp[] = {
#ifdef sparc
	"/usr/local/lib/gcc-cpp", "-undef",		/* GNU preprocessor */
/*	"/usr/local/lib/cpp", "-N", "-D__STDC__=1",	/* dmr's preprocessor */
	"-Dsparc",
#else
	"/usr/gnu/lib/gcc-cpp", "-undef",
	"-Dmc68000", "-Dmc68020",
#endif
	"-Dsun", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include/ansi", "-I/usr/include", 0 };
char *com[] = { "/usr/local/lib/rcc", "$1", "$2", "$3", 0 };
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
	"$2", "", "", "-lm", "", "-lc", 0 };

static char *libprefix = "/cmnusr/local/lib/ldb/";
extern char *concat();

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		ld[16] = "-lg";
	else if (strcmp(arg, "-p") == 0 && strcmp(ld[9], "/usr/lib/crt0.o") == 0) {
		ld[9]  = "/usr/lib/mcrt0.o";
		ld[17] = "-lc_p";
	} else if (strcmp(arg, "-pg") == 0 && strcmp(ld[9], "/usr/lib/crt0.o") == 0) {
		ld[9]  = "/usr/lib/gcrt0.o";
		ld[17] = "-lc_p";
	} else if (strcmp(arg, "-b") == 0 && access("/usr/local/lib/bbexit.o", 4) == 0)
		ld[13] = "/usr/local/lib/bbexit.o";
	else if (strncmp(arg, "-B", 2) == 0)
		libprefix = arg + 2;
#ifdef sparc
	else if (strcmp(arg, "-G") == 0 && strcmp(ld[9], "/usr/lib/crt0.o") == 0) {
	        com[0] = concat(libprefix, "sparc/rcc");
		ld[0]  = "/cmnusr/local/lib/ldb/ldb-ld";
   	        ld[9]  = concat(libprefix, "sparc/crt0.o");
		ld[14] = concat(libprefix, "sparc/Cnub.o");
	} else if (strncmp(arg, "ld=", 3) == 0)
	        ld[0]  = &arg[3];
#else
	else if (strcmp(arg, "-G") == 0 && strcmp(ld[9], "/usr/lib/crt0.o") == 0) {
	        com[0] = concat(libprefix, "sun3/rcc");
		ld[0]  = "/cmnusr/local/lib/ldb/ldb-ld";
   	        ld[9]  = concat(libprefix, "sun3/crt0.o");
		ld[14] = concat(libprefix, "sun3/Cnub.o");
	}
#endif
	else if (strcmp(arg, "-f68881") == 0)
		;
	else
		return 0;
	return 1;
}
