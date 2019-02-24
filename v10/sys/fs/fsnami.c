/*
 * namei code for fs filesystem
 */

#include "sys/param.h"
#include "sys/systm.h"
#include "sys/inode.h"
#include "sys/buf.h"
#include "sys/filsys.h"
#include "sys/dir.h"
#include "sys/user.h"

struct dent {
	struct direct dir;
	off_t	off;
	int	nentry;
};

fsnami(p, flagp, follow)
struct nx *p;
register struct argnamei *flagp;
{
	register struct inode *dp;
	register char *cp;
	register struct buf *bp;
	register struct inode *dip;
	register int i;
	struct dent dent;
	struct inode *domkfile();
	ino_t dsearch();

	cp = p->cp;
	dp = p->dp;

	/*
	 * dp is the inode to search; cp is the pathname to find.
	 * dp must be a directory, with execute permissions.
	 * (this test is done too early; it forbids a filesystem
	 * consisting of a single file with i-number ROOTINO)
	 */
dirloop:
	if((dp->i_mode&IFMT) != IFDIR) {
		u.u_error = ENOTDIR;
		goto outnull;
	}
	if (access(dp, IEXEC))
		goto outnull;
	for (i=0; *cp!='\0' && *cp!='/'; i++) {
		if (i >= DIRSIZ) {
			u.u_error = ENOENT;
			goto outnull;
		}
		dent.dir.d_name[i] = *cp++;
	}
	while (i < DIRSIZ)
		dent.dir.d_name[i++] = '\0';
	while (*cp == '/')
		cp++;
	if (dent.dir.d_name[0] == '\0') {	/* null name, e.g. "/" or "" */
		if (flagp->flag != NI_SEARCH) {
			u.u_error = ENOENT;
			goto outnull;
		}
		/*
		 * should pass NI_CREAT too, but dp is known to be a directory
		 */
		goto out;
	}
	u.u_segflg = SEGSYS;	/* for various routines that write directories */
	if (dsearch(dp, &dent) == 0) {
		if (u.u_error)
			goto outnull;
		/*
		 * name not found
		 * if we wanted to create or link, that's OK
		 */
		if (*cp == 0) switch (flagp->flag) {
		case NI_CREAT:
		case NI_NXCREAT:
		case NI_MKDIR:
		case NI_LINK:
			dip = dp;
			dp = domkfile(dip, flagp, &dent);
			iput(dip);
			goto out;
		}
		u.u_error = ENOENT;
		goto outnull;
	}
	/*
	 * name found
	 * did we want to remove it?
	 * (do before ".." check)
	 */
	if(flagp->flag == NI_DEL && *cp == 0) {
		dormfile(dp, &dent);
		goto outnull;
	}
	if(flagp->flag == NI_RMDIR && *cp == 0) {
		dormdir(dp, &dent);
		goto outnull;
	}
	/*
	 * special code for ".." in a root directory
	 */
	if (dent.dir.d_name[0] == '.'
	&&  dent.dir.d_name[1] == '.'
	&&  dent.dir.d_name[2] == '\0') {
		if (dp == rootdir || dp == u.u_rdir)
			goto dirloop;
		if (dent.dir.d_ino == ROOTINO && dp->i_number == ROOTINO) {
			dip = dp->i_mpoint;
			i = dp->i_fstyp;
			iput(dp);
			dp = dip;
			plock(dp);
			dp->i_count++;
			while (*--cp == '/')
				;
			cp--;		/* skip over ".." and / */
			if (dp->i_fstyp != i)
				goto outmore;
			goto dirloop;
		}
	}
	/*
	 * fetch the inode for the filename we found
	 */
	dip = dp;
	prele(dip);
	dp = iget(dip, dip->i_dev, dent.dir.d_ino);
	if (dp == NULL) {
		idec(dip);
		goto out;	/* sic */
	}
	if (dp->i_count == 1 && fsiread(dip, dp) < 0) {
		idec(dip);
		dp = NULL;	/* fsiread iput it */
		goto out;
	}
	if(dip->i_fstyp != dp->i_fstyp) {
		idec(dip);
		goto outmore;
	}
	/*
	 * symlink?  If so and if wanted, follow.
	 */
	if ((dp->i_mode&IFMT)==IFLNK && (follow || *cp!='\0')) {
		char *ocp;

		ocp = cp;
		while (*cp++)
			;
		i = cp - ocp;		/* strlen(ocp) + 1 for NUL */
		if (dp->i_size > BSIZE(dp->i_dev)
		||  dp->i_size + 1 + i > p->nlen	/* +1 for '/' */
		||  ++p->nlink>8) {
			u.u_error = ELOOP;
			idec(dip);
			goto outnull;
		}
		cp = p->nbuf;
		if (i == 1)	/* empty pathname, just put NUL there */
			cp[dp->i_size] = 0;
		else {
			bcopy(ocp, cp + dp->i_size + 1, i);	/* remaining pathname */
			cp[dp->i_size] = '/';
		}
		bp = bread(dp->i_dev, bmap(dp, (daddr_t)0, B_READ));
		if (bp->b_flags & B_ERROR) {
			brelse(bp);
			idec(dip);
			goto outnull;
		}
		bcopy(bp->b_un.b_addr, cp, dp->i_size);	/* new prefix */
		brelse(bp);
		i = dp->i_fstyp;
		iput(dp);
		if (*cp != '/')
			dp = dip;
		else {
			idec(dip);
			while (*cp == '/')
				cp++;
			if ((dp = u.u_rdir) == NULL)
				dp = rootdir;
			plock(dp);
			dp->i_count++;
		}
		if (i != dp->i_fstyp)
			goto outmore;
		goto dirloop;
	}
	idec(dip);
	/*
	 * more pathname to walk?
	 */
	if (*cp)
		goto dirloop;
	/*
	 * final checks before returning:
	 * -- some operations are in error if filename exists
	 * -- save last piece of filename for accounting
	 */
	if (flagp->flag == NI_LINK || flagp->flag == NI_NXCREAT
	||  flagp->flag == NI_MKDIR) {
		u.u_error = EEXIST;
		goto outnull;
	}
	if(flagp->flag == NI_SEARCH && flagp->un.buf)
		bcopy(dent.dir.d_name, flagp->un.buf,
			MIN(flagp->len, sizeof(dent.dir.d_name)));

	/*
	 * here to return the inode in dp
	 */
out:
	p->dp = dp;
	return(0);

	/*
	 * here to discard dp and return NULL
	 */
outnull:
	iput(dp);
	p->dp = NULL;
	return (0);

	/*
	 * here if we crossed into another filesystem's domain:
	 * return dp (the first inode in that filesystem)
	 * and cp (the remainder of the pathname)
	 * and value 1, so namei knows it should continue
	 */
outmore:
	p->cp = cp;
	p->dp = dp;
	return (1);
}

