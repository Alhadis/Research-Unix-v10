/*
 *	Codes for the third argument to the user-supplied function
 *	which is passed as the second argument to ftw
 */

#include "config.h"

#define	FTW_F	0	/* file */
#define	FTW_D	1	/* directory */
#define	FTW_DNR	2	/* directory without read permission */
#define	FTW_NS	3	/* unknown type, stat failed */
#define	FTW_DP	4	/* directory, postorder visit */
#define	FTW_SL	5	/* Symbolic link */

#include DIR_H
