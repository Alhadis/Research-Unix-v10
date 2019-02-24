/*
 * definitions for the server half of the face server
 */

/*
 * internal file
 * for a regular file,
 *	data is the name of the external file containing the image
 * for a directory,
 *	data is the stuff to return on a read
 *	dir is the real (internal) directory:
 * 	there are nfiles entries;
 *	room for nalloc entries.
 */

typedef struct {
	int ino;		/* i-number for stat */
	long ta, tm, tc;	/* times, for stat */
	long size;		/* file size, for stat and read */
	int nlinks;		/* internal reference count */
	char *data;
	short nalloc;		/* how much space in dir */
	short nfiles;		/* how much used in dir */
	struct Dirent *dir;	/* directory contents */
} File;

#define	isdir(f)	((f)->dir != 0)

/*
 * internal directory entry
 */

#define	DIRNSIZE	20

typedef struct Dirent {
	char name[DIRNSIZE];
	File *file;
} Dirent;

File *newfile(), *lookfile(), *lookent(), *enterfile();
char *emalloc(), *erealloc();
