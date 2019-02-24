/*
 * This sets the "page size" for directories.
 * Requirements are DEV_BSIZE <= DIRBLKSIZ <= MINBSIZE with
 * DIRBLKSIZ a power of two.
 * Dennis Ritchie feels that directory pages should be atomic
 * operations to the disk, so we use DEV_BSIZE.
 */
#define DIRBLKSIZ 512

/*
 * This limits the directory name length. Its main constraint
 * is that it appears twice in the user structure. (u. area)
 */
#define MAXNAMLEN 255

struct	direct {
	u_long	d_ino;
	short	d_reclen;
	short	d_namlen;
	char	d_name[MAXNAMLEN + 1];
	/* typically shorter */
};

struct _dirdesc {
	int	dd_fd;
	long	dd_loc;
	long	dd_size;
	char	dd_buf[DIRBLKSIZ];
};

#undef DIRSIZ
inline int DIRSIZ(direct* dp) {
	return	~(sizeof(ino_t) - 1) &
			  (sizeof(struct direct)
			- MAXNAMLEN
			+ dp->d_namlen
			+ sizeof(ino_t) - 1);
}
typedef	struct _dirdesc DIR;

/*
 * functions defined on directories
 */
extern DIR *opendir(char*);
extern direct *readdir(DIR*);
extern long telldir(DIR*);
extern void seekdir(DIR*,long);
inline void rewinddir(DIR* dirp) { seekdir(dirp, 0); }
extern void closedir(DIR*);
