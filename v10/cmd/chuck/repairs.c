#include "fs.h"

repairs()
{	int i, j, k;
	if(erptr <= 0) {
		pmesg("all ok\n");
		return;
	}
	i = errcnts[Einvalid] + errcnts[Esuperino] + errcnts[Esuperfree]
		+ errcnts[Efreelist];
	if(!flags['i'] && i == erptr) {
		pmesg("fixing super block\n");
		fixfree();
		if(!flags['w'])
			return;
		if(bwrite(1, (char *)&sblk, 1))
			fatal("write failed\n");
		pmesg("done\n");
		return;
	}
	if(!flags['i'] && i + errcnts[Enullable] == erptr) {
		pmesg("clearing %d inodes safely\n", errcnts[Enullable]);
		/* even if it is a directory.  no links means Edot, Edotdot... */
		for(j = 0; j < erptr; j++)
			if(erlist[j].type == Enullable)
				clri(erlist[j].a);
		fixfree();
		return;
	}
	/* now we enter upon the realm of the speculative */
	if(!flags['i'] && !errcnts[Edup]) {
		pmesg("hi ho hi ho, it's off to work we go\n");
		if(j = errcnts[Enullable])
			pmesg("clearing %d inodes safely\n", j);
		for(j = 0; j < erptr; j++)
		switch(erlist[j].done? 0: erlist[j].type) {
		case 0:
			continue;
		default:
			pmesg("%s %d %d, not handled\n", errnm(erlist[j].type),
				erlist[j].a, erlist[j].b);
			exitcode++;
			continue;
		case Ehole:
			pmesg("%s ends with a hole, remove or copy\n", prino(erlist[j].a));
			continue;
		case Enullable:
			clri(erlist[j].a);
			erlist[j].done = 1;
			continue;
		case Enotdot: case Enotdotdot: case Edotino: case Ebadparent:
			k = erlist[j].a;
			if(k > ROOTINO)
				fixdots(k);
			continue;
		case Elinkcnt:
			k = erlist[j].a;
			if(imap[k].type == Weird) {
				if(fixweird(k))
					continue;
				pmesg("weird ino %d (strange type): fix manually\n", k);
				exitcode = 1;
				continue;
			}
			if(imap[k].nrefs == 0)
				attach(k);
			else if(imap[k].type != Unalloc)
				fixlinks(erlist[j].a);
			else if(imap[k].nrefs == 1)
				expunge(k);
			else {
				pmesg("%s has %d references!  Expunging.\n", prino(k),
					imap[k].nrefs);
				expunge(k);
				exitcode = 1;	/* things are not ok */
				continue;
			}
			erlist[j].done = 1;
			continue;
		case Ebadname:
			pmesg("%s has a weird name, look at it\n", prino(erlist[j].b));
			fixnames(erlist[j].a);
			continue;
		case Ebadino:	/* these are lost files */
			pmesg("removing bad inos from %s\n", prino(erlist[j].a));
			fixnames(erlist[j].a);
			continue;
		case Einvalid: case Efreelist: case Esuperino:
			continue;	/* fixfree() comes later */
		case Eattach:
			attach(erlist[j].a);
			erlist[j].done = 1;
			continue;
		case Efakeroot:
			attach(erlist[j].a);
			fixdots(erlist[j].a);
			continue;
		}
		fixfree();
		return;
	}
	rpterrs();
	if(flags['i'])
		interact();
	else
		exitcode = 1;
}

