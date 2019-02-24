#ifndef __FTWLK
#define __FTWLK

/*
 *	Codes for the third argument to the user-supplied function
 *	which is passed as the second argument to ftwlk...
 */

#define	FTWLK_F	0	/* file */
#define	FTWLK_D	1	/* directory */
#define	FTWLK_DNR	2	/* directory without read permission */
#define	FTWLK_NS	3	/* unknown type, stat failed */
#define FTWLK_DP	4	/* directory, postorder visit */
#define FTWLK_SL	5	/* symbolic link */
#define FTWLK_NSL 6	/* stat failed (errno = ENOENT) on symbolic link */

/*	Values the user-supplied function may wish to assign to
	component quit of struct FTWLK...
 */

#define FTWLK_SKD 1	/* skip this directory (2nd par = FTWLK_D) */
#define FTWLK_SKR 2	/* skip rest of current directory */
#define FTWLK_FOLLOW 3	/* follow symbolic link */

struct FTWLK { int quit, base, level;
#ifndef FTWLK_more_to_come
	};
#endif

#endif
