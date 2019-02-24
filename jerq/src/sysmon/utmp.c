#include <stdio.h>
#include <utmp.h>

/* imported */
extern char *getlogin();
extern char *cttyname();
extern char *strncpy();
extern int strncmp();
extern long lseek(), time();

/* global */
static struct utmp utmp;
static struct utmp empty;
static int slot;

/* predeclared */
static int utmpslot();
static int chutmp();

/* get the information for a utmp entry */
extern int
getutmp()
{
	char *getlogin(), *cttyname();
	char *logname, *ttyname;

	/* get tty name */
	if ((ttyname = cttyname()) == NULL)
		return -1;
	strncpy (utmp.ut_line, ttyname+5, sizeof(utmp.ut_line));

	/* get user name */
	if ((logname = getlogin()) == NULL)
		return -1;
	strncpy (utmp.ut_name, logname, sizeof(utmp.ut_name));
	return 0;
}

/* put the entry into utmp */
extern int
setutmp()
{
	slot = ttyslot(utmp.ut_line);
	if (slot <= 0)
		return -1;
	return chutmp (slot, &utmp);
}

/* take the entry out of utmp */
extern void
resetutmp()
{
	if (slot > 0) {
		strncpy(utmp.ut_name, empty.ut_name, sizeof(utmp.ut_name));
		chutmp (slot, &utmp);
	}
}

static int
chutmp(slot, up)
	int slot;
	struct utmp *up;
{
	int fd;

	if ((fd = open("/etc/utmp", 2)) < 0)
		return -1;
	if (lseek(fd, (long)(slot*sizeof(utmp)), 0) < 0) {
		close(fd);
		return -1;
	}
	up->ut_time = time((long *)0);
	if (write(fd, (char *)up, sizeof(struct utmp)) != sizeof(struct utmp)) {
		close(fd);
		return -1;
	} else {
		close(fd);
		return 0;
	}
}
