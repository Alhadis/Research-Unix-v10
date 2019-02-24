#include <stdio.h>
#include "mail.h"
#include "string.h"
#include "dest.h"
#include "aux.h"
#include "process.h"

/* pipe an address through a command to translate it */
extern dest *
translate(dp)
	dest *dp;
{
	process *pp;
	string *line;
	dest *rv=NULL;

	pp = proc_start(s_to_c(dp->repl1), (stream *)NULL, outstream(),
			outstream(), -1);
	if (pp == NULL) {
		dp->status = d_resource;
		return NULL;
	}
	line = s_new();
	while (s_read_line(pp->std[1]->fp, line) != NULL)
		;
	rv = s_to_dest(s_restart(line), dp);
	s_restart(line);
	while (s_read_line(pp->std[2]->fp, line) != NULL)
		;
	if ((dp->pstat = proc_wait(pp)) != 0) {
		dp->repl2 = line;
		rv = NULL;
	} else
		s_free(line);
	proc_free(pp);
	return rv;
}
