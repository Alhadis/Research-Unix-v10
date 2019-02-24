#include "decl.h"
#include <dir.h>

int
isdir (Path& p)
{
	struct stat sb;

	if (stat (p, &sb) < 0)
		return 0;
	return (sb.st_mode & S_IFMT) == S_IFDIR;
}

static char*
charconv (String& s)
{
	char *p = new char[s.length() + 1];
	s.dump (p);
	return p;
}

Path_list
dircontents (Path& p)
{
	Path_list r;
	char *cp = charconv (String(p));
	DIR *df = opendir (cp);

	if (df) {
		direct *dp;
		while (dp = readdir (df)) {
			Path ds = dp->d_name;
			if (ds != "." && ds != "..") {
				Path_list_iterator i (r);
				Path x;

				while (i.peekX (x) && x < ds)
					i.next();

				i.insert (ds);
			}
		}

		closedir (df);
	}

	delete cp;

	return r;
}
