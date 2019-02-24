/* big-endian MIPS running MIPS's UNIX System V at Princeton U. */

#include <string.h>

char *cpp[] = { 
/*	"/usr/gnu/lib/gcc-cpp", "-undef",		/* GNU preprocessor */
	"/usr/local/lib/cpp", "-N", "-D__STDC__=1",	/* dmr's preprocessor */
	"-DLANGUAGE_C", "-D_LANGUAGE_C", "-Dmips", "-Dhost_mips", "-DSYSTYPE_BSD43",
	"-D_SYSTYPE_BSD43", "-DMIPSEB", "-D_MIPSEB", "$1", "$2", "$3", 0 };
char *com[] = { "/usr/local/lib/rcc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include/ansi", "-I/bsd43/usr/include2.0",
	"-I/bsd43/usr/include", "-I/usr/include", 0 };
char *as[] = { "/usr/bin/as", "-o", "$3", "$1", "-nocpp", "$2", 0 };
char *ld[] = { "/usr/bin/ld", "-systype", "/bsd43/", "-o", "$3",
	"/bsd43/usr/lib/cmplrs/cc/crt1.o", "$1", "$2", "", "",
	"/sysv/usr/lib/libc.a", "-lc", "/bsd43/usr/lib/cmplrs/cc/crtn.o", 0 };

static char *libprefix = "/cmnusr/local/lib/ldb/";
extern char *concat();

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0 
             && strcmp(ld[5], "/bsd43/usr/lib/cmplrs/cc/crt1.o") == 0) {
		ld[5] = "/bsd43/usr/lib/cmplrs/cc/mcrt1.o";
		ld[9] = "/bsd43/usr/lib/cmplrs/cc/libprof1.a";
	} else if (strcmp(arg, "-b") == 0 && access("/usr/local/lib/bbexit.o", 4) == 0)
		ld[8] = "/usr/local/lib/bbexit.o";
	else if (strcmp(arg, "-G") == 0 
             && strcmp(ld[5], "/bsd43/usr/lib/cmplrs/cc/crt1.o") == 0) {
	        com[0] = concat(libprefix, "mipseb/rcc");
		ld[0]  = "/n/fs/cmnusr/local/lib/ldb/ldb-ld";
   	        ld[5]  = concat(libprefix, "mipseb/crt1.o");
		ld[9]  = concat(libprefix, "mipseb/Cnub.o");
	} else
		return 0;
	return 1;
}
