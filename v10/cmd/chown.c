/*
 * chown uid[,gid] file ...
 */

#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

int uid, gid;
int nouid, nogid;

main (argc, argv)
	char *argv[];
{
	register c;
	int status;
	struct stat stbuf;
	int u, g;
	
	if (argc < 3) {
		printf("usage: chown uid[,gid] file ...\n");
		exit(4);
	}
	uidgid(argv[1]);
	status = 0;
	for(c=2; c<argc; c++) {
		if (stat(argv[c], &stbuf) < 0) {
			perror(argv[c]);
			status = 1;
			continue;
		}
		u = nouid ? stbuf.st_uid : uid;
		g = nogid ? stbuf.st_gid : gid;
		if(u == stbuf.st_uid && g == stbuf.st_gid)
			;
		else if(chown(argv[c], u, g) < 0) {
			perror(argv[c]);
			status = 1;
		}
	}
	exit(status);
}

uidgid(ug)
char *ug;
{
	char *g;
	struct passwd *pwd;
	struct group *grp;
	char *strchr();
	struct passwd *getpwnam();
	struct group *getgrnam();

	if ((g = strchr(ug, ',')) != NULL)
		*g++ = '\0';
	if (*ug == '\0')
		nouid++;
	else if (isnumber(ug))
		uid = atoi(ug);
	else if ((pwd = getpwnam(ug)) != NULL)
		uid = pwd->pw_uid;
	else {
		fprintf(stderr, "unknown user id: %s\n", ug);
		exit(4);
	}
	if (g == NULL || *g == '\0')
		nogid++;
	else if (isnumber(g))
		gid = atoi(g);
	else if ((grp = getgrnam(g)) != NULL)
		gid = grp->gr_gid;
	else {
		fprintf(stderr, "unknown group id: %s\n", g);
		exit(4);
	}
}

isnumber(s)
char *s;
{
	register c;

	if(*s == '-') s++;
	while(c = *s++)
		if(!isdigit(c))
			return(0);
	return(1);
}
