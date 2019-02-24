/*
 * G. S. Fowler
 * AT&T Bell Laboratories
 *
 * universe support
 *
 * symbolic link external representation has trailing '\0' and $(...) style
 * conditionals where $(...) corresponds to a kernel object (i.e., probably
 * not environ)
 *
 * universe symlink conditionals use $(UNIVERSE)
 */

#include "FEATURE/universe"

#ifdef _bin_universe

#include <limits.h>
#include <ls.h>

#ifdef _sys_universe
#include <sys/universe.h>
#endif

#ifndef PATH_MAX
#define PATH_MAX	1024
#endif

#ifdef NUMUNIV
#define UNIV_MAX	NUMUNIV
#else
#define UNIV_MAX	univ_max
extern char*		univ_name[];
extern int		univ_max;
#endif

extern char		univ_cond[];
extern int		univ_size;

#else

#define UNIV_SIZE	9

extern char		univ_env[];

#endif

#include <errno.h>

extern int		errno;
