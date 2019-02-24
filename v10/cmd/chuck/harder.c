#include "fs.h"
int rerunmsg;

attach(n)
{	int lf;
	char x[24];
	if(imap[n].type == Unalloc) {
		pmesg("ino %d is unallocated, not put in lost+found\n", n);
		return;
	}
	pmesg("re-attaching ino %d\n", n);
	lf = nams(ROOTINO, "lost+found");
	if(lf <= 0)
		lf = mkdir(ROOTINO, "lost+found");
	if(lf <= 0) {
		pmesg("can't find or create /lost+found.  HELP!\n");
		exitcode = 1;
		return;
	}
	sprintf(x, "%d", n);
	if(addtodir(lf, n, x) >= 0) {
		imap[n].nrefs++;
		fixlinks(n);
	}
}

mkdir(n, s)
char *s;
{	int i, b;
	struct dinode *dip;
	struct direct *dp;
	i = alloci(Dir);
	dip = (struct dinode *) (buf + 2*bsize + (i-1)*sizeof(*dip));
	dip->di_mode = (IFDIR | 0777);
	dip->di_uid = dip->di_gid = 0;
	b = allocb(i, First);
	dip->di_nlink = imap[i].nrefs = 2;	/* a little lie */
	dip->di_ctime = dip->di_mtime = dip->di_atime = time(0);
	/* now write . and .. */
	memset(buf, 0, bsize);
	dp = (struct direct *) buf;
	dp->d_ino = i;
	strcpy(dp->d_name, ".");
	dp++;
	dp->d_ino = n;
	strcpy(dp->d_name, "..");
	if(bwrite(b, buf, 1)) {
		pmesg("write of new dir blk %d failed\n", b);
		return(-1);
	}
	/* now stick it in its parent */
	if(addtodir(n, i, "lost+found") >= 0) {
		wrti(i);
		return(i);
	}
	pmesg("addtodir failed\n");
	return(-1);
}

fndfr(dir, pbno, n)
int *pbno;
{	struct direct *dp;
	int i;
	if(bread(n, buf, 1)) {
		pmesg("couldn't read dir block %d\n", n);
		return(-1);
	}
	*pbno = n;
	dp = (struct direct *) buf;
	for(i = 0; i < bsize/sizeof(*dp); i++, dp++)
		if(dp->d_ino == 0)
			return(i);
	return(-1);
}

addtodir(dir, ino, s)
char *s;
{	int i, bno;
	struct direct *dp;
	i = nami(dir, ino);
	if(i >= 0) {
		pmesg("ino %d already in dir (%d,%s) (it's ok)\n", ino, dir, prname(dir));
		/* since you cared enough to ask for it: */
		imap[ino].parent = dir;
		return(-1);
	}
	i = dirsrch(dir, &bno, fndfr);
	if(i < 0) {
		bno = allocb(dir, Other);
		wrti(dir);
		memset(buf, 0, bsize);
		i = 0;
	}
	dp = (struct direct *)buf + i;
	dp->d_ino = ino;
	strncpy(dp->d_name, s, DIRSIZ);
	imap[ino].parent = dir;
	/* if ino was a directory, its wretched pointers are wrong */
	if(flags['w'] && bwrite(bno, buf, 1) < 0) {
		pmesg("couldn't rewrite dir block %d\n", bno);
		return(-1);
	}
	if(!rerunmsg++)
		pmesg("!!counts may be wrong, RERUN chuck!\n");
	return(0);
}

alloci(t)
{	int i;
	for(i = 2; i < ninode; i++)
		if(imap[i].type == Unalloc) {
			imap[i].type = t;
			/* and adjust some counts? */
			return(i);
		}
	return(-1);
}

/* doesn't understand about holes BUG*/
allocb(ino, t)
{	int i, j;
	long addr[NADDR];
	struct dinode *dip;
	dip = (struct dinode *) (buf + 2*bsize + (ino-1)*sizeof(*dip));
	l3tol(addr, dip->di_addr, NADDR);
	for(j = 0; j < NADDR-3; j++)	/* secret knowledge!! */
		if(!addr[j])
			break;
	if(j >= NADDR-3) {
		pmesg("won't allocate indirect block (bug)%d %d\n");	/* BUG */
		return(-1);
	}
	dip->di_size += 4096;
	for(i = fblk; i < lblk; i++)
		if(bmap[i].type == Free) {
			bmap[i].ino = ino;
			bmap[i].type = t;
			addr[j] = i;
			ltol3(dip->di_addr, addr, NADDR);
			/* and adjust some counts? */
			return(i);
		}
	pmesg("no more free blocks\n");
	return(-1);
}

expg(dir, x, n)
{	struct direct *dp;
	static struct direct nildp;
	int i;
	if(bread(n, buf, 1)) {
		pmesg("expg read block %d failed\n", n);
		return(-1);
	}
	dp = (struct direct *) buf;
	for(i = 0; i < bsize/sizeof(*dp); i++ , dp++)
		if(dp->d_ino == x) {
			*dp = nildp;
			if(flags['w'] && bwrite(n, buf, 1))
				pmesg("couldn't rewrite dir block\n");
		}
	return(-1);	/* in case it occurs many times in the directory */
}
/* remove the ref to the inode from a directory it's in */
/* if you want more than one, run it lots of times */
expunge(n)
{	int i, j;
	j = imap[n].parent;
	if(!j || imap[j].type != Dir) {
		pmesg("parent is %d, not dir\n", j);
		return(-1);
	}
	i = dirsrch(j, n, expg);
}

