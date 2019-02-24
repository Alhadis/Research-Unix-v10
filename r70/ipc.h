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
#define IPC_OPEN	0x0	/* open an original connection */
#define IPC_CREAT	0x1	/* create a server channel */
#define IPC_HANDOFF	0x2	/* pass a request onto someone else */
#define IPC_CAREFUL	0x4	/* check protections before opening file */
#define IPC_REDIAL	0x8	/* this means ask the network to redial
				 *  the call as the new machine and user
				 */

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

/* qnet stuff */
typedef struct qtuple{
	struct qtuple *next;
	char *value;
	char *type;
} Qtuple;

typedef struct qset{
	struct qset *next;
	Qtuple *this;
} Qset;

extern Qset *qset();
extern char *qvalue();
extern int freeQtuple();
extern int freeQset();
