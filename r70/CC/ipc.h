/* information returned by listen */
struct ipcinfo {
	int rfd;	/* fd for reply */
	int cfd;	/* fd for communications */
	char *myname;	/* translated part of name */
	char *name;	/* untranslated part of name */
	char *param;	/* parameters used to set up call */
	char *machine;	/* machine id of caller */
	char *user;	/* user name of caller */
	int uid, gid;	/* uid, gid of caller */
	char flags;	/* type of communication */
};

/* communication flags */
#define IPC_OPEN	0x0
#define IPC_CREAT	0x1
#define IPC_HANDOFF	0x2
#define IPC_CAREFUL	0x4

/* library definitions */
extern "C" {
	extern int ipcopen(const char*, const char*);
	extern int ipccreat(const char*, const char*);
	extern ipcinfo* ipclisten(int);
	extern ipcinfo* ipctrylisten(int);
	extern int ipcaccept(ipcinfo*);
	extern int ipcdaccept(ipcinfo*, int, const char*);
	extern int ipcreject(ipcinfo*, int, const char*);
	extern int ipclogin(int);
	extern int ipcrogin(int, int);
	extern char* ipcpath(const char*, const char*, const char*);
	extern int ipcexec(const char*, const char*, const char*);
}

/* import */
extern int errno;
extern char* errstr;
extern char* ipcname;
