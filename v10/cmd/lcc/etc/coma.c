/* vax running UNIX v9 at Bell Labs */

#include <string.h>

char *cpp[] = { "/usr/lib/gcc-cpp", "-undef", "-Dvax", "-DV9", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/include/libc", "-I/usr/include/lcc", 0 };
char *com[] = { "/usr/lib/rcc", "$1", "$2", "$3", 0 };
char *as[] =  { "/bin/as", "-o", "$3", "$1", "$2", 0 };
char *ld[] =  { "/bin/ld", "-o", "$3", "/lib/crt0.o", "-X", "$1", "$2", "", "-lc", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0) {
		ld[3] = "/lib/mcrt0.o";
	} else if (strcmp(arg, "-b") == 0)
		ld[7] = "/usr/lib/bbexit.o";
	else
		return 0;
	return 1;
}
