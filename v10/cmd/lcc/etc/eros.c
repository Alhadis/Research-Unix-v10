/* SGI big-endian MIPSes (Indigos) running IRIX 4.0.1 at U Utah */
/* from Nelson H. F. Beebe <beebe@eros.math.utah.edu> */

#include <string.h>

char *cpp[] = {
	"/usr/local/lib/gcc-cpp",
/* These options match the output of "cc -v -E foo.c" on IRIX 4.0.1 */
	"-undef",
	"-D__EXTENSIONS__",
/*	"-trigraphs",	/* uncomment to get trigraphs */
	"-undef",
	"-p",
	"-DLANGUAGE_C",
	"-D_LANGUAGE_C",
	"-Dsgi",
	"-DSVR3",
	"-D__sgi",
	"-D__SVR3",
	"-Dunix",
	"-Dmips",
	"-Dhost_mips",
	"-D__unix",
	"-D__mips",
	"-D__host_mips",
	"-DSYSTYPE_SYSV",
	"-D_SYSTYPE_SYSV",
	"-D_MIPSEB",
	"-DMIPSEB",
	"$1",
	"$2",
	"$3",
	0
};

char *com[] =  { "/usr/local/lib/rcc", "$1", "$2", "$3", 0 };
char *include[] = { "-I/usr/local/include/ansi", "-I/usr/include", 0 };
char *as[] = { "/usr/bin/as", "-o", "$3", "$1", "-nocpp", "$2", 0 };
char *ld[] = { "/usr/bin/ld", "-o", "$3", "/usr/lib/crt1.o", "$1", "$2", "",
	"-lc", "/usr/lib/crtn.o", 0 };

int option(arg) char *arg; {
	if (strcmp(arg, "-g") == 0)
		;
	else if (strcmp(arg, "-p") == 0) {
		ld[3] = "/usr/lib/mcrt1.o";
		ld[6] = "-lprof1";
	} else
		return 0;
	return 1;
}
