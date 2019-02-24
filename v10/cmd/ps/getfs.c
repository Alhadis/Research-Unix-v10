#include "ps.h"
#include <fstab.h>
#include <sys/inode.h>

static List namelist;
static Entry *prev;

char *
getfs(ip)
register struct inode *ip;
{
	register Entry *ep;

	if (ip->i_fstyp)	/* temp */
		return 0;
	if((ep = prev) && ep->e_val == ip->i_dev)
		return ep->e_name;
	for(ep = namelist.begin; ep; ep = ep->next) {
		if(ep->e_val == ip->i_dev ||
		    (major(ep->e_val) == major(ip->i_dev) && minor(ep->e_val) == 255)) {
			prev = ep;
			return ep->e_name;
		}
	}
	return 0;
}

getfstab()
{
	register Entry *ep;
	register struct fstab *fp;
	register char *p;
	char *strrchr();
	struct stat statb;

	while(fp = getfsent()) {
		if(stat(fp->fs_spec, &statb) == -1)
			continue;
		if((ep = Malloc(Entry, 1)) == 0)
			break;
		ep->next = namelist.begin;
		namelist.begin = ep;
		namelist.nentry++;
		ep->e_val = statb.st_rdev;
		if((p = strrchr(fp->fs_spec, '/')) == 0)
			p = fp->fs_spec;
		else
			p++;
		strncpy(ep->e_name, p, sizeof(ep->e_name));
	}
	endfsent();
}
