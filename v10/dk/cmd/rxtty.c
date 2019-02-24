#include <stdio.h>
#include <dk.h>

/*
 * program to run a command
 * on another cpu on Datakit
 */

int     rem;            /* remote file descriptor */
int     psize;          /* packet size */
int     t;

char    *std    = "-";

char buf[512];
FILE *CAT;

#define MAXCHARS 8192
char args[MAXCHARS];

char *bldargs(argv)
	register char *argv[];
{
	register char *s=args, *t;
	while(t= *argv++){	/* assignment = */
		while(*s = *t++)
			if(s++ >= &args[MAXCHARS-1]){
				fprintf(stderr, "rx: arg list too long\n");
				exit(1);
			}
		*s++=' ';
	}
	s[-1]='\0';
	return(args);
}

char *
basename(s)
	register char *s;
{
	register char *t;
	extern char *rindex();
	t=rindex(s, '/');
	return(t? t+1 : s);
}

main(argc, argv)
char **argv;
{
int filc;
int cat;
char **filv;
char *host;

        filv = &std;
        filc = 1;

        cat = 1;
        CAT = fdopen(cat, "w");
	host=basename(argv[0]);
	if(strcmp(host, "rxtty")==0 || strcmp(host, "rexec")==0){
		host=argv[1];
		argv++;
	}
	if(host==0){
		fprintf(stderr, "usage: rx host command\n");
		exit(1);
	}
	if(argv[1]==0){
		execl("/usr/bin/dcon", host, "-x", host, 0);
		execl("/bin/dcon", host, "-x", host, 0);
		fprintf(stderr, "rx: cannot exec dcon\n");
		exit(1);
	}
        rem = ttyexec(host, bldargs(&argv[1]));
        if (rem<0) {
		fprintf(stderr, "rx: call to %s failed\n", host);
                exit(1);
	}
        t = fork();
        if (t < 0) {
                fprintf(stderr, "rx: can't fork\n");
                exit(1);
        }

        if (t == 0) {
                to_proc(filc, filv);
        } else {
                argv[0][0] = 'F';
                from_proc();
        }
	exit(0) ;

}

to_proc(filc, filv)
char **filv;
{
register cc;

                while (1) {
                        cc = read(0, buf, sizeof buf);
                        if (cc <= 0)
                                break;
                        write(rem, buf, cc);
                }

        /*
         * flush buffer to proc,
         * then send logical EOF to
         * make it go away.
         */
done:
	write(rem, "", 0) ;
}

from_proc()
{
FILE *REM;
register cc;
static char buf[BUFSIZ];

        while ((cc = read(rem, buf, sizeof buf)) > 0) {
                write(1,  buf, cc);
        }
        kill(t, 2);
}

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
/****
	strcat(dialstr, ".") ;
	strcat(dialstr, cmdname) ;
 ****/
	rem = tdkdial(dialstr, 2) ;
	if (rem < 0) {
		fprintf(stderr, "can't rexec\n");
		return(-1);
	}
	v = dkproto(rem, dkp_ld);
	if (v < 0) {
		printf("can't start protocol\n");
		close(rem);
		return(-1);
	}
	if (tdklogin(rem) < 0)
		return -1 ;
	write(rem, cmd, strlen(cmd));
	write(rem, "\n", 1) ;
	return(rem);
}
