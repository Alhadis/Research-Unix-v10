/*
 * pick one of three versions of the `portable' directory-reading
 * routines: research, bsd, system V.
 * there really should be at most two.
 *
 * <sys/types.h> has already been included.
 * the result should provide struct dirent.
 */

#define	DIR_RES	1
#define	DIR_BSD	0
#define	DIR_SV	0

#if DIR_RES
#include <ndir.h>
#define	dirent	direct
#endif

#if DIR_BSD
#include <sys/dir.h>
#define	dirent	direct
#endif

#if DIR_SV
#include <dirent.h>
/* already struct dirent */
#endif
