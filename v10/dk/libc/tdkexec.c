#include <dk.h>
#include <stdio.h>
#include <sys/ioctl.h>

tdkexec(hname, cmd)
char *hname, *cmd;
{
	return(_tdkexec(hname, cmd, "exec"));
}

ttyexec(hname, cmd)
char *hname, *cmd ;
{
	return (_tdkexec(hname, cmd, "ttyexec")) ;
}

mesgexec(hname, cmd)
char *hname, *cmd;
{
	return(_tdkexec(hname, cmd, "mesgexec"));
}

_tdkexec(hname, cmd, service)
char *hname, *cmd;
char * service ;
{
	int rem, v;
	extern int pk_ld, dkp_ld, tty_ld, dt_ld ;
	char dialstr[64] ;
	char cmdname[32] ;
	extern char *dkerror;
	register char *cp1, *cp2 ;

	strcpy(dialstr, hname) ;
	strcat(dialstr, ".") ;
	strcat(dialstr, service) ;
	cp1 = cmdname ; cp2 = cmd ;
	while (*cp2 && (*cp2 != ' ')) {
		if (*cp2 == '/')
			cp1 = cmdname ;
		else
			*cp1++ = *cp2 ;
		cp2++ ;
	}
	*cp1 = '\0' ;
	strcat(dialstr, ".") ;
	strcat(dialstr, cmdname) ;
	rem = tdkdial(dialstr, 2) ;
	if (rem < 0) {
		return(-1);
	}
	v = dkproto(rem, dkp_ld);
	if (v < 0) {
		dkerror = "tdkexec can't turn on protocol";
		close(rem);
		return(-1);
	}
	if (tdklogin(rem) < 0)
		return -1 ;
	write(rem, cmd, strlen(cmd));
	write(rem, "\n", 2) ;
	return(rem);
}
