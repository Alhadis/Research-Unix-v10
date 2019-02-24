/*
 * Inode structure as returned from stat
 */
typedef struct
{
	int	i_dev;
	int	i_ino;
	int	i_mode;
	char	i_nlink;
	char	i_uid;
	char	i_gid;
	char	i_size0;
	char	*i_size1;
	int	i_addr[8];
	long	i_atime;
	long	i_mtime;
} STATBUF;

/* modes */
#define	IALLOC	0100000
#define	IFMT	060000
#define	IFDIR	040000
#define	IFCHR	020000
#define	IFBLK	060000
#define	ILARG	010000
#define	ISUID	04000
#define	ISGID	02000
#define ISVTX	01000
#define	IREAD	0400
#define	IWRITE	0200
#define	IEXEC	0100
