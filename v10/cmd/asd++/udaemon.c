#include "decl.h"
#include <fstream.h>

/* udaemon: do the processing for a single user */

/* Does this string represent the name of a status file? */
static int
isstat (String s)
{
	if (s.length() <= 2)
			return 0;
	return String (s (s.length()-2, 2)) == ".s";
}

/* Return the data file name corresponding to this string */
static String
datafile (String s)
{
	return String (s(0,s.length()-2)) + ".d";
}

/* Read a status file and return its contents */
static map
readstatus (String s)
{
	map m;
	char *buf = new char[s.length() + 1];
	s.dump (buf);
	filebuf fb;
	if (fb.open (buf, input)) {
		istream i (&fb);
		i >> m;
	}
	delete buf;
	return m;
}

/* Rewrite a map into a status file */
static void
writestatus (Path file, map m)
{
	String tempfile = file.rmlast() & "stemp";
	char *buf = new char[tempfile.length() + 1];
	tempfile.dump (buf);
	filebuf fb;
	if (fb.open (buf, output)) {
		ostream o (&fb);
		o << m;
		o.flush();
		if (o) {
			unlink (file);
			link (tempfile, file);
			unlink (tempfile);
		}
	}
	delete buf;
}

static int
listsearch (String_list sl, String s)
{
	String_list_iterator i (sl);
	String t;
	while (i.nextX (t)) {
		if (s == t)
			return 1;
	}
	return 0;
}

String_list deadmach;

/* Do all the work for a status file */
static void
dofile (Path sfile, Path dfile)
{
	map statmap = readstatus (sfile);
	mel_list_iterator mli (statmap.l);
	mel m;
	int keepfile = 0, sendfile = 0;
	while (mli.nextX (m)) {
		if (m.p.strchr('#') < 0 &&
		    (m.q.length() == 0 || m.s.length() != 0)) {
			m.q = String_list();
			if (listsearch (deadmach, m.p)) {
				keepfile = 1;
				m.s = "deferred for sequence";
				mli.replace (m);
			} else {
				m.s = domach (m.p, dfile, m.q);
				if (m.s.length() == 0) {
					sendfile = 1;
					if (m.q.length() == 0)
						mli.remove();
					else
						mli.replace (m);
				} else {
					keepfile = 1;
					deadmach += m.p;
					mli.replace (m);
				}
			}
		}
		writestatus (sfile, statmap);
	}

	int rc = 0;

	if (sendfile || !keepfile) {
		rc = system ("mail " + logname() + " <" + String(sfile));
	}

	if (!keepfile && rc == 0) {
		unlink (sfile);
		unlink (dfile);
	}
}

main()
{
	String ln = logname();
	Path myspool = spooldir & ln;

	umask (022);
	lock (myspool);

	Path_list files = dircontents (myspool);
	Path_list_iterator pli (files);
	Path f;

	while (pli.nextX (f)) {
		if (isstat (f))
			dofile (myspool & f, myspool & datafile(f));
	}
	unlock (myspool);
	return 0;
}
