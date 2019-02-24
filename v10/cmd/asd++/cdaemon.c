#include "decl.h"

extern "C" {
	extern int setlogname (char*);
}

main()
{
	Path_list dirs = dircontents (spooldir);
	Path_list_iterator pli (dirs);
	Path p;
	while (pli.nextX (p)) {
		Path dir = spooldir & p;

		/*
		 * Try to remove the directory.  If we fail,
		 * that means it isn't empty, so there's work to do.
		 */
		if (rmdir (dir) < 0) {
			/* If the lock file exists, leave it alone */
			if (access (dir & "lock", 0) < 0) {
				struct stat sb;
				int pid = fork();
				int rc;
				switch (pid) {
				case -1:		// better luck next time
					break;

				case 0:			// child
					if (stat (dir, &sb) >= 0) {
						charstr ln = String(p);
						charstr d = String(Path(etcdir) & "udaemon");
						setlogname (ln);
						setgid (sb.st_gid);
						setuid (sb.st_uid);
						execl (d, "udaemon", (char*) 0);
					}
					exit (1);

				default:		// parent
					while ((rc = wait (0)) != -1 &&
					    rc != pid)
						;
					break;
				}
			}
		}
	}
	return 0;
}
