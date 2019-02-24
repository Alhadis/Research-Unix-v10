#include "decl.h"

Path
mkpath (String st)
{
	Path p;

	p.s = st;
	return p;
}

Path
Path::operator & (const Path& sub) const
{
	String dir = s;
	String subdir = sub.s;

	if (subdir.length() == 0)
		return *this;

	if (subdir[0] == '/' || dir.length() == 0)
		return sub;

	if (dir == "/")
		return mkpath ('/' + subdir);

	return mkpath (dir + '/' + subdir);
}

Path
Path::first() const
{
	int n = s.strchr('/');

	if (n < 0)
		return s;

	if (n == 0)
		return "/";

	return mkpath (s(0,n));
}

Path
Path::last() const
{
	int n = s.strrchr('/');

	if (n < 0)
		return mkpath (s);

	return mkpath (s(n+1,s.length()-n-1));
}

Path
Path::rmfirst() const
{
	int n = s.strchr('/');

	if (n < 0)
		return mkpath ("");

	return mkpath (s(n+1,s.length()-n-1));
}

Path
Path::rmlast() const
{
	int n = s.strrchr('/');

	if (n < 0)
		return mkpath ("");

	if (n == 0)
		return mkpath ("/");

	return mkpath (s(0,n));
}

Path homedir()
{
	char *p = getenv ("HOME");
	return p? p: "";
}

ostream& operator << (ostream& o, const Path& p)
{
	o << p.s;
	return o;
}

// remove multiple or trailing slashes
String
pathnorm (String s)
{
	String r;
	int l = s.length();
	int state = 0;

	for (int i = 0; i < l; i++) {
		char c = s[i];
		switch (state) {
		case 0:			// initial state
			r += c;
			if (c == '/')
				state = 1;
			else
				state = 2;
			break;

		case 1:			// seen initial slashes only
			if (c != '/') {
				r += c;
				state = 2;
			}
			break;

		case 2:			// seen a component character
			if (c != '/')
				r += c;
			else
				state = 3;
			break;

		case 3:			// seen a non-leading slash
			if (c != '/') {
				r += '/';
				r += c;
				state = 2;
			}
			break;
		}
	}

	return r;
}