/*
 * create a new file of some sort in directory dp
 *   name is in dent->d_name
 *   offset is in dent->d_off
 *   return inode if it is needed
 */
struct inode *
domkfile(dp, flagp, dent)
register struct inode *dp;
struct argnamei *flagp;
struct dent *dent;
{
	register struct inode *dip;
	struct direct x[2];
	register i;

	if(access(dp, IWRITE))
		return(NULL);
	if (dp->i_nlink == 0) {
		u.u_error = EINVAL;
		return (NULL);
	}
	u.u_count = sizeof(struct direct);
	u.u_base = (caddr_t)&dent->dir;
	u.u_offset = ltoL(dent->off);
	switch(flagp->flag) {

	case NI_CREAT:	/* create a new file */
	case NI_NXCREAT:
		dip = ialloc(dp);
		if(dip == NULL)
			return(NULL);
		dip->i_flag |= IACC|IUPD|ICHG;
		dip->i_mode = flagp->un.mode;
		if((dip->i_mode & IFMT) == 0)
			dip->i_mode |= IFREG;
		dip->i_nlink = 1;
		dip->i_uid = u.u_uid;
		dip->i_gid = dp->i_mode & ISGID ? u.u_gid : dp->i_gid;
		if (u.u_uid && !groupmember(dip->i_gid))
			dip->i_mode &= ~ISGID;
		fsiupdat(dip, &time, &time, 1);
		dent->dir.d_ino = dip->i_number;
		flagp->un.mode = ~flagp->un.mode;
		writei(dp);
		return(dip);

	case NI_MKDIR:	/* make a new directory */
		dip = ialloc(dp);
		if(dip == NULL)
			return(NULL);
		dent->dir.d_ino = dip->i_number;
		dip->i_mode = flagp->un.mode;
		dip->i_nlink = 1;
		dip->i_uid = u.u_uid;
		dip->i_gid = dp->i_gid;
		dip->i_flag |= IACC|IUPD|ICHG;
		x[0].d_ino = dip->i_number;
		x[1].d_ino = dp->i_number;
		for(i = 0; i < DIRSIZ; i++)
			x[0].d_name[i] = x[1].d_name[i] = 0;
		x[0].d_name[0] = x[1].d_name[0] = x[1].d_name[1] = '.';
		u.u_count = sizeof(x);
		u.u_base = (caddr_t)x;
		u.u_offset = ltoL(0);
		u.u_segflg = SEGSYS;
		writei(dip);
		if (u.u_error) {
			dip->i_nlink--;
			iput(dip);
			return(NULL);
		}
		dip->i_nlink++;
		dp->i_nlink++;
		fsiupdat(dip, &time, &time, 1);
		u.u_count = sizeof(struct direct);
		u.u_base = (caddr_t)&dent->dir;
		u.u_offset = ltoL(dent->off);
		writei(dp);
		iput(dip);
		return(NULL);

	case NI_LINK:	/* make a link */
		if(dp->i_dev != flagp->un.il->i_dev) {
			u.u_error = EXDEV;
			return(NULL);
		}
		dent->dir.d_ino = flagp->un.il->i_number;
		writei(dp);
		return(NULL);
	}
	panic("domkfile");
}

