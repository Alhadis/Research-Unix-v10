#include <stdio.h>
#include <dk.h>

/*
 * program to run a command
 * on another cpu on Datakit.
 * Suitable for setuid to daemon, and checks for argument frogs
 * and a restricted list of commands
 */

int     rem;            /* remote file descriptor */
int     t;
char	*bldargs();
char	buf[BUFSIZ];
char	*cmdfile = "/etc/server";	/* legal commands here */

char	frogs[] = "\n&;^|`<>()";

#define MAXCHARS 8192
char args[MAXCHARS];

main(argc, argv)
char **argv;
{

	if (argc < 3) {
		fprintf(stderr, "usage: server <machine> <command>\n");
		exit(1);
	}
	setuid(geteuid());
        rem = tdkexec(argv[1], bldargs(argc-2, &argv[2]));
        if (rem<0) {
		extern char *dkerror;
		fprintf(stderr, "server: %s: %s\n", argv[1], dkerror);
                exit(1);
	}
        t = fork();
        if (t < 0) {
                fprintf(stderr,"server: can't fork\n");
                exit(1);
        }
        if (t == 0)
                to_proc();
        else
                from_proc();
	exit(0);
}

to_proc()
{
	register cc;

	while ((cc = read(0, buf, sizeof buf)) > 0)
		write(rem, buf, cc);
        /*
         * send some EOFs to
         * make it go away.
         */
        write(rem, 0, 0);
        write(rem, 0, 0);
        write(rem, 0, 0);
}

from_proc()
{
	register cc;

        while ((cc = read(rem, buf, sizeof buf)) > 0)
                write(1,  buf, cc);
        kill(t, 9);
}

char *
bldargs(argc, argv)
register char *argv[];
{
	register char *s, **t;
	FILE *f;
	char cmd[100];
	int ok = 0;

	if ((f = fopen(cmdfile, "r")) == NULL) {
		fprintf(stderr, "server can't open %s\n", cmdfile);
		exit(1);
	}
	while (fscanf(f, "%s", cmd) != EOF)
		if (strcmp(cmd, argv[0]) == 0) {
			ok = 1;
			break;
		}
	if (!ok) {
		fprintf(stderr, "server: disallowed command %s\n", argv[0]);
		exit(1);
	}
	while (argc) {
		if (strlen(args)+strlen(*argv)+3 >= MAXCHARS) {
			fprintf(stderr, "server: arg list too long\n");
			exit(1);
		}
		for (s = frogs; *s; s++) {
			if (index(*argv, *s)) {
				fprintf(stderr, "server: disallowed char %c\n",
				    *s);
				exit(1);
			}
		}
		strcat(args, *argv);
		strcat(args, " ");
		argc--;
		argv++;
	}
	return(args);
}
