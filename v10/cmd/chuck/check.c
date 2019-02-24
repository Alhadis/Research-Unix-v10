#include "fs.h"
#include "sys/fblk.h"
enum {Byte=8};	/* number of bits per byte */
int btypes[NADDR] = {Data, Data, Data, Data, Data, Data, Data, Data, Data, Data,
		Ind, Ind2, Ind3};
int dtypes[NADDR] = {First, Other, Other, Other, Other, Other, Other, Other,
		Other, Other, Ind, Ind2, Ind3};

firstsuper()
{	int i;
	if(bread(1, (char *)&sblk, 1)) {
		perror("superblock");
		fatal("couldn't read superblock of %s\n", file);
	}
	inopb = bsize / sizeof(struct dinode);
	ninode = (sblk.s_isize-2) * inopb + 1;
	fblk = sblk.s_isize;
	pblk = sblk.s_fsize;
	if(bsize >= 4096 && BITMAP*Byte*sizeof(long)+fblk < pblk) {
		i = (pblk + (bsize*Byte-1))/(bsize*Byte);
		lblk = pblk - i;
	}
	else
		lblk = pblk;
	if(flags['v'])
		pmesg("ninode %d isize %d lblk %d pblk %d\n", ninode, fblk, lblk,
			pblk);
	if(ninode < 2 || lblk <= fblk || pblk < lblk)
		fatal("absurd(%d inodes, lblk=%d, pblk=%d)\n", ninode, fblk, pblk);
	/* allocate all the space */
	buf = (char *) malloc(buflen = fblk * bsize);
	if(!buf)	/* we've been too greedy, cache this stuff FIX*/
		fatal("couldn't alloc %d bytes for buffers\n", buflen);
	imap = (im *) malloc(ninode*sizeof(im));
	bmap = (bm *) malloc(pblk*sizeof(bm));
	dmap = (dm *) malloc((dmaplen = 1000)*sizeof(dm));
	if(!imap || !bmap || !dmap)
		fatal("couldn't alloc imap, bmap, or dmap\n");
	memset((char *)imap, 0, ninode*sizeof(im));
	memset((char *)bmap, 0, pblk*sizeof(bm));
	if(flags['v'])
		pmesg("alloc: %d for bufs, %d for imap, %d for bmap\n", buflen,
			ninode*sizeof(im), pblk*sizeof(bm));
	timer("alloc");
	if(bread(0, buf, fblk))	/* fragile, FIX */
		fatal("couldn't read inodes\n");
	bmap[0].type = Boot;
	bmap[1].type = Sblock;
	for(i = 2; i < fblk; i++)
		bmap[i].type = Inode;
	/* and, if necessary, the extra bitmaps */
	if(pblk > lblk) {
		freeb = (char *) malloc(bsize * (pblk-lblk));
		if(!freeb || bread(lblk, freeb, pblk-lblk))
			fatal("couldn't read bitmap blocks\n");
		for(i = lblk; i < pblk; i++)
			bmap[i].type = Bits;
	}
}

scaninodes()
{	register int ino, *lp, *p;
	register struct dinode *dp;
	register im *ip;
	register bm *bp;
	int addr[NADDR], *ty, last;
	ino = 0;
	ip = imap+1;
	dp = (struct dinode *) (buf+2*bsize);
	for(ino = 1; ino < ninode; ino++, dp++, ip++) {
		switch(dp->di_mode & IFMT) {
		default:
			pmesg("ino %d weird mode 0%o\n", ino, dp->di_mode);
			ip->type = Weird;
			continue;
		case IFDIR:
			ip->type = Dir;
			ty = dtypes;
			break;
		case IFCHR:
			ip->type = Chr;
			continue;
		case IFBLK:
			ip->type = Blk;
			continue;
		case IFREG:
			ip->type = Reg;
			ty = btypes;
			break;
		case IFLNK:
			ty = btypes;
			ip->type = Lnk;
			if(dp->di_size > bsize || dp->di_size == 0)
				inoerr(Elinksize, ino);
			break;
		case 0:
			if(!dp->di_mode && !dp->di_size && !dp->di_nlink)
				ip->type = Unalloc;
			else
				ip->type = Weird;
			continue;
		}
		/* now look through the blocks in the inode */
		l3tol(addr, dp->di_addr, NADDR);
		p = addr;
		lp = addr + NADDR;
		for(last = -1; p < lp; p++) {
			if(*p == 0)
				continue;
			if(*p < fblk || *p >= lblk) {
				inoerr(Ebadaddr, ino, *p);
				continue;
			}
			bp = bmap + *p;
			if(bp->ino)
				dupblock(bp, bp->type, 0);
			bp->ino = ino;
			bp->type = ty[p-addr];
			blkcnts[bp->type]++;
			last = p-addr;
			switch(last) {
			case 10:	/* indirect */
				bp->bnum = NADDR-3;
				continue;
			case 11:
				bp->bnum = bsize/sizeof(long) + NADDR-3;
				continue;
			/* hard to tell if triple blocks are an error, so... */
			case 12:
				if(bsize >= 4096)
					bp->type = Other, inoerr(Etriple, ino, *p);
				else
					bp->bnum = (bsize/sizeof(long)) *
						(bsize/sizeof(long) + 1) + NADDR-3;
				continue;
			}
		}
		if(ip->type == Dir && (addr[0] < fblk || addr[0] >= lblk))
			direrror(Efirst, ino);
		ip->last = last;
	}
}

