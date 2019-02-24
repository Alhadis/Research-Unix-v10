static	char sccsid[] = "@(#)diff.c 4.1 10/9/80";

#include "diff.h"
/*
 * diff - driver and subroutines
 */

char	diff[] = DIFF;
char	diffh[] = DIFFH;

main(argc, argv)
	int argc;
	char **argv;
{
	register char *argp;

	dummy = malloc(0);	/* shut off malloc cache in v9 */
	dummy = malloc(1);
	status = 2;
	whichtemp = 0;		/* no temp files used yet! */
	diffargv = argv;
	argc--, argv++;
	while (argc > 2 && argv[0][0] == '-') {
		argp = &argv[0][1];
		argv++, argc--;
		while (*argp) switch(*argp++) {

		case 'e':
			opt = D_EDIT;
			continue;
		case 'f':
			opt = D_REVERSE;
			continue;
		case 'b':
			bflag = 1;
			continue;
		case 'B':
			bflag = 2;
			continue;
		case 'c':
			opt = D_CONTEXT;
			if (isdigit(*argp)) {
				context = atoi(argp);
				while (isdigit(*argp))
					argp++;
				if (*argp) {
					fprintf(stderr,
					    "diff: -c: bad count\n");
					done();
				}
				argp = "";
			} else
				context = 3;
			continue;
		case 'h':
			hflag++;
			continue;
		case 'r':
			rflag++;
			continue;
		case 's':
			sflag++;
			continue;
		default:
			fprintf(stderr, "diff: -%s: unknown option\n",
			    --argp);
			done();
		}
	}
	if (argc != 2) {
		fprintf(stderr, "diff: two filename arguments required\n");
		done();
	}
	file1 = argv[0];
	file2 = argv[1];
	if (hflag && opt) {
		fprintf(stderr,
		    "diff: -h doesn't support -e, -f, -c, or -I\n");
		done();
	}
	if (!strcmp(file1, "-"))
		stb1.st_mode = S_IFREG;
	else if (stat(file1, &stb1) < 0) {
		fprintf(stderr, "diff: ");
		perror(file1);
		done();
	}
	if (!strcmp(file2, "-"))
		stb2.st_mode = S_IFREG;
	else if (stat(file2, &stb2) < 0) {
		fprintf(stderr, "diff: ");
		perror(file2);
		done();
	}
	if ((stb1.st_mode & S_IFMT) == S_IFDIR &&
	    (stb2.st_mode & S_IFMT) == S_IFDIR) {
		diffdir(argv);
	} else
		diffreg();
	done();
}

char *
savestr(cp)
	register char *cp;
{
	register char *dp = malloc(strlen(cp)+1);

	if (dp == 0) {
		fprintf(stderr, "diff: ran out of memory\n");
		done();
	}
	strcpy(dp, cp);
	return (dp);
}

min(a,b)
	int a,b;
{

	return (a < b ? a : b);
}

max(a,b)
	int a,b;
{

	return (a > b ? a : b);
}

done()
{
	if (whichtemp) unlink(tempfile[0]);
	if (whichtemp==2) unlink(tempfile[1]);
	exit(status);
}

char *
talloc(n)
{
	register char *p;
	p = malloc((unsigned)n);
	if(p!=NULL)
		return(p);
	noroom();
}

char *
ralloc(p,n)	/*compacting reallocation */
char *p;
{
	register char *q;
/*  uses discredited realloc doctrine
	free(p);
	free(dummy);
	dummy = malloc(1);
*/
	q = realloc(p, (unsigned)n);
	if(q==NULL)
		noroom();
	return(q);
}

noroom()
{
	fprintf(stderr, "diff: files too big, try -h\n");
	done();
}
