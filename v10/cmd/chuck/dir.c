#include "fs.h"

dirblocks()
{	int i;
	i = -1;
loop:
	if(++i >= lblk)
		return;
	switch(bmap[i].type) {
	default:
		pmesg("strange blk type %d in dirblocks\n", bmap[i].type);
		bmap[i].type = Bad;
		goto loop;
	case Unk: case Sblock: case Inode: case Free: case Data: case Ind:
	case Ind2: case Ind3: case Bits: case Ioerr: case Bad: case Boot:
		goto loop;
	case First:
		if(bread(i, buf, 1)) {
			pmesg("dir %d first read %d failed\n", bmap[i].ino, i);
			bmap[i].type = Ioerr;
			direrror(Efirstio, bmap[i].ino);
			goto loop;
		}
		dofirst(i);
		doother(i);
		goto loop;
	case Other:
		if(bread(i, buf, 1)) {
			pmesg("dir %d blk read %d failed\n", bmap[i].ino, i);
			bmap[i].type = Ioerr;
			direrror(Edirio, bmap[i].ino);
			goto loop;
		}
		doother(i);
		goto loop;
	}
}

/* check that the first two entries are named . and .., and that .'s ino is ours */
/* also check the directory's length somewhere FIX */
dofirst(i)
{	struct direct *dp;
	int ino = bmap[i].ino;
	dp = (struct direct *) buf;
	if(dp->d_name[0] != '.' || dp->d_name[1] != 0)
		direrror(Enotdot, ino, 0);
	if(dp->d_ino != ino)
		direrror(Edotino, ino, dp->d_ino);
	dp++;
	if(dp->d_name[0] != '.' || dp->d_name[1] != '.' || dp->d_name[2] != 0)
		direrror(Enotdotdot, ino, 0);
	else {
		if(imap[ino].dotdot)
			direrror(Emulti, ino, imap[ino].dotdot);
		imap[ino].dotdot = dp->d_ino;	/* unchecked FIX */
	}
	if(ino == imap[ino].dotdot && ino != ROOTINO)
		direrror(Efakeroot, ino, 0);
}
	
doother(n)
{	register int ino = bmap[n].ino, j;
	register struct direct *dp = (struct direct *) buf, *dend;
	register char *p;
	register im *ip;
	dend = (struct direct *) (buf + bsize);
	if(dmapptr+(dend-dp) >= dmaplen) {
		dmaplen *= 3;
		dmap = (dm *) realloc((char *) dmap, dmaplen*sizeof(dm));
		if(!dmap)
			fatal("doother realloc(%d) failed\n", dmaplen);
	}
	for(; dp < dend; dp++) {
		if(dp->d_ino == 0)
			continue;
		if(dp->d_ino < 1 || dp->d_ino >= ninode) {
			direrror(Ebadino, ino, n);
			continue;
		}
		ip = imap + dp->d_ino;
		if(ip->type == Dir) {
			dmap[dmapptr].dino = ino;
			dmap[dmapptr].ino = dp->d_ino;
			dmapptr++;
		}
		else if(ip->type == Unalloc) {
			direrror(Ebadino, ino, n);
			/* should we look more closely at inode? FIX */
		}
		ip->nrefs++;
		p = dp->d_name;
		if(p[0] != '.' || (p[1] != '.' && p[1]) || p[2] != 0)
			ip->parent = ino;	/* ., .. don't help */
		for(j = 0; j < DIRSIZ; j++, p++)
			if(*p <= 0 || *p > 127)
				break;
		if(j >= DIRSIZ)
			continue;
		if(*p) {
			direrror(Ebadname, ino, dp->d_ino);
			continue;
		}
		for(; j < DIRSIZ; j++, p++)
			if(*p)
				break;
		if(j >= DIRSIZ)
			continue;
		direrror(Ebadname, ino, dp->d_ino);
	}
}

dcmp(a, b)
register dm *a, *b;
{
	return(a->dino - b->dino);
}

dirtree()
{	register int i, last = -1;
	/* sort by directory */
	qsort((char *)dmap, dmapptr, sizeof(dm), dcmp);
	for(i = 0; i < dmapptr; i++)
		if(dmap[i].dino != last) {
			last = dmap[i].dino;
			imap[last].ptr = i;
		}
	/* is dotdot really a parent pointer? */
	for(i = 1; i < ninode; i++) {
		if(imap[i].type != Dir)
			continue;
		if(dirin(i, imap[i].dotdot))
			continue;
		direrror(Ebadparent, i, imap[i].dotdot);
	}
	/* are they all accessible from the root? */
	if(imap[ROOTINO].type != Dir)
		fatal("root ino %d not a directory\n", ROOTINO);	/* FIX */
	chtree(ROOTINO);
	for(i = 0; i < ninode; i++) {
		if(imap[i].type != Dir)
			continue;
		if(!imap[i].seen)
			direrror(Eattach, i, 0);
	}
}

chtree(n)
{	dm *d;
	if(!imap[n].ptr && n != ROOTINO) {
		pmesg("dir %s contains nothing\n", prino(n));
		return;
	}
	imap[n].seen = 1;
	for(d = dmap + imap[n].ptr; d->dino == n; d++)	/* end condition? FIX */
		if(!imap[d->ino].seen)
			chtree(d->ino);
}
		
dirin(child, parent)
{	int n, i;
	for(n = imap[parent].ptr; n < dmapptr && dmap[n].dino == parent; n++)
		if(dmap[n].ino == child)
			return(1);
	return(0);
}
