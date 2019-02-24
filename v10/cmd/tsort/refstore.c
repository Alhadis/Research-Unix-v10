/* On machines that address memory in segments, reffetch and refstore
 * can be rewritten to save and store data in a different segment than
 * the rest of the program uses.
 */

#define XINC 8000
#include "ts.h"

static unsigned XMAX = 0;
static struct refc *x = 0;
char *malloc(), *realloc();

reffetch(i, rc)
unsigned i;
struct refc *rc;
{
	struct refc *xi;

	if (i >= XMAX) scream("i too big in reffetch\n");
	xi = x + i;
	rc->rcnext = xi->rcnext;
	rc->rcref = xi->rcref;
	}

refstore(i, rc)
unsigned i;
struct refc *rc;
{
	struct refc *xi;

	if (i >= XMAX) {
		XMAX += XINC;
		if (!x) x = (struct refc *) malloc(XINC*sizeof(struct refc));
		else x = (struct refc *) realloc(x, XMAX*sizeof(struct refc));
		if (!x) scream("not enough memory for refstore!");
		}
	if (i >= XMAX) scream("i too big in refstore\n");
	xi = x + i;
	xi->rcnext = rc->rcnext;
	xi->rcref = rc->rcref;
	}
