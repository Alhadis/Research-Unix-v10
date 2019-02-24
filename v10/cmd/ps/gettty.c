#include "ps.h"
#include <stdio.h>
#include <ndir.h>

char *
gettty(dev, node)
{
	register char *nm;
	if (dev == NODEV) {	/* cheap hack: assume it's a pipe */
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

initttys()
{
	char dirname[100];	/* arbitrary size */
	List *lp;
	FILE *fp;

	if ((fp = fopen("/lib/ttydevs", "r")) == NULL) {
		devlist = getdir("/dev", devlist);
		return;
	}
	while (fgets(dirname, sizeof(dirname), fp)) {
		dirname[strlen(dirname)-1] = 0;
		if ((lp = getdir(dirname, devlist)) != NULL)
			devlist = lp;
	}
	fclose(fp);
}

char *
lookdir(node)
register node;
{
	register List *lp;
	register Entry *ep;
	if (node)
		for (lp = devlist; lp; lp = lp->next)
			for (ep = lp->begin; ep; ep = ep->next)
				if (ep->e_val == node)
					return ep->e_name;
	return 0;
}

List *
getdir(dirname, list)
char *dirname;
List *list;
{
	register List *lp;
	register Entry *ep;
	register DIR *dirp;
	register struct direct *dp;

	if((dirp = opendir(dirname)) == 0)
		return 0;
	if((lp = Malloc(List, 1)) == 0) {
		closedir(dirp);
		return 0;
	}
	lp->next = list;
	lp->begin = 0;
	lp->nentry = 0;
	for(dp = readdir(dirp); dp; dp = readdir(dirp)) {
		if(dp->d_name[0] == '.') {
			if(dp->d_name[1] == '\0')
				continue;
			if(dp->d_name[1] == '.' && dp->d_name[2] == '\0')
				continue;
		}
		if((ep = Malloc(Entry, 1)) == 0)
			break;
		ep->next = lp->begin;
		lp->begin = ep;
		lp->nentry++;
		ep->e_val = dp->d_ino;
		strncpy(ep->e_name, dp->d_name, sizeof(ep->e_name));
	}
	closedir(dirp);
	return lp;
}
