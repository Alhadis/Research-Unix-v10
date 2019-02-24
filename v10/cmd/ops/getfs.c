#include "ps.h"

#define NNAMES	16

char *fspts[3];

struct direct *fsbeg, *fsend, *fsprev;

#ifdef TEST
main(argc, argv)
char **argv;
{
	register struct direct *fs; int dev; char *getfs();
	getfstab();
	if (argc <= 1)
		for (fs=fsbeg; fs < fsend; fs++)
			printf("%3d,%3d %.14s\n",
				major(fs->d_ino), minor(fs->d_ino), fs->d_name);
	else while ((argc -= 2) > 0) {
		dev = atoi(*++argv);
		dev = makedev(dev, atoi(*++argv));
		printf("%3d,%3d %.14s\n", major(dev), minor(dev), getfs(dev));
	}
	exit(0);
}
#endif

char *
getfs(dev)
register dev;
{
	register struct direct *fs;

	if ((fs = fsprev) && fs->d_ino == dev)
		return fs->d_name;
	for (fs=fsbeg; fs < fsend; fs++)
		if (fs->d_ino == dev ||
		    (major(fs->d_ino) == major(dev) && minor(fs->d_ino) == 255)) {
			fsprev = fs;
			return fs->d_name;
		}
	return 0;
}

getfstab()
{
	char *buffer, *bufpt, *nlpt, *strchr(), *strrchr(), *sindex();
	struct stat fd_st;
	register struct direct *fs = 0;
	register char *p; int fd;

	if (fsbeg)
		return 0;

	if ((fd = open("/etc/fstab", 0)) < 0)
		return 1;

	if (fstat(fd, &fd_st) || (buffer = malloc((int)fd_st.st_size+1)) == 0)
		return close(fd), 1;

	if (!Read(fd, buffer, (int)fd_st.st_size))
		return free(buffer), close(fd), 1;
	buffer[fd_st.st_size] = 0;

	for (bufpt=buffer; nlpt=strchr(bufpt, '\n'); bufpt=nlpt) {
		*nlpt++ = 0;
		if ((p = strchr(bufpt, ':')) == 0)
			continue;
		*p = 0;
		if (stat(bufpt, &fd_st))
			continue;
		if ((fs = Nalloc(struct direct, fspts, NNAMES)) == 0)
			break;
		if (p = strrchr(bufpt, '/'))
			++p;
		else
			p = bufpt;
		strncpy(fs->d_name, p, DIRSIZ);
		fs->d_ino = fd_st.st_rdev;
	}
	free(buffer), close(fd);

	if ((fd = open("/usr/net/friends", 0)) < 0)
		return 1;

	if (fstat(fd, &fd_st) || (buffer = malloc((int)fd_st.st_size+1)) == 0)
		return close(fd), 1;

	if (!Read(fd, buffer, (int)fd_st.st_size))
		return free(buffer), close(fd), 1;
	buffer[fd_st.st_size] = 0;

	for (bufpt=buffer; nlpt=strchr(bufpt, '\n'); bufpt=nlpt) {
		*nlpt++ = 0;
		if ((bufpt = sindex(bufpt, " \t")) == 0)
			continue;
		if ((bufpt = strrchr(bufpt, '/')) == 0)
			continue;
		if ((p = sindex(++bufpt, " \t")) == 0)
			continue;
		*p = 0;
		if ((fs = Nalloc(struct direct, fspts, NNAMES)) == 0)
			break;
		strncpy(fs->d_name, bufpt, DIRSIZ);
		fs->d_ino = makedev(atoi(++p), 255);
	}
	free(buffer), close(fd);

	if (stat(bufpt = "/proc", &fd_st) == 0 &&
	   (fs = Nalloc(struct direct, fspts, NNAMES))) {
		strncpy(fs->d_name, bufpt+1, DIRSIZ);
		fs->d_ino = fd_st.st_dev;
	}
	if (fs) {
		fsbeg = (struct direct *)fspts[0];
		fsend = (struct direct *)fspts[1];
	}
	return 0;
}

char *
sindex(s, c)
register char *s, *c;
{
	register char *p = 0, *q;
	while (*c) {
		if ((q = strchr(s, *c++)) == 0)
			continue;
		if (p == 0 || q < p)
			p = q;
	}
	return p;
}
