#include "ps.h"

#define GETC(fd, n, p, buffer) \
		(--n > 0 || \
		(n = read(fd, p = buffer, sizeof buffer)) > 0 ? *p++ : -1)

#define NNAMES	100

char *unpts[3];

enum States { START, NAME, SKCOL, UID, SKNL };

struct direct *unbeg, *unend, *unprev;

#ifdef TEST
main(argc, argv)
char **argv;
{
	register struct direct *up; int uid; char *getuname();
	getlogins();
	if (argc <= 1)
		for (up=unbeg; up < unend; up++)
			printf("%5d %.14s\n", up->d_ino, up->d_name);
	else while (--argc > 0) {
		uid = atoi(*++argv);
		printf("%5d %.14s\n", uid, getuname(uid));
	}
	exit(0);
}
#endif

char *
getuname(uid)
register uid;
{
	register struct direct *up;

	if ((up = unprev) && up->d_ino == uid)
		return up->d_name;
	for (up=unbeg; up < unend; up++)
		if (up->d_ino == uid) {
			unprev = up;
			return up->d_name;
		}
	return "?";
}

getlogins()
{
	char buffer[1024];
	register fd, c, state, n; register struct direct *up = 0;
	register char *p; int nbuf = 0;

	if (unbeg)
		return 0;

	if ((fd = open("/etc/passwd", 0)) < 0)
		return 1;

	state = (int)START;
	while ((c = GETC(fd, nbuf, p, buffer)) > 0) switch (state) {
	case START:
		if ((up = Nalloc(struct direct, unpts, NNAMES)) == 0)
			goto out;
		n = 0;
		up->d_ino = 0;
		state = (int)NAME;
		/* fall through */
	case NAME:
		if (c == ':') {
			if (n < DIRSIZ)
				up->d_name[n++] = 0;
			state = (int)SKCOL;
		} else if (n < DIRSIZ)
			up->d_name[n++] = c;
		break;
	case SKCOL:
		if (c == ':')
			state = (int)UID;
		break;
	case UID:
		if (c >= '0' && c <= '9')
			up->d_ino = 10*up->d_ino + c - '0';
		else
			state = (int)SKNL;
		break;
	case SKNL:
		if (c == '\n')
			state = (int)START;
		break;
	}
out:
	if (up) {
		unbeg = (struct direct *)unpts[0];
		unend = (struct direct *)unpts[1];
	}
	close(fd);
	return 0;
}
