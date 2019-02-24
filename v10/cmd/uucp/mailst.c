/*	@(#)mailst.c	1.3	*/

#include "uucp.h"
VERSION(@(#)mailst.c	1.3);

/*
 * fork and execute a mail command sending 
 * string (str) to user (user).
 * If file is non-null, the file is also sent.
 * (this is used for mail returned to sender.)
 *	user	 -> user to send mail to
 *	str	 -> string mailed to user
 *	infile	 -> optional stdin mailed to user
 *	errfile	 -> optional stderr mailed to user
 *
 * a upas-ism -- call rmail and make up our own From line,
 * so mail comes from uucp, not some arbitrary user
 */
char *ctime();
long time();

mailst(user, str, infile, errfile)
char *user, *str, *infile, *errfile;
{
	register FILE *fp, *fi;
	char cmd[BUFSIZ];
	char *c;
	long now;

	/* get rid of some stuff that could hurt */
	if ( (c = strpbrk(user, ";&|<>^`\\('\"")) != NULL)
		*c = NULLCHAR;
	(void) sprintf(cmd, "%s rmail %s", PATH, user);
	if ((fp = popen(cmd, "w")) == NULL)
		return;
	now = time((long *)0);
	fprintf(fp, "From uucp %.24s remote from %s\n", ctime(&now), Myname);
	fprintf(fp, "From: %s!uucp\n", Myname);
	(void) fprintf(fp, "%s\n", str);

	/* copy back stderr */
	if (*errfile != '\0' && NOTEMPTY(errfile) && (fi = fopen(errfile, "r")) != NULL) {
		fputs("\n\t===== stderr was =====\n", fp);
		if (xfappend(fi, fp) != SUCCESS)
			fputs("\n\t===== well, i tried =====\n", fp);
		(void) fclose(fi);
		fputc('\n', fp);
	}

	/* copy back stdin */
	if (*infile != '\0' && NOTEMPTY(infile) && (fi = fopen(infile, "r")) != NULL) {
		fputs("\n\t===== stdin was =====\n", fp);
		if (xfappend(fi, fp) != SUCCESS)
			fputs("\n\t===== well, i tried =====\n", fp);
		(void) fclose(fi);
		fputc('\n', fp);
	}

	(void) pclose(fp);
}
#ifndef	V7
static char un[2*NAMESIZE];
setuucp(p)
char *p;
{
   char **envp;

    envp = Env;
    for ( ; *envp; envp++) {
	if(PREFIX("LOGNAME", *envp)) {
	    (void) sprintf(un, "LOGNAME=%s",p);
	    envp[0] = &un[0];
	}
    }
}
#else
/*ARGSUSED*/
setuucp(p) char	*p; {}
#endif
