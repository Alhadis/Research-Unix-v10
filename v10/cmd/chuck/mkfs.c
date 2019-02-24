#include "fs.h"

/* always bit-mapped.  only question is how many blocks of inodes */
mknew()
{	int n;
	if(pblk < 4)
		fatal("%d blocks is too few\n", pblk);
	fd = open(file, 2);
	inopb = bsize/sizeof(struct dinode);
	fblk = (pblk-2)/(1+inopb);
	ninode = (fblk-2)*inopb;
	if(ninode > 65536) {	/* temporary, I hope FIX */
		fblk = 65536/inopb + 2;
	}
	else if(ninode <= 0)
		fblk = 3;
	ninode = (fblk-2)*inopb;
	if(fblk + BITMAP*8*sizeof(long) < pblk)
		lblk = pblk - (pblk + 8*bsize - 1)/(8*bsize);
	else
		lblk = pblk;
	pmesg("%d blocks, %d inodes, fblk %d, lblk %d (%d data blocks)\n",
		pblk, ninode, fblk, lblk, lblk-fblk);
	/* we could give the sucker a chance to adjust this */
	if(qry("is this ok? (yq)\n") != 'y')
		fatal("too bad, quitting\n");
	n = pblk - lblk + fblk;	/* inodes plus bit maps */
	buf = (char *) malloc(bsize * n);
	memset(buf, 0, bsize * n);
	newsuper();
	sblk.U.B.S_valid = 1;
	newroot();
	newbits();
	writeem();
	exit(0);
}

newsuper()
{	int i;
	long *p;
	sblk.s_isize = fblk;
	sblk.s_fsize = pblk;
	sblk.s_tinode = ninode - 1;
	for(i = 0; i < NICINOD; i++) {
		if(i+3 >= ninode)	/* maybe >? */
			break;
		sblk.s_inode[i] = i+3;
	}
	sblk.s_ninode = i;
	sblk.s_time = time(0);
	sblk.s_tfree = lblk-fblk-1;
	if(pblk > lblk)
		return;
	p = sblk.U.B.S_bfree;
	for(i = 0; i < (lblk-fblk)/(8*sizeof(long)); i++)
		p[i] = ~0;
	/* and the (possible) last bunch */
	for(i = i*8*sizeof(long) + fblk; i < lblk; i++)
		setfree(p, i-fblk);
	/* fblk is used by the root */
	*p &= ~1;
}

setfree(p, n)
long *p;
{	int j, k;
	j = n/(8*sizeof(int));
	k = n%(8*sizeof(int));
	p[j] |= (1 << k);
}

/* the root goes in inode 2, and in block fblk */
newroot()
{	struct dinode *p = (struct dinode *) (buf + 2*bsize);
	struct direct *d = (struct direct *) (buf + bsize);
	p++;	/* root is inode 2 */
	p->di_mode = IFDIR | (~umask(0) & 0777);	/* sets umask */
	p->di_nlink = 2;
	p->di_uid = getuid();
	p->di_gid = getgid();
	p->di_size = 32;	/* temporary, I hope FIX */
	p->di_atime = p->di_mtime = p->di_ctime = time(0);
	ltol3(p->di_addr, &fblk, 1);
	d->d_ino = 2;
	strcpy(d->d_name, ".");
	d++;
	d->d_ino = 2;
	strcpy(d->d_name, "..");
}

newbits()
{	int i;
	long *p;
	if(pblk <= lblk)
		return;
	p = (long *)(buf + fblk*bsize);
	for(i = fblk+1; i % (8*sizeof(int)); i++)
		setfree(p, i);
	for(; i+8*sizeof(int) < lblk; i += 8*sizeof(int))
		p[i/(8*sizeof(int))] = ~0;
	for(; i < lblk; i++)
		setfree(p, i);
/*	sblk.U.N.S_flag = 1;	/* bit map blocks */
/*	sblk.U.N.S_bsize = 4096*8;*/
}
writeem()
{
	fd = open(file, 2);
	if(fd < 0)
		fatal("can't write\n");
	if(bwrite(pblk-1, buf, 1))
		fatal("can't write last block (%d)\n", pblk-1);
	if(pblk > lblk)
		if(bwrite(lblk, buf+fblk*bsize, pblk-lblk))
			fatal("couldn't write bit maps\n");
	if(bwrite(fblk, buf+bsize, 1))
		fatal("couldn't write root dir\n");
	if(bwrite(2, buf+2*bsize, fblk-2))
		fatal("couldn't write inodes\n");
	if(bwrite(1, (char *)&sblk, 1))
		fatal("couldn't write superblock\n");
}
