#include <stdio.h>
#include <pwd.h>

main(argc, argv)
char **argv;
{
	register struct passwd *p;
	struct passwd *getpwnam();
	struct passwd *getpwuid();
	register char *dir;
	int err = 0;

	if(argc < 2) {
		if((p=getpwuid(getuid()))!=NULL ) {
			printf("%s\n",p->pw_dir);
			exit(0);
		}
		printf("\n");
		exit(1);
	}
	while( --argc ) {
		argv++;
		setpwent();
		if( (p=getpwnam(*argv))!=NULL )
			printf("%s\n", p->pw_dir);
		else {
			printf("\n");
			fprintf(stderr,"logdir: unknown user %s\n", *argv);
			err++;
		}
	}
	exit(err);
}
