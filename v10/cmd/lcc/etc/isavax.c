/* VAXes running UNIX 4.3BSD or ULTRIX at Princeton University */

#include <string.h>

char *cpp[] = { "/usr/gnu/lib/gcc-cpp", "-undef", "-Dvax",
	"$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include/ansi", 0 };
char *com[] = { "/usr/local/lib/rcc", "$1", "$2", "$3", 0 };
char *as[] = { "/bin/as", "-J", "-o", "$3", "$1", "$2", 0 };
char *ld[] = { "/bin/ld", "-o", "$3", "/lib/crt0.o", "-X",
	"$1", "$2", "", "", "-lm", "", "-lc", 0 };

static char *libprefix = "/cmnusr/local/lib/ldb/";
extern char *concat();

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		ld[10]  = "-lg";
	else if (strcmp(arg, "-p") == 0 && strcmp(ld[3], "/lib/crt0.o") == 0) {
		ld[3]  = "/lib/mcrt0.o";
		ld[11] = "-lc_p";
	} else if (strcmp(arg, "-pg") == 0 && strcmp(ld[3], "/lib/crt0.o") == 0) {
		ld[3]  = "/usr/lib/gcrt0.o";
		ld[11] = "-lc_p";
	} else if (strcmp(arg, "-b") == 0
	&& access("/usr/local/lib/bbexit.o", 4) == 0)
		ld[7]  = "/usr/local/lib/bbexit.o";
	else if (strncmp(arg, "-B", 2) == 0)
		libprefix = arg + 2;
	else if (strcmp(arg, "-G") == 0 && strcmp(ld[3], "/lib/crt0.o") == 0) {
	        com[0] = concat(libprefix, "vax/rcc");
		ld[0]  = "/cmnusr/local/lib/ldb/ldb-ld";
   	        ld[3]  = concat(libprefix, "vax/crt0.o");
		ld[8]  = concat(libprefix, "vax/Cnub.o");
	} else
		return 0;
	return 1;
}
