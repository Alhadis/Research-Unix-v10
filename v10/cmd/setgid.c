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
		printf("usage: setgid groupname [command]\n");
		exit(1);
	}
	if((grp=getgrnam(argv[1])) == NULL) {
		printf("%s: no such group\n", argv[1]);
		exit(2);
	}
	if((pwd=getpwuid(getuid())) == NULL) {
		printf("You do not exist!\n");
		exit(3);
	}
	for(i=0;grp->gr_mem[i];i++) 
		if(strcmp(grp->gr_mem[i], pwd->pw_name) == 0)
			break;
	if(grp->gr_mem[i] == 0 && strcmp(grp->gr_name,"other")) {
		printf("Sorry\n");
		exit(4);
	}

	if(grp->gr_passwd[0] != '\0' && pwd->pw_passwd[0] == '\0') {
		if(strcmp(grp->gr_passwd, crypt(getpass("Password:"),grp->gr_passwd)) != 0) {
			printf("Sorry\n");
			exit(5);
		}
	}
	if(setgid(grp->gr_gid) < 0) {
		perror("setgid");
		exit(6);
	}
	if(setuid(getuid()) < 0) {
		perror("setuid");
		exit(7);
	}
	for (i=NSYSFILE; i<NOFILE; i++)
		close(i);
	if(argc < 3) {
		execl(((pwd && pwd->pw_shell[0])? pwd->pw_shell: "/bin/sh"), "sh", 0);
		printf("No shell!\n");
	} else {
		execvp(argv[2], &argv[2]);
		perror(argv[2]);
	}
	exit(8);
}
