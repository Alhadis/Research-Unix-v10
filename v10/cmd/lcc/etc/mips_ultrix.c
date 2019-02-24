#include <string.h>

char *cpp[] = { "/v/lib/gcc-cpp", "-undef", "-DLANGUAGE_C", "-D_LANGUAGE_C", "-Dsgi", "-DSVR3",
	"-Dmips", "-Dhost_mips", "-DSYSTYPE_SYSV", "-D_SYSTYPE_SYSV", "-DMIPSEB", "-D_MIPSEB",
	"$1", "$2", "$3", 0 };
char *com[] =  { "/usr/cwf/book/mill/mill1/mips/rcc", "$2", "$1", 0 };
char *include[] = { "-I/v/include/lcc", 0 };
char *as[] =  { "/bin/echo", "-n", 0 };
char *ld[] =  { "/usr/cwf/book/mill/millcom", "$3", "$2",  0 };

int option(arg) char *arg; {
	return 1;
}
