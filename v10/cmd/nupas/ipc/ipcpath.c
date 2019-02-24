#include <stdio.h>
#include <string.h>

extern int ipcdebug;

#define CAT(x) if (x) for(cp=x;bp-buf<sizeof(buf)-1&&*cp;) *bp++ = *cp++; *bp='\0'

/*
 *	Create an ipc path that starts in the /cs directory
 *
 *	ipcpath("x!y!z", "d", "s") -> "x!y!z"
 *	ipcpath("x!y", "d", "s") -> "x!y!s"
 *	ipcpath("x", "d", "s") -> "d!x!s"
 *
 *	all other combinations abort with an error
 */
char *
ipcpath(machine, defdialer, service)
	char *machine;
	char *defdialer;
	char *service;
{
	char *field[3];
	static char buf[256];
	char ibuf[sizeof buf];
	char *bp = buf;

	*bp = '\0';

	if (machine == NULL)
		machine = "";
	strcpy(ibuf, machine);
	if (ipcdebug)
		printf("ipcpath called with %s, %s, %s\n",
			ibuf, defdialer, service);

	setfields("!");

	switch (getfields(ibuf, field, 3)) {
	case 3:	bp = strcat(bp, field[0]);
		bp = strcat(bp, "!");
		bp = strcat(bp, field[1]);
		bp = strcat(bp, "!");
		bp = strcat(bp, field[2]);
		break;

	case 2:	bp = strcat(bp, field[0]);
		bp = strcat(bp, "!");
		bp = strcat(bp, field[1]);
		bp = strcat(bp, "!");
		if (service!=NULL && *service!='\0')
			bp = strcat(bp, service);
		break;

	case 1:	bp = strcat(bp, defdialer);
		bp = strcat(bp, "!");
		bp = strcat(bp, field[0]);
		bp = strcat(bp, "!");
		if (service!=NULL && *service!='\0')
			bp = strcat(bp, service);
		break;

	default:
		fprintf(stderr, "Illegal machine: %s\n", machine);
		exit(1);
	}
	if(ipcdebug)
		printf("ipcpath returning %s\n", buf);
	return buf;
}
