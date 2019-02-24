/* HP 300 running UNIX 4.3 BSD at Univeristy of Utah */

#include <string.h>

char *cpp[] = { "/usr/local/lib/gcc-cpp", "-undef", "-Dhp300", "-Dmc68000", "-Dmc68020", "$1", "$2", "$3", 0 };
char *include[] = { "-I/n/austin/u/drh/include/mc_sun", 0 };
char *com[] = { "/n/austin/u/drh/bin/mc68020/rcc", "$1", "$2", "$3", 0 };
char *as[] = { "/bin/as", "-o", "$3", "$1", "-mc68020", "$2", 0 };
char *ld[] = { "/usr/local/lib/gcc-ld" , "-o", "$3", "/lib/crt0.o",
	"$1", "$2", "-lm", "", "-lc", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0) {
		ld[7] = "-lc_p";
		ld[3] = "/usr/lib/gcrt0.o";
	} else if (strcmp(arg, "-p") == 0) {
		ld[3] = "/usr/lib/mcrt0.o";
		ld[7] = "-lc_p";
	} else if (strcmp(arg, "-pg") == 0) {
		ld[8] = "/usr/lib/gcrt0.o";
		ld[15] = "-lc_p";
	} else if (strcmp(arg, "-f68881") == 0)
		;
	else
		return 0;
	return 1;
}
