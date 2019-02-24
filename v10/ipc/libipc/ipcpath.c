#include "defs.h"
#include <signal.h>

#define CAT(x) if (x) for(cp=x;bp-buf<sizeof(buf)-1&&*cp;) *bp++=*cp++; *bp='\0'

/*
 *	Create an ipc path that starts in the /cs directory
 *
 *	ipcpath("x", "d", "s") -> "/cs/d!x!s"
 *	ipcpath("x", "/d", "s") -> "/d!x!s"
 *	ipcpath("x!y", "d", "s") -> "/cs/x!y!s"
 *	ipcpath("x!y!z", "d", "s") -> "/cs/x!y!z"
 *	ipcpath("", "", "s") -> "/cs/s"
 *	ipcpath("x", "", "") -> "/cs/x"
 *
 *	all other combinations are undefined
 */
char *
ipcpath(machine, defdialer, service)
	char *machine;
	char *defdialer;
	char *service;
{
	char buf[256];
	static char buf2[sizeof(buf)+sizeof(CSROOT)];
	char *bp, *bang1;
	char *cp;

	bp = buf;
	if (machine!=NULL && *machine!='\0') {
		if ((bang1=strchr(machine, '!'))==NULL){
			if(defdialer!=NULL && *defdialer!='\0'){
				CAT(defdialer);
				CAT("!");
			}
			CAT(machine);
			if (service!=NULL && *service!='\0') {
				CAT("!");
				CAT(service);
			}
		} else {
			CAT(machine);
			if (strchr(bang1+1, '!')==NULL && service!=NULL
			&& *service!='\0') {
				CAT("!");
				CAT(service);
			}
		}
	} else {
		if (service!=NULL && *service!='\0') {
			CAT(service);
		}
	}
	if (*buf=='/')
		*buf2 = '\0';
	else
		strcpy(buf2, CSROOT);
	strcat(buf2, buf);
	return buf2;
}
