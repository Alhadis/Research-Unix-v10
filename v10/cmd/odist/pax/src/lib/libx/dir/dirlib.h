/*
 * AT&T Bell Laboratories
 *
 * directory stream access library private definitions
 * handles 3d and preroot file systems
 */

#include <preroot.h>

#undef	DIR
#define DIR		_SYS_DIR_

#undef	opendir
#define opendir		_SYS_opendir

#undef	readdir
#define readdir		_SYS_readdir

#undef	rewinddir
#define rewinddir	_SYS_rewinddir

#undef	seekdir
#define seekdir		_SYS_seekdir

#undef	telldir
#define telldir		_SYS_telldir

#undef	closedir
#define closedir	_SYS_closedir

#define _dir_local	1

#include <lclparam.h>
#include <fsinfo.h>
#include <dirent.h>
#include <limits.h>
#include <hash.h>

#undef	DIR

typedef struct
{
	int		dd_fd;		/* file descriptor		*/
	int		dd_loc;		/* offset in block		*/
	int		dd_size;	/* valid data in block		*/
	char*		dd_buf;		/* directory block		*/

	/* the rest are for 3d and preroot fs */

	unsigned char	covered[sizeof(int)*CHAR_BIT];
					/* covered directory fds	*/
	ino_t		opaque;		/* opaque inode number		*/
	HASHTABLE*	overlay;	/* directory overlay hash	*/
	short		numfd;		/* number of covered fds	*/
	short		nxtfd;		/* next covered fd index	*/
	short		fs3d;		/* 3d in effect			*/
} DIR;					/* directory stream descriptor	*/

#undef		opendir
extern DIR*	opendir();

#undef		readdir
extern struct dirent*	readdir();

#undef	rewinddir
extern void	rewinddir();

#undef	seekdir
extern void	seekdir();

#undef	telldir
extern long	telldir();

#undef		closedir
extern void	closedir();

#ifndef	DIRBLKSIZ
#ifdef	DIRBLK
#define DIRBLKSIZ	DIRBLK
#else
#ifdef	DIRBUF
#define DIRBLKSIZ	DIRBUF
#else
#ifdef	DEV_BSIZE
#define DIRBLKSIZ	DEV_BSIZE
#else
#define DIRBLKSIZ	4096
#endif
#endif
#endif
#endif
