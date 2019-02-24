/* information returned by listen */
typedef struct {
	int rfd;	/* fd for reply */
	int cfd;	/* fd for communications */
	char *myname;	/* translated part of name */
	char *name;	/* untranslated part of name */
	char *param;	/* parameters used to set up call */
	char *machine;	/* machine id of caller */
	char *user;	/* user name of caller */
	int uid, gid;	/* uid, gid of caller */
	char flags;	/* type of communication */
} ipcinfo;

/* communication flags */
#define IPC_OPEN	0x0
#define IPC_CREAT	0x1
#define IPC_HANDOFF	0x2
#define IPC_CAREFUL	0x4

/* library definitions */
extern int ipcopen(char *, char *);
extern int ipccreat(char *, char *);
extern ipcinfo *ipclisten(int);
extern ipcinfo *ipctrylisten(int);
extern int ipcaccept(ipcinfo *);
extern int ipcdaccept(ipcinfo *, int, char *);
extern int ipcreject(ipcinfo *, int, char *);
extern int ipclogin(int);
extern int ipcrogin(int, int);
extern char *ipcpath(char *, char *, char *);
extern int ipcexec(char *, char *, char *);

/* import */
extern int errno;
extern char *errstr;
extern char *ipcname;
