/*
 * rewinddir
 *
 * rewind directory stream
 * provided for POSIX compatibility
 */

#include "dirlib.h"

#undef	rewinddir

void
rewinddir(dirp)
register DIR	*dirp;
{
	seekdir(dirp, 0L);
}
