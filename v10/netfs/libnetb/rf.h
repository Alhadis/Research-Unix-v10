/*
 * remote file access library
 * for building file system daemons
 */

typedef struct {
	char *fs;	/* fs-specific local data */
	long ino;
	char dev;
	char type;	/* directory or regular */
	short mode;
	short nlink;
	short uid, gid;
	short rdev;	/* is this useful? */
	long size;
	long ta;
	long tm;
	long tc;
} Rfile;

extern int fserrno;
extern int rfdebug;
extern char *rfclient;

/*
 * service routines
 * provided by the user,
 * but this is what they should look like
 */
Rfile *fsinit();	/* (int argc, char **argv) */
Rfile *fswalk();	/* (Rfile *dir, char *name) */
Rfile *fscreate();	/* (Rfile *dir, char *newname, int mode, uid, gid) */
int fsdelete();		/* (Rfile *dir, char *name) */
int fslink();		/* (Rfile *dir, char *newname, Rfile *ofile) */
int fsmkdir();		/* (Rfile *dir, char *name) */
int fsrmdir();		/* (Rfile *dir, char *name, int mode, uid, gid) */
int fsdone();		/* (Rfile *f) -- discard this Rfile */
int fsstat();		/* (Rfile *f) -- fill it in with stat info */
int fsread();		/* (Rfile *f, long off, char *buf, int len) */
int fswrite();		/* (Rfile *f, long off, char *buf, int len) */
int fsdirread();	/* (Rfile *f, long off, char *buf, int len, long *newoff) */
int fsupdate();		/* (Rfile *f, Rfile *attr) -- copy attributes from attr */
			/* mode, size, uid, gid, ta, tm */

/*
 * permission mapping
 */

/*
 * local name to local ID mapping
 */
typedef struct {
	char name[9];
	short id;
} Idmap;

/*
 * local name to remote name mapping;
 * used only for exception list
 */
typedef struct {
	char sname[9];
	char cname[9];
	int sid;
} Namemap;

Idmap *rfmkidmap();	/* (char *filename, Namemap *exlist) */
extern Idmap *rfuidmap, *rfgidmap;
extern int rfuid, rfgid;
extern int rfotherdeny;

#define	RFNOID	(-1)	/* unmapped user- or group-id */

/*
 * file modes
 * these correspond to V7 Unix
 * only the low nine bits (permissions) are used by the library
 * other bits will be passed literally to the client
 * if it expects unix-like modes
 */
#define	RFPRD	04		/* read permission */
#define	RFPWR	02		/* write permission */
#define	RFPEX	01		/* execute permission */
#define	RFPDS	RFPEX		/* directory search permission */

#define	RFPOTHER	0	/* shift to make `other' permissions */
#define	RFPGROUP	3	/* shift to make `group' permissions */
#define	RFPOWNER	6	/* shift to make `owner' permissions */

/*
 * file types
 * other types are just not allowed for now
 * (but some protocols may sneak them through in the mode)
 */
#define	RFTREG	0	/* regular file */
#define	RFTDIR	1	/* directory */

/*
 * error codes in fserrno
 * these intentionally match Unix errno values,
 * and in fact errno values will usually do,
 * but the library doesn't promise to preserve
 * exact values that aren't here
 */

#define	RFEOWNER	1	/* not file owner */
#define	RFENOENT	2	/* no such file or directory */
#define	RFEIO		5	/* general I/O error */
#define	RFENXIO		6	/* no such file address */
#define	RFEACCES	13	/* no permission */
#define	RFEBUSY		16	/* something is in use and can't be used again */
#define	RFEEXIST	17	/* file exists when it shouldn't */
#define	RFEXDEV		18	/* link across devices (or just not allowed) */
#define	RFENOTDIR	20	/* not a directory and should be */
#define	RFEISDIR	21	/* is a directory and shouldn't be */
#define	RFEINVAL	22	/* invalid arg or function; catchall for internal errors */
#define	RFENOSPC	28	/* no space on disk or in buffer (dirread) */
