/*
 * on success:
 *	returns the login name of the invoker.
 *	bug: returns pointer to static area.
 * on failure:
 *	returns 0.
 */

#include <utmp.h>
#include <pwd.h>

char	*strcpy();
char	*strncpy();
char	*cttyname();
struct passwd *getpwuid();
struct passwd *getpwnam();

char *
getlogin()
{
	register int ufd;
	register char *tp;
	register struct passwd *pwp;
	struct utmp ub;
	static char logbuf[sizeof(ub.ut_name)+1];
	int uid;

	logbuf[0] = '\0';
	if (getlogname(logbuf) >= 0 && logbuf[0] != '\0')
		return (logbuf);
	if ((tp = cttyname()) == 0)
		goto trypasswd;
	if (strncmp(tp, "/dev/", 5) == 0)
		tp += 5;
	if ((ufd = open("/etc/utmp", 0)) < 0)
		goto trypasswd;
	while (read(ufd, (char *) &ub, sizeof(ub)) == sizeof(ub)) {
		if (strncmp(ub.ut_line, tp, sizeof(ub.ut_line)))
			continue;
		strncpy(logbuf, ub.ut_name, sizeof(ub.ut_name));
		logbuf[sizeof(ub.ut_name)] = '\0';
		/* begin trim of padded blanks (old systems) */
		for (tp=logbuf; *tp != 0 && *tp != ' '; tp++)
			;
		*tp = '\0';
		/* end trim */
		break;
	}
	close(ufd);
trypasswd:
	uid = getuid();
	if (logbuf[0]) {
		pwp = getpwnam(logbuf);
		if (pwp && pwp->pw_uid==uid)
			return(logbuf);
	}
	if ((pwp = getpwuid(uid)) == 0)
		return(0);
	strcpy(logbuf, pwp->pw_name);
	return(logbuf);
}
