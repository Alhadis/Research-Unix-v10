/*
 * System dependent header files for MIPS RISC/OS using
 * BSD compilation options.
 */

#include <sys/types.h>
#include <dirent.h>
#define MAXPATHLEN	MAXNAMLEN
typedef struct dirent Direct;

#define SIGRETURN void
typedef void (*SIG_TYP)();

#include </usr/include/bsd/syslog.h>
#include "/v/include/logs.h"

#define SETLOGMASK	setlogmask

#define	DEFAULT_LOG_LEVEL	LOG_DEBUG

#define DIALER	"mx"		/* for the real Internet */
