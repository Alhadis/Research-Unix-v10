/*
 *	store mbox owner through pointer.  return 0 (success), -1 (failure)
 *	hard version for systems that let you give files away;
 *	the owner of a mbox is determined from its name -- unless
 *	the set-uid bit is on (!) .  In that case, the file must have
 *	been created by its present owner, else anyone could become root.
 *	If the set-uid bit is on, return the file owner and group.
 */

#include <pwd.h>
#include <types.h>
#include <stat.h>
#include "string.h"
#include "aux.h"
#include "ugid.h"
#include "mail.h"

mboxowner (mbox, ugidp)
	char *mbox;
	struct ugid *ugidp;
{
	char *u;
	struct passwd *pw, *getpwnam();
	struct stat statb;

	if (stat (mbox, &statb) >= 0 && (statb.st_mode & S_ISUID)) {
		ugidp->uid = statb.st_uid;
		ugidp->gid = statb.st_gid;
		return 0;
	}

	u = basename(mbox);
	if (u == NULL)
		return -1;

	pw = getpwnam(u);
	if (pw == NULL)
		return -1;

	ugidp->uid = pw->pw_uid;
	ugidp->gid = pw->pw_gid;
	return 0;
}
