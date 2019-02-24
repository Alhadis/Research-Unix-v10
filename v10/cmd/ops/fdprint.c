#include "ps.h"
#include <sys/text.h>
#include <sys/inode.h>
#include <sys/file.h>

char *
fdprint(cp, pp, up)
char *cp; register struct proc *pp; register struct user *up;
{
	register j, k;
	struct text x; struct file f; struct inode i;
	char ident[NOFILE], prefix[8];

	if (fflag <= 1)
		sprintf(prefix, "%5d ", pp->p_pid);
	else
		strcpy(prefix, "      ");

	if (Kread(pp->p_textp, &x) && Kread(x.x_iptr, &i))
		cp += sprintf(cp, "%sprog: ", prefix), cp = iprint(cp, &i);
	if (Kread(up->u_cdir, &i))
		cp += sprintf(cp, "%scdir: ", prefix), cp = iprint(cp, &i);
	if (Kread(up->u_rdir, &i))
		cp += sprintf(cp, "%srdir: ", prefix), cp = iprint(cp, &i);
	for (k = 0; k < NOFILE; k++)
		ident[k] = 0;
	for (k = 0; k < NOFILE; k++) if (!ident[k]) {
		if (!Kread(up->u_ofile[k], &f) || !Kread(f.f_inode, &i))
			continue;
		cp += sprintf(cp, "%sfd %d", prefix, k);
		for (j=k+1; j<NOFILE; j++)
			if (up->u_ofile[j] == up->u_ofile[k])
				cp += sprintf(cp, ",%d", j), ++ident[j];
		if (f.f_offset.hi == 0)
			cp += sprintf(cp, " at %d: ", f.f_offset.lo);
		else
			cp += sprintf(cp, " at 0x%x%.8x: ", f.f_offset.hi, f.f_offset.lo);
		cp = iprint(cp, &i);
	}
	return cp;
}

char *
iprint(cp, ip)
register char *cp; register struct inode *ip;
{
	register char *p; char *lookdir();

	switch (ip->i_mode & IFMT) {
	case IFCHR:
	case IFBLK:
		if (p = lookdir(ip->i_number))
			cp += sprintf(cp, "%s\n", p);
		else
			cp += sprintf(cp, "(%d/%d)\n",
				major(ip->i_un.i_rdev), minor(ip->i_un.i_rdev));
		return cp;
	case IFREG:
		cp += sprintf(cp, ip->i_sptr ? "stream" : "file"); break;
	case IFDIR:
		cp += sprintf(cp, "dir"); break;
	default:
		cp += sprintf(cp, "[%o06]", ip->i_mode); break;
	}
	cp += sprintf(cp, " %d on ", ip->i_number);
	if (ip->i_dev == 0 && ip->i_fstyp == 2)
		ip->i_dev = makedev(255, 255);
	if (p = getfs(ip->i_dev))
		cp += sprintf(cp, "%s\n", p);
	else
		cp += sprintf(cp, "(%d/%d)\n",
			major(ip->i_dev), minor(ip->i_dev));
	return cp;
}