rpterrs()
{	int i, j, k, cnts[32];
	struct dinode *dp;
	if(erptr > 30 && !flags['v']) {
		pmesg("%d errors\n", erptr);
		for(i = 0; i < 32; i++)
			cnts[i] = 0;
		for(i = 0; i < erptr; i++)
			cnts[erlist[i].type]++;
		for(i = 0; i < 32; i++)
			if(cnts[i])
				pmesg("%s\t%d\n", errnm(i), cnts[i]);
		return;
	}
	for(i = 0; i < erptr; i++)
		switch(erlist[i].type) {
		default:
			pmesg("%s %d %d\n", errnm(erlist[i].type),
				erlist[i].a, erlist[i].b);
			continue;
		case Efakeroot:
			pmesg("own parent %s\n", prino(erlist[i].a));
			continue;
		case Edup:
			j = erlist[i].b;
			k = bmap[erlist[i].a].ino;
			pmesg("dup block %d %s %s\n", erlist[i].a, prino(j), prino(k));
			continue;
		case Elinkcnt:
			j = erlist[i].a;
			dp = (struct dinode *)(buf + 2*bsize + (j-1)*sizeof(*dp));
			pmesg("Elinkcnt(%s) refs %d(parent %s)\n", prino(j), imap[j].nrefs,
				prino(imap[j].parent));
			continue;
		case Ebadparent:
			j = erlist[i].a;
			k = erlist[i].b;
			pmesg("Ebadparent: (%s) not in (%s)\n", prino(j), prino(k));
			continue;
		case Ebadname:
			j = erlist[i].a;
			k = erlist[i].b;
			pmesg("Ebadname (%s) (in %s)\n", prino(j), prino(k));
			continue;
		}
}

fixfree()
{	int i, j, *p;
	sblk.s_ninode = sblk.s_tfree = 0;
	sblk.s_tinode = tinode;
	for(i = 2, j = 0; i < ninode && j < NICINOD; i++)
		if(imap[i].type == Unalloc) {
			sblk.s_inode[j++] = i;
			sblk.s_ninode++;
		}
	sblk.s_lasti = sblk.s_inode[0];
	sblk.s_nbehind = 0;
	if(bsize < 4096) {	/* stupid test, FIX */
		fix1free();
		return;
	}
	if(pblk > lblk) {
		for(i = 0; i < BITMAP; i++)
			sblk.U.B.S_bfree[i] = 0;
		memset(freeb, 0, (pblk-lblk)*bsize);
		for(j = i = 0, p = (long *) freeb; i < pblk; i++, j++) {
			if(j >= 32) {
				j = 0;
				p++;
			}
			if(bmap[i].type == Free || bmap[i].type == Unk) {
				sblk.s_tfree++;
				*p |= (1 << j);
			}
		}
		if(bwrite(lblk, freeb, pblk-lblk)) {
			exitcode = 1;
			pmesg("bit map block write failed\n");
		}
	}
	else {
		for(i = 0, p = sblk.U.B.S_bfree; i < BITMAP; i++)
			*p++ = 0;
		for(i = fblk, j = 0, p = sblk.U.B.S_bfree; i < pblk; i++, j++) {
			if(j >= 32) {
				j = 0;
				p++;
			}
			if(bmap[i].type == Free || bmap[i].type == Unk) {
				sblk.s_tfree++;
				*p |= (1 << j);
			}
		}
	}
	sblk.U.B.S_valid = 1;
	if(!flags['w'])
		return;
	if(bwrite(1, (char *)&sblk, 1))
		fatal("super block write failed\n");
}

fix1free()
{	/* fix this someday FIX */
	pmesg("get fsck to fix the free list!\n");
	if(!flags['w'])
		return;
	if(bwrite(1, (char *)&sblk, 1))
		fatal("super block write failed\n");
}

fixweird(n)
{	struct dinode *dp;
	dp = (struct dinode *) (buf + (n-1)*sizeof(*dp) + 2*bsize);
	if(dp->di_mode)
		return(0);
	if(dp->di_size > 0)
		return(0);
	if(imap[n].nrefs > 1)
		return(0);
	pmesg("removing weird ino %s\n", prino(n));
	if(imap[n].nrefs == 1)
		expunge(n);
	else {
		clri(n);
		pmesg("and rerun chuck\n");
	}
	return(1);
}
	
fixlinks(n)
{	struct dinode *dp;
	dp = (struct dinode *) (buf + (n-1)*sizeof(*dp) + 2*bsize);
	if(imap[n].type == Unalloc) {
		pmesg("ino %d unallocated, link count unadjusted\n", n);
		return;
	}
	pmesg("%s getting %d as links\n", prino(n), imap[n].nrefs);
	dp->di_nlink = imap[n].nrefs;
	if(flags['w'])
		wrti(n);
}

