#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys/filio.h>
#include <sys/ttyio.h>
#include <ipc.h>
#include <regexp.h>
#include <sys/types.h>
#include <sys/times.h>
#include <time.h>

#define MAX_FAILURES	3	/* default max login/data failures */
#define KEY_FILE	"/usr/net/authmgr.keys"
#define CONTROL_FILE	"/usr/net/authmgr.conf"

/* disallow - information on who we refuse to authenticate */
struct disallow {
	struct disallow* next;
	char logname[10];
};

struct secmap {
	struct secmap* next;
	regexp *prog;		/* the regular expression to match */
	char   *secid;		/* the security ID to use if it matches */
};

struct destmap {
	struct destmap* next;
	regexp *prog;		/* the regular expression to match */
	char   *dest;		/* the security ID to use if it matches */
};

struct keytypes {
	char *s;
	char *(*chal)();	/* proc to compute a challenge */
	int (*comp)();		/* proc to test a response's validity */
};

/* keyinfo -- information from the key file */
struct keyinfo {
	struct keytypes* kt;
	char logname[12];
	char key[64];
	long expire;		/* the integer yyyymmdd */
};
