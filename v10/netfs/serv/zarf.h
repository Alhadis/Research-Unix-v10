/*
 * private data for unix file server;
 * stored in Rfile.fs
 */

typedef struct {
	int fd;
	int flags;
	char *name;
} Fsfile;

#define	CANTREAD	01
#define	CANTWRITE	02
#define	WONTREAD	04
#define	WONTWRITE	010
#define	DIDDIR		020	/* need to dircleanup on close */
#define	NOSTAT		040	/* stat info invalid */

#define	fsp(f)	((Fsfile *)(f)->fs)
