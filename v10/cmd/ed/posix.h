#include <sys/types.h>
#ifndef __LIMITS
#include <limits.h>
#endif /* __LIMITS */
#ifndef __ERRNO
#include <errno.h>
#endif /* __ERRNO */
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#include <grp.h>
#include <pwd.h>
#include <utmp.h>

#include <sys/stat.h>
#include <sys/times.h>
#include <sys/wait.h>

#ifdef __TO_DO_SOMETIME__
#include <termios.h>
#include <utime.h>
#include <sys/utsname.h>
#endif /* __TO_DO_SOMETIME__ */
