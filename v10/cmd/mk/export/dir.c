#include	<sys/types.h>
#include	<sys/stat.h>
#ifndef SEQUENT
#include	<ndir.h>
#else
#include	<dir.h>
#endif

#ifdef	SEQUENT
#define		DIRSIZE	MAXNAMELEN
#else
#define		DIRSIZE	14
#endif
#ifndef	MAXNAMELEN
#define	MAXNAMELEN	255
#endif
#ifndef DIRSIZ
#define DIRSIZ(dp) \
    ((sizeof(struct direct) - MAXNAMLEN + (dp)->d_namlen + sizeof(ino_t) - 1) &\
    ~(sizeof(ino_t) - 1))
#endif

extern char *strncpy(), *malloc();

DIR *
opendir(name)
register char *name;
{
	DIR dirbuf, *dirp;
	struct stat statb;
	char buf[MAXNAMELEN+1];
	register char *s;

	strncpy(buf, name, MAXNAMELEN);
	buf[MAXNAMELEN-1] = 0;
	if ((dirbuf.dd_fd = open(buf, 0)) < 0)
		return((DIR *)0);
	if (fstat(dirbuf.dd_fd, &statb)!=0 || (statb.st_mode & S_IFMT)!=S_IFDIR){
		close(dirbuf.dd_fd);
		return((DIR *)0);
	}
	dirbuf.dd_loc = 0;
	dirp = (DIR *)malloc(sizeof(DIR));
	*dirp = dirbuf;
	return(dirp);
}

void
closedir(dirp)
DIR *dirp;
{
	close(dirp->dd_fd);
	free((char *)dirp);
}

/*
 * read an old stlye directory entry and present it as a new one
 */
#define	ODIRSIZ	14

struct	olddirect {
	ino_t	d_ino;
	char	d_name[ODIRSIZ];
#ifdef	CRAY
	char	d_pad[10];
#endif
};

/*
 * get next entry in a directory.
 */
struct direct *
readdir(dirp)
	register DIR *dirp;
{
	register struct olddirect *dp;
	static struct direct dir;

	for (;;) {
		if (dirp->dd_loc == 0) {
			dirp->dd_size = read(dirp->dd_fd, dirp->dd_buf, 
			    DIRBLKSIZ);
			if (dirp->dd_size <= 0)
				return((struct direct *)0);
		}
		if (dirp->dd_loc >= dirp->dd_size) {
			dirp->dd_loc = 0;
			continue;
		}
		dp = (struct olddirect *)(dirp->dd_buf + dirp->dd_loc);
		dirp->dd_loc += sizeof(struct olddirect);
		if (dp->d_ino == 0)
			continue;
		dir.d_ino = dp->d_ino;
		strncpy(dir.d_name, dp->d_name, ODIRSIZ);
		dir.d_name[ODIRSIZ] = '\0'; /* insure null termination */
		dir.d_namlen = strlen(dir.d_name);
		dir.d_reclen = DIRSIZ(&dir);
		return (&dir);
	}
}
