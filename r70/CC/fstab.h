/*
 * fstab(5) (which filesystems are there?) and
 * mtab(5) (which filesystems are mounted?)
 *
 * bugs: swapping has nothing to do with filesystems
 */

/*
 * fstab things
 */
#define	FSTAB		"/etc/fstab"
#define	FSNMLG		32

#define FSTABNARGS	5

struct	fstab{
	char	fs_spec[FSNMLG];	/* file to be mounted */
	char	fs_file[FSNMLG];	/* mount point */
	int	fs_ftype;		/* file system type */
	int	fs_flags;		/* integer flags */
	int	fs_passno;		/* pass number on parallel fsck */
};

/*
 * special filesystem values
 * all < 0
 */

#define	FSNONE	(-1)	/* ignore; use to comment out */
#define	FSSWAP	(-2)	/* swap area */

struct	fstab *getfsent();
struct	fstab *getfsspec(char*);
struct	fstab *getfsfile(char*);
int	setfsent();
int	endfsent();

/*
 * mtab things
 */

struct mtab {
	char	file[FSNMLG];		/* mount point */
	char	spec[FSNMLG-1];		/* file */
	char	type;			/* filsys type */
};

#define	MTAB	"/etc/mtab"		/* the file */
