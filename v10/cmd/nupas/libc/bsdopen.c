#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "string.h"
#include "mail.h"

/*
 *  open a file for appending.  if the file doesn't exist, create it.
 *  return the file descriptor.
 */
appendopen(file)
	char *file;
{
	int out;
	long lseek();

	lock(file);
	out = open(file, 2);
	if (out < 0)
		out = creat(file, 0666);
	else
		chmod(file, 0666);
	lseek(out, 0L, 2);
	return out;
}

/*
 *  close an append only file
 */
appendclose(fd)
	int fd;
{
	close(fd);
	unlock();
}

/*
 *  lock and open file.  If the file doesn't exist and mode!=0, create it.
 *  return the file descriptor.
 */

FILE *
lockopen(file, omode, pmode, uid, gid)
	char *file;
	char *omode;
	int pmode;
{
	FILE *fp;
	struct stat sbuf;
	int newfile;

	lock(file);
	if(stat(file, &sbuf)<0)
		newfile = 1;
	else
		newfile = 0;
	fp = fopen(file, omode);
	if(fp==NULL){
		unlock();
		return NULL;
	}
	if(newfile){
		chown(file, uid, gid);
		chmod(file, pmode);
	}
	return fp;
}

/*
 *  close and unlock a file.
 */
lockclose(fp)
	FILE *fp;
{
	fclose(fp);
	unlock();
}

/*
 *  reopen a file without losing the lock.
 */
FILE *
lockreopen(file, omode, fp)
	char *file;
	char *omode;
	FILE *fp;
{
	return freopen(file, omode, fp);
}

/*
 *  unlink ALL locks
 */
cleanlocks()
{
	unlock();
}
#ifndef lint
static	char *sccsid = "@(#)lock.c 1.5 88/02/08 SMI"; /* from S5R2 1.1 */
#endif

/*
 * mailx -- a modified version of a University of California at Berkeley
 *	mail program
 *
 * Stuff to do version 7 style locking.
 */


char	*maillock	= ".lock";		/* Lock suffix for mailname */
#ifdef USG
char	*lockname	= "/usr/mail/tmXXXXXX";
#else
char	*lockname	= "/usr/spool/mail/tmXXXXXX";
#endif
char	locktmp[30];				/* Usable lock temporary */
static char		curlock[50];		/* Last used name of lock */
static	int		locked;			/* To note that we locked it */

/*
 * Lock the specified mail file by setting the file mailfile.lock.
 * We must, of course, be careful to unlink the lock file by a call
 * to unlock before we stop.  The algorithm used here is to see if
 * the lock exists, and if it does, to check its modify time.  If it
 * is older than 5 minutes, we assume error and set our own file.
 * Otherwise, we wait for 5 seconds and try again.
 */

lock(file)
char *file;
{
	register int f;
	struct stat sbuf;
	long curtime;

	if (file == (char *)0) {
		printf("Locked = %d\n", locked);
		return(0);
	}
	if (locked)
		return(0);
	strcpy(curlock, file);
	strcat(curlock, maillock);
	strcpy(locktmp, lockname);
	mktemp(locktmp);
	unlink(locktmp);
	for (;;) {
		f = lock1(locktmp, curlock);
		if (f == 0) {
			locked = 1;
			return(0);
		}
		if (stat(curlock, &sbuf) < 0)
			return(0);
		time(&curtime);
		if (curtime < sbuf.st_ctime + 300) {
			sleep(5);
			continue;
		}
		unlink(curlock);
	}
}

/*
 * Remove the mail lock, and note that we no longer
 * have it locked.
 */

unlock()
{

	unlink(curlock);
	locked = 0;
}

/*
 * Attempt to set the lock by creating the temporary file,
 * then doing a link/unlink.  If it fails, return -1 else 0
 */

lock1(tempfile, name)
	char tempfile[], name[];
{
	register int fd;

	fd = creat(tempfile, 0);
	if (fd < 0)
		return(-1);
	close(fd);
	if (link(tempfile, name) < 0) {
		unlink(tempfile);
		return(-1);
	}
	unlink(tempfile);
	return(0);
}

/* Return true if file has been locked by us or another program using the same
 * lock name scheme.
 * But remove the lock file if the locking process has gone away.
 */

int
islocked(file)
	char *file;
{
	char thelock[50];
	struct stat sbuf;
	long curtime;

	strcpy(thelock, file);
	strcat(thelock, maillock);
	if(locked && strcmp(curlock, thelock)==0)
		return(1);
	for (;;) {
		if (stat(thelock, &sbuf) < 0)
			return(0);
		time(&curtime);
		if (curtime < sbuf.st_ctime + 300) {
			sleep(5);
			continue;
		}
		unlink(thelock);
		return(0);
	}
}
