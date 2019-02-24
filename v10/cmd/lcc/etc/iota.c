/* mc68020s running Sun UNIX 4.0 at Bell Labs */

#include <string.h>

char *cpp[] = { "/usr/local/lib/gcc-cpp", "-undef", "-Dsun", "-Dmc68000", "-Dmc68020", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include", "-I/usr/local/include/lcc", 0 };
char *com[] = { "/usr/local/lib/rcc", "$1", "$2", "$3", 0 };
char *as[] = { "/bin/as", "-o", "$3", "$1", "-mc68020", "$2", 0 };
char *ld[] = { "/bin/ld" , "-o", "$3", "-dc", "-dp", "-e", "start", "-X",
		"/usr/lib/crt0.o", "/usr/lib/Mcrt1.o", "-L/usr/lib/f68881",
		"$1", "$2", "", "-lm", "", "-lc", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		ld[15] = "-lg";
	else if (strcmp(arg, "-p") == 0) {
		ld[8] = "/usr/lib/mcrt0.o";
		ld[16] = "-lc_p";
	} else if (strcmp(arg, "-pg") == 0) {
		ld[8] = "/usr/lib/gcrt0.o";
		ld[16] = "-lc_p";
	} else if (strcmp(arg, "-b") == 0)
		ld[13] = "/usr/local/lib/bbexit.o";
	else if (strcmp(arg, "-f68881") == 0)
		;
	else
		return 0;
	return 1;
}
