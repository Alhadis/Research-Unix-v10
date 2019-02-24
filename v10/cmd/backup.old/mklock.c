#include	"backup.h"
#include	<errno.h>
#include	<libc.h>
#include	<pwd.h>
#include	<grp.h>

main()
{
	char buf[256], dir[256];
	struct passwd *p;
	struct group *g;

	backupdir(dir);
	sprint(buf, "%s/%s", dir, LOCK);
	if(access(buf, 0) < 0){
		if(creat(buf, 01666) < 0){
			logprint("mklock(%s) failed errno=%d\n", buf, errno);
			exit(1);
		}
		if((p = getpwnam("daemon")) && (g = getgrnam("sys")))
			chown(buf, p->pw_uid, g->gr_gid);
	}
	exit(0);
}
