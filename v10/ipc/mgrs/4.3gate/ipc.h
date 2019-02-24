/* information returned by listen */
typedef struct {
	int rfd;	/* fd for reply */
	int cfd;	/* fd for communications */
	int net;
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
extern int ipcopen();
extern int ipccreat();
extern ipcinfo *ipclisten();
extern ipcinfo *ipctrylisten();
extern int ipcaccept();
extern int ipcdaccept();
extern int ipcreject();
extern int ipclogin();
extern int ipcrogin();
extern char *ipcpath();
extern int ipcexec();

/* import */
extern int errno;
extern char *errstr;
extern char *ipcname;