/*
 * delete a non-directory file
 */
dormfile(dp, dent)
register struct inode *dp;
register struct dent *dent;
{
	register struct inode *dip;

	if(access(dp, IWRITE))
		return;
	if(dp->i_number == dent->dir.d_ino) {	/* for '.' */
		dip = dp;
		dp->i_count++;
	} else {
		dip = iget(dp, dp->i_dev, dent->dir.d_ino);
		if (dip == NULL)
			return;
		if (dip->i_count == 1 && fsiread(dp, dip) < 0)
			return;
	}
	if(dip->i_mpoint != dp->i_mpoint) {
		u.u_error = EBUSY;
		iput(dip);
		return;
	}
	if((dip->i_mode&IFMT) == IFDIR && !suser()) {
		iput(dip);
		return;
	}
	if(dip->i_flag&ITEXT)
		xrele(dip);		/* free busy text */
	u.u_base = (caddr_t)&dent->dir;
	u.u_count = sizeof(struct direct);
	u.u_offset = ltoL(dent->off);
	dent->dir.d_ino = 0;
	writei(dp);		/* segflg already set*/
	dip->i_nlink--;
	dip->i_flag |= ICHG;
	iput(dip);
}

/*
 * remove a directory (fsnami)
 * dp is inode of containing directory
 * dent->dir is the entry (which exists)
 * u.u_offset is the offset in containing directory of this entry
 */