indblocks(old, new)
register old;
{	register int *p, *ep;
	register bm *bp, *obp, *mx = bmap + lblk;
	int nty, delta, last;

	switch(old) {
	default:
		delta = 1;
		break;
	case Ind2:
		delta = bsize/sizeof(long);
		break;
	case Ind3:
		delta = bsize/sizeof(long) * bsize/sizeof(long);
		break;
	}
	ep = (int *)(buf+bsize);
	for(obp = bmap+1; obp < mx; obp++) {
		if(obp->type != old)
			continue;
		if(bread(obp-bmap, buf, 1)) {
			pmesg("ino %d block %d couldn't read\n", obp->ino,
				obp-bmap);
			obp->type = Ioerr;
			inoerr(Ebadread, obp->ino, obp-bmap);
			continue;
		}
		nty = obp->type == Dir && new == Data? Other: new;
		for(last = -1, p = (long *)buf; p < ep; p++) {
			if(*p == 0)
				continue;
			if(*p < fblk || *p >= lblk) {
				inoerr(Ebadaddr, bmap[obp-bmap].ino);
				break;
			}
			bp = bmap + *p;
			if(bp->ino)
				dupblock(bp, obp->type, new);
			last = bp->bnum = obp->bnum + delta*(p-(long *)buf);
			bp->ino = obp->ino;
			bp->type = nty;
			blkcnts[nty]++;
		}
		if(last == -1)
			pmesg("ino %d had ind block (%d) all 0\n", obp->ino,
				obp-bmap);
		if(imap[obp->ino].last < last)
			imap[obp->ino].last = last;
	}
}

dochecks()
{	int i, j, k;
	im *ip = imap + ROOTINO;
	struct dinode *dp = (struct dinode *)(buf + 2*bsize);
	for(i = ROOTINO, dp += i-1; i < ninode; i++, ip++, dp++) {
		j = dp->di_nlink;
		if(ip->type != Unalloc && ip->nrefs == 0 && j == 0)
			inoerr(Enullable, i, 0);
		else if(ip->nrefs != j)
			inoerr(Elinkcnt, i, ip->nrefs-j);
		switch(ip->type) {
		case Dir:
			if(dp->di_size < 32)	/* no room for . and .. */
				direrror(Eshortdir, i, dp->di_size);
		case Lnk: case Reg:
			k = ip->last;
			if(dp->di_size == 0 && k == -1)
				continue;
			j = (dp->di_size-1)/bsize;
			if(j == k)
				continue;
			if(j < k)	/* blks after end */
				inoerr(Eshort, i, k);
			if(j > k)	/* ends with holes */
				inoerr(Ehole, i, k);
		}	
	}
	dirtree();
}

checksuper()
{	register int i, cnt, j, k;
	bm *bp;
	int last;
	if(!sblk.U.B.S_valid)
		supererr(Einvalid, 0, 0);
	for(cnt = i = 0; i < NICINOD && i < sblk.s_ninode; i++) {
		j = sblk.s_inode[i];
		if(j == 0)	/* really? */
			continue;
		if(j >= ninode || imap[j].type != Unalloc) {
			supererr(Esuperino, j, 0);
			break;
		}
		cnt++;
	}
	if(i >= NICINOD && cnt != sblk.s_ninode)
		supererr(Esuperino, 0, cnt);
	for(i = 1, cnt = 0; i < ninode; i++)
		if(imap[i].type == Unalloc)
			cnt++;
	tinode = cnt;
	if(cnt != sblk.s_tinode)
		supererr(Esuperino, cnt, sblk.s_tinode);
	if(bsize < 4096) {	/* there should be a file system type FIX */
		ch1free();
		return;
	}
	if(pblk > lblk) {
		chbitmap();
		return;
	}
	cnt = fblk;
	bp = bmap + cnt;
	for(i = 0; i < BITMAP; i++, cnt += 32) {
		k = sblk.U.B.S_bfree[i];
		for(j = 0; j < 32; j++, bp++) {
			if((k & (1<<j)) == 0) {
				continue;
			}
			if(cnt+j >= lblk) {
				supererr(Esuperfree, cnt+j, 0);
				return;
			}
			if(bp->type == Unk) {
				bp->type = Free;
				blkcnts[Free]++;
			}
			else {
				supererr(Esuperfree, cnt+j, bp->type);
				return;
			}
		}
	}
	for(i = 1, bp = bmap + i; i < pblk; i++, bp++)
		if(bp->type == Unk) {
			supererr(Efreelist, i, bp->type);
			return;
		}
	tfree = blkcnts[Free];
	if(tfree != sblk.s_tfree)
		supererr(Efreelist, sblk.s_tfree, tfree);
}

