/* Sun-4s running Sun UNIX 4.0 at Bell Labs */

#include <string.h>

char *cpp[] = { "/usr/add-on/GNU/lib/gcc-cpp", "-undef", "-Dsun", "-Dsparc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include/ast", "-I/usr/local/include/lcc", "-I/usr/include", 0 };
char *com[] = { "/usr/local/lib/rcc", "$1", "$2", "$3", 0 };
char *as[] = { "/bin/as", "-o", "$3", "$1", "$2", 0 };
char *ld[] = { "/bin/ld" , "-o", "$3", "-dc", "-dp", "-e", "start", "-X", "/usr/lib/crt0.o",
	"$1", "$2", "", "-lm", "", "-lc", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		ld[13] = "-lg";
	else if (strcmp(arg, "-p") == 0) {
		ld[8]  = "/usr/lib/mcrt0.o";
		ld[14] = "-lc_p";
	} else if (strcmp(arg, "-pg") == 0) {
		ld[8]  = "/usr/lib/gcrt0.o";
		ld[14] = "-lc_p";
	} else if (strcmp(arg, "-b") == 0)
		ld[11] = "/usr/local/lib/bbexit.o";
	else
		return 0;
	return 1;
}
