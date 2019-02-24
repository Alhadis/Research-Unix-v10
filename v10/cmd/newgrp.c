#include <stdio.h>
#include <grp.h>
#include <pwd.h>
#include <sys/param.h>

struct	group	*getgrnam(), *grp;
struct	passwd	*getpwuid(), *pwd;
char	*getpass(), *crypt();

main(argc,argv)
int	argc;
char	**argv;
{
	register i;
	if(argc < 2) {
		fprintf(stderr, "usage: newgrp groupname [command]\n");
		exit(1);
	}
	if((grp=getgrnam(argv[1])) == NULL) {
		fprintf(stderr, "%s: no such group\n", argv[1]);
		exit(1);
	}
	if((pwd=getpwuid(getuid())) == NULL) {
		fprintf(stderr, "You do not exist!\n");
		exit(1);
	}
	for(i=0;grp->gr_mem[i];i++) 
		if(strcmp(grp->gr_mem[i], pwd->pw_name) == 0)
			break;
	if(grp->gr_mem[i] == 0 && strcmp(grp->gr_name,"other")) {
		fprintf(stderr, "Sorry\n");
		exit(1);
	}

	if(grp->gr_passwd[0] != '\0' && pwd->pw_passwd[0] == '\0') {
		if(strcmp(grp->gr_passwd, crypt(getpass("Password:"),grp->gr_passwd)) != 0) {
			fprintf(stderr, "Sorry\n");
			exit(1);
		}
	}
	if(setgid(grp->gr_gid) < 0) {
		perror("setgid");
		exit(1);
	}
	done(argc, argv);
}

done(argc, argv)
int argc;
char **argv;
{
	register i;

	setuid(getuid());
	for (i=NSYSFILE; i<NOFILE; i++)
		close(i);
	if(argc < 3) {
		execl(((pwd && pwd->pw_shell[0])? pwd->pw_shell: "/bin/sh"), "sh", 0);
		fprintf(stderr, "No shell!\n");
	} else {
		execvp(argv[2], &argv[2]);
		perror(argv[2]);
		exit(1);
	}
	exit(1);
}
