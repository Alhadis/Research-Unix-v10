#include "authmgr.h"

/*
 * the chal (challenge) routine should take one parameter, the keyinfo
 * structure returned by getkeyinfo (below) and should
 * return a (char *), containing a printable ASCII key to challenge the
 * user with.  The NULL string is a valid return value, in which case the
 * assumed user response will be a non-echoed password.
 *
 * the comp routine should take two parameters, the same keyinfo structure
 * and the (char *) response from the user.  chal and comp may share
 * addtional state.  the comp routine returns 0 if the response should
 * be accepted, nonzero otherwise.
 *
 * The first entry, "passwd" gets special treatment, 
 */
char *passwdchal();
int passwdcomp();
char *atallachal();
int atallacomp();

struct keytypes keytypemap[] = {
	"passwd", passwdchal, passwdcomp,	/* Must be first */
	"atalla", atallachal, atallacomp,	/* Atalla Confidante DES box */
	0, 0, 0					/* Must be last */
};

char *badchal();
int badcomp();

static struct keytypes badtype = {
	"", badchal, badcomp,			/* for disallowed users */
};
extern struct disallow *badlist;

struct keyinfo *
getkeyinfo(logname, usepasswd)
	char *logname;
	int usepasswd;
{
	int i;
	FILE* kf;
	char line[128];
	char *c, *c2;
	struct disallow *dp;
	struct keytypes *ktp;
	static struct keyinfo k;
#define NFLDS	10
	char *fld[NFLDS];

	for(i=0; i<sizeof(k.logname); i++)
		k.logname[i] = 0;
	for(i=0; i<sizeof(k.key); i++)
		k.key[i] = 0;
	k.kt = &badtype;

	/* check if this login has been disallowed */
	for(dp = badlist; dp; dp = dp->next) {
		if(strcmp(logname, dp->logname) == 0)
			return &k;
	}

	k.expire = 99991231;	/* 31 December 9999 */

	if(usepasswd) {
		k.kt = &keytypemap[0];
		strcpy(k.logname, logname);
		k.key[0] = '\0';
	} else {
		if((kf = fopen(KEY_FILE, "r")) == NULL)
			return &k;
	
		setfields(":\n");

		while(fgets(line, 128, kf) != NULL) {
			i = getfields(line, fld, NFLDS);
			if(i >= 3 &&
			   strcmp(logname, fld[0]) == 0 &&
			   strlen(fld[2]) <= sizeof(k.key)) {
				/* found the entry */
				strcpy(k.logname, fld[0]);
				strcpy(k.key, fld[2]);
				if(strcmp(fld[3], "") != 0)
					k.expire = atol(fld[3]);
				if(strlen(fld[3]) <= sizeof(k.expire))
					strcpy(k.expire, fld[3]);
				else

				for(ktp = &keytypemap[0]; ktp->s; ktp++) {
					if(strcmp(ktp->s, fld[1]) == 0) {
						k.kt = ktp;
						break;
					}
				}
				break;
			}
		}
	
		(void) fclose(kf);
	}
	return &k;
}

char*
badchal(kp)
	struct keyinfo *kp;
{
	return 0;
}

int
badcomp(s)
	char *s;
{
	return 1;
}
