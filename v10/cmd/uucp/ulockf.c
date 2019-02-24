/*	%W%
*/
#include "uucp.h"
VERSION(%W%);

#ifdef	V7
#define O_RDONLY	0
#endif

static void	stlock();
static int	onelock();

#ifdef ATTSVKILL
/*
 * create a lock file (file).
 * If one already exists, send a signal 0 to the process--if
 * it fails, then unlink it and make a new one.
 *
 * input:
 *	file - name of the lock file
 *	atime - is unused, but we keep it for lint compatibility with non-ATTSVKILL
 *
 * return:
 *	0	-> success
 *	FAIL	-> failure
 */
ulockf(file, atime)
register char *file;
time_t atime;
{
#ifdef	ASCIILOCKS
	static	char pid[SIZEOFPID+2] = { '\0' }; /* +2 for '\n' and NULL */
#else
	static int pid = -1;
#endif

	static char tempfile[MAXNAMESIZE];

#ifdef V8
	char *cp;
#endif

#ifdef	ASCIILOCKS
	if (pid[0] == '\0') {
		(void) sprintf(pid, "%*d\n", SIZEOFPID, getpid());
#else
	if (pid < 0) {
		pid = getpid();
#endif
		(void) sprintf(tempfile, "%s/LTMP.%d", X_LOCKDIR, getpid());
	}

#ifdef V8	/* this wouldn't be a problem if we used lock directories */
		/* some day the truncation of system names will bite us */
	cp = strrchr(file, '/');
	if (cp++ != CNULL)
	    if (strlen(cp) > V8DIRSIZ)
		*(cp+V8DIRSIZ) = NULLCHAR;
#endif V8
	if (onelock(pid, tempfile, file) == -1) {
		(void) unlink(tempfile);
		if (checkLock(file))
			return(FAIL);
		else {
		    if (onelock(pid, tempfile, file)) {
			(void) unlink(tempfile);
			DEBUG(4,"ulockf failed in onelock()\n","");
			return(FAIL);
		    }
		}
	}

	stlock(file);
	return(0);
}

/*
 * check to see if the lock file exists and is still active
 * - use kill(pid,0) - (this only works on ATTSV and some hacked
 * BSD systems at this time)
 * return:
 *	0	-> success (lock file removed - no longer active
 *	FAIL	-> lock file still active
 */
checkLock(file)
register char *file;
{
	register int ret;
	int lpid = -1;
#ifdef	ASCIILOCKS
	char alpid[SIZEOFPID+2];	/* +2 for '\n' and NULL */
#endif
	int fd;
	extern int errno;

	fd = open(file, O_RDONLY);
	DEBUG(4, "ulockf file %s\n", file);
	if (fd == -1) {
	    if (errno == ENOENT)  /* file does not exist -- OK */
		return(0);
	    DEBUG(4,"Lock File--can't read (errno %d) --remove it!\n", errno);
	    goto unlk;
	}
#ifdef	ASCIILOCKS
	ret = read(fd, (char *) alpid, SIZEOFPID+1); /* +1 for '\n' */
	(void) close(fd);
	if (ret != (SIZEOFPID+1)) {
#else
	ret = read(fd, (char *) &lpid, sizeof(int));
	(void) close(fd);
	if (ret != sizeof(int)) {
#endif

	    DEBUG(4, "Lock File--bad format--remove it!\n", "");
	    goto unlk;
	}
#ifdef	ASCIILOCKS
	lpid = atoi(alpid);
#endif
	if ((ret=kill(lpid, 0)) == 0 || errno == EPERM) {
	    DEBUG(4, "Lock File--process still active--not removed\n","");
	    return(FAIL);
	}
	else { /* process no longer active */
	    DEBUG(4, "kill pid (%d), ", lpid);
	    DEBUG(4, "returned %d", ret);
	    DEBUG(4, "--ok to remove lock file (%s)\n", file);
	}
unlk:
	
	if (unlink(file) != 0) {
		DEBUG(4,"ulockf failed in unlink()\n","");
		return(FAIL);
	}
	return(0);
}
#else

/*
 * check to see if the lock file exists and is still active
 * - consider the lock expired after SLCKTIME seconds.
 * return:
 *	0	-> success (lock file removed - no longer active
 *	FAIL	-> lock file still active
 */
checkLock(file)
register char *file;
{
	register int ret;
	int lpid = -1;
	int fd;
	extern int errno;
	struct stat stbuf;
	time_t ptime, time();

	fd = open(file, 0);
	DEBUG(4, "ulockf file %s\n", file);
	if (fd == -1) {
	    if (errno == ENOENT)  /* file does not exist -- OK */
		return(0);
	    DEBUG(4,"Lock File--can't read (errno %d) --remove it!\n", errno);
	    goto unlk;
	}
	ret = stat(file, &stbuf);
	if (ret != -1) {
		(void) time(&ptime);
		if ((ptime - stbuf.st_ctime) < SLCKTIME) {

			/*
			 * file not old enough to delete
			 */
			return(FAIL);
		}
	}
unlk:
	DEBUG(4, "--ok to remove lock file (%s)\n", file);
	
	if (unlink(file) != 0) {
		DEBUG(4,"ulockf failed in unlink()\n","");
		return(FAIL);
	}
	return(0);
}


/*
 * create a lock file (file).
 * If one already exists, the create time is checked for
 * older than the age time (atime).
 * If it is older, an attempt will be made to unlink it
 * and create a new one.
 * return:
 *	0	-> success
 *	FAIL	-> failure
 */
ulockf(file, atime)
register char *file;
time_t atime;
{
	register int ret;
	struct stat stbuf;
#ifdef	ASCIILOCKS
	static	char pid[SIZEOFPID+2] = { '\0' }; /* +2 for '\n' and null */
#else
	static int pid = -1;
#endif
	static char tempfile[MAXNAMESIZE];
	time_t ptime, time();
#ifdef V8
	char *cp;
#endif


#ifdef	ASCIILOCKS
	if (pid[0] == '\0') {
		(void) sprintf(pid, "%*d\n", SIZEOFPID, getpid());
#else
	if (pid < 0) {
		pid = getpid();
#endif
		(void) sprintf(tempfile, "%s/LTMP.%d", X_LOCKDIR, getpid());
	}
#ifdef V8	/* this wouldn't be a problem if we used lock directories */
		/* some day the truncation of system names will bite us */
	cp = strrchr(file, '/');
	if (cp++ != CNULL)
	    if (strlen(cp) > V8DIRSIZ)
		*(cp+V8DIRSIZ) = NULLCHAR;
#endif V8
	if (onelock(pid, tempfile, file) == -1) {

		/*
		 * lock file exists
		 * get status to check age of the lock file
		 */
		(void) unlink(tempfile);
		ret = stat(file, &stbuf);
		if (ret != -1) {
			(void) time(&ptime);
			if ((ptime - stbuf.st_ctime) < atime) {

				/*
				 * file not old enough to delete
				 */
				return(FAIL);
			}
		}
		ret = unlink(file);
		ret = onelock(pid, tempfile, file);
		if (ret != 0)
			return(FAIL);
	}
	stlock(file);
	return(0);
}
#endif

#define MAXLOCKS 10	/* maximum number of lock files */
char *Lockfile[MAXLOCKS];
int Nlocks = 0;

/*
 * put name in list of lock files
 * return:
 *	none
 */
static
void
stlock(name)
char *name;
{
	register int i;
	char *p;

	for (i = 0; i < Nlocks; i++) {
		if (Lockfile[i] == NULL)
			break;
	}
	ASSERT(i < MAXLOCKS, "TOO MANY LOCKS", "", i);
	if (i >= Nlocks)
		i = Nlocks++;
	p = calloc((unsigned) strlen(name) + 1, sizeof (char));
	ASSERT(p != NULL, "CAN NOT ALLOCATE FOR", name, 0);
	(void) strcpy(p, name);
	Lockfile[i] = p;
	return;
}

/*
 * remove all lock files in list
 * return:
 *	none
 */
void
rmlock(name)
register char *name;
{
	register int i;
#ifdef V8
	char *cp;

	if (name != CNULL) {
		cp = strrchr(name, '/');
		if (cp++ != CNULL)
	    if (strlen(cp) > V8DIRSIZ)
			*(cp+V8DIRSIZ) = NULLCHAR;
	}
#endif V8


	for (i = 0; i < Nlocks; i++) {
		if (Lockfile[i] == NULL)
			continue;
		if (name == NULL || EQUALS(name, Lockfile[i])) {
			(void) unlink(Lockfile[i]);
			(void) free(Lockfile[i]);
			Lockfile[i] = NULL;
		}
	}
	return;
}



/*
 * remove a lock file
 * return:
 *	0	-> success
 *	FAIL	-> failure
 */
delock(s)
char *s;
{
	char ln[MAXNAMESIZE];

	(void) sprintf(ln, "%s.%s", LOCKPRE, s);
	BASENAME(ln, '/')[MAXBASENAME] = '\0';
	rmlock(ln);
}


/*
 * create system lock
 * return:
 *	0	-> success
 *	FAIL	-> failure
 */
mlock(sys)
char *sys;
{
	char lname[MAXNAMESIZE];

	(void) sprintf(lname, "%s.%s", LOCKPRE, sys);
	BASENAME(lname, '/')[MAXBASENAME] = '\0';
	return(ulockf(lname, SLCKTIME) < 0 ? FAIL : 0);
}


/*
 * update access and modify times for lock files
 * return:
 *	none
 */
void
ultouch()
{
	register int i;
	time_t time();

	struct ut {
		time_t actime;
		time_t modtime;
	} ut;

	ut.actime = time(&ut.modtime);
	for (i = 0; i < Nlocks; i++) {
		if (Lockfile[i] == NULL)
			continue;
		utime(Lockfile[i], &ut);
	}
	return;
}

/*
 * makes a lock on behalf of pid.
 * input:
 *	pid - process id
 *	tempfile - name of a temporary in the same file system
 *	name - lock file name (full path name)
 * return:
 *	-1 - failed
 *	0  - lock made successfully
 */
static
onelock(pid,tempfile,name)
#ifdef	ASCIILOCKS
char *pid;
#else
int pid;
#endif 
char *tempfile, *name;
{	
	register int fd;
	char	cb[100];

	fd=creat(tempfile, 0444);
	if(fd < 0){
		(void) sprintf(cb, "%s %s %d",tempfile, name, errno);
		logent("ULOCKC", cb);
		if((errno == EMFILE) || (errno == ENFILE))
			(void) unlink(tempfile);
		return(-1);
	}
#ifdef	ASCIILOCKS
	(void) write(fd, pid, SIZEOFPID+1);	/* +1 for '\n' */
#else
	(void) write(fd,(char *) &pid,sizeof(int));
#endif
	(void) chmod(tempfile,0444);	/* silly */
	(void) chown(tempfile, UUCPUID, UUCPGID);
	(void) close(fd);
	if(link(tempfile,name)<0){
		DEBUG(4, "%s: ", sys_errlist[errno]);
		DEBUG(4, "link(%s, ", tempfile);
		DEBUG(4, "%s)\n", name);
		if(unlink(tempfile)< 0){
			(void) sprintf(cb, "ULK err %s %d", tempfile,  errno);
			logent("ULOCKLNK", cb);
		}
		return(-1);
	}
	if(unlink(tempfile)<0){
		(void) sprintf(cb, "%s %d",tempfile,errno);
		logent("ULOCKF", cb);
	}
	return(0);
}
