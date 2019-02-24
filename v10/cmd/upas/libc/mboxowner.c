/*
 *	store mbox owner through pointer.  return 0 (success), -1 (failure)
 *	easy version for systems that don't let you give files away
 */

#include <types.h>
#include <stat.h>
#include "ugid.h"

mboxowner (mbox, ugidp)
	char *mbox;
	struct ugid *ugidp;
{
	struct stat sb;

	if (stat(mbox,&sb) < 0)
		return -1;

	ugidp->uid = sb.st_uid;
	ugidp->gid = sb.st_gid;
	return 0;
}
