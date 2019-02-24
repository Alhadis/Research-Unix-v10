#define QPREFIX "Q."
#define CTL "ctl"
#define DATA "data"
#define BUSY "busy"
#define DONE "done"
#define FILES "files"

#define MAXLINE 256
#define MAXNAME 20

/* system dependent */
extern void canon(char *sys, char cansys[]);
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

/* eprintf.c */
extern char *prog;
extern void eprintf(char *fmt, ...);

/* path.c */
extern char *getpath(FILE *);

/* scanq.c */
extern void scanq(char *remsys, void (*func)(char *, int, char *));

/* tryperm.c */
extern int tryperm(int fd);
