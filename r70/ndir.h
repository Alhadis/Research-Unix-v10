/* Copyright (c) 1982 Regents of the University of California */
/* and modified by pjw in 1986 */

/*
 * this must be a power of 2 and a multiple of all the ones in the system
 */
#define DIRBLKSIZ 512

/*
 * This limits the directory name length. Its main constraint
 * is that it appears twice in the user structure. (u. area) in bsd systems
 */
#define MAXNAMLEN 255

struct	direct {
	unsigned long	d_ino;
	short	d_reclen;
	short	d_namlen;
	char	d_name[MAXNAMLEN + 1];
	/* typically shorter */
};

struct _dirdesc {
	int	dd_fd;
	long	dd_loc;		/* where we left off in dd_buf */
	long	dd_size;	/* bytes back from system */
	long	dd_offset;	/* lseek at beginning of dd_buf */
	char	dd_buf[DIRBLKSIZ];
};

/*
 * useful macros.
 */
#define NDIRSIZ(dp) \
    ((sizeof(struct direct) - MAXNAMLEN + (dp)->d_namlen + sizeof(ino_t) - 1) &\
    ~(sizeof(ino_t) - 1))
typedef	struct _dirdesc DIR;
#ifndef	NULL
#define	NULL	0
#endif

/*
 * functions defined on directories
 */
extern DIR *opendir();
extern struct direct *readdir();
extern long telldir();
extern void seekdir();
#define rewinddir(dirp)	seekdir((dirp), 0)
extern void closedir();
