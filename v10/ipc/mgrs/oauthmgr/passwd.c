#include "authmgr.h"
#include <pwd.h>

char*
passwdchal(kp)
	struct keyinfo *kp;
{
	return 0;
}

int
passwdcomp(kp, s)
	struct keyinfo *kp;
	char *s;
{
	struct passwd* pw;

	if(strcmp(kp->logname, "none")==0)
		return 0;
	if(strlen(kp->key) > 0) {
		if(strcmp(s, crypt(s, kp->key)) == 0)
			return 0;
	} else {
		pw = getpwnam(kp->logname);
		if(pw != 0 && strlen(pw->pw_passwd) >= 2 &&
			strcmp(pw->pw_passwd, crypt(s, pw->pw_passwd)) == 0)
			return 0;
	}

	return 1;
}

