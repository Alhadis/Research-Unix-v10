typedef struct {
	char *name;	/* last component of pathname */
	int namelen;	/* strlen(name) */
	long age;	/* seconds of unmodified existence */
	char isdir;	/* 1 --> it's a directory */
} dirent;

extern int dirsel();