wrti(n)	/* write the block of inodes containing inode n */
{	int i;
	i = (n-1)/inopb + 2;
	if(bwrite(i, buf + i*bsize, 1))
		pmesg("write of block containing ino %d failed\n", n);
}

clri(n)
{	struct dinode *dp;
	static struct dinode nild;
	dp = (struct dinode *) (buf + (n-1)*sizeof(*dp) + 2*bsize);
	switch(imap[n].type) {
	case Dir: case Reg: case Lnk:
		tossblocks(n);
	}
	*dp = nild;
	imap[n].type = Unalloc;
	tinode++;
	if(flags['w'])
		wrti(n);
}

tossblocks(n)
{	struct dinode *dp;
	int addr[NADDR], i;
	dp = (struct dinode *) (buf + (n-1)*sizeof(*dp) + 2*bsize);
	l3tol(addr, dp->di_addr, NADDR);
	for(i = 1; i < NADDR-3; i++)
		if(addr[i]) {
			bmap[addr[i]].type = Free;
			tfree++;
		}
	if(addr[10])
		freeind(addr[10]);
	if(addr[11])
		freedbl(addr[10]);
	if(addr[12])
		pmesg("ino %d, triply indirect block?  rerun chuck to fix free list\n",
			n);
}

freeind(n)
{	int i, *p;
	bmap[n].type = Free;
	if(bread(n, buf, 1))
		fatal("freeind, could't read block %d\n", n);
	for(i = 0, p = (int *)buf; i < bsize/sizeof(int); i++, p++)
		if(*p) {
			bmap[*p].type = Free;
			tfree++;
		}
}

freedbl(n)
{	int i, *p;
	bmap[n].type = Free;
	if(bread(n, buf+bsize, 1))
		fatal("freeind2, couldn't read %d\n", n);
	for(i = 0, p = (int *)(buf+bsize); i < bsize/sizeof(int); i++, p++)
		if(*p)
			freeind(n);
}

interact()
{	int i, j;
	struct dinode *dp;
	for(i = 0; i < erptr; i++)
	switch(erlist[i].type) {
	default:
		pmesg("%s not yet doable\n", errnm(erlist[i].type));
		continue;
	case Enotdot:
		pmesg("Enotdot (dot not self) %s\n", prino(erlist[i].a));
		continue;
	case Edotino:
		pmesg("Edotino (dot illegal) %s\n", prino(erlist[i].a));
		continue;
	case Enotdotdot:
		pmesg("Enotdotdot (dotdot illegal) %s\n", prino(erlist[i].a));
		continue;
	case Ebadparent:
		pmesg("Ebadparent (dotdot not parent) %s\n", prino(erlist[i].a));
		continue;
	case Elinkcnt:
		j = erlist[i].a;
		dp = (struct dinode *) (buf + 2*bsize + (j-1)*sizeof(*dp));
		pmesg("Elinkcnt, %s, %d links %d refs\n", prino(j), dp->di_nlink,
			imap[j].nrefs);
		if(imap[j].nrefs > 0 || imap[j].type == Weird) 
			switch(qry("clri, links-changed-to-refs, skip:\n")) {
			case 's':
				continue;
			case 'c':
				pmesg("clearing ino %d\n", j);
				clri(j);
				continue;
			case 'l':
				pmesg("fixing links ino %d\n", j);
				fixlinks(j);
				continue;
			}
		switch(qry("attach to /lost+found, expunge:\n")) {
		case 'a':
			attach(j);
			continue;
		case 'e':
			expunge(j);
			continue;
		}
		continue;
	case Enullable:
		j = erlist[i].a;
		pmesg("Enullable ino %s\n", prino(j));
		if(qry("clri, or skip\n") == 'c') {
			pmesg("clearing ino %d\n", j);
			clri(j);
		}
		continue;
	case Einvalid:
		if(qry("valid flag not set, set it?\n") == 'y') {
			sblk.U.B.S_valid = 1;
			if(bwrite(1, (char *) &sblk, 1)) 
				pmesg("couldn't rewrite super block\n");
		}
		continue;
	}
	if(qry("fix the super block?\n") == 'y') {
		pmesg("fixing the superblock\n");
		fixfree();
	}
}
