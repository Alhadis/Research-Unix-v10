#include <stdio.h>
#include "mail.h"
#include "string.h"
#include "dest.h"
#include "aux.h"
#include "process.h"

/*
 *  Run a command to authorize or refuse entry.  Return status 0 means
 *  authorize, -1 means refuse.
 */
extern
authorize(dp)
	dest *dp;
{
	process *pp;
	string *errstr;

	dp->authorized = 1;
	pp = proc_start(s_to_c(dp->repl1), (stream *)NULL, (stream *)NULL,
			outstream(), -1);
	if (pp == NULL)
		return;
	errstr = s_new();
	while (s_read_line(pp->std[2]->fp, errstr) != NULL)
		;
	if ((dp->pstat = proc_wait(pp)) != 0) {
		dp->repl2 = errstr;
		dp->status = d_noforward;
	} else
		s_free(errstr);
	proc_free(pp);
}
