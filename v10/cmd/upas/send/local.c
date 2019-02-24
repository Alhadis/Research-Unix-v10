#include <stdio.h>
#include <pwd.h>
#include "mail.h"
#include "string.h"
#include "dest.h"
#include "aux.h"
#include "process.h"
#include "ugid.h"
#include <sys/stat.h>

/* cheap foriegn import */
extern struct passwd * getpwnam();
extern FILE *lockopen();

/* expand a local destination into a list of destinations */
extern dest *
expand_local(dp)
	dest *dp;
{
	FILE *fp;
	string *file = s_new();
	string *line = s_new();
	dest *rv;
	int status;
	struct passwd *pw;
	struct stat sbuf;
	struct ugid ubuf;

	/* security - can't have a / in the original address */
	if (strchr(s_to_c(dp->addr), '/')!=NULL) {
		dp->status = d_syntax;
		return NULL;
	}

	/* make the mail file name */
	if (dp->repl1 == NULL)
		abspath(s_to_c(dp->addr), MAILROOT, file);
	else
		abspath(s_to_c(dp->repl1), MAILROOT, file);
	fp = lockopen(s_to_c(file), "r", 0, 0, 0);

	/* check for forwarding or piping */
	if (fp != NULL) {
		fstat(fileno(fp), &sbuf);
		(void)s_getline(fp, line);
		lockclose(fp);
		status = delivery_status(line);
	} else
		status = MF_NOMBOX;

	switch(status) {
	case MF_PIPE:
		dp->repl1 = s_clone(line);
		dp->status = d_pipeto;
		if (mboxowner(s_to_c(file),&ubuf) < 0)
			dp->status = d_unknown;
		else {
			dp->uid = ubuf.uid;
			dp->gid = ubuf.gid;
		}
		break;
	case MF_FORWARD:
		rv = s_to_dest(line, dp);
		if(rv==NULL)
			dp->status = d_badmbox;
		s_free(line);
		s_free(file);
		return rv;
	case MF_NORMAL:
		dp->status = d_cat;
		break;
	case MF_NOMBOX:
		/* no mailbox.  deliver only if in /etc/passwd. */
		pw = getpwnam(basename(s_to_c(file)));
		if (pw != NULL) {
			dp->uid = pw->pw_uid;
			dp->gid = pw->pw_gid;
			dp->status = d_cat;
		} else
			dp->status = d_unknown;
		break;
	case MF_NOTMBOX:
		/* security - don't cat onto non-mailboxes */
		dp->status = d_badmbox;
		break;
	}
	s_free(line);
	s_free(file);
	return NULL;
}
