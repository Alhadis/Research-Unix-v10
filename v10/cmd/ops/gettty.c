#include "ps.h"

char *
lookdir(node)
register node;
{
	register Dirnode *np;
	register struct direct *dp;
	if (node)
		for (np = devlist; np; np = np->next)
			for (dp = np->begin; dp < np->end; dp++)
				if (dp->d_ino == node)
					return dp->d_name;
	return 0;
}

char *
gettty(dev, node)
{
	register char *nm;
	if (dev == 0xffff) {
		static char buf[8];
		sprintf(buf, "p%4.4x", node&0xffff);
		return buf;
	}
	nm = lookdir(node);
	if (nm == 0)
		return "?";
	if (nm[0] == 't' && nm[1] == 't' && nm[2] == 'y')
		nm += 3;
	return nm;
}

Dirnode *
getdir(dirname, list)
char *dirname; Dirnode *list;
{
	struct stat sbuf;
	register fd, n;
	register Dirnode *np; register struct direct *dp;

	if ((fd = open(dirname, 0)) < 0)
		return 0;
	if (fstat(fd, &sbuf) < 0 || (np = Malloc(Dirnode, 1)) == 0) {
		close(fd); return 0;
	}
	n = sbuf.st_size/sizeof(struct direct);
	if ((dp = Malloc(struct direct, n)) == 0) {
		free(np); close(fd); return 0;
	}
	if (!Read(fd, dp, sbuf.st_size)) {
		free(np); free(dp); close(fd); return 0;
	}
	np->next = list;
	np->begin = dp;
	np->end = dp + n;
	close(fd);
	return np;
}
