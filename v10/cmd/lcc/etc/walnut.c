/* DEC little-endian MIPSes running ULTRIX at DECSRC */

#include <string.h>

char *cpp[] = {
/*	"/usr/lib/cmplrs/cc/cpp", "-undef",		/* GNU preprocessor */
	"/udir/hanson/bin/mipsel/cpp", "-N", "-D__STDC__=1",	/* dmr's preprocessor */
	"-DLANGUAGE_C", "-D_LANGUAGE_C", "-D__LANGUAGE_C",
	"-D_unix", "-D__unix", "-Dultrix", "-D_ultrix", "-D__ultrix",
	"-Dmips", "-D_mips", "-D__mips",
	"-Dhost_mips", "-D_host_mips", "-D__host_mips",
	"-DMIPSEL", "-D_MIPSEL", "-D__MIPSEL",
	"$1", "$2", "$3", 0 };
char *com[] =  { "/udir/hanson/bin/mipsel/rcc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/udir/hanson/include/ansi", "-I/usr/include", 0 };
char *as[] =  { "/bin/as", "-o", "$3", "", "$1", "-nocpp", "-EL", "$2", 0 };
char *ld[] =  { "/usr/bin/ld", "-o", "$3", "/usr/lib/crt0.o", "$1", "$2", "", "", "-lm", "-lc", 0 };

static char *libprefix = "/cmnusr/local/lib/ldb/";
extern char *concat();

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		as[3] = "-g";
	else if (strcmp(arg, "-p") == 0 && strcmp(ld[3], "/usr/lib/crt0.o") == 0) {
		ld[3] = "/usr/lib/mcrt0.o";
		ld[7] = "/usr/lib/libprof1.a";
	} else if (strcmp(arg, "-b") == 0 && access("/udir/hanson/bin/mipsel/bbexit.o", 4) == 0)
		ld[6] = "/udir/hanson/bin/mipsel/bbexit.o";
	else if (strncmp(arg, "-B", 2) == 0)
		libprefix = arg + 2;
	else if (strcmp(arg, "-G") == 0 && strcmp(ld[3], "/usr/lib/crt0.o") == 0) {
	        com[0] = concat(libprefix, "mipsel/rcc");
		ld[0]  = "/cmnusr/local/lib/ldb/ldb-ld";
   	        ld[3]  = concat(libprefix, "mipsel/crt0.o");
		ld[7]  = concat(libprefix, "mipsel/Cnub.o");
	} else
		return 0;
	return 1;
}
