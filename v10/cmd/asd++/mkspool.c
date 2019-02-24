#include "decl.h"
#include <pwd.h>

/*
 *	mkspool() -- ensure the proper spool directories exist
 *	with the proper modes and ownerships.  This program should
 *	be run setuid to root.
 */

static const int dirmode = 0755;

struct ugid {
	int uid, gid;
	ugid (String&);
	~ugid();
};

ugid::
ugid (String& s)
{
	char* cp = new char[s.length() + 1];
	s.dump (cp);
	struct passwd *pw = getpwnam (cp);
	delete cp;
	if (!pw) {
		cerr << "mkspool: cannot recognize logname " << s << "\n";
		exit (1);
	}
	uid = pw->pw_uid;
	gid = pw->pw_gid;
}

ugid::
~ugid()
{
	endpwent();
}

static void
forcedir (Path& dir, String& uname)
{
	ugid u (uname);

	if (access (dir, 0) == 0) {
		if (!isdir (dir)) {
			cerr << "mkspool: "
				<< dir << " exists and is not a directory\n";
			exit (1);
		}
		chmod (dir, dirmode);
	} else
		mkdir (dir, dirmode);
	chown (dir, u.uid, u.gid);
}

main()
{
	String l = logname();
	forcedir (spooldir, "root");
	lock (spooldir);
	Path mydir = spooldir & l;
	forcedir (mydir, l);
	close (creat (mydir & "dummy", 0444));
	unlock (spooldir);
	cout << mydir << "\n";
	return 0;
}