chbitmap()
{	register long *p;
	register int i, k, j;
	register bm *bp = bmap;
	for(i = 0, p = (long *)freeb; i < pblk; i += 32, p++) {
		k = *p;
		for(j = 0; j < 32; j++, bp++) {
			if((k & (1 << j)) == 0)
				continue;
			if(i+j >= lblk) {
				supererr(Esuperfree, i+j, 0);
				continue;
			}
			if(bp->type == Unk) {
				bp->type = Free;
				blkcnts[Free]++;
			}
			else {
				supererr(Esuperfree, i+j, bp->type);
				return;
			}
		}
	}
	for(i = 1, bp = bmap + i; i < pblk; i++, bp++)
		if(bp->type == Unk) {
			supererr(Efreelist, i, bp->type);
			return;
		}
	tfree = blkcnts[Free];
	if(tfree != sblk.s_tfree)
		supererr(Efreelist, sblk.s_tfree, tfree);
}

ch1free()
{	int i, j, k;
	daddr_t *p;
	/* these could be unified if S_nfree were an int, not a short */
	for(i = 0; i < sblk.U.R.S_nfree; i++) {
		j = sblk.U.R.S_free[i];
		if(j >= fblk && j < lblk && !bmap[j].ino) {
			if(bmap[j].type)
				adderr(Edup, j, 0);
			bmap[j].type = Free;
			blkcnts[Free]++;
		}
		else if(j)
			inoerr(Ebadaddr, 0, j);
	}
	if((j = sblk.U.R.S_free[0]) == 0)
		return;
loop:
	if(bread(j, buf, 1)) {
		pmesg("couldn't read free blk %d\n", j);
		bmap[j].type = Ioerr;
		exitcode = 1;
		return;
	}
	k = ((struct fblk *)buf)->df_nfree;
	for(p = ((struct fblk *)buf)->df_free, i = 0; i < k; i++) {
		j = p[i];
		if(j >= fblk && j < lblk && !bmap[j].ino) {
			if(bmap[j].type)
				adderr(Edup, j, 0);
			bmap[j].type = Free;
			blkcnts[Free]++;
		}
		else if(j)
			inoerr(Ebadaddr, 0, j);
	}
	if(j = p[0])
		goto loop;
}

report()
{	int i, cnts[32], j, k;
	for(i = 0; i < iptr; i++) {
		j = iarg[i];
		if(j < 1 || j >= ninode) {
			pmesg("arg ino %d out of range\n", j);
			continue;
		}
		pmesg("arg ino %s\n", prino(j));
		if(imap[j].type == Dir) {
			for(k = 0; k < pblk; k++)
				if(bmap[k].ino == j && bmap[k].type == First)
					pmesg(" (first blk %d)\n", k);
		}
	}
	for(i = 0; i < bptr; i++) {
		j = barg[i];
		if(j < 0 || j > pblk) {
			pmesg("arg blk %d out of range\n", j);
			continue;
		}
		pmesg("arg blk %d(%s) %s\n", j, btype(bmap[j].type),
			prino(bmap[j].ino));
	}
	if(!flags['v'])
		return;
	pmesg("dmapptr %d erptr %d\n", dmapptr, erptr);
	pmesg("\tinodes:\n");
	for(i = 0; i < 32; i++)
		cnts[i] = 0;
	for(i = 1; i < ninode; i++)
		cnts[imap[i].type]++;
	for(i = 0; i < 32; i++)
		if(cnts[i])
			pmesg("%s %d\n", itype(i), cnts[i]);
	pmesg("\tblocks:\n");
	for(i = 0; i < 32; i++)
		if(blkcnts[i])
			pmesg("%s %d\n", btype(i), blkcnts[i]);
	rpterrs();
}
