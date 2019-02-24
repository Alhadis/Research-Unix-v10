#define MAXLINE 256
#define MAXNAME 20

struct tarbuf {
	char name[100];		/* strncpy format */
	char mode[8];		/* octal NUL-terminated */
	char uid[8];		/* octal */
	char gid[8];		/* octal */
	char size[12];		/* octal */
	char mtime[12];		/* octal */
	char chksum[8];		/* octal sum of header bytes; set to
				   blanks while computing chksum */
	char typeflag[1];	/* see below */
	char linkname[100];	/* strncpy */
	char magic[6];		/* see below */
	char version[2];	/* see below */
	char uname[32];		/* strcpy format */
	char gname[32];		/* strcpy */
	char devmajor[8];	/* octal */
	char devminor[8];	/* octal */
	char prefix[155];	/* strncpy */
	char zeropad[12];
};

#define TSIZE (sizeof (struct tarbuf))

#define TMAGIC "ustar"		/* ustar\0 */
#define TMAGLEN 6
#define TVERSION "00"		/* strncpy, n == 2 */
#define TVERSLEN 2

/* typeflag values */
#define AREGTYPE '\0'		/* historical regular file */
#define REGTYPE '0'		/* regular file */
#define LNKTYPE '1'		/* hard link */
#define SYMTYPE '2'		/* reserved (symbolic link) */
#define CHRTYPE '3'		/* character special */
#define BLKTYPE '4'		/* block special */
#define DIRTYPE '5'		/* directory */
#define FIFOTYPE '6'		/* fifo special */
#define CONTTYPE '7'		/* reserved (socket?) */

/* mode bits */
#define TSUID 04000		/* set user id on execution */
#define TSGID 02000		/* set group id on execution */
#define TSVTX 01000		/* reserved */
#define TUREAD 00400		/* read by owner */
#define TUWRITE 00200		/* write by owner */
#define TUEXEC 00100		/* exec/search by owner */
#define TGREAD 00040		/* read by group */
#define TGWRITE 00020		/* write by group */
#define TGEXEC 00010		/* exec/search by group */
#define TOREAD 00004		/* read by other */
#define TOWRITE 00002		/* write by other */
#define TOEXEC 00001		/* exec/search by other */
#define TMASK 07777		/* mask of all bits (not in POSIX) */

#define TNAMEMAX 256
#define TLINKMAX 100
#define TUGNAMEMAX 31

/* incore equivalent of a tar buffer */
struct tarhdr {
	char name[TNAMEMAX + 1];
	int mode;
	int uid;
	int gid;
	long size;
	time_t mtime;
	char typeflag;
	char linkname[TLINKMAX + 1];
	char uname[TUGNAMEMAX + 1];
	char gname[TUGNAMEMAX + 1];
	int devmajor;
	int devminor;
};

/* system dependent */
extern void canonsys(char *sys, char cansys[]);
extern int lcreat(char *name, int mode);

/* chat.c */
extern int getline(char line[]);
extern long fdcopy(int dfd, int sfd, long size, unsigned long *crc);
extern int recvfile(int fd);
extern int sendfile(int fd);

/* chkperm.c */
extern int chkperm(int fd);

/* crc.c */
extern void crcinit(int *aux);
extern unsigned long crcincr(unsigned char *, size_t, unsigned long, int *);

/* id.c */
extern int gidnum(char *);
extern char *gidstr(int);
extern int uidnum(char *);
extern char *uidstr(int);

/* print.c */
extern char *prog;
extern void openlog(char *logfile);
extern void eprintf(char *fmt, ...);
extern void lprintf(char *fmt, ...);

/* quote.c */
void quote(char *qs, char *us);
void unquote(char *us, char *qs);

/* scanq.c */
extern void scanq(char *remsys, void (*func)(char *, int, char *));

/* sub.c */
struct sub {
	char *ostr;
	char *nstr;
	struct sub *next;
};

extern void addsub(struct sub **, char *, char *);
extern int sub(struct sub **, int, char [], int);

/* tarhdr.c */
extern int thdrget(struct tarhdr *hdr, struct tarbuf *buf);
extern int thdrput(struct tarbuf *buf, struct tarhdr *hdr);

/* tryperm.c */
extern int tryperm(int fd);

/* util.c */
extern void *xmalloc(size_t);
extern void *xrealloc(void *, size_t);
extern char *xstrdup(char *);
extern long fpcopy(FILE *, FILE *, long);
extern void discard(FILE *, long);
