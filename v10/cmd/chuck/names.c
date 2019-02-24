#include "fs.h"
#include "pwd.h"

char *
stralloc(s)
char *s;
{	char *p;
	p = (char *) malloc(strlen(s) + 1);
	if(p)
		strcpy(p, s);
	return(p);
}

char *
prname(n)	/* find a name for inode n */
{	int i = 0;
	xbuf[0] = 0;
	switch(imap[n].type) {
	case Unalloc:
		return("(Unalloc)");
	case Weird:
		return("(Weird)");
	case Dir:
		if(imap[n].type == Dir)
			i = imap[n].dotdot;
		/* and then a random stab, probably find some bogus .. */
	default:
		if(!i)
			i = imap[n].parent;
		if(i == 0) {
			strcat(xbuf, "??");
			return(stralloc(xbuf));
		}
		prdirnm(i);
		nami(i, n);
		return(stralloc(xbuf));
	}
}

char *
prino(n)
{	char *s;
	struct passwd *x;
	struct dinode *dp;
	if(n < 1 || n >= ninode)
		sprintf(xbuf, "ino %d", n);
	else {
		dp = (struct dinode *) (buf + 2*bsize + (n-1)*sizeof(*dp));
		x = getpwuid(dp->di_uid);
		sprintf(xbuf, "ino %d is %s[%s:%s]len %d lnks %d", n, prname(n),
			x?x->pw_name: "(who?)", itype(imap[n].type), dp->di_size,
			dp->di_nlink);
	}
	return(stralloc(xbuf));
}

prdirnm(n)
{	int i;
	if(n == ROOTINO) {
		strcat(xbuf, "/");
		return;
	}
	i = imap[n].dotdot;
	if(i == n || i == 0) {
		strcat(xbuf, "?/");
		return;
	}
	prdirnm(i);
	nami(i, n);
}
nami(dir, ino)
{	int blksrch(), i;
	i = dirsrch(dir, ino, blksrch);
	if(i < 0) {
		strcat(xbuf, "?");
		return(-1);
	}
}

dirsrch(dir, arg, f)
int (*f)();
{	int i, addr[NADDR], j, *p;
	struct dinode *dp;
	dp = (struct dinode *) (buf + 2*bsize + (dir-1)*sizeof(*dp));
	l3tol(addr, dp->di_addr, NADDR);
	for(i = 0; i < NADDR-3; i++) {
		if(addr[i] == 0)
			continue;
		j = f(dir, arg, addr[i]);
		if(j >= 0)
			return(j);
	}
	if(!addr[10]) {	/* NADDR-3? */
		return(-1);
	}
	if(bread(addr[10], buf+bsize, 1)) {
		pmesg("dir %d couldn't read ind block %d\n", dir, addr[10]);
		return(-1);
	}
	p = (int *) (buf+bsize);
	for(i = 0; i < bsize/sizeof(int); i++) {
		if(p[i] == 0)
			continue;
		j = f(dir, arg, p[i]);
		if(j >= 0)
			return(j);
	}
	return(-1);
}

blksrch(dir, ino, n)
{	struct direct *dp;
	int i;
	if(bread(n, buf, 1)) {
		pmesg("dir %d couldn't read dir block %d\n", dir, n);
		return(-1);
	}
	dp = (struct direct *)buf;
	for(i = 0; i < bsize/sizeof(*dp); i++, dp++)
		if(dp->d_ino == ino) {
			strncat(xbuf, dp->d_name, DIRSIZ);
			if(imap[ino].type == Dir)
				strcat(xbuf, "/");
			return(1);
		}
	return(-1);
}

nams(dir, s)
char *s;
{	int nmsrch();
	return(dirsrch(dir, s, nmsrch));
}

nmsrch(dir, s, n)
char *s;
{	struct direct *dp;
	int i;
	if(bread(n, buf, 1)) {
		pmesg("dir %d couldn't read dir block %d\n", dir, n);
		return(-1);
	}
	dp = (struct direct *)buf;
	for(i = 0; i < bsize/sizeof(*dp); i++, dp++) {
		if(!dp->d_ino || strncmp(s, dp->d_name, DIRSIZ))
			continue;
		return(dp->d_ino);
	}
	return(-1);
}
