/* mc68020 running Sun UNIX 3.5 at CIT, Princeton U. */

#include <string.h>

char *cpp[] = { "/usr/princeton/lib/gcc/gcc-cpp", "-undef", "-Dsun", "-Dmc68020",
	"$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/princeton/include/ansi", 0 };
char *com[] = { "/usr/princeton/lib/rcc", "$1", "$2", "$3", 0 };
char *as[] = { "/bin/as", "-o", "$3", "$1", "-mc68020", "$2", 0 };
char *ld[] = { "/bin/ld" , "-o", "$3", "-dc", "-dp", "-e", "start", "-X", "/usr/lib/crt0.o",
	"/usr/lib/Mcrt1.o", "-L/usr/lib/f68881", "$1", "$2", "-lm", "", "-lc", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		ld[14] = "-lg";
	else if (strcmp(arg, "-p") == 0) {
		ld[8] = "/usr/lib/mcrt0.o";
		ld[15] = "-lc_p";
	} else if (strcmp(arg, "-pg") == 0) {
		ld[8] = "/usr/lib/gcrt0.o";
		ld[15] = "-lc_p";
	} else if (strcmp(arg, "-f68881") == 0)
		;
	else
		return 0;
	return 1;
}
