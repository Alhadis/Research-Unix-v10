#include <errno.h>

#ifndef NULL
#define NULL 0
#endif

#define ABORT _ipcabort
#define NULLINFO (ipcinfo*)NULL
#define ROOTUID 0
#define PATHLEN 256

/* routines used by the conenction service */
extern int ipcdial();
extern int _info_read();
extern int _info_write();
extern int _reply_read();
extern int _reply_write();
extern int _ipcabort();

/* string routines */
extern char *strchr();
extern char *strcpy();
extern char *strcat();
extern char *strncat();

/* general purpose routines */
extern int logevent();
extern int detach();

/* to fake out lint & cyntax */
#ifdef LINT
#define USE(a) lintuse((int)a)
extern int lintuse();
#else
#define USE(a)
#endif LINT

/* root of connection service */
#define CSROOT "/cs/"

/* stretch buffers */
extern int _strcat();
typedef struct {
	char *ptr;
	int len;
} stretch;
