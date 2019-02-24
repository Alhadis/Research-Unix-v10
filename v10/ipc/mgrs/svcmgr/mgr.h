#include <ipc.h>
#include <stdio.h>
#include <libc.h>
#include <regexp.h>

/*
 * table of regular expressions
 */

typedef struct cregexp {
	struct cregexp *next;
	char *re;		/* uncompiled reg exp */
	regexp *prog;		/* compiled reg exp */
	int ref;		/* reference count */
} Cregexp;
extern Cregexp *newre();
extern int freerep();
extern int execre();

/*
 * table of user mappings
 * one per line in the mapping files
 */

typedef struct mapping{
	Cregexp *from;	/* originating machine */
	Cregexp *user;	/* originating user */
	Cregexp *serv;	/* service(s) requested */
	char *luser;	/* local user name to use */
	struct mapping *next;
} Mapping;
extern Mapping *newmap();
extern int freemap();
extern int addmap();

/*
 * action instances
 * a linked list of these exist for each service
 */

typedef struct action {
	int (*func)();	/* funciton to use for this action */
	char *arg;	/* arguments or NULL */
	int accept;	/* true if ipcaccept to be done by action */
	struct action *next;
} Action;
extern Action *newaction();
extern int freeaction();

/*
 * table of services
 * one per line in the file of services (hopefully)
 */

typedef struct service {
	char *name;	/* name of service (e.g. `uucp') */
	Cregexp *from;	/* originating machine */
	int listen;	/* fd for listening */
	Action *ap;	/* list of actions to perform */
	int accept;	/* true if ipcaccept to be done by the service */
	long lasttime;
	struct service *same;
	struct service *next;
} Service;
extern Service *newservice();
extern int freeservice();
extern int addservice();

/*
 * request returned by listen
 */
typedef struct {
	ipcinfo *i;	/* from the ipclisten */
	Service *s;	/* service requested */
	char *line;	/* password line from authentication */
	char *args;	/* args gotten by from getargs */
	char *term;	/* terminal type */
	int errfd;	/* standard error (if different from stdout) */
} Request;
extern Request *newrequest();
extern int freerequest();

extern int logevent();

#define	ARB 512
