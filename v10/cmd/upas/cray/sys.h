/*
 * System dependent header files for MIPS CRAY/UNICOS using
 * BSD compilation options.
 */

#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define	MAILGROUP	6
#define	MAILMODE	0660
#define MAXPATHLEN	MAXNAMLEN
typedef struct dirent Direct;

#define SIGRETURN void
typedef void (*SIG_TYP)();

#include </usr/include/sys/syslog.h>
#define LOG_SMTP	LOG_LOCAL5
#define LOG_SMTPD	LOG_LOCAL3
#define LOG_SMTPSCHED	LOG_LOCAL6

#define SETLOGMASK	setlogmask

#define	DEFAULT_LOG_LEVEL	LOG_DEBUG

#define DIALER	"mx"		/* for the real Internet */