dormdir(dp, dent)
register struct inode *dp;
register struct dent *dent;
{
	register struct inode *dip;
	register int nentry;
	struct dent tdent;
	ino_t dotino, dotdotino;
	ino_t dsearch();

	if(access(dp, IWRITE))
		return;
	if(dp->i_number == dent->dir.d_ino
	 || strncmp(dent->dir.d_name, "..", DIRSIZ)==0) { /* gets "." and "" */
		u.u_error = EINVAL;
		return;
	}
	if((dip = iget(dp, dp->i_dev, dent->dir.d_ino)) == NULL)
		return;
	if(dip->i_number == ROOTINO) {
		u.u_error = EINVAL;
		iput(dip);
		return;
	}
	if(dip->i_mpoint != dp->i_mpoint) {
		u.u_error = EBUSY;
		iput(dip);
		return;
	}
	if (dip->i_count == 1 && fsiread(dp, dip) < 0)
		return;
	if((dip->i_mode & IFMT) != IFDIR) {
		u.u_error = ENOTDIR;
		iput(dip);
		return;
	}
	/* search for ., .., other entries in dir */
	cpdirent("/", &tdent);		/* cannot be found; just count entries */
	dsearch(dip, &tdent);
	nentry = tdent.nentry;
	tdent.dir.d_name[0] = '.';
	if (dotino = dsearch(dip, &tdent))
		nentry--;
	tdent.dir.d_name[1] = '.';
	if(dotdotino = dsearch(dip, &tdent))
		nentry--;
	if (nentry > 0) {
		u.u_error = EHASF;	/* removing nonempty directory */
		iput(dip);
		return;
	}
	if (dotino) {
		if (dip->i_number == dotino)
			dip->i_nlink--;
		/* else error? */
	}
	if (dotdotino) {
		if (dp->i_number == dotdotino)
			dp->i_nlink--;
		/* else error? */
	}
	u.u_base = (caddr_t)&dent->dir;
	u.u_count = sizeof(struct direct);
	u.u_offset = ltoL(dent->off);
	dent->dir.d_ino = 0;
	writei(dp);
	dp->i_flag |= ICHG;
	fsiupdat(dp, &time, &time, 1);
	dip->i_nlink--;
	dip->i_flag |= ICHG;
	iput(dip);
}

cpdirent(s, dent)
register char *s;
register struct dent *dent;
{
	register char *dp = dent->dir.d_name;

	while (dp < &dent->dir.d_name[DIRSIZ]) {
		*dp++ = *s;
		if (*s)
			s++;
	}
}

/*
 * search directory ip for entry dent->dir.d_name
 *  success: return ino, leave dent->dir with copy of entry,
 *           dent->off pointing at entry
 *	     dent->nentry with count of entries
 *  fail:    return 0, leave dent->off pointing at empty slot
 */
ino_t
dsearch(ip, dent)
struct inode *ip;
struct dent *dent;
{
	register struct direct *dp, *dpe;
	register char *nm;
	register off_t off;
	struct buf *bp;
	register int bsize;
	register daddr_t n, nblock;

	dent->dir.d_ino = 0;
	nm = dent->dir.d_name;
	bsize = BSIZE(ip->i_dev);
	dent->nentry = 0;
	bp = NULL;
	dent->off = -1;
	nblock = (ip->i_size+bsize-1) / bsize;

	for (n=0, off=0; n<nblock; n++) {
		if (bp)
			brelse(bp);
		bp = bread(ip->i_dev, bmap(ip, n, B_READ));
		if (bp->b_flags & B_ERROR) {
			u.u_error = EIO;
			goto out;
		}
		dp = (struct direct *)bp->b_un.b_addr;
		dpe = &dp[min(bsize, ip->i_size-off) / sizeof(struct direct)];
		for (; dp < dpe; dp++, off+=sizeof(struct direct)) {
			if (dp->d_ino == 0) {
				if (dent->off<0)
					dent->off = off;
				continue;
			}
			dent->nentry++;
			if (nm[2]==dp->d_name[2]	/* hash */
			 && strncmp(nm, dp->d_name, DIRSIZ) == 0) {
				dent->off = off;
				dent->dir.d_ino = dp->d_ino;
				goto out;
			}
		}
	}
out:
	if (dent->off<0)
		dent->off = off;
	if (bp)
		brelse(bp);
	return(dent->dir.d_ino);
}