fixdots(ino)
{	struct dinode *dp = (struct dinode *)(buf + 2*bsize + (ino-1)*sizeof(*dp));
	int addr[NADDR], i;
	if(imap[ino].type == Unalloc)	/* someone else really fixed it */
		return;
	pmesg("fixdots %s\n", prino(ino));
	if(imap[ino].type != Dir) {
		pmesg("fixdir called on non-dir %s\n", prino(ino));
		return;
	}
	/* now redo all the checking and fix everything in sight */
	l3tol(addr, dp->di_addr, NADDR);
	if(fixfirst(ino, dp, addr[0]) < 0) {
		pmesg("too hard, try rerunning chuck\n");
		exitcode++;
		rerunmsg++;
		return;
	}
	for(i = 0; i < erptr; i++)
	switch(erlist[i].type) {
	case Enotdot: case Edotino: case Enotdotdot: case Ebadparent:
		if(erlist[i].a == ino)
			erlist[i].done = 1;
	}
}

fixfirst(ino, dp, addr)
struct dinode *dp;
{	struct direct *d;
	static struct direct nildir;
	int i;
	if(addr < fblk || addr >= lblk) {
		pmesg("first block address %d bogus\n", addr);	/* FIX */
		return(-1);
	}
	if(bread(addr, buf, 1) < 0) {
		pmesg("couldn't read %d\n", addr);
		return(-1);
	}
	d = (struct direct *) buf;
	if(firstspace() < 0)	/* make space at beginning for . and .. */
		return(-1);
	/* this strategy may leave some link counts wrong, FIX */
	/* also, who made dp->d_size large enough? FIX */
	*d = nildir;
	d->d_ino = ino;
	strcpy(d->d_name, ".");
	d++;
	i = imap[ino].parent;	/* what if we are a directory? FIX */
	if(i < ROOTINO || i >= ninode) {
		pmesg("parent is %d, and so illegal\n", i);
		return(-1);
	}
	*d = nildir;
	d->d_ino = i;
	strcpy(d->d_name, "..");
	if(flags['w'] && bwrite(addr, buf, 1) < 0) {
		pmesg("couldn't rewrite %d\n");
		return(-1);
	}
	return(0);
}

firstspace()
{	struct direct *dp = (struct direct *)buf;
	int i, cnt, baddot, baddotdot, fa, fb;
	for(fa = fb = cnt = i = 0; i < bsize/sizeof(*dp); i++) {
		if(dp->d_ino)
			continue;
		cnt++;
		if(!fa) {
			fa = i;
			continue;
		}
		if(!fb)
			fb = i;
	}
	dp = (struct direct *) buf;
	baddot = strcmp(dp->d_name, ".");
	baddotdot = strcmp(dp[1].d_name, "..");
	if(cnt - baddot - baddotdot < 0) {
		pmesg("first block of directory overfull\n");
		return(-1);	/* too hard FIX */
	}
	if(baddot)
		dp[fa] = dp[0];
	if(baddotdot)
		dp[fb] = dp[1];
	return(0);
}

xfxnm(dir, ret, n)
int *ret;
{	struct direct *dp;
	int i, j, cnt = 0;
	if(bread(n, buf, 1)) {
		pmesg("dir %d couldn't read dir block %d\n", dir, n);
		return(*ret = -1);
	}
	dp = (struct direct *)buf;
	for(i = 0; i < bsize/sizeof(*dp); i++, dp++) {
		if(!dp->d_ino)
			continue;
		if(dp->d_ino == dir && dp->d_name[0] != '.') {	/* FIX */
			dp->d_ino = 0;	/* dirs can't refer to themselves */
			/* link count now wrong, in safe direction */
		}
		if(dp->d_ino < 1 || dp->d_ino >= ninode)
			dp->d_ino = 0;	/* bad inumber */
		if(imap[dp->d_ino].type == Unalloc) {	/* ambitious FIX*/
			if(cnt++ == 10)
				pmesg("...\n");
			else if(cnt < 10)
				pmesg("dir entry %d,%.16s unalloc inode\n", dp->d_ino,
					dp->d_name);
			dp->d_ino = 0;
		}
		for(j = 0; j < DIRSIZ; j++) {
			if(!dp->d_name[j])
				break;
			if(dp->d_name[j] & 0x80) {	/* ascii! */
				dp->d_name[j] &= 0x7f;
				j--;	/* did we zero it? */
				continue;
			}
		}
		if(j == 0) {
			pmesg("ino %d in dir %d had no name\n", dp->d_ino, dir);
			pmesg("being called #%d\n", dp->d_ino);
			sprintf(dp->d_name, "#%d", dp->d_ino);
			continue;
		}
		for(; j < DIRSIZ;j++)
			dp->d_name[j] = 0;
	}
	if(flags['w'] && bwrite(n, buf, 1)) {
		pmesg("couldn't rewrite blk %d ino %d\n", n, dir);
		return(*ret = -1);
	}
	return(-1);
}
fixnames(ino)
{	int retcode = 0, j;
	(void) dirsrch(ino, &retcode, xfxnm);
	if(retcode < 0) {
		pmesg("fixnames failed\n");
		return;
	}
	for(j = 0; j < erptr; j++)
		if(erlist[j].type == Ebadname && erlist[j].a == ino)
			erlist[j].done = 1;
}
