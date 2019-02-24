/*
 * System dependent header files for research
 */

#include <sys/types.h>
#include <ndir.h>
#include <dir.h>
#include <errno.h>
#include "syslog.h"

#define MAXPATHLEN DIRSIZ
typedef struct direct Direct;

#define SIGRETURN int

#define DEFAULT_LOG_LEVEL	LOG_INFO

/*
 *
 * Syslogs used to keep track of users/intruders.  These are the assignments
 * of the LOCAL logs.
 *
 */

#define	LOG_INETD	LOG_LOCAL0
#define LOG_FTPD	LOG_LOCAL1
#define LOG_TELNETD	LOG_LOCAL2
#define LOG_SMTPD	LOG_LOCAL3
#define LOG_PROXY	LOG_LOCAL4
#define LOG_SMTP	LOG_LOCAL5
#define LOG_SMTPSCHED	LOG_LOCAL6
