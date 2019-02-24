#include "uucp.h"

#define USAGE	"[-xNUM] [-uNUM]"

short Uopt;
void cleanup();

void logent(){}		/* to load ulockf.c */

main(argc, argv, envp)
char *argv[];
char **envp;
{
	register int i;

	Uopt = 0;
	Env = envp;

	(void) strcpy(Progname, "uusched");
	while ((i = getopt(argc, argv, "u:x:")) != EOF) {
		switch(i){
		case 'x':
			Debug = atoi(optarg);
			if (Debug <= 0)
				Debug = 1;
			break;
		case 'u':
			Uopt = atoi(optarg);
			if (Uopt <= 0)
				Uopt = 1;
			break;
		default:
			(void) fprintf(stderr, "\tusage: %s %s\n",
			    Progname, USAGE);
			cleanup(1);
		}
	}
	if (argc != optind) {
		(void) fprintf(stderr, "\tusage: %s %s\n", Progname, USAGE);
		cleanup(1);
	}
	if (toomany())
		cleanup(0);
	if (chdir(SPOOL) < 0) {
		DEBUG(4, "can't chdir to spooldir\n", 0);
		cleanup(101);
	}
	while (work())
		;		/* keep going till nothing to do */
	cleanup(0);
}

/*
 * are there too many uuscheds already?
 */
toomany()
{
	FILE *fp;
	int maxnum;
	char lockname[MAXNAMESIZE];

	if ((fp = fopen(LMTUUSCHED, "r")) == NULL) {
		DEBUG(4, "%s not found\n", LMTUUSCHED);
		return (0);	/* assume OK */
	}
	if (fscanf(fp, "%d", &maxnum) != 1) {
		DEBUG(4, "%s contains nonsense\n", LMTUUSCHED);
		fclose(fp);
		return (0);
	}
	fclose(fp);
	DEBUG(4, "limit %d scheds\n", maxnum);
	while (--maxnum >= 0) {
		sprintf(lockname, "%s.%d", S_LOCK, maxnum);
		if (ulockf(lockname, (time_t)X_LOCKTIME) == 0) {
			DEBUG(4, "grabbed lock %d\n", maxnum);
			return (0);
		}
	}
	DEBUG(4, "too many scheds\n", 0);
	return (1);
}

/*
 * one pass through the spool directory
 * return nonzero if we did some work
 */
work()
{
	DIR *spool;
	int ndone;
	char ent[NAMESIZE];

	DEBUG(4, "work\n", 0);
	ndone = 0;
	if ((spool = opendir(".")) == NULL) {
		DEBUG(4, "can't opendir .\n", 0);
		cleanup(101);
	}
	while (gnamef(spool, ent) == TRUE) {
		if (strncmp(ent, "LCK.", 4) == 0)
			continue;
		if (!DIRECTORY(ent))
			continue;
		DEBUG(4, "try %s: ", ent);
		ndone += onework(ent);
	}
	closedir(spool);
	return (ndone);
}

onework(dir)
char *dir;
{
	DIR *dp;
	char file[NAMESIZE];
	char lockname[MAXNAMESIZE];

	if (callok(dir) != 0) {
		DEBUG(4, "not ok to call\n", 0);
		return (0);
	}
	if ((dp = opendir(dir)) == NULL) {
		DEBUG(4, "can't open\n", 0);
		return (0);
	}
	while (gnamef(dp, file) == TRUE) {
		if (file[0] == CMDPRE && file[1] == '.') {
			DEBUG(4, "found work ...", 0);
			closedir(dp);
			sprintf(lockname, "%s.%s", LOCKPRE, dir);
			if (checkLock(lockname) == FAIL) {
				DEBUG(4, "locked\n", 0);
				return (0);
			}
			DEBUG(4, "call it\n", 0);
			return (exuucico(dir));
		}
	}
	DEBUG(4, "no work\n", 0);
	closedir(dp);
	return (0);
}

exuucico(name)
char *name;
{
	char cmd[BUFSIZ];
	int ret;
	char uopt[5];
	char sopt[BUFSIZ];
	int pid, rpid;

	(void) sprintf(sopt, "-s%s", name);
	if (Uopt)
	    (void) sprintf(uopt, "-x%.1d", Uopt);

	if ((pid = fork()) == 0) {
	    if (Uopt)
	        (void) execle(UUCICO, "UucicO", "-r1", uopt, sopt, (char *)0, Env);
	    else
	        (void) execle(UUCICO, "UucicO", "-r1", sopt, (char *)0, Env);

	    cleanup(100);
	}
	while ((rpid = wait(&ret)) > 0 && rpid != pid)
		;

	DEBUG(3, "ret=%d, ", ret);
	DEBUG(3, "errno=%d\n", errno);
	return (ret == 0);
}


void
cleanup(code)
int	code;
{
	DEBUG(4, "cleanup %d\n", code);
	rmlock(CNULL);
	exit(code);
}
