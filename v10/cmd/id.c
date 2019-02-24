#include <stdio.h>
#include <pwd.h>
#include <grp.h>

int getopt();

main (argc, argv)
	int argc;
	char **argv;
{
	int uid, gid, euid, egid, c, flags=0;
	static char stdbuf[BUFSIZ];
	char ln[64];

	setbuf (stdout, stdbuf);

	uid = getuid();
	gid = getgid();
	euid = geteuid();
	egid = getegid();
	ln[0] = '\0';
	getlogname (ln);

	while ((c = getopt (argc, argv, "ugUGl")) >= 0) {
		flags++;
		switch (c) {
		case 'u':
			puid ("", euid);
			break;
		case 'g':
			pgid ("", egid);
			break;
		case 'U':
			puid ("", uid);
			break;
		case 'G':
			pgid ("", gid);
			break;
		case 'l':
			plogin ("", ln);
			break;
		}
		putchar ('\n');
	}

	if (!flags) {
		puid ("uid=", uid);
		pgid (" gid=", gid);
		if (uid != euid)
			puid (" euid=", euid);
		if (gid != egid)
			pgid (" egid=", egid);
		plogin (" login=", ln);
		putchar ('\n');
	}
	return 0;
}

puid (s, id)
	char *s;
	int id;
{
	struct passwd *pw;
	struct passwd *getpwuid();

	printf ("%s%d", s, id);
	setpwent();
	pw = getpwuid(id);
	if (pw)
		printf ("(%s)", pw->pw_name);
}

pgid (s, id)
	char *s;
	int id;
{
	struct group *gr;
	struct group *getgrgid();

	printf ("%s%d", s, id);
	setgrent();
	gr = getgrgid(id);
	if (gr)
		printf ("(%s)", gr->gr_name);
}

plogin (s, l)
	char *s, *l;
{
	if (*l)
		printf ("%s%s", s, l);
}
